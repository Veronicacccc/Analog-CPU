#include <stdio.h>
#include "common.h"
#include "output.h"

void printsituation(struct registers *REG)
{
	printf ("\nid = %d\n", REG->id);								// �߳�ID. 
	printf ("ip = %d\n", REG->ip);                     				// ��������������ֵ 
	printf ("flag = %d\n", REG->flag);								// �����־�Ĵ�����ֵ 
	printf ("ir = %d\n", REG->ir);									// ���ָ��Ĵ�����ֵ 
	printf ("ax1 = %d ax2 = %d ", REG->ax[0], REG->ax[1]);          // ����������ݼĴ����͵�ַ�Ĵ�����ֵ 
	printf ("ax3 = %d ax4 = %d\n", REG->ax[2], REG->ax[3]);
	printf ("ax5 = %d ax6 = %d ", REG->ax[4], REG->ax[5]);
	printf ("ax7 = %d ax8 = %d\n", REG->ax[6], REG->ax[7]);
}

void print(struct memory *RAM)
{
	short i;
	printf ("\ncodeSegment :\n");
	for (i = 0; i < 128; i++)										// ���������ڴ��� 16 * 8��ֵ 
	{
		if (i%8 == 7)												// һ��8���� 
			printf ("%d\n", RAM->codeRAM[i]);
		else
		    printf ("%d ", RAM->codeRAM[i]);
	}
	printf ("\ndataSegment :\n");
	for (i = 0; i < 256; i++)                                        // ������ݶ��ڴ��� 16 *16��ֵ  
	{
		if (i % 16 == 15)												// һ��16���� 
			printf ("%d\n", RAM->dataRAM[i]);
		else
		    printf ("%d ", RAM->dataRAM[i]);
	}
}
