#include <windows.h>
#define MAX 32768					// 内存规模，最大的数组元素数

struct registers{	       		    // 用于模拟各寄存器的状态 
	short id;						// 线程 
	int ip;							// 模拟程序计数器 
	short flag;						// 模拟标志寄存器
	int ir;							// 模拟指令寄存器 
	short ax[8];				    // 模拟数据和地址寄存器  
};

struct memory{	       		        // 用于内存 
	int codeRAM[MAX];				// 模拟代码内存 
	short dataRAM[MAX];				// 模拟数据内存 
};

int binaryToInt(char *str);			// 将二进制字符串转为十进制数
HANDLE hMutex;						// 创建线程所用句柄 
int new_sign;						// 同步锁 
int exit_sign;
