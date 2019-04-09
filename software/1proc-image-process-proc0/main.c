#include "system.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include <stdint.h>
#include "sys/time.h"
#include "../common/debug.h"
#include "../common/common.h"

/*OBS: the image is black and white with 1 byte of color depht*/
/*
 * @file main.c
 */

/*--------- Macros-------*/
/*------Compilation Options ------*/
#define RW_SIZE 4 /*block size*/
#define IMG_FILL_GRADIENT 0

/*------ Constrants ------*/
#define IMG_W (640) /*! image total width*/
#define IMG_H (480) /*! image total height*/

#define BLOCK_W (IMG_W)
#define BLOCK_H (IMG_H/2)

#define IMG_THRESHOLD (127)
#define IMAGE_BASE ((uint8_t *)SDRAM_CONTROLLER_BASE)

#define ACC_MAT(mat,y_pos,x_pos,x_size) ((mat) + (x_pos) +((y_pos)*(x_size)))

/*------- Function pre-declaration --------*/

void fill_image(uint8_t *, uint32_t, uint32_t); //__attribute__((always_inline));
uint8_t read_block(uint8_t*,uint8_t *,uint32_t,uint32_t);
void process_block_simple(uint8_t *block,uint32_t,uint32_t,uint8_t(*operation)(uint8_t value));

uint8_t test_thresh(uint8_t val)
{
  return (val>IMG_THRESHOLD?255:0);
}
/*------- Global Variables --------*/

/*OBS: the image is black and white with 1 byte of color depht*/
clock_t start=0,now=0;

uint8_t image_block [BLOCK_H*BLOCK_W]; /*! memory block used as buffer*/

/*--------- Main ---------*/
int main(void)
{
	DEBUG("hello there");
	while(1)
	{
		start = clock();
		fill_image((alt_u8 *) IMAGE_BASE,IMG_H,IMG_W);
		now=clock();
		DEBUG_NUM("time to fill mat:",now-start);

    for(uint8_t h=0;h<IMG_H; h+=BLOCK_H)
      {
        for(uint8_t w=0;w<IMG_W; w+= BLOCK_W)
          {
            err_t err;
            err = read_block(IMAGE_BASE, image_block, h, w);
            process_block_simple(image_block, BLOCK_H, BLOCK_W, test_thresh);
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
 * apply (operation) to block, substituting the value for the returned value,
 * only useful for operations independent of adjacent pixels ("operation" must be reentrant and idempotent).
 */
void process_block_simple(
                          uint8_t* block, /*!pointer to memory block*/
                          uint32_t block_h, /*!block height*/
                          uint32_t block_w, /*!block width*/
                          uint8_t (*operation)(uint8_t value)/*function to apply to value*/
                          )
{
  uint32_t v=0,r=0;
	for(uint32_t h=0; h<block_h; ++h)
    {
    for(uint32_t w=0; w<block_w; ++w)
      {
        v=*ACC_MAT(block,h,w,block_w); /*read the value*/
        r=operation(v);/* apply operation to it*/
        *ACC_MAT(block,h,w,block_w)=r;/*store back*/
      }
    }
}

/*
 * @brief read a block of memory from flash.
 */
uint8_t read_block( uint8_t * image_base 	/*! base address in SDRAM*/,
                    uint8_t * image_block 	/*! pointer to local memory block*/,
                    uint32_t block_start_h  /*! block starting position */,
                    uint32_t block_start_w  /*! block starting position */
                    )
{
	/*remember that the image_block is a 2d array*/
	uint32_t v = 0;
  uint32_t block_end_h = BLOCK_H + block_start_h;  block_end_h = block_end_h > IMG_H ? IMG_H : block_end_h;
  uint32_t block_end_w = BLOCK_W + block_start_w;  block_end_w = block_end_w > IMG_W ? IMG_W : block_end_w;

  for(uint8_t h=block_start_h; h<BLOCK_H; ++h)
	{
		for(uint8_t w=block_start_w; w<BLOCK_W; w+=RW_SIZE)
		{
			v = IORD_32DIRECT(image_base,(w+h*BLOCK_W)>>2);
			for(uint8_t t=0;t<3;t++)
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
 * @brief load a test "image" onto SDRAM
 *
 */
void fill_image(uint8_t * image_base, uint32_t image_height, uint32_t image_width)
{
	/*fill image with white to gray gradient*/
	uint32_t i,j;

	for(i=0;i<image_height;++i) /*access the image as a contiguous region of memory */
	{
		for(j=0;j<image_width;j+=RW_SIZE)
		{
#if IMG_FILL_GRADIENT == 1 /*use a black to white gradient*/
#if RW_SIZE == 1
			uint8_t v;
		 v = ((j*255)/image_width);
			IOWR_8DIRECT(image_base,j+i*image_width,v);
#elif RW_SIZE == 2
			uint16_t v;
			v =		((j*255)/image_width) |((((j+1)*255)/image_width) << 8);
			IOWR_16DIRECT(image_base,(j+i*image_width)>>1,v);
#elif RW_SIZE == 4
			uint32_t v,r;
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
