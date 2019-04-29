/*--------- include --------*/
#include "system.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include "alt_types.h"
//#include "sys/alt_timestamp.h"
#include "sys/times.h"
//#include <stdint.h>
#include <unistd.h>
#include "sys/time.h"
#include "debug.h"
#include "common.h"
#include "altera_avalon_mutex.h"
#include "altera_avalon_mutex_regs.h"

/*OBS: the image is black and white with 1 byte of color depht*/
/*
 * @file main.c
 */

/*--------- Macros-------*/
/*------Compilation Options ------*/
#define RW_SIZE 4 /*block size*/
#define IMG_FILL_GRADIENT 1

/*------ Constrants ------*/
#define IMG_W (640) /*! image total width*/
#define IMG_H (480) /*! image total height*/

#define BLOCK_W (IMG_W)
#define BLOCK_H (IMG_H/2)

#define IMG_THRESHOLD (127)
#define MAT_BASE ((alt_u8 *)SDRAM_CONTROLLER_BASE)
#define CPUID ALT_CPU_CPU_ID_VALUE

#define ACC_MAT(mat,y_pos,x_pos,x_size) ((mat) + (x_pos) +((y_pos)*(x_size)))

#define MUTEX_LOCK(mut) altera_avalon_mutex_lock(mut, CPUID)
#define MUTEX_UNLOCK(mut) altera_avalon_mutex_unlock(mut)

/*------- Function pre-declaration --------*/

void fill_image(alt_u8 *, alt_u32 , alt_u32); //__attribute__((always_inline));
alt_u8 read_block(alt_u8*,alt_u8 *,alt_u32,alt_u32);
alt_u8 write_block(alt_u8 *, alt_u8 *, alt_u32, alt_u32);
void process_block_simple(alt_u8 *block,alt_u32,alt_u32,alt_u8(*operation)(alt_u8 value)) __attribute__((hot));
inline alt_u8 test_thresh(alt_u8) __attribute__((hot, always_inline));
/*
  test function used as parameter for process block simple
 */
inline alt_u8 test_thresh(alt_u8 val)
{
  return (val>IMG_THRESHOLD?255:0);
}

/*------- Global Variables --------*/

/*OBS: the image is black and white with 1 byte of color[?] depht*/
clock_t start=0,now=0;
alt_mutex_dev * mutex = NULL;

alt_u8 image_block [BLOCK_H*BLOCK_W]; /*! memory block used as buffer*/

/*--------- Main ---------*/
int main(void)
{
	DEBUG("hello there");
  mutex = altera_avalon_mutex_open("/dev/mutex");
  ASSERT_ERROR(mutex == NULL,/*do something (error condition)*/);
  //altera_avalon_mutex_first_lock(mutex);

#if CPUID != 0 /*wait*/
  usleep(1000000);
#else  /*obtain mutex and fill in test image*/
  altera_avalon_mutex_lock(mutex,CPUID);
  start = clock();
  fill_image(MAT_BASE,IMG_H,IMG_W);
	now=clock();
  DEBUG_NUM("time to fill initial mat:",now-start);
  altera_avalon_mutex_unlock(mutex);
#endif
	while(1)
	{

    start=clock();
  for(alt_u32 h=0;h<IMG_H; h+=BLOCK_H)
    {
      for(alt_u32 w=0;w<IMG_W; w+= BLOCK_W)
        {
          err_t err;
          /*read a block from memory*/
          altera_avalon_mutex_lock(mutex,CPUID);
          err = read_block(MAT_BASE, image_block, h, w);
          altera_avalon_mutex_unlock(mutex);

          /*process a block*/
          start=clock();
          process_block_simple(image_block, BLOCK_H, BLOCK_W, test_thresh);
          now=clock();
          DEBUG_NUM("processing this block took:", now-start);

          /*write back a block to memory*/
          MUTEX_LOCK(mutex);
          write_block(MAT_BASE,image_block,h,w);
          MUTEX_UNLOCK(mutex);
        }
    }
  /*read memory blocks until there's none to process*/
  /*for each block iterate inside it*/
	}
	return 0;
}


/*---- Function Definition-----------------*/
/*-----------------------------------------*/
/*
 * @brief apply (operation) to block, substituting the value for the returned value,
 * only useful for operations independent of adjacent pixels ("operation" must be reentrant and idempotent).
 */
void process_block_simple(
                          alt_u8* block, /*!pointer to memory block*/
                          alt_u32 block_h, /*!block height*/
                          alt_u32 block_w, /*!block width*/
                          alt_u8 (*operation)(alt_u8 value)/*function to apply to value*/
                          )
{
  alt_u32 v=0,r=0;
	for(alt_u32 h=0; h<block_h; ++h)
    {
    for(alt_u32 w=0; w<block_w; ++w)
      {
        v=*ACC_MAT(block,h,w,block_w); /*read the value*/
        r=operation(v);/* apply operation to it*/
        *ACC_MAT(block,h,w,block_w)=r;/*store back*/
      }
    }
}

/*
 * @brief write a block back to memory
 */
alt_u8 write_block(
                   alt_u8 * image_base 	/*! base address in SDRAM*/,
                    alt_u8 * image_block 	/*! pointer to local memory block*/,
                    alt_u32 block_start_h  /*! block starting h position */,
                    alt_u32 block_start_w  /*! block starting w  position */
                    )
{
	alt_u32 v = 0;
  alt_u32 block_end_h = BLOCK_H + block_start_h;
  alt_u32 block_end_w = BLOCK_W + block_start_w;

  /*stop at image boundaries*/
  block_end_h = block_end_h > IMG_H ? IMG_H : block_end_h;
  block_end_w = block_end_w > IMG_W ? IMG_W : block_end_w;

  for(alt_u8 h=block_start_h; h<BLOCK_H; ++h)
    {
      for(alt_u8 w=block_start_w; w<BLOCK_W; w+=RW_SIZE)
        {
          v = IORD_32DIRECT(image_base,(w+h*BLOCK_W)>>2);
          for(alt_u8 t=0;t<3;t++)
            {
              *ACC_MAT(image_block, h, w+t, BLOCK_W) = (v & 0xff);
              //*(image_block+((w+t)+h*BLOCK_W)) = (v  & 0xff); /*extract the lower byte from v*/
              v >>= 8; /*shift v 8 bits*/
            }
        }
    }
	return ERR_OK;
}

/*
 * @brief read a block of memory from flash.
 */
alt_u8 read_block( alt_u8 * image_base 	/*! base address in SDRAM*/,
                    alt_u8 * image_block 	/*! pointer to local memory block*/,
                    alt_u32 block_start_h  /*! block starting position */,
                    alt_u32 block_start_w  /*! block starting position */
                    )
{
	/*remember that the image_block is a 2d array*/
	alt_u32 v = 0;
  alt_u32 block_end_h = BLOCK_H + block_start_h;  block_end_h = block_end_h > IMG_H ? IMG_H : block_end_h;
  alt_u32 block_end_w = BLOCK_W + block_start_w;  block_end_w = block_end_w > IMG_W ? IMG_W : block_end_w;

  for(alt_u8 h=block_start_h; h<BLOCK_H; ++h)
	{
		for(alt_u8 w=block_start_w; w<BLOCK_W; w+=RW_SIZE)
		{
			v = IORD_32DIRECT(image_base,(w+h*BLOCK_W)>>2);
			for(alt_u8 t=0;t<3;t++)
			{
        *ACC_MAT(image_block, h, w+t, BLOCK_W) = (v & 0xff);
        //*(image_block+((w+t)+h*BLOCK_W)) = (v  & 0xff); /*extract the lower byte from v*/
        v >>= 8; /*shift v 8 bits*/
			}
		}
	}
	return ERR_OK;
}



/**
 * @brief load a test matrix onto SDRAM
 *
 */
void fill_image(alt_u8 * image_base, alt_u32 image_height, alt_u32 image_width)
{
	/*fill image with white to gray gradient*/
	alt_u32 i,j;

	for(i=0;i<image_height;++i) /*access the image as a contiguous region of memory */
	{
		for(j=0;j<image_width;j+=RW_SIZE)
		{
#if IMG_FILL_GRADIENT == 1 /*use a black to white gradient*/
#if RW_SIZE == 1
      alt_u8 v;
		 v = ((j*255)/image_width);
			IOWR_8DIRECT(image_base,j+i*image_width,v);
#elif RW_SIZE == 2
			alt_u16 v;
			v =		((j*255)/image_width) |((((j+1)*255)/image_width) << 8);
			IOWR_16DIRECT(image_base,(j+i*image_width)>>1,v);
#elif RW_SIZE == 4
			alt_u32 v,r;
			v =		((j*255)/image_width) | ((((j+1)*255)/image_width) << 8) |
					((((j+2)*255)/image_width) << 16) |  ((((j+3)*255)/image_width) << 24);
			IOWR_32DIRECT(image_base,(j+i*image_width)>>2,v);
			/*
			r=IORD_32DIRECT(image_base,(j+i*image_width)>>2);
			if(v=!r)
			{
				DEBUGF("fault at j:%x i:",j,i);
				break;
			}
			*/
#endif
#else /*use a solid color*/
#if RW_SIZE == 1
			IOWR_8DIRECT(image_base,i,0x7f);
#elif RW_SIZE == 2
			IOWR_16DIRECT(image_base,i>>1 /* x/2 == x >> 1*/,0x7f7f);
#elif RW_SIZE == 4
			IOWR_32DIRECT(image_base,i>>2 /* x/4 == x >> 2*/,0x7f7f7f7f);
#else
#error "please define a valid block size in bytes (1,2 or 4) "
#endif
#endif
		}
	}
}

/*-------------------------------------------------------------------------------------------------------------------------*/
