#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "read.h"

void read(char code[][33],int *codeRAM, int id)
{
	short i = 0;                              // 读取文件中的第i行字符串 
	char str[33] = {'\0'};                    // 缓冲字符串，存储读取的一条字符串 
	FILE *fPtr;								  // 用于读取文件的指针
	if (id == 1)							  // 不同的线程打开不同的文件 
		fPtr = fopen("dict1.dic", "r");
	else if (id == 2)
		fPtr = fopen("dict2.dic", "r");
	if (fPtr != NULL){     					  // 可以成功打开文件 
		if(!feof(fPtr)) { 
			fgets(str, 33, fPtr);                    // 读取第一行字符串
			strcpy(code[i], str);                    // 依次把所有字符串存储到数组中
			if (id == 1) 							 // 把指令解析了并存入代码段内存 
				codeRAM[i] = binaryToInt(str);
			else if (id == 2)
				codeRAM[64 + i] = binaryToInt(str);
		}
		while (!feof(fPtr) && i <= MAX && codeRAM[i] != 0){     // 当未碰到文件结束符或可读取的指令个数未达到最大值时，一直读取指令
		    if (!feof(fPtr))						 			// 读取换行符 
				fgetc(fPtr);
			if (!feof(fPtr)) {                         			// 读取下一条指令 
				i++;
				fgets(str, 33, fPtr);
				strcpy(code[i], str);                        	// 依次把所有字符串存储到数组中 
				if (id == 1) 							        // 把指令解析了并存入代码段内存 
					codeRAM[i] = binaryToInt(str);
				else if (id == 2)
					codeRAM[64 + i] = binaryToInt(str); 
				strcpy(str, "\0");						  		// 缓冲字符串置为空
			}
		}
		fclose(fPtr);                                // 结束读取指令的循环，关闭文件 
	}
	fPtr = NULL; 
	return ;
}
