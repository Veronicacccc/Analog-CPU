#include <string.h>
#include <math.h>
#include "common.h" 

int binaryToInt(char *str)
{
	short length;                                         // �ַ������� 
	short i = 0;										  // �ַ����еĵ�i���ַ� 
	int num = 0;									      // ���������������ʮ������ 
	length = strlen(str);
	while (i < length)
	{ 
		num = num + (str[i]-'0')*pow(2, length-i-1);
		i++;
	}
	return num;
}
