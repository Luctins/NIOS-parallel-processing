#include "system.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include <stdint.h>
#include "sys/time.h"
#include "../debug.h"
#include "../common.h"

/*OBS: the image is black and white with 1 byte of color depht*/
/*
 * @file main.c
 */

/*--------- Macros-------*/
/*------Compilation Options ------*/
#define RW_SIZE 4 /*block size*/
#define IMG_FILL_GRADIENT 0

/*------ Constrants ------*/
#define IMG_W (800) /*! image total width*/
#define IMG_H (600) /*! image total height*/

#define BLOCK_W (IMG_W/2)
#define BLOCK_H (IMG_H)

#define IMG_THRESHOLD (127)

/*------- Function pre-declaration --------*/

void fill_image(uint8_t *, uint16_t, uint16_t); //__attribute__((always_inline));
uint8_t read_block(uint8_t*,uint8_t *,uint32_t,uint32_t);
void process_block_simple(uint8_t *block,uint16_t,uint16_t,uint8_t*(operation)(uint8_t value));

/*------- Global Variables --------*/

/*OBS: the image is black and white with 1 byte of color depht*/
clock_t start=0,now=0;

uint8_t image_block [BLOCK_H][BLOCK_W]; /*! memory block used as buffer*/

/*--------- Main ---------*/
int main(void)
{
	DEBUG("hello there");
	while(1)
	{
		start = clock();
		fill_image((alt_u8 *) SDRAM_CONTROLLER_BASE,IMG_H,IMG_W);
		now=clock();
		DEBUG_NUM("time to fill mat:",now-start);

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
void process_block_simple(uint8_t* block,uint16_t block_h, uint16_t block_w, uint8_t *(operation)(uint8_t value))
{
	for(uint8_t i=0; i<BLOCK_H; ++i)
    {
    for(uint8_t j=0; j<BLOCK_W; ++j)
      {

      }
    }
}

/*
 * @brief read a block of memory from flash.
 */
uint8_t read_block( uint8_t * image_base 	/*! base address in SDRAM*/,
					uint8_t * image_block 	/*! pointer to local memory block*/,
					uint32_t block_start_i  /*! block starting position (i)*/,
					uint32_t block_start_j  /*! block starting position (j)*/)
{
	/*remember that the image_block is a 2d array*/
	uint32_t v = 0;
	for(uint8_t i=block_start_i; i<BLOCK_H; ++i)
	{
		for(uint8_t j=block_start_j; j<BLOCK_W; j+=RW_SIZE)
		{
			v = IORD_32DIRECT(image_base,(j+i*BLOCK_W))>>2);
			for(uint8_t t=0;t<3;t++)
			{
				*(image_block+((j+t)+i*BLOCK_W)) = (v >> (8*t)) & 0xff; /*extract the lower byte from v*/
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
