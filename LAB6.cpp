#include <dos.h>
#include <conio.h>
#include <stdio.h>

enum keyboad_indicator 
{
	SCROLL_LOCK = 0x01, NUM_LOCK = 0x02, CAPS_LOCK = 0x04
};

void interrupt newInt9(...);					// ������� ��������� ����������
void interrupt(*oldInt9)(...);					// ��������� �� ���������� ����������

void set_indicator(unsigned char mask);				// ������� ���������� ������������
void blinking(void);							// ������� ������� ������������

int check_ret = 1;								// ���� ������ / ������������� ��������� �������� ������
int fl_exit = 0;								// ���� ������ �� ���������
int fl_blincking_ON = 0;						// ���� ������� ������������
int count = 0;									// ������� ��������� ��������

void main() 
{
	clrscr();
	disable();
	oldInt9 = getvect(9);						// ��������� ��������� �� ������ ����������
	setvect(9, newInt9);						// ������ ��� �� �����  
	enable();

	while (!fl_exit)
	{											// ���� �� ���������� ���� ������
		if (fl_blincking_ON)							// ���� ���������� ���� ������� ����������� 
			blinking();							// ������ ������������
	}

	disable();
	setvect(9, oldInt9);						// ��������������� ������ ���������� ����������
	enable();

	return;
}
// ������� ��������� ����������
void interrupt newInt9(...)
{
	printf("\nFor blincking press '1'...\n\n");
	unsigned char value = 0;
	value = inp(0x60);							// �������� �������� �� ����� 60h
	if (value == 0x01) fl_exit = 1;				// ������������� ���� ������, ���� ������ Esc  

	if (value == 0x02 && fl_blincking_ON == 0)       // ��������� ��� ����� ���� �������, ���� ������ ������� 1
		fl_blincking_ON = 1;
	else if (value == 0x02 && fl_blincking_ON == 1)
		fl_blincking_ON = 0;

	if (value == 0xFE && fl_blincking_ON == 1)	//FE - ��������� ������
	{											// ���� ��� ������������� ��������� ���������� �������
		check_ret = 1;							// ������������� ���� ��������� �������� �����
		count++;
		if (count > 3)
		{
			printf("\n\nError!\n");
			fl_exit = 1;
			outp(0x20, 0x20);							// ����� ����������� ����������
			return;
		}
	}
	else check_ret = 0;

	printf(" %x", value);
	outp(0x20, 0x20);							// ����� ����������� ����������
}
// ������� ���������� ������������
void set_indicator(unsigned char mask)
{
	check_ret = 1;
	while (check_ret)							// ���� ��� ������������� ��������� ���������� �������
	{											
		while ((inp(0x64) & 0x02) != 0x00);		// ������� ������������ �������� ������ ����������
		outp(0x60, 0xED);						// ���������� � ���� ������� ���������� ������������
		delay(50);
	}

	check_ret = 1;
	while (check_ret)							// ���� ��� ������������� ��������� ���������� �������
	{											
		while ((inp(0x64) & 0x02) != 0x00);		// ������� ������������ �������� ������ ����������
		outp(0x60, mask);						// ���������� � ���� ������� ����� ��� ��������� ����������� 
		delay(50);
	}
}
// ������� ������� ������������
void blinking()
{
	/*
		�������� �������: Scroll lock -> Caps Lock -> Num Lock -> Num Lock -> Caps Lock -> Scroll Lock
	*/
	set_indicator(SCROLL_LOCK);							// ��������� Scroll Lock
	delay(200);

	set_indicator(CAPS_LOCK);							// ��������� Caps Lock
	delay(200);

	set_indicator(NUM_LOCK);							// ��������� Num Lock	
	delay(200);

	set_indicator(0x00);								// ���������� ���� �����������
	delay(200);

	set_indicator(NUM_LOCK);							// ��������� Num Lock	
	delay(250);

	set_indicator(CAPS_LOCK);							// ��������� Caps Lock
	delay(250);

	set_indicator(SCROLL_LOCK);							// ��������� Scroll Lock 	
	delay(250);

	set_indicator(0x00);								// ���������� ���� �����������
	delay(250);
}
