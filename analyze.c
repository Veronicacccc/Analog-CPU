#include <stdlib.h>  	 // abs()
#include <string.h>  	 // strlen()
#include <math.h>
#include "common.h"
#include "analyze.h"

void storeInArray(char *str, short *decimalOfStr)
{
	char ministr[17];                       // �����ַ�������ȡ�ַ�����ȡ��һ���� 
	short i;
	for (i = 0; i <= 7; i++)				// ��ȡ�ַ����е�0~7λ 
		ministr[i] = str[i];
	ministr[i] = '\0';                      // �ַ�����β��Ϊ�� 
	decimalOfStr[0] = (short)binaryToInt(ministr); // ���������ַ���תΪʮ������ 
	strcpy(ministr,"\0");                   // �����ַ�����Ϊ�� 
	for (i = 0; i <= 7; i++)                // ͬ�������������ַ����еĵ�8~15λ 
		ministr[i] = str[i+8];
	ministr[i] = '\0';
	decimalOfStr[1] = (short)binaryToInt(ministr);
	strcpy(ministr,"\0");
	if (str[16] == '0')                     // �ַ�����16~31λ����������Ϊ�����͸��� 
	{                                       // �ַ����ĵ�16λΪ'0'��������Ϊ������ֱ�ӽ��������ַ���תΪʮ������ 
		for (i = 0; i <= 15; i++)
			ministr[i] = str[i+16];
		ministr[i] = '\0';
		decimalOfStr[2] = (short)binaryToInt(ministr);
	}
	else                                    // �ַ����ĵ�16λΪ'1'��������Ϊ���������������ַ����е�17~31λȡ��������ʮ���������ټ�һ���ٳ� -1 
	{
		decimalOfStr[2] = 0;
		for(i = 17; i <= 31; i++){          // ����ȡ�ַ����е�17~31λ�ַ� 
			decimalOfStr[2] = decimalOfStr[2]*2 + abs(str[i]-'1');  //  �ַ� - '1'ȡ����ֵ��������ַ���ȡ�� 
		}
		decimalOfStr[2] = (decimalOfStr[2]+1) * (-1);
	}
}
