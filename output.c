#include <stdio.h>
#include "common.h"
#include "output.h"

void printsituation(struct registers *REG)
{
	printf ("\nid = %d\n", REG->id);								// 线程ID. 
	printf ("ip = %d\n", REG->ip);                     				// 输出程序计数器的值 
	printf ("flag = %d\n", REG->flag);								// 输出标志寄存器的值 
	printf ("ir = %d\n", REG->ir);									// 输出指令寄存器的值 
	printf ("ax1 = %d ax2 = %d ", REG->ax[0], REG->ax[1]);          // 输出所有数据寄存器和地址寄存器的值 
	printf ("ax3 = %d ax4 = %d\n", REG->ax[2], REG->ax[3]);
	printf ("ax5 = %d ax6 = %d ", REG->ax[4], REG->ax[5]);
	printf ("ax7 = %d ax8 = %d\n", REG->ax[6], REG->ax[7]);
}

void print(struct memory *RAM)
{
	short i;
	printf ("\ncodeSegment :\n");
	for (i = 0; i < 128; i++)										// 输出代码段内存中 16 * 8个值 
	{
		if (i%8 == 7)												// 一行8个数 
			printf ("%d\n", RAM->codeRAM[i]);
		else
		    printf ("%d ", RAM->codeRAM[i]);
	}
	printf ("\ndataSegment :\n");
	for (i = 0; i < 256; i++)                                        // 输出数据段内存中 16 *16个值  
	{
		if (i % 16 == 15)												// 一行16个数 
			printf ("%d\n", RAM->dataRAM[i]);
		else
		    printf ("%d ", RAM->dataRAM[i]);
	}
}
