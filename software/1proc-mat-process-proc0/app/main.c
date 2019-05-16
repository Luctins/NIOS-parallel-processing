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
//#include "sys/clock.h"
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
#define IMG_W (800) /*! image total width*/
#define IMG_H (800) /*! image total height*/

#define BLOCK_W (400)
#define BLOCK_H (400)
#define IMG_THRESHOLD (127)
#define MAT_BASE ((alt_u8 *)SDRAM_CONTROLLER_BASE)
#define CPUID ALT_CPU_CPU_ID_VALUE
#define PROC0_CPUID 1


#define ACC_MAT(mat,x_pos,y_pos,x_size) ((mat) + (x_pos) +((y_pos)*(x_size)))

#define MUTEX_LOCK(mut) altera_avalon_mutex_lock(mut, CPUID)
#define MUTEX_UNLOCK(mut) altera_avalon_mutex_unlock(mut)

#define DEBUG_OK() DEBUG("OK at: " STR(__LINE__))

/*------- Function pre-declaration --------*/

alt_u8 read_block(alt_u8 * const ,alt_u8 * const ,const alt_u32, const alt_u32, const alt_u32, const alt_u32);

void fill_image(alt_u8 * const, const alt_u32 , const alt_u32); //__attribute__((always_inline));

alt_u8 write_block(alt_u8 * const, alt_u8 * const, const alt_u32, const alt_u32 , const alt_u32, const alt_u32);

void process_block_simple(alt_u8 * const ,const alt_u32,const alt_u32,alt_u8(* const operation)(alt_u8 value)) __attribute__((hot));
alt_u8 test_thresh(alt_u8) __attribute__((hot, always_inline));


/*
  test function used as parameter for process_block_simple
 */

inline alt_u8 test_thresh(alt_u8 val)
{
  return (val>IMG_THRESHOLD?255:0);
}

/*------- Global Variables --------*/

/*OBS: the image is black and white with 1 byte of color[?] depht*/
alt_mutex_dev * mutex = NULL;
static alt_u8 image_block [BLOCK_H*BLOCK_W]; /*! memory block used as buffer*/

/*--------- Main ---------*/
int main(void)
{
  clock_t start=0,now=0;
	DEBUG("hello there");
  mutex = altera_avalon_mutex_open("/dev/mutex_0");
  ASSERT_ERROR(mutex == NULL,/*do something (error condition)*/);
  DEBUG("opened mutex OK");

#if CPUID != PROC0_CPUID /*wait*/
  usleep(1000000);
#else  /*obtain mutex and fill in test image*/
  start = clock();
  MUTEX_LOCK(mutex);
  fill_image(MAT_BASE,IMG_H,IMG_W);
  MUTEX_UNLOCK(mutex);
	now=clock();
  DEBUG_NUM("time to fill initial mat:",now-start);
#endif
	while(1)
	{
    clock_t outloop_start = clock();
  for(alt_u32 h=0;h<IMG_H; h+=BLOCK_H)
    {
      DEBUG_NUM("h: ",h);
      for(alt_u32 w=0;w<IMG_W; w+= BLOCK_W)
        {
          DEBUG_NUM("w: ",w);
          err_t err;
          /*read a block from memory*/
          MUTEX_LOCK(mutex);
          err = read_block(MAT_BASE, image_block, h, w);
          MUTEX_UNLOCK(mutex);
          DEBUG("read block");

          /*process a block*/
          start=clock();
          /*TODO: implement logic to stop at image borders (e.g. use variable size blocks)*/
          process_block_simple(image_block, BLOCK_H, BLOCK_W, test_thresh);
          now=clock();
          DEBUG_NUM("processing this block took:", now-start);

          /*write back a block to memory*/
          MUTEX_LOCK(mutex);
          write_block(MAT_BASE,image_block,h,w);
          MUTEX_UNLOCK(mutex);
        }
    }

      clock_t outloop_now = clock();
      DEBUG_NUM("total time: ",outloop_now - outloop_start);
#if CPUID != PROC0_CPUID /*wait*/
#else  /*obtain mutex and fill in test image*/
      start = clock();
      MUTEX_LOCK(mutex);
      fill_image(MAT_BASE,IMG_H,IMG_W);
      MUTEX_UNLOCK(mutex);
      now=clock();
      DEBUG_NUM("time to fill sintetic image:",now-start);
#endif

#endif
  /*read memory blocks until there's none to process*/
  /*for each block iterate inside it*/
	return 0;
}

  //block_end_h = block_end_h > IMG_H ? IMG_H : block_end_h;
  //block_end_w = block_end_w > IMG_W ? IMG_W : block_end_w;

/*---- Function Definition-----------------*/

/*
 * @brief read a block of memory from flash to a local buffer.
 */
alt_u8 read_block(
                  alt_u8 * const source_base   /*! base address to read from in SDRAM*/,
                  alt_u8 * const dest 	/*! pointer to local memory block*/,
                  const alt_u32 w_start  /*! block starting position */,
                  const alt_u32 h_start  /*! block starting position */,
                  const alt_u32 w_len /*! block length */,
                  const alt_u32 h_len /*! block lenght */
                  )
{
	alt_u32 v = 0;
  alt_u32 h_end = h_start + h_len -1;
  alt_u32 w_end = w_start + w_len -1;

  for(alt_u32 h=h_start; h<h_end; ++h)
    {
      for(alt_u32 w=w_start; w<w_end; w+=RW_SIZE) /*step in 1,2,4 bit chunks (words)*/
        {
          v = IORD_32DIRECT(source_base,ACC_MAT(0,w,h,w_len)>>2); /*acess SDRAM, the right shift is to convert the address to 32 bit words*/

          /*write v to the image, as 4 bytes in series like (B0,B1,B2,B3), by storing each byte then shifting v to the right*/
          *ACC_MAT(dest, w  , h, w_len) = (v & 0xff); v >>= 8;
          *ACC_MAT(dest, w+1, h, w_len) = (v & 0xff); v >>= 8;
          *ACC_MAT(dest, w+2, h, w_len) = (v & 0xff); v >>= 8;
          *ACC_MAT(dest, w+3, h, w_len) = (v & 0xff); v >>= 8;
        }
    }
	return ERR_OK;
}


/**
   @brief write a block back to memory
 */
alt_u8 write_block(
                   alt_u8 * const dest_base 	/*! base address in SDRAM*/,
                    alt_u8 * const src 	/*! pointer to local memory block*/,
                   const alt_u32 w_start  /*! block starting position */,
                   const alt_u32 h_start  /*! block starting position */,
                   const alt_u32 w_len /*! block length */,
                   const alt_u32 h_len /*! block lenght */
                    )
{
  alt_u32 v=0;
  alt_u32 h_end = h_start + h_len;
  alt_u32 w_end = w_start + w_len;

  for(alt_u32 h=h_start; h< h_end; ++h)
    {
      for(alt_u32 w=w_start; w<w_end; w+=RW_SIZE) /*step in 1,2,4 bit chunks (words)*/
        {
          v =
            *ACC_MAT(src, w  , h, w_len) |
            *ACC_MAT(src, w+1, h, w_len) << 8 |
            *ACC_MAT(src, w+2, h, w_len)  << 16 |
            *ACC_MAT(src, w+3, h, w_len)  << 24 ;
          IOWR_32DIRECT(dest_base,ACC_MAT(0,w,h,w_len)>>2,v);
        }
    }
	return ERR_OK;
}


/**
   @brief apply (operation) to block, substituting the value for the returned value, only useful for operations independent of adjacent pixels ("operation" must be reentrant and idempotent).
 */
void process_block_simple(
                          alt_u8 * const block, /*!pointer to memory block*/
                          const alt_u32 w_len, /*!block height*/
                          const alt_u32 h_len, /*!block width*/
                          alt_u8 (* const operation)(alt_u8 value)/*function to apply to value*/
                          )
{
  alt_u32 v=0,r=0;
	for(alt_u32 h=0; h<h_len; ++h)
    {
      //VAR_DUMPN(h);
    for(alt_u32 w=0; w<w_len; ++w)
      {
        v=*ACC_MAT(block,h,w,w_len); /*read the value*/
        r=operation(v);/* apply operation to it*/
        *ACC_MAT(block,h,w,w_len)=r;/*store back*/
        //VAR_DUMPN(w);
      }
    }
}


/**
   @brief load a sintetic matrix onto SDRAM

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
			v =
        ((j*255)/image_width) |
        ((((j+1)*255)/image_width) << 8);
			IOWR_16DIRECT(image_base,(j+i*image_width)>>1,v);
#elif RW_SIZE == 4
			alt_u32 v,r;
			v =
        ((j*255)/image_width)             |
        ((((j+1)*255)/image_width) << 8)  |
				((((j+2)*255)/image_width) << 16) |
        ((((j+3)*255)/image_width) << 24);
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
