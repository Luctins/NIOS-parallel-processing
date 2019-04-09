#include "system.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include <stdint.h>
#include "sys/time.h"
#include "debug.h"
#include "common.h"

/*--------- Macros/constants -------*/
#define IMG_WRITE_BSIZE 1/*block size*/
#define IMG_FILL_GRADIENT 1

#define IMG_W (800)
#define IMG_H (600)

/*------- Function pre-declaration --------*/

void fill_image(uint8_t *, uint32_t, uint32_t); //__attribute__((always_inline));
void print_num(uint32_t);

/*------- Global Variables --------*/

/*OBS: the image is black and white with 1 byte of color depht*/

clock_t start=0,now=0;

/*--------- Main ---------*/
int a = 127;

int main(void)
{
	DEBUG("hello there");
	print_num(123456789);
	while(1)
	{
    a = a >> 2;
		start = clock();
		fill_image((alt_u8 *) SDRAM_CONTROLLER_BASE,IMG_H,IMG_W);
		now=clock();
		DEBUG_NUM("now-start:",now-start);
	}
	return 0;
}


/*---- Function Definition-----------------*/
/*-----------------------------------------*/

/*
 * @brief fill image memory range
 *
 */
void fill_image(uint8_t * image_base, uint32_t image_height, uint32_t image_width)
{
	/*fill image with white to gray gradient*/
	uint32_t i,j;

	for(i=0;i<image_height;++i) /*access the image as a contiguous region of memory */
    {
    for(j=0;j<image_width;j+=IMG_WRITE_BSIZE)
      {
#if IMG_FILL_GRADIENT == 1 /*use a black to white gradient*/
#if IMG_WRITE_BSIZE == 1

			uint8_t v;
		 v = ((j*255)/image_width);
			IOWR_8DIRECT(image_base,j+i*image_width,v);
#elif IMG_WRITE_BSIZE == 2
			uint16_t v;
			v =		((j*255)/image_width) |((((j+1)*255)/image_width) << 8);
			IOWR_16DIRECT(image_base,(j+i*image_width)>>1,v);
#elif IMG_WRITE_BSIZE == 4
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
#if IMG_WRITE_BSIZE == 1
			IOWR_8DIRECT(image_base,i,0x7f);
#elif IMG_WRITE_BSIZE == 2
			IOWR_16DIRECT(image_base,i>>1 /* x/2 == x >> 1*/,0x7f7f);
#elif IMG_WRITE_BSIZE == 4
			IOWR_32DIRECT(image_base,i>>2 /* x/4 == x >> 2*/,0x7f7f7f7f);
#else
#error "please define a valid block size in bytes (1,2 or 4) "
#endif
#endif
		}
	}
}
