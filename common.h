#include <windows.h>
#define MAX 32768					// �ڴ��ģ����������Ԫ����

struct registers{	       		    // ����ģ����Ĵ�����״̬ 
	short id;						// �߳� 
	int ip;							// ģ���������� 
	short flag;						// ģ���־�Ĵ���
	int ir;							// ģ��ָ��Ĵ��� 
	short ax[8];				    // ģ�����ݺ͵�ַ�Ĵ���  
};

struct memory{	       		        // �����ڴ� 
	int codeRAM[MAX];				// ģ������ڴ� 
	short dataRAM[MAX];				// ģ�������ڴ� 
};

int binaryToInt(char *str);			// ���������ַ���תΪʮ������
HANDLE hMutex;						// �����߳����þ�� 
int new_sign;						// ͬ���� 
int exit_sign;
