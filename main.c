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

unsigned __stdcall Fun1Proc(struct memory* RAM); // 线程1
unsigned __stdcall Fun2Proc(struct memory* RAM); // 线程2

int main()
{   
	HANDLE hThread1, hThread2;
    hMutex = CreateMutex (NULL, FALSE, NULL);	 // 创建互斥对象 
    struct memory RAM;						 	 // 创建模拟内存 
    RAM.dataRAM[0] = 100;						 // 地址为16384的内存 初始化为100		
    new_sign = 0;								 // 同步锁 
    exit_sign = 0;
    
    hThread1 = (HANDLE)_beginthreadex (NULL, 0, Fun1Proc, &RAM, 0, NULL);  // 创建线程1 
    hThread2 = (HANDLE)_beginthreadex (NULL, 0, Fun2Proc, &RAM, 0, NULL);  // 创建线程2 
    WaitForSingleObject (hThread1, INFINITE);// 等子线程1结束 
    CloseHandle (hThread1);
    WaitForSingleObject (hThread2, INFINITE);// 等子线程2结束 
    CloseHandle (hThread2);

	print(&RAM);					        // 输出内存状态
    return 0;
}

unsigned __stdcall Fun1Proc(struct memory* RAM)
{
	short i = 0;							// 控制读取内存中的哪一条指令 
	short j;								// 用于完成指令的跳转 
	char str[33];							// 缓冲字符串，存储一条读取的指令 
	char code[MAX][33];						// 存储从文件中读入的由“0”“1”组成的字符串
	struct registers REG;					// 模拟寄存器
	short decimalOfStr[3];					// 存储一条指令的操作码 操作对象 立即数解析后的十进制数 
	REG.id = 1;							    // 各模拟寄存器初始化
	REG.ip = 0;
	REG.flag = 0;
	REG.ir = 0;
	REG.ax[0] = 0;
	read(code, RAM->codeRAM, 1); 			// 读取文件中的所有指令并存储 
	strcpy(str, code[0]);                   // 读取第一条指令    
	while (RAM->codeRAM[i] != 0)			// 当没有读取到停机指令时，一直按顺序地或跳转地读取指令 
	{
		storeInArray(str, decimalOfStr);	// 将二进制字符串中的操作码 操作对象 立即数解析成的十进制数存入到数组中  
		j = execute(decimalOfStr, &REG, RAM);  // 执行指令，j用于控制下一次读取哪一条指令
		exit_sign = 0;
		while (exit_sign != 1) {			// 等待线程1的输出 
			if (!new_sign) {					 
				new_sign = 1;
				printsituation(&REG);       // 输出此时各模拟寄存器的状态
				exit_sign = 1;
				new_sign = 0;
			}
		}
		if (j == 0)                         // 若j为0，则此指令不是跳转指令，下一条指令按顺序读取 
			i++;
		else								// 若j不为0，则此指令是跳转指令，下一条指令将跳转到相应位置读取 
			i += j;
		strcpy(str, code[i]);				// 读取指令 
	}
	REG.ip += 4;							// 读取到了停机指令，结束循环，程序计数器改变，指令寄存器为0 
	REG.ir = 0;
	exit_sign = 0;
	while (exit_sign != 1) {				// 等待线程1的输出 
		if (!new_sign) {					 
			new_sign = 1;
			printsituation(&REG);           // 输出停机指令后的各寄存器状态
			exit_sign = 1;
			new_sign = 0;
		}
	}								
	_endthreadex(0);                        // 结束线程 
	return 0;
}

unsigned __stdcall Fun2Proc(struct memory* RAM)
{
	short i = 0;							// 控制读取内存中的哪一条指令 
	short j;								// 用于完成指令的跳转 
	char str[33];							// 缓冲字符串，存储一条读取的指令 
	char code[MAX][33];						// 存储从文件中读入的由“0”“1”组成的字符串
	struct registers REG;					// 模拟寄存器 
	short decimalOfStr[3];					// 存储一条指令的操作码 操作对象 立即数解析后的十进制数 
	REG.id = 2;							    // 各模拟寄存器初始化
	REG.ip = 256;
	REG.flag = 0;
	REG.ir = 0;
	REG.ax[0] = 0;
	read(code, RAM->codeRAM , 2); 			// 读取文件中的所有指令并存储 
	strcpy(str, code[0]);                   // 读取第一条指令    
	while (RAM->codeRAM[i] != 0)			// 当没有读取到停机指令时，一直按顺序地或跳转地读取指令 
	{
		storeInArray(str, decimalOfStr);	// 将二进制字符串中的操作码 操作对象 立即数解析成的十进制数存入到数组中 
		j = execute(decimalOfStr, &REG, RAM);  // 执行指令，j用于控制下一次读取哪一条指令
		exit_sign = 0;
		while (exit_sign != 2) {			// 等待线程2的输出 
			if (!new_sign) {					 
				new_sign = 1;
				printsituation(&REG);       // 输出此时各模拟寄存器的状态
				exit_sign = 2;
				new_sign = 0;
			}
		}
		if (j == 0)                         // 若j为0，则此指令不是跳转指令，下一条指令按顺序读取 
			i++;
		else								// 若j不为0，则此指令是跳转指令，下一条指令将跳转到相应位置读取 
			i += j;
		strcpy(str, code[i]);				// 读取指令 
	}
	REG.ip += 4;							// 读取到了停机指令，结束循环，程序计数器改变，指令寄存器为0 
	REG.ir = 0;
	exit_sign = 0;					
	while (exit_sign != 2) {				// 等待线程2的输出 
		if (!new_sign) {					 
			new_sign = 1;
			printsituation(&REG);           // 输出停机指令后的各寄存器状态
			exit_sign = 2;
			new_sign = 0;
		}
	}
	_endthreadex(0);                        // 结束线程  
	return 0;
}
