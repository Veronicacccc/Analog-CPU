#include <windows.h>
#include <process.h>
#include <stdio.h>
#include "common.h"
#include "execute.h"

short transfer(short task, short num, struct registers* REG, struct memory* RAM);	// ִ�д���ָ��
short sum(short task, short num, struct registers* REG, struct memory* RAM);		// ִ�����ָ�� 
short subtract(short task, short num, struct registers* REG, struct memory* RAM);	// ִ�����ָ�� 
short product(short task, short num, struct registers* REG, struct memory* RAM);	// ִ�����ָ�� 
short division(short task, short num, struct registers* REG, struct memory* RAM);	// ִ������ָ�� 
short AND(short task, short num, struct registers* REG, struct memory* RAM);		// ִ���߼���ָ�� 
short OR(short task, short num, struct registers* REG, struct memory* RAM);			// ִ���߼���ָ�� 
short NOT(short task, struct registers* REG, struct memory* RAM);					// ִ���߼���ָ�� 
short compare(short task, short num, struct registers* REG, struct memory* RAM);	// ִ�бȽ�ָ��
short skip(short task, short num, struct registers* REG, struct memory* RAM);		// ִ����תָ�� 
short input(short task, struct registers* REG, struct memory* RAM);					// ִ��������� 
short output(short task, struct registers* REG, struct memory* RAM);				// ִ��������� 
short createmutex(short task, struct registers* REG, struct memory* RAM);			// ���󻥳���� 
short releasemutex(short task, struct registers* REG, struct memory* RAM);			// �ͷŻ������ 
short sleep(short task, short num, struct registers* REG, struct memory* RAM);		// ִ�����߲���


short execute(short *decimalOfStr, struct registers* REG, struct memory* RAM)
{
	switch(decimalOfStr[0])    // ���ݲ�������ֵ�Ĳ�ִͬ�в�ָͬ�� 
	{
		case 1:      // ����ָ�� 
			return transfer(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 2:      // ���ָ�� 
			return sum(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 3:		 // ���ָ�� 
			return subtract(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 4:		 // ���ָ�� 
			product(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 5:		 // ����ָ�� 
			return division(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 6:		 // �߼���ָ�� 
			return AND(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 7:		 // �߼���ָ�� 
			return OR(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 8:		 // �߼���ָ�� 
			return NOT(decimalOfStr[1], REG, RAM);
		case 9:		 // �Ƚ�ָ�� 
		{
			REG->ip += 4;                           // Ϊ�˽�ʡ�����ڵ��������ѼĴ���״̬�ĸı�д������ 
			REG->ir = 2304 + decimalOfStr[1];
			return compare(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		}
		case 10:		 // ��תָ�� 
			return skip(decimalOfStr[1], decimalOfStr[2], REG, RAM);
		case 11:		 // ����ָ�� 
			return input(decimalOfStr[1], REG, RAM);
		case 12:		 // ���ָ�� 
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
	REG->ip += 4;                    // ��������� +4 
	REG->ir = 256 + task;            // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������ 
	if (task % 16 == 0)              // ��������ֻ�����ݼĴ���
		REG->ax[task/16-1] = num;    // ֱ�Ӱ��������浽���ݼĴ����� 
	else                             // �������������� 
	{
		if (task/16 <= 4 && task%16 <= 4)              // ��������Ϊ�����ݼĴ��� + ���ݼĴ��� 
			REG->ax[task/16-1] = REG->ax[task%16-1];
		else if(task/16 <= 4 && task%16 >= 5)          // ��������Ϊ�����ݼĴ��� + ��ַ�Ĵ���
			REG->ax[task/16-1] = RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
		else if (task/16 >= 5 && task%16 <= 4)	       // ��������Ϊ����ַ�Ĵ��� + ���ݼĴ���
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = REG->ax[task%16-1];
		else if (task/16 >= 5 && task%16 >= 5)         // ��������Ϊ����ַ�Ĵ��� + ��ַ�Ĵ���
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
	}
	return 0;
}

short sum(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // ��������� +4  
	REG->ir = 512 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	if (task % 16 == 0)              // ��������ֻ��һ�� 
	{
		if (task/16 <= 4)            // �������������ݼĴ��� 
			REG->ax[task/16-1] += num;
		else                         // ���������ǵ�ַ�Ĵ��� 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] += num;
	}
	else                             // ��������������                            
	{
		if (task/16 <= 4 && task%16 <= 4)              // ��������Ϊ�����ݼĴ��� + ���ݼĴ��� 
			REG->ax[task/16-1] += REG->ax[task%16-1];
		else if(task/16 <= 4 && task%16 >= 5)          // ��������Ϊ�����ݼĴ��� + ��ַ�Ĵ���
			REG->ax[task/16-1] += RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
		else if (task/16 >= 5 && task%16 <= 4)	       // ��������Ϊ����ַ�Ĵ��� + ���ݼĴ���
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] += REG->ax[task%16-1];
		else if (task/16 >= 5 && task%16 >= 5)         // ��������Ϊ����ַ�Ĵ��� + ��ַ�Ĵ���
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] += RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
	}
	return 0;
}

short subtract(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // ��������� +4
	REG->ir = 768 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	if (task % 16 == 0)			     // ��������ֻ��һ��
	{
		if (task/16 <= 4)			 // �������������ݼĴ��� 
			REG->ax[task/16-1] -= num;
		else						 // ���������ǵ�ַ�Ĵ��� 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] -= num;
	}
	else                             // ��������������  
	{
		if (task/16 <= 4 && task%16 <= 4)              // ��������Ϊ�����ݼĴ��� + ���ݼĴ��� 
			REG->ax[task/16-1] -= REG->ax[task%16-1];
		else if(task/16 <= 4 && task%16 >= 5)          // ��������Ϊ�����ݼĴ��� + ��ַ�Ĵ��� 
			REG->ax[task/16-1] -= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
		else if (task/16 >= 5 && task%16 <= 4)         // ��������Ϊ����ַ�Ĵ��� + ���ݼĴ��� 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] -= REG->ax[task%16-1];
		else if (task/16 >= 5 && task%16 >= 5)         // ��������Ϊ����ַ�Ĵ��� + ��ַ�Ĵ��� 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] -= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
	}
	return 0;
}

short product(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // ��������� +4
	REG->ir = 1024 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	if (task % 16 == 0)			     // ��������ֻ��һ��
	{
		if (task/16 <= 4)			 // �������������ݼĴ��� 
			REG->ax[task/16-1] *= num;
		else						 // ���������ǵ�ַ�Ĵ��� 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] *= num;
	}
	else                             // ��������������  
	{
		if (task/16 <= 4 && task%16 <= 4)              // ��������Ϊ�����ݼĴ��� + ���ݼĴ��� 
			REG->ax[task/16-1] *= REG->ax[task%16-1];
		else if(task/16 <= 4 && task%16 >= 5)          // ��������Ϊ�����ݼĴ��� + ��ַ�Ĵ��� 
			REG->ax[task/16-1] *= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
		else if (task/16 >= 5 && task%16 <= 4)         // ��������Ϊ����ַ�Ĵ��� + ���ݼĴ��� 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] *= REG->ax[task%16-1];
		else if (task/16 >= 5 && task%16 >= 5)         // ��������Ϊ����ַ�Ĵ��� + ��ַ�Ĵ��� 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] *= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
	}
	return 0;
}

short division(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // ��������� +4
	REG->ir = 1280 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	if (task % 16 == 0)			     // ��������ֻ��һ��
	{
		if (task/16 <= 4)			 // �������������ݼĴ��� 
			REG->ax[task/16-1] /= num;
		else						 // ���������ǵ�ַ�Ĵ��� 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] /= num;
	}
	else                             // ��������������  
	{
		if (task/16 <= 4 && task%16 <= 4)              // ��������Ϊ�����ݼĴ��� + ���ݼĴ��� 
			REG->ax[task/16-1] /= REG->ax[task%16-1];
		else if(task/16 <= 4 && task%16 >= 5)          // ��������Ϊ�����ݼĴ��� + ��ַ�Ĵ��� 
			REG->ax[task/16-1] /= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
		else if (task/16 >= 5 && task%16 <= 4)         // ��������Ϊ����ַ�Ĵ��� + ���ݼĴ��� 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] /= REG->ax[task%16-1];
		else if (task/16 >= 5 && task%16 >= 5)         // ��������Ϊ����ַ�Ĵ��� + ��ַ�Ĵ��� 
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] /= RAM->dataRAM[(REG->ax[task%16-1]-16384)/2];
	}
	return 0;
}

short AND(short task, short num, struct registers* REG, struct memory* RAM)
{
	REG->ip += 4;                    // ��������� +4
	REG->ir = 1536 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	if (task % 16 == 0)			     // ��������ֻ��һ��			
	{
		if (task/16 <= 4)
			REG->ax[task/16-1] = (num && REG->ax[task/16-1]);
		else
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = (num && RAM->dataRAM[(REG->ax[task/16-1]-16384)/2]);
	}
	else                             // �������������� 
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
	REG->ip += 4;                    // ��������� +4
	REG->ir = 1792 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	if (task % 16 == 0)			     // ��������ֻ��һ��
	{
		if (task/16 <= 4)
			REG->ax[task/16-1] = (num || REG->ax[task/16-1]);
		else
			RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = (num || RAM->dataRAM[(REG->ax[task/16-1]-16384)/2]);
	}
	else                             // �������������� 
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
	REG->ip += 4;                    // ��������� +4
	REG->ir = 2048 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	if (task % 16 == 0)			     // �����������ݼĴ��� 
		REG->ax[task/16-1] = !(REG->ax[task/16-1]);
	else                             // ��������Ϊ��ַ�Ĵ��� 
		RAM->dataRAM[(REG->ax[task%16-1]-16384)/2] = !(RAM->dataRAM[(REG->ax[task%16-1]-16384)/2]);
	return 0;
}

short compare(short task, short num, struct registers* REG, struct memory* RAM)
{
	if (task % 16 == 0 && task/16 <= 4) {			// ��������ֻ�����ݼĴ��� 
		if (REG->ax[task/16-1] > num)               // ���ݼĴ����е������������Ƚϣ�������ڱ�־�Ĵ����� 
			REG->flag = 1;
		else if (REG->ax[task/16-1] < num)
			REG->flag = -1;
		else if (REG->ax[task/16-1] == num)
			REG->flag = 0;
	}
	else if (task % 16 == 0 && task/16 >= 5) {						// ��������ֻ�ǵ�ַ�Ĵ��� 
		if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] > num)       // ��ַ�Ĵ�����ָ�ڴ棨dataRAM����ַ�����洢�������������Ƚϣ�������ڱ�־�Ĵ�����
			REG->flag = 1;
		else if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] < num)
			REG->flag = -1;
		else if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] == num)
			REG->flag = 0;
	}
	else if (task % 16 != 0 && task/16 <= 4 && task%16 <= 4) {      // ��������Ϊ�����ݼĴ��� + ���ݼĴ��� 
		if (REG->ax[task/16-1] > REG->ax[task%16-1])				// �ȽϺ󣬽�����ڱ�־�Ĵ����� 
			REG->flag = 1;
		else if (REG->ax[task/16-1] < REG->ax[task%16-1])
			REG->flag = -1;
		else if (REG->ax[task/16-1] == REG->ax[task%16-1])
			REG->flag = 0;		
	}
	else if(task % 16 != 0 && task/16 <= 4 && task%16 >= 5) {      				// ��������Ϊ�����ݼĴ��� + ��ַ�Ĵ��� 
		if (REG->ax[task/16-1] > RAM->dataRAM[(REG->ax[task%16-1]-16384)/2])	// �ȽϺ󣬽�����ڱ�־�Ĵ�����
			REG->flag = 1;
		else if (REG->ax[task/16-1] < RAM->dataRAM[(REG->ax[task%16-1]-16384)/2])
			REG->flag = -1;
		else if (REG->ax[task/16-1] == RAM->dataRAM[(REG->ax[task%16-1]-16384)/2])
			REG->flag = 0;	
	}
	else if (task % 16 != 0 && task/16 >= 5 && task%16 <= 4) {     			    // ��������Ϊ����ַ�Ĵ��� + ���ݼĴ��� 
		if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] > REG->ax[task%16-1])	// �ȽϺ󣬽�����ڱ�־�Ĵ�����
			REG->flag = 1;
		else if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] < REG->ax[task%16-1])
			REG->flag = -1;
		else if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] == REG->ax[task%16-1])
			REG->flag = 0;
	}
	else if (task % 16 != 0 && task/16 >= 5 && task%16 >= 5) {     										// ��������Ϊ����ַ�Ĵ��� + ��ַ�Ĵ��� 
		if (RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] > RAM->dataRAM[(REG->ax[task%16-1]-16384)/2])	// �ȽϺ󣬽�����ڱ�־�Ĵ�����
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
	REG->ir =2560 + task;		// ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	if(task == 0)				// ��������ת 
	{
		REG->ip += num; 		// ��������� +num 
		return num/4;			// ��ת��num/4��ָ�������������
	}
	else if (task == 1 && REG->flag == 0)   // flag = 0ʱ��ת 
	{
		REG->ip += num;                     // ��������� +num
		return num/4;						// ��ת��num/4��ָ�� ��������������
	}
	else if (task == 2 && REG->flag == 1)   // flag = 1ʱ��ת 
	{
		REG->ip += num;                     // ��������� +num
		return num/4;						// ��ת��num/4��ָ�� ��������������
	}
	else if (task == 3 && REG->flag == -1)  // flag = -1ʱ��ת 
	{
		REG->ip += num;                     // ��������� +num
		return num/4;						// ��ת��num/4��ָ�� ��������������
	}
	else        				 // ��������תָ������������ת 
	{
		REG->ip += 4;			 // ��������� +4
		return 0;				 // ��ת�� 0 ��ָ�� ��������������
    }
}

short input(short task, struct registers* REG, struct memory* RAM)
{
	short num;
	printf ("in:\n");
	scanf ("%hd", &num);
	if (task/16 <= 4)			     // ��������Ϊ���ݼĴ��� 
		REG->ax[task/16-1] = num;
	else			                 // ��������Ϊ��ַ�Ĵ��� 
		RAM->dataRAM[(REG->ax[task/16-1]-16384)/2] = num;
	REG->ip += 4;                    // ��������� +4
	REG->ir = 2816 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	return 0;
}

short output(short task, struct registers* REG, struct memory* RAM)
{
	exit_sign = 0; 
	while (exit_sign != 3)					 // �ȴ���� 
	{
		if (!new_sign)
		{					 
			new_sign = 1;
			printf ("id = %d    out: ", REG->id );
			if (task/16 <= 4)			     // ��������Ϊ���ݼĴ��� 
				printf ("%d", REG->ax[task/16-1]);
			else			                 // ��������Ϊ��ַ�Ĵ��� 
				printf ("%d", RAM->dataRAM[(REG->ax[task/16-1]-16384)/2]);
			printf ("\n");
			exit_sign = 3;
			new_sign = 0;
		}
	}
	REG->ip += 4;                    // ��������� +4
	REG->ir = 3072 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	return 0;
}

short createmutex(short task, struct registers* REG, struct memory* RAM)
{
	WaitForSingleObject (hMutex, INFINITE);		// ���󻥳���� 
	REG->ip += 4;                    			// ��������� +4
	REG->ir = 3328 + task;			 			// ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	return 0; 
}

short releasemutex(short task, struct registers* REG, struct memory* RAM)
{
	ReleaseMutex (hMutex);			 // �ͷŻ������ 
	REG->ip += 4;                    // ��������� +4
	REG->ir = 3584 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	return 0;
}

short sleep(short task, short num, struct registers* REG, struct memory* RAM)
{
	Sleep(num); 					 // �߳�����
	REG->ip += 4;                    // ��������� +4
	REG->ir = 3840 + task;			 // ָ��Ĵ������������Ӧ��ʮ������ * 2^8 + ���������Ӧ��ʮ������
	return 0;
}
