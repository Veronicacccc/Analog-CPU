#include <string.h>
#include <math.h>
#include "common.h" 

int binaryToInt(char *str)
{
	short length;                                         // 字符串长度 
	short i = 0;										  // 字符串中的第i个字符 
	int num = 0;									      // 二进制数解析后的十进制数 
	length = strlen(str);
	while (i < length)
	{ 
		num = num + (str[i]-'0')*pow(2, length-i-1);
		i++;
	}
	return num;
}
