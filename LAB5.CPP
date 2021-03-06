#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>
//���������� ������� ����������
int miliSeconds = 0;
//��������� �� ������ ���������� ���������� ���
void interrupt far(*oldInt70h)(...);
//��������� �� ������ ���������� ���������� ����������
void interrupt far(*oldInt4Ah)(...);
//����� ���������� ����������
void interrupt newInt4Ahandler(...);
//����� ���������� ���
void interrupt newInt70handler(...);	

void freeze(void);
void unfreeze(void);

int BCDToInteger(int bcd);
unsigned char IntToBCD(int value);
void getTime(void);
void setTime(void);
void delay_time(void);
void wait(void);
void enableAlarm(void);
void disableAlarm(void);

void main()
{
	char c, value;
	clrscr();
	printf("\n1. Show time\n2. Set time\n3. Delay time\n4. Enable alarm\n5. Disable alarm\n0. Exit\n\n");
	while (c != 0)
	{
		c = getch();
		switch (c) 
		{
			case '1': getTime(); break;
			case '2': setTime(); break;
			case '3': delay_time(); break;
			case '4': enableAlarm(); break;
			case '5': disableAlarm(); break;
			case '0': return;
		}
		printf("\n1. Show time\n2. Set time\n3. Delay time\n4. Enable alarm\n5. Disable alarm\n0. Exit\n\n");
	}
}

void wait(void) 
{
	do
	{								
		outp(0x70, 0x0A);		//������� ��������� 1(UIP - ������ ���������� ��������� ����� 0 -���� ��������)
								//DV - 3 ���� ������ �������� ������� ���������� �����(��� ���������� ������ ������ ���� ��� � 010 - 32.768���)
								//RS - 4 ���� ��������� ������� �������� �������� �������(��� ���������� ������ ������ ���� ��� � 0110 - 1.024)
	} while (inp(0x71) & 0x80);	//������� ���� ���� ������ ����������		
}

void getTime(void) 
{
	int value;

	wait();									//������� ����
	outp(0x70, 0x04);						//������������� �������� �����
	value = inp(0x71);						//�������� �������� �����	
	printf("%d:", BCDToInteger(value));		//��������� �������-���������� ��� � � ������������� int

	wait();
	outp(0x70, 0x02);											
	value = inp(0x71);						//�������� �������� �����
	printf("%d:", BCDToInteger(value));

	wait();
	outp(0x70, 0x00);											
	value = inp(0x71);						//�������� �������� �������
	printf("%d   ", BCDToInteger(value));

	wait();
	outp(0x70, 0x07);											
	value = inp(0x71);						//�������� �������� ����
	printf("%d.", BCDToInteger(value));

	wait();
	outp(0x70, 0x08);										
	value = inp(0x71);						//�������� �������� ������
	printf("%d.", BCDToInteger(value));

	wait();
	outp(0x70, 0x09);						
	value = inp(0x71);						//�������� �������� ����
	printf("%d   ", BCDToInteger(value));

	wait();
	outp(0x70, 0x06);									
	value = inp(0x71);						//�������� �������� ��� ������
	switch (BCDToInteger(value))
	{
		case 1: printf("Sunday\n"); break;
		case 2: printf("Monday\n"); break;
		case 3: printf("Tuesday\n"); break;
		case 4: printf("Wednesday\n"); break;
		case 5: printf("Thursday\n"); break;
		case 6: printf("Friday\n"); break;
		case 7: printf("Saturday\n"); break;	
		default: printf("Day of week is not set\n"); break;
	}
}

void setTime(void) 
{
	int value;
	freeze();							
	//��������� ������
	do 
	{
		printf("Enter hour: ");
		fflush(stdin);
		scanf("%d", &value);
		if (value > 23 || value < 0)
			printf("Error incorrect value!\n");
	} while (value > 23 || value < 0);
	outp(0x70, 0x04);							
	outp(0x71, IntToBCD(value));				//��������� �������� �����

	do 
	{
		printf("Enter minute: ");
		fflush(stdin);
		scanf("%d", &value);
		if (value > 59 || value < 0)
			printf("Error incorrect value!\n");
	} while (value > 59 || value < 0);
	outp(0x70, 0x02);							
	outp(0x71, IntToBCD(value));				//��������� �������� �����

	do 
	{
		printf("Enter second: ");
		fflush(stdin);
		scanf("%d", &value);
		if (value > 59 || value < 0)
			printf("Error incorrect value!\n");
	} while (value > 59 || value < 0);
	outp(0x70, 0x00);								
	outp(0x71, IntToBCD(value));				//��������� �������� ������

	do
	{
		printf("Enter week day number(1...7): ");
		fflush(stdin);
		scanf("%d", &value);
		if (value > 7 || value < 1)
			printf("Error incorrect value!\n");
	} while (value > 7 || value < 1);
	outp(0x70, 0x06);							
	outp(0x71, IntToBCD(value));				//6-�������� �� ��� ������

	do
	{
		printf("Enter day of month(1...31): ");
		fflush(stdin);
		scanf("%d", &value);
		if (value > 31 || value < 1)
			printf("Error incorrect value!\n");
	} while (value > 31 || value < 1);
	outp(0x70, 0x07);							
	outp(0x71, IntToBCD(value));				//7-����

	do 
	{
		printf("Enter mounth(1...12): ");
		fflush(stdin);
		scanf("%d", &value);
		if (value > 21 || value < 1)
			printf("Error incorrect value!\n");
	} while (value > 12 || value < 1);
	outp(0x70, 0x08);							
	outp(0x71, IntToBCD(value));				//8-�����

	do
	{
		printf("Enter year(0...99): ");
		fflush(stdin);
		scanf("%d", &value);
		if (value > 99 || value < 0)
			printf("Error incorrect value!\n");
	} while (value > 99 || value < 0);
	outp(0x70, 0x09);							
	outp(0x71, IntToBCD(value));				//9-���

	unfreeze();							
}

void enableAlarm(void)
{

	int value, second, minute, hour;
	//���� ������ ��� ��������� ����������
	do
	{
		printf("Enter hour: ");
		fflush(stdin);
		scanf("%d", &hour);
	} while (hour > 23 || hour < 0);

	do
	{
		printf("Enter minute: ");
		fflush(stdin);
		scanf("%d", &minute);
	} while (minute > 59 || minute < 0);

	do 
	{
		printf("Enter second: ");
		fflush(stdin);
		scanf("%d", &second);
	} while (second > 59 || second < 0);

	//��� ������ ������� 70 ����, ��� ������ �/��� ������ 71
	outp(0x70, 0x01);				
	outp(0x71, IntToBCD(second));	//��������� ������ ��� ����������

	outp(0x70, 0x03);				
	outp(0x71, IntToBCD(minute));	//������ ��� ����������

	outp(0x70, 0x05);				
	outp(0x71, IntToBCD(hour));		//���� ��� ����������

	outp(0x70, 0xB);				//������� ��������� 2
	outp(0x71, inp(0x71) | 0x20);	//0001 0100 - 5-�� ��� AIE ����� � 1(���������� ���������� ���������), 2-�� ��� � 1(DM-������ ������� � ���� � �������� ����)

	//��������� ������ ����������� ����������
	disable();

	oldInt4Ah = getvect(0x4A);		//���������� ������
	setvect(0x4A, newInt4Ahandler);	//������������� �����
	
	enable(); 
}

//�������������� ������� �������� ���������� ����� ������������ ����������
void disableAlarm()
{
	if (oldInt4Ah == NULL) {
		printf("Alarm is already disable or not set.\n");
		return;
	}
	_disable();
	
	setvect(0x4A, oldInt4Ah);			//������������� ������ ���������� ����������
	outp(0xA1, (inp(0xA0) | 0x01));
	outp(0x70, 0x0B);
	outp(0xA1, (inp(0x71) & 0xDF));		//11011111 ��� ������� ���������� ����������, ���������� ���� 0 ��� - � 1 � 5 ��� - � 0
	
	_enable();

	printf("Alarm is disable.\n");
}

void freeze(void) 
{
	unsigned char value;

	wait();											//�������� �����

	outp(0x70, 0x0B);		
	value = inp(0x71);								//������� ��������� �������� 0�0�
	value |= 0x80;									//��������� ���������� �����,� ������ ��������� UPD 7-�� ���� �� 1
	outp(0x70, 0x0B);
	outp(0x71, value);								//������� ����� �������� � ������� � ������������� ����� �������		
}

void unfreeze(void)
{
	unsigned char value;
	
	wait();											//�������� �����							
	
	outp(0x70, 0x0B);								//������� ��������� 2
	value = inp(0x71);								//���������� ���������
	value -= 0x80;									//��������� ���������� �����, �.�. UPD 7-�� ��� �������� �� ����
	outp(0x70, 0x0B);
	outp(0x71, value);								//���������� ����� �������� �������� 0�0�,���������� ����� ���������
}

//����� ���������� ���������� �������� ���
void interrupt newInt70handler(...)
{
	miliSeconds++;									//���������� ������� ����������															
	outp(0x70, 0x0C);								//������� ��������� 3(������� ����� ���������� �����)					
	inp(0x71);										
	outp(0x20, 0x20);								//�������� ����������� ���������� ������ EOI(MASTER)
	outp(0xA0, 0x20);								//�������� ����������� ���������� ������ EOI(SLAVE)
}

//�������� ���������� ��� ������������ ����������
void interrupt newInt4Ahandler(...) 
{
	fflush(stdout);
	write(1, "Wake UP!\0",8);
	//��������������� ������ ��������
	disableAlarm();						
}

//�������� �� N ��
void delay_time(void) 
{
	unsigned long delayPeriod;
	unsigned char value;

	do
	{
		printf("Enter delay time in milliseconds: ");
		fflush(stdin);
		scanf("%ld", &delayPeriod);
	} while (delayPeriod < 1);
	printf("Time before delay: ");
	getTime();											//�������� ������� �����
	printf("Delaying ...\n");

	_disable();											//��������� ����������						
	oldInt70h = getvect(0x70);							//���������� ������ ���������� ����������
	setvect(0x70, newInt70handler);						//������������� ���� ���������� ����������
	_enable();											//��������� ����������


	outp(0xA1, inp(0xA1) & 0xFE);						//�������������� ����� ������� ������� �� ��� 11111110, � ������ ���� 0-��������� ����������							
	outp(0x70, 0x0B);									//������� ��������� 2	
	outp(0x71, inp(0x71) | 0x40);						//01000000 ������������� � 6-�� ��� 1-PIE ��������� ������������� ����������			

	miliSeconds = 0;
	while (miliSeconds != delayPeriod);					//���� ��������	 
	printf("End delay of %d ms\n", msCounter);

	_disable();
	setvect(0x70, oldInt70h);							//�������� ������� �������������� ������� ���������� 		
	_enable();

	printf("Time after delay: ");
	getTime();											//������� ������� �����, ��� ��������� ����� �� �������� � ����� ��
}

//������� BCD-���� � ������ ��������
int BCDToInteger(int bcd) 
{
	return bcd % 16 + bcd / 16 * 10;
}

unsigned char IntToBCD(int value) 
{
	return (unsigned char)((value / 10) << 4) | (value % 10);
}