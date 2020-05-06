#include <dos.h>
#include <conio.h>
#include <stdio.h>

enum keyboad_indicator 
{
	SCROLL_LOCK = 0x01, NUM_LOCK = 0x02, CAPS_LOCK = 0x04
};

void interrupt newInt9(...);					// Функция обработки прерывания
void interrupt(*oldInt9)(...);					// Указатель на обработчик прерывания

void set_indicator(unsigned char mask);				// Функция управления индикаторами
void blinking(void);							// Функция мигания индикаторами

int check_ret = 1;								// Флаг ошибки / необходимости повторной передачи данных
int fl_exit = 0;								// Флаг выхода из программы
int fl_blincking_ON = 0;						// Флаг мигания индикаторами
int count = 0;									// Счетчик повторной передачи

void main() 
{
	clrscr();
	disable();
	oldInt9 = getvect(9);						// Сохраняем указатель на старый обработчик
	setvect(9, newInt9);						// Меняем его на новый  
	enable();

	while (!fl_exit)
	{											// Пока не установлен флаг выхода
		if (fl_blincking_ON)							// Если установлен флаг мигания индикаторов 
			blinking();							// мигаем индикаторами
	}

	disable();
	setvect(9, oldInt9);						// Восстанавливаем старый обработчик прерывания
	enable();

	return;
}
// Функция обработки прерывания
void interrupt newInt9(...)
{
	printf("\nFor blincking press '1'...\n\n");
	unsigned char value = 0;
	value = inp(0x60);							// Получаем значение из порта 60h
	if (value == 0x01) fl_exit = 1;				// Устанавливаем флаг выхода, если нажата Esc  

	if (value == 0x02 && fl_blincking_ON == 0)       // Поставить или снять флаг мигания, если нажата клавиша 1
		fl_blincking_ON = 1;
	else if (value == 0x02 && fl_blincking_ON == 1)
		fl_blincking_ON = 0;

	if (value == 0xFE && fl_blincking_ON == 1)	//FE - произошла ошибка
	{											// Если нет подтверждения успешного выполнения команды
		check_ret = 1;							// Устанавливаем флаг повторной передачи байта
		count++;
		if (count > 3)
		{
			printf("\n\nError!\n");
			fl_exit = 1;
			outp(0x20, 0x20);							// Сброс контроллера прерываний
			return;
		}
	}
	else check_ret = 0;

	printf(" %x", value);
	outp(0x20, 0x20);							// Сброс контроллера прерываний
}
// Функция управления индикаторами
void set_indicator(unsigned char mask)
{
	check_ret = 1;
	while (check_ret)							// Пока нет подтверждения успешного выполнения команды
	{											
		while ((inp(0x64) & 0x02) != 0x00);		// Ожидаем освобождения входного буфера клавиатуры
		outp(0x60, 0xED);						// Записываем в порт команду управления индикаторами
		delay(50);
	}

	check_ret = 1;
	while (check_ret)							// Пока нет подтверждения успешного выполнения команды
	{											
		while ((inp(0x64) & 0x02) != 0x00);		// Ожидаем освобождения входного буфера клавиатуры
		outp(0x60, mask);						// Записываем в порт битовую маску для настройки индикаторов 
		delay(50);
	}
}
// Функция мигания индикаторами
void blinking()
{
	/*
		Алгоритм мигания: Scroll lock -> Caps Lock -> Num Lock -> Num Lock -> Caps Lock -> Scroll Lock
	*/
	set_indicator(SCROLL_LOCK);							// индикатор Scroll Lock
	delay(200);

	set_indicator(CAPS_LOCK);							// индикатор Caps Lock
	delay(200);

	set_indicator(NUM_LOCK);							// индикатор Num Lock	
	delay(200);

	set_indicator(0x00);								// выключение всех индикаторов
	delay(200);

	set_indicator(NUM_LOCK);							// индикатор Num Lock	
	delay(250);

	set_indicator(CAPS_LOCK);							// индикатор Caps Lock
	delay(250);

	set_indicator(SCROLL_LOCK);							// индикатор Scroll Lock 	
	delay(250);

	set_indicator(0x00);								// выключение всех индикаторов
	delay(250);
}
