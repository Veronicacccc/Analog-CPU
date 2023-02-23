#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <process.h>
#include "common.h"
#include "read.h"
#include "analyze.h"
#include "execute.h"
#include "output.h"

unsigned __stdcall Fun1Proc(struct memory* RAM); // �߳�1
unsigned __stdcall Fun2Proc(struct memory* RAM); // �߳�2

int main()
{   
	HANDLE hThread1, hThread2;
    hMutex = CreateMutex (NULL, FALSE, NULL);	 // ����������� 
    struct memory RAM;						 	 // ����ģ���ڴ� 
    RAM.dataRAM[0] = 100;						 // ��ַΪ16384���ڴ� ��ʼ��Ϊ100		
    new_sign = 0;								 // ͬ���� 
    exit_sign = 0;
    
    hThread1 = (HANDLE)_beginthreadex (NULL, 0, Fun1Proc, &RAM, 0, NULL);  // �����߳�1 
    hThread2 = (HANDLE)_beginthreadex (NULL, 0, Fun2Proc, &RAM, 0, NULL);  // �����߳�2 
    WaitForSingleObject (hThread1, INFINITE);// �����߳�1���� 
    CloseHandle (hThread1);
    WaitForSingleObject (hThread2, INFINITE);// �����߳�2���� 
    CloseHandle (hThread2);

	print(&RAM);					        // ����ڴ�״̬
    return 0;
}

unsigned __stdcall Fun1Proc(struct memory* RAM)
{
	short i = 0;							// ���ƶ�ȡ�ڴ��е���һ��ָ�� 
	short j;								// �������ָ�����ת 
	char str[33];							// �����ַ������洢һ����ȡ��ָ�� 
	char code[MAX][33];						// �洢���ļ��ж�����ɡ�0����1����ɵ��ַ���
	struct registers REG;					// ģ��Ĵ���
	short decimalOfStr[3];					// �洢һ��ָ��Ĳ����� �������� �������������ʮ������ 
	REG.id = 1;							    // ��ģ��Ĵ�����ʼ��
	REG.ip = 0;
	REG.flag = 0;
	REG.ir = 0;
	REG.ax[0] = 0;
	read(code, RAM->codeRAM, 1); 			// ��ȡ�ļ��е�����ָ��洢 
	strcpy(str, code[0]);                   // ��ȡ��һ��ָ��    
	while (RAM->codeRAM[i] != 0)			// ��û�ж�ȡ��ͣ��ָ��ʱ��һֱ��˳��ػ���ת�ض�ȡָ�� 
	{
		storeInArray(str, decimalOfStr);	// ���������ַ����еĲ����� �������� �����������ɵ�ʮ���������뵽������  
		j = execute(decimalOfStr, &REG, RAM);  // ִ��ָ�j���ڿ�����һ�ζ�ȡ��һ��ָ��
		exit_sign = 0;
		while (exit_sign != 1) {			// �ȴ��߳�1����� 
			if (!new_sign) {					 
				new_sign = 1;
				printsituation(&REG);       // �����ʱ��ģ��Ĵ�����״̬
				exit_sign = 1;
				new_sign = 0;
			}
		}
		if (j == 0)                         // ��jΪ0�����ָ�����תָ���һ��ָ�˳���ȡ 
			i++;
		else								// ��j��Ϊ0�����ָ������תָ���һ��ָ���ת����Ӧλ�ö�ȡ 
			i += j;
		strcpy(str, code[i]);				// ��ȡָ�� 
	}
	REG.ip += 4;							// ��ȡ����ͣ��ָ�����ѭ��������������ı䣬ָ��Ĵ���Ϊ0 
	REG.ir = 0;
	exit_sign = 0;
	while (exit_sign != 1) {				// �ȴ��߳�1����� 
		if (!new_sign) {					 
			new_sign = 1;
			printsituation(&REG);           // ���ͣ��ָ���ĸ��Ĵ���״̬
			exit_sign = 1;
			new_sign = 0;
		}
	}								
	_endthreadex(0);                        // �����߳� 
	return 0;
}

unsigned __stdcall Fun2Proc(struct memory* RAM)
{
	short i = 0;							// ���ƶ�ȡ�ڴ��е���һ��ָ�� 
	short j;								// �������ָ�����ת 
	char str[33];							// �����ַ������洢һ����ȡ��ָ�� 
	char code[MAX][33];						// �洢���ļ��ж�����ɡ�0����1����ɵ��ַ���
	struct registers REG;					// ģ��Ĵ��� 
	short decimalOfStr[3];					// �洢һ��ָ��Ĳ����� �������� �������������ʮ������ 
	REG.id = 2;							    // ��ģ��Ĵ�����ʼ��
	REG.ip = 256;
	REG.flag = 0;
	REG.ir = 0;
	REG.ax[0] = 0;
	read(code, RAM->codeRAM , 2); 			// ��ȡ�ļ��е�����ָ��洢 
	strcpy(str, code[0]);                   // ��ȡ��һ��ָ��    
	while (RAM->codeRAM[i] != 0)			// ��û�ж�ȡ��ͣ��ָ��ʱ��һֱ��˳��ػ���ת�ض�ȡָ�� 
	{
		storeInArray(str, decimalOfStr);	// ���������ַ����еĲ����� �������� �����������ɵ�ʮ���������뵽������ 
		j = execute(decimalOfStr, &REG, RAM);  // ִ��ָ�j���ڿ�����һ�ζ�ȡ��һ��ָ��
		exit_sign = 0;
		while (exit_sign != 2) {			// �ȴ��߳�2����� 
			if (!new_sign) {					 
				new_sign = 1;
				printsituation(&REG);       // �����ʱ��ģ��Ĵ�����״̬
				exit_sign = 2;
				new_sign = 0;
			}
		}
		if (j == 0)                         // ��jΪ0�����ָ�����תָ���һ��ָ�˳���ȡ 
			i++;
		else								// ��j��Ϊ0�����ָ������תָ���һ��ָ���ת����Ӧλ�ö�ȡ 
			i += j;
		strcpy(str, code[i]);				// ��ȡָ�� 
	}
	REG.ip += 4;							// ��ȡ����ͣ��ָ�����ѭ��������������ı䣬ָ��Ĵ���Ϊ0 
	REG.ir = 0;
	exit_sign = 0;					
	while (exit_sign != 2) {				// �ȴ��߳�2����� 
		if (!new_sign) {					 
			new_sign = 1;
			printsituation(&REG);           // ���ͣ��ָ���ĸ��Ĵ���״̬
			exit_sign = 2;
			new_sign = 0;
		}
	}
	_endthreadex(0);                        // �����߳�  
	return 0;
}
