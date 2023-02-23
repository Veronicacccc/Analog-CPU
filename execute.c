#include <windows.h>
#include <process.h>
#include <stdio.h>
#include "common.h"
#include "execute.h"

short transfer(short task, short num, struct registers* REG, struct memory* RAM);	// 执行传送指令
short sum(short task, short num, struct registers* REG, struct memory* RAM);		// 执行求和指令 
short subtract(short task, short num, struct registers* REG, struct memory* RAM);	// 执行求差指令 
short product(short task, short num, struct registers* REG, struct memory* RAM);	// 执行求积指令 
short division(short task, short num, struct registers* REG, struct memory* RAM);	// 执行求商指令 
short AND(short task, short num, struct registers* REG, struct memory* RAM);		// 执行逻辑与指令 
short OR(short task, short num, struct registers* REG, struct memory* RAM);			// 执行逻辑或指令 
short NOT(short task, struct registers* REG, struct memory* RAM);					// 执行逻辑非指令 
short compare(short task, short num, struct registers* REG, struct memory* RAM);	// 执行比较指令
short skip(short task, short num, struct registers* REG, struct memory* RAM);		// 执行跳转指令 
short input(short task, struct registers* REG, struct memory* RAM);					// 执行输入操作 
short output(short task, struct registers* REG, struct memory* RAM);				// 执行输出操作 
short createmutex(short task, struct registers* REG, struct memory* RAM);			// 请求互斥对象 
short releasemutex(short task, struct registers* REG, struct memory* RAM);			// 释放互斥对象 
short sleep(short task, short num, struct registers* REG, struct memory* RAM);		// 执行休眠操作


short execute(short *decimalOfStr, struct registers* REG, struct memory* RAM)
{
	switch(decimalOfStr[0])    // 根据操作码数值的不同执行不同指令 
	{
		case 1:      // 传送指令 
			return transfer(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 2:      // 求和指令 
			return sum(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 3:		 // 求差指令 
			return subtract(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 4:		 // 求积指令 
			product(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 5:		 // 求商指令 
			return division(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 6:		 // 逻辑与指令 
			return AND(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 7:		 // 逻辑或指令 
			return OR(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 8:		 // 逻辑非指令 
			return NOT(decimalOfStr[1], REG, RAM);
		case 9:		 // 比较指令 
		{
			REG->ip += 4;                           // 为了节省函数内的行数，把寄存器状态的改变写在这了 
			REG->ir = 2304 + decimalOfStr[1];
			return compare(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		}
		case 10:		 // 跳转指令 
			return skip(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 11:		 // 输入指令 
			return input(decimalOfStr[1], REG, RAM);
		case 12:		 // 输出指令 
			return output(decimalOfStr[1], REG, RAM);
		case 13:
			return createmutex(decimalOfStr[1], REG, RAM);
		case 14:
			return releasemutex(decimalOfStr[1], REG, RAM);
		case 15:
			return sleep(decimalOfStr[1], decimalOfStr[2], REG, RAM);
	}
}

short transfer(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // 程序计算器 +4 
	REG->ir = 256 + task;            // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数 
	if (task % 16 == 0)              // 操作对象只有数据寄存器
		REG->ax[task/16-1] = num;    // 直接把立即数存到数据寄存器中 
	else                             // 操作对象有两个 
	{
		if (task/16 <= 4 && task%16 <= 4)              // 操作对象为：数据寄存器 + 数据寄存器 
			REG->ax[task/16-1] = REG->ax[task%16-1];
		else if(task/16 <= 4 && task%16 >= 5)          // 操作对象为：数据寄存器 + 地址寄存器
			REG->ax[task/16-1] = RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
		else if (task/16 >= 5 && task%16 <= 4)	       // 操作对象为：地址寄存器 + 数据寄存器
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = REG->ax[task%16-1];
		else if (task/16 >= 5 && task%16 >= 5)         // 操作对象为：地址寄存器 + 地址寄存器
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
	}
	return 0;
}

short sum(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // 程序计算器 +4  
	REG->ir = 512 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	if (task % 16 == 0)              // 操作对象只有一个 
	{
		if (task/16 <= 4)            // 操作对象是数据寄存器 
			REG->ax[task/16-1] += num;
		else                         // 操作对象是地址寄存器 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] += num;
	}
	else                             // 操作对象有两个                            
	{
		if (task/16 <= 4 && task%16 <= 4)              // 操作对象为：数据寄存器 + 数据寄存器 
			REG->ax[task/16-1] += REG->ax[task%16-1];
		else if(task/16 <= 4 && task%16 >= 5)          // 操作对象为：数据寄存器 + 地址寄存器
			REG->ax[task/16-1] += RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
		else if (task/16 >= 5 && task%16 <= 4)	       // 操作对象为：地址寄存器 + 数据寄存器
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] += REG->ax[task%16-1];
		else if (task/16 >= 5 && task%16 >= 5)         // 操作对象为：地址寄存器 + 地址寄存器
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] += RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
	}
	return 0;
}

short subtract(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // 程序计算器 +4
	REG->ir = 768 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	if (task % 16 == 0)			     // 操作对象只有一个
	{
		if (task/16 <= 4)			 // 操作对象是数据寄存器 
			REG->ax[task/16-1] -= num;
		else						 // 操作对象是地址寄存器 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] -= num;
	}
	else                             // 操作对象有两个  
	{
		if (task/16 <= 4 && task%16 <= 4)              // 操作对象为：数据寄存器 + 数据寄存器 
			REG->ax[task/16-1] -= REG->ax[task%16-1];
		else if(task/16 <= 4 && task%16 >= 5)          // 操作对象为：数据寄存器 + 地址寄存器 
			REG->ax[task/16-1] -= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
		else if (task/16 >= 5 && task%16 <= 4)         // 操作对象为：地址寄存器 + 数据寄存器 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] -= REG->ax[task%16-1];
		else if (task/16 >= 5 && task%16 >= 5)         // 操作对象为：地址寄存器 + 地址寄存器 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] -= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
	}
	return 0;
}

short product(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // 程序计算器 +4
	REG->ir = 1024 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	if (task % 16 == 0)			     // 操作对象只有一个
	{
		if (task/16 <= 4)			 // 操作对象是数据寄存器 
			REG->ax[task/16-1] *= num;
		else						 // 操作对象是地址寄存器 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] *= num;
	}
	else                             // 操作对象有两个  
	{
		if (task/16 <= 4 && task%16 <= 4)              // 操作对象为：数据寄存器 + 数据寄存器 
			REG->ax[task/16-1] *= REG->ax[task%16-1];
		else if(task/16 <= 4 && task%16 >= 5)          // 操作对象为：数据寄存器 + 地址寄存器 
			REG->ax[task/16-1] *= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
		else if (task/16 >= 5 && task%16 <= 4)         // 操作对象为：地址寄存器 + 数据寄存器 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] *= REG->ax[task%16-1];
		else if (task/16 >= 5 && task%16 >= 5)         // 操作对象为：地址寄存器 + 地址寄存器 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] *= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
	}
	return 0;
}

short division(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // 程序计算器 +4
	REG->ir = 1280 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	if (task % 16 == 0)			     // 操作对象只有一个
	{
		if (task/16 <= 4)			 // 操作对象是数据寄存器 
			REG->ax[task/16-1] /= num;
		else						 // 操作对象是地址寄存器 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] /= num;
	}
	else                             // 操作对象有两个  
	{
		if (task/16 <= 4 && task%16 <= 4)              // 操作对象为：数据寄存器 + 数据寄存器 
			REG->ax[task/16-1] /= REG->ax[task%16-1];
		else if(task/16 <= 4 && task%16 >= 5)          // 操作对象为：数据寄存器 + 地址寄存器 
			REG->ax[task/16-1] /= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
		else if (task/16 >= 5 && task%16 <= 4)         // 操作对象为：地址寄存器 + 数据寄存器 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] /= REG->ax[task%16-1];
		else if (task/16 >= 5 && task%16 >= 5)         // 操作对象为：地址寄存器 + 地址寄存器 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] /= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
	}
	return 0;
}

short AND(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // 程序计算器 +4
	REG->ir = 1536 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	if (task % 16 == 0)			     // 操作对象只有一个			
	{
		if (task/16 <= 4)
			REG->ax[task/16-1] = (num && REG->ax[task/16-1]);
		else
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = (num && RAM->dataRAM[(REG->ax[task/16-1]-16384)/2]);
	}
	else                             // 操作对象有两个 
	{
		if (task/16 <= 4 && task%16 <= 4)
			REG->ax[task/16-1] = (REG->ax[task/16-1] && REG->ax[task%16-1]);
		else if(task/16 <= 4 && task%16 >= 5)
			REG->ax[task/16-1] = (REG->ax[task/16-1] && RAM->dataRAM[(REG->ax[task%16-1]-16384)/2]);
		else if (task/16 >= 5 && task%16 <= 4)
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] && REG->ax[task%16-1]);
		else if (task/16 >= 5 && task%16 >= 5)
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] && RAM->dataRAM[(REG->ax[task%16-1]-16384)/2]);
	}
	return 0;
}

short OR(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // 程序计算器 +4
	REG->ir = 1792 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	if (task % 16 == 0)			     // 操作对象只有一个
	{
		if (task/16 <= 4)
			REG->ax[task/16-1] = (num || REG->ax[task/16-1]);
		else
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = (num || RAM->dataRAM[(REG->ax[task/16-1]-16384)/2]);
	}
	else                             // 操作对象有两个 
	{
		if (task/16 <= 4 && task%16 <= 4)
			REG->ax[task/16-1] = (REG->ax[task/16-1] || REG->ax[task%16-1]);
		else if(task/16 <= 4 && task%16 >= 5)
			REG->ax[task/16-1] = (REG->ax[task/16-1] || RAM->dataRAM[(REG->ax[task%16-1]-16384)/2]);
		else if (task/16 >= 5 && task%16 <= 4)
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] || REG->ax[task%16-1]);
		else if (task/16 >= 5 && task%16 >= 5)
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] || RAM->dataRAM[(REG->ax[task%16-1]-16384)/2]);
	}
	return 0;
}

short NOT(short task, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // 程序计算器 +4
	REG->ir = 2048 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	if (task % 16 == 0)			     // 操作对象数据寄存器 
		REG->ax[task/16-1] = !(REG->ax[task/16-1]);
	else                             // 操作对象为地址寄存器 
		RAM->dataRAM[(REG->ax[task%16-1]-16384)/2] = !(RAM->dataRAM[(REG->ax[task%16-1]-16384)/2]);
	return 0;
}

short compare(short task, short num, struct registers* REG, struct memory* RAM)
{
	if (task % 16 == 0 && task/16 <= 4) {			// 操作对象只是数据寄存器 
		if (REG->ax[task/16-1] > num)               // 数据寄存器中的数与立即数比较，结果存在标志寄存器中 
			REG->flag = 1;
		else if (REG->ax[task/16-1] < num)
			REG->flag = -1;
		else if (REG->ax[task/16-1] == num)
			REG->flag = 0;
	}
	else if (task % 16 == 0 && task/16 >= 5) {						// 操作对象只是地址寄存器 
		if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] > num)       // 地址寄存器所指内存（dataRAM）地址中所存储的数与立即数比较，结果存在标志寄存器中
			REG->flag = 1;
		else if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] < num)
			REG->flag = -1;
		else if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] == num)
			REG->flag = 0;
	}
	else if (task % 16 != 0 && task/16 <= 4 && task%16 <= 4) {      // 操作对象为：数据寄存器 + 数据寄存器 
		if (REG->ax[task/16-1] > REG->ax[task%16-1])				// 比较后，结果存在标志寄存器中 
			REG->flag = 1;
		else if (REG->ax[task/16-1] < REG->ax[task%16-1])
			REG->flag = -1;
		else if (REG->ax[task/16-1] == REG->ax[task%16-1])
			REG->flag = 0;		
	}
	else if(task % 16 != 0 && task/16 <= 4 && task%16 >= 5) {      				// 操作对象为：数据寄存器 + 地址寄存器 
		if (REG->ax[task/16-1] > RAM->dataRAM[(REG->ax[task%16-1]-16384)/2])	// 比较后，结果存在标志寄存器中
			REG->flag = 1;
		else if (REG->ax[task/16-1] < RAM->dataRAM[(REG->ax[task%16-1]-16384)/2])
			REG->flag = -1;
		else if (REG->ax[task/16-1] == RAM->dataRAM[(REG->ax[task%16-1]-16384)/2])
			REG->flag = 0;	
	}
	else if (task % 16 != 0 && task/16 >= 5 && task%16 <= 4) {     			    // 操作对象为：地址寄存器 + 数据寄存器 
		if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] > REG->ax[task%16-1])	// 比较后，结果存在标志寄存器中
			REG->flag = 1;
		else if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] < REG->ax[task%16-1])
			REG->flag = -1;
		else if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] == REG->ax[task%16-1])
			REG->flag = 0;
	}
	else if (task % 16 != 0 && task/16 >= 5 && task%16 >= 5) {     										// 操作对象为：地址寄存器 + 地址寄存器 
		if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] > RAM->dataRAM[(REG->ax[task%16-1]-16384)/2])	// 比较后，结果存在标志寄存器中
			REG->flag = 1;
		else if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] < RAM->dataRAM[(REG->ax[task%16-1]-16384)/2])
			REG->flag = -1;
		else if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] == RAM->dataRAM[(REG->ax[task%16-1]-16384)/2])
			REG->flag = 0;
	}
	return 0;
}

short skip(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ir =2560 + task;		// 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	if(task == 0)				// 无条件跳转 
	{
		REG->ip += num; 		// 程序计算器 +num 
		return num/4;			// 跳转了num/4条指令（告诉主函数）
	}
	else if (task == 1 && REG->flag == 0)   // flag = 0时跳转 
	{
		REG->ip += num;                     // 程序计算器 +num
		return num/4;						// 跳转了num/4条指令 （告诉主函数）
	}
	else if (task == 2 && REG->flag == 1)   // flag = 1时跳转 
	{
		REG->ip += num;                     // 程序计算器 +num
		return num/4;						// 跳转了num/4条指令 （告诉主函数）
	}
	else if (task == 3 && REG->flag == -1)  // flag = -1时跳转 
	{
		REG->ip += num;                     // 程序计算器 +num
		return num/4;						// 跳转了num/4条指令 （告诉主函数）
	}
	else        				 // 不满足跳转指令条件，不跳转 
	{
		REG->ip += 4;			 // 程序计算器 +4
		return 0;				 // 跳转了 0 条指令 （告诉主函数）
    }
}

short input(short task, struct registers* REG, struct memory* RAM)
{
	short num;
	printf ("in:\n");
	scanf ("%hd", &num);
	if (task/16 <= 4)			     // 操作对象为数据寄存器 
		REG->ax[task/16-1] = num;
	else			                 // 操作对象为地址寄存器 
		RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = num;
	REG->ip += 4;                    // 程序计算器 +4
	REG->ir = 2816 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	return 0;
}

short output(short task, struct registers* REG, struct memory* RAM)
{
	exit_sign = 0; 
	while (exit_sign != 3)					 // 等待输出 
	{
		if (!new_sign)
		{					 
			new_sign = 1;
			printf ("id = %d    out: ", REG->id );
			if (task/16 <= 4)			     // 操作对象为数据寄存器 
				printf ("%d", REG->ax[task/16-1]);
			else			                 // 操作对象为地址寄存器 
				printf ("%d", RAM->dataRAM[(REG->ax[task/16-1]-16384)/2]);
			printf ("\n");
			exit_sign = 3;
			new_sign = 0;
		}
	}
	REG->ip += 4;                    // 程序计算器 +4
	REG->ir = 3072 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	return 0;
}

short createmutex(short task, struct registers* REG, struct memory* RAM)
{
	WaitForSingleObject (hMutex, INFINITE);		// 请求互斥对象 
	REG->ip += 4;                    			// 程序计算器 +4
	REG->ir = 3328 + task;			 			// 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	return 0; 
}

short releasemutex(short task, struct registers* REG, struct memory* RAM)
{
	ReleaseMutex (hMutex);			 // 释放互斥对象 
	REG->ip += 4;                    // 程序计算器 +4
	REG->ir = 3584 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	return 0;
}

short sleep(short task, short num, struct registers* REG, struct memory* RAM)
{
	Sleep(num); 					 // 线程休眠
	REG->ip += 4;                    // 程序计算器 +4
	REG->ir = 3840 + task;			 // 指令寄存器：操作码对应的十进制数 * 2^8 + 操作对象对应的十进制数
	return 0;
}
