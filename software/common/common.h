#ifndef COMMON_H
#define COMMON_H

/* @file: common.h
 * @author: Lucas Martins Mendes
 * Functions common to all processors.
  *
  */

/*---------- Type Declaration ---------*/
enum err_codes {
	ERR_OK =0,
	ERR_MEM
};
typedef enum err_codes err_t;
/*---------- Function declaration ----------*/
void print_num(uint32_t);
void delay_ms(uint32_t);

/*----------- Function Definition ----------*/

/**
 * @brief print a number as a decimal(because of memory constraints the limited printf only works for hex and strings, no decimals).
 *
 */
void print_num(uint32_t num)
{
	char buff[15],tmp;
	char * str = buff;
	uint8_t dp=0; /*number of decimal places*/
	if(num == 0)
	{
		*str ='0';
		++str;
	}
	else
	{
		while(num>0)
		{
			*(str)='0'+(num%10); /*set the digit as the remainder of dividing by ten (because '0' + 1 = '1')*/
			++str;/*advance the pointer by one*/
			num -= num%10; /*remove the last digit*/
			num /= 10; /*shift the number one *decimal* place*/
		}
		dp=str-buff;
		/*this is needed otherwise the number would be written in reverse*/
		for(uint8_t i = 0;i<(dp>>1/*dp/2*/);++i)
		{
			tmp = buff[i];
			buff[i] = buff[dp-i-1];
			buff[dp-i-1] = tmp;
		}
	}
	*str='\0'; /*null termination*/
	DEBUG_OUTPUT(buff);
}

/*
 * delay for x msec
 */
void delay_ms(uint32_t delay)
{
	uint32_t i,j;
	for(i=0;i<delay;i++)
	{
		for(j=0;j<1000;j++);
	}

}

#endif
