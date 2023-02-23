#include <stdlib.h>  	 // abs()
#include <string.h>  	 // strlen()
#include <math.h>
#include "common.h"
#include "analyze.h"

void storeInArray(char *str, short *decimalOfStr)
{
	char ministr[17];                       // 缓冲字符串，存取字符串截取的一部分 
	short i;
	for (i = 0; i <= 7; i++)				// 截取字符串中的0~7位 
		ministr[i] = str[i];
	ministr[i] = '\0';                      // 字符串结尾置为空 
	decimalOfStr[0] = (short)binaryToInt(ministr); // 将二进制字符串转为十进制数 
	strcpy(ministr,"\0");                   // 缓冲字符串置为空 
	for (i = 0; i <= 7; i++)                // 同理，接下来处理字符串中的第8~15位 
		ministr[i] = str[i+8];
	ministr[i] = '\0';
	decimalOfStr[1] = (short)binaryToInt(ministr);
	strcpy(ministr,"\0");
	if (str[16] == '0')                     // 字符串第16~31位的立即数分为正数和负数 
	{                                       // 字符串的第16位为'0'，立即数为正数，直接将二进制字符串转为十进制数 
		for (i = 0; i <= 15; i++)
			ministr[i] = str[i+16];
		ministr[i] = '\0';
		decimalOfStr[2] = (short)binaryToInt(ministr);
	}
	else                                    // 字符串的第16位为'1'，立即数为负数，将二进制字符串中的17~31位取反解析成十进制数，再加一，再乘 -1 
	{
		decimalOfStr[2] = 0;
		for(i = 17; i <= 31; i++){          // 依次取字符串中的17~31位字符 
			decimalOfStr[2] = decimalOfStr[2]*2 + abs(str[i]-'1');  //  字符 - '1'取绝对值，则完成字符的取反 
		}
		decimalOfStr[2] = (decimalOfStr[2]+1) * (-1);
	}
}
