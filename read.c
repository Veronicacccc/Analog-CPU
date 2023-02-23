#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "read.h"

void read(char code[][33],int *codeRAM, int id)
{
	short i = 0;                              // ��ȡ�ļ��еĵ�i���ַ��� 
	char str[33] = {'\0'};                    // �����ַ������洢��ȡ��һ���ַ��� 
	FILE *fPtr;								  // ���ڶ�ȡ�ļ���ָ��
	if (id == 1)							  // ��ͬ���̴߳򿪲�ͬ���ļ� 
		fPtr = fopen("dict1.dic", "r");
	else if (id == 2)
		fPtr = fopen("dict2.dic", "r");
	if (fPtr != NULL){     					  // ���Գɹ����ļ� 
		if(!feof(fPtr)) { 
			fgets(str, 33, fPtr);                    // ��ȡ��һ���ַ���
			strcpy(code[i], str);                    // ���ΰ������ַ����洢��������
			if (id == 1) 							 // ��ָ������˲����������ڴ� 
				codeRAM[i] = binaryToInt(str);
			else if (id == 2)
				codeRAM[64 + i] = binaryToInt(str);
		}
		while (!feof(fPtr) && i <= MAX && codeRAM[i] != 0){     // ��δ�����ļ���������ɶ�ȡ��ָ�����δ�ﵽ���ֵʱ��һֱ��ȡָ��
		    if (!feof(fPtr))						 			// ��ȡ���з� 
				fgetc(fPtr);
			if (!feof(fPtr)) {                         			// ��ȡ��һ��ָ�� 
				i++;
				fgets(str, 33, fPtr);
				strcpy(code[i], str);                        	// ���ΰ������ַ����洢�������� 
				if (id == 1) 							        // ��ָ������˲����������ڴ� 
					codeRAM[i] = binaryToInt(str);
				else if (id == 2)
					codeRAM[64 + i] = binaryToInt(str); 
				strcpy(str, "\0");						  		// �����ַ�����Ϊ��
			}
		}
		fclose(fPtr);                                // ������ȡָ���ѭ�����ر��ļ� 
	}
	fPtr = NULL; 
	return ;
}
