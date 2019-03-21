#include "system.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include <stdint.h>
#include "sys/time.h"
#include "../debug.h"

/*--------- Macros/constants -------*/
#define IMG_WRITE_BSIZE 4 /*block size*/

#define IMG_H (600)
#define IMG_W (800)

#define IMG_THRESHOLD (127)

#define IMG_FILL_GRADIENT 0
/*------- Function pre-declaration --------*/

void fill_image(uint8_t *, uint16_t, uint16_t); //__attribute__((always_inline));
void print_num(uint32_t);

/*------- Global Variables --------*/

/*OBS: the image is black and white with 1 byte of color depht*/

clock_t start=0,now=0;

/*--------- Main ---------*/
int a = 127;
int main(void)
{
	DEBUG("hello there");
	//print_num(123456789);
#if 1
	while(1)
	{
		start = clock();
		fill_image((alt_u8 *) SDRAM_CONTROLLER_BASE,IMG_H,IMG_W);
		now=clock();
		print_num(now-start);
	}
#endif
	return 0;
}


/*---- Function Definition-----------------*/
/*-----------------------------------------*/

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
			IOWR_32DIRECT(image_base,(j+(i*image_width))/4,v);
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

/**
 * @brief print a number (because of memory constraints the limited printf only works for hex and strings, no decimals
 *
 */
void print_num(uint32_t num)
{
	char buff[15],tmp;
	char * str = buff;
	uint8_t dp=0; /*number of decimal places*/
	while(num>0)
	{
		*(str)='0'+(num%10); /*set the digit as the remainder of dividing by ten (because '0' + 1 = '1')*/
		++str;/*advance the pointer by one*/
		num -= num%10;
		num /= 10; /*shift the number one *decimal* place*/
		++dp;
	}
	/*this is needed otherwise the number would be written in reverse*/
	for(uint8_t i = 0;i<(dp>>1/*dp/2*/);++i)
	{
		tmp = buff[i];
		buff[i] = buff[dp-i-1];
		buff[dp-i-1] = tmp;
	}
	*str='\0'; /*null termination*/
	DEBUG_OUTPUT(buff);
}
