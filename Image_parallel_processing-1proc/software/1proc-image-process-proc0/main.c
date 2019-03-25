#include "system.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include <stdint.h>
#include "sys/time.h"
#include "../debug.h"
#include "../common.h"

/*OBS: the image is black and white with 1 byte of color depht*/
/*
 * \intr
 */

/*--------- Macros-------*/
/*------Compilation Options ------*/
#define IMG_WRITE_BSIZE 4 /*block size*/
#define IMG_FILL_GRADIENT 0

/*------ Constrants ------*/
#define IMG_W (800) /*! image total width*/
#define IMG_H (600) /*! image total height*/

#define BLOCK_W (BLOCK_W/2)
#define BLOCK_H (IMG_H)

#define IMG_THRESHOLD (127)

/*------- Function pre-declaration --------*/

void fill_image(uint8_t *, uint16_t, uint16_t); //__attribute__((always_inline));
uint8_t read_block(uint8_t *,uint8_t);
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

		for(uint32_t i=0;i<BLOCK_H;++i)
		{
		}
	}
	return 0;
}


/*---- Function Definition-----------------*/
/*-----------------------------------------*/
/*
 * apply (operation) to block, substituting the value for the returned value, only for operations independent of adjacent pixels
 */
void process_block_simple(uint8_t* block,uint16_t block_h, uint16_t block_w, uint8_t *(operation)(uint8_t value))
{

}
/*
 * read a block of memory from flash.
 */
uint8_t read_block(	uint8_t *image_block, /*! pointer to memory block*/
					uint8_t block_num 	/*! block number, counter top to bottom left to right (in a image)*/)
{
	/*remember that the image_block is a 2d array*/
	uint32_t v = 0;
	for(uint8_t h=0; h<BLOCK_H; ++h)
	{
		for(uint8_t w; w<BLOCK_W; ++w)
		{

		}
	}
	return ERR_OK;
}

/**
 * @brief fill image memory range
 *
 */
void fill_image(uint8_t * image_base, uint16_t image_height, uint16_t image_width)
{
	/*fill image with white to gray gradient*/
	uint16_t i,j;
	for(i=0;i<image_height;++i) /*write image in "lines" for better memory performance*/
	{
		for(j=0;j<image_width;j+=IMG_WRITE_BSIZE)
		{
#if IMG_FILL_GRADIENT == 1 /*use a white to black gradient*/
#if IMG_WRITE_BSIZE == 1
			uint8_t v;
		 v =		(j* (255/image_width));
			IOWR_8DIRECT(image_base,j+(i*image_width),v);
#elif IMG_WRITE_BSIZE == 2
			uint16_t v;
			v =		(j* (255/image_width)) |
					(((j+1)*(255/image_width)) >> 8);
			IOWR_16DIRECT(image_base,(j+(i*image_width))/2,v);
#elif IMG_WRITE_BSIZE == 4
			uint32_t v;
			v =		(j* (255/image_width)) 		|
					(((j+1)*(255/image_width)) >> 8) 	|
					(((j+2)*(255/image_width)) >> 16) 	|
					(((j+3)*(255/image_width)) >> 24);
			declarationIOWR_32DIRECT(image_base,(j+(i*image_width))/4,v);
#else
#error "please define a valid block size in bytes "
#endif
#else /*use a solid color*/
#if IMG_WRITE_BSIZE == 1
			IOWR_8DIRECT(image_base,j+(i*image_width),0x7f);
#elif IMG_WRITE_BSIZE == 2
			IOWR_16DIRECT(image_base,(j+(i*image_width))/2,0x7f7f);
#elif IMG_WRITE_BSIZE == 4
			IOWR_32DIRECT(image_base,(j+(i*image_width))/4,0x7f7f7f7f);
#endif
#endif
		}
	}
	DEBUG_OUTPUT("\n");
}
