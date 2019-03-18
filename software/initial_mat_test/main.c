#include "system.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include <stdint.h>
#include "sys/time.h"

/*--------- Macros/constants -------*/

#define IMG_H (600)
#define IMG_W (800)

#define DEBUG_OUTPUT_F alt_printf
#define DEBUG_OUTPUT alt_putstr

#define DEBUG(msg) 						\
	DEBUG_OUTPUT(msg);					\
	DEBUG_OUTPUT("\n")

#define DEBUGF(msg,...)				\
	DEBUG_OUTPUT_F(msg, __VA_ARGS__ );	\
	DEBUG_OUTPUT("\n")

#define PRINT_NUM(str,number)  		\
	DEBUG_OUTPUT("started at ");  	\
	print_num(start);              	\
	DEBUG_OUTPUT("\n")

#define THRESHOLD (127)

/*------- Function pre-declaration --------*/

void fill_image(uint16_t, uint16_t, uint16_t);
void print_num(uint32_t);

/*------- Global Variables --------*/

/*OBS: the image is black and white with 1 byte of color depht*/

clock_t start=0,now=0;

/*--------- Main ---------*/

int main(void)
{
	DEBUG("hello there");
	print_num(123456);
#if 0
	while(1)
	{
		start =clock();

		fill_image(SDRAM_CONTROLLER_BASE,IMG_H,IMG_W);
		now=clock;
		PRINT_NUM("took :",now-start);

	}
	return 0;
#endif
}


/*---- Function Definition-----------------*/
/*-----------------------------------------*/

/**
 * @brief fill image memory range
 *
 */
void fill_image(uint16_t image_base, uint16_t image_height, uint16_t image_width)
{
	/*fill image with white to gray gradient*/
	uint8_t i,j,v;
	for(i=0;i<image_height;i++) /*write image in "lines" for better memory performance*/
	{
		for(j=0;j<image_width;j++)
		{
			v = j*255/800;
			IOWR_8DIRECT(image_base,j+i*image_height,v); /*checar depois tamanho ponteiro (e internamente castado pra alt_u8)*/
			DEBUGF("wrote %x at %x", v, j);
		}
	}
}

/**
 * @brief print a number (because of memory constraints the limited print only works for hex and strings, no decimals
 *
 */
void print_num(uint32_t num)
{
	char buff[15];
	char * str = buff;
	while(num>0)
	{
		*(str)='0'+(num%10); /*set the digit as the remainder of dividing by ten (because '0' + 1 = '1')*/
		++str;/*advance the pointer by one*/
		num -= num%10;
		num /= 10; /*shift the number one *decimal* place*/
	}
	*str='\0'; /*null termination*/
	DEBUG(buff);
}
