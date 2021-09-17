#pragma once

#include <iostream>
#include <string>
#include <exception>
#include"MyExceptions.h"
#include"Menu.h"
#include"User.h"

using namespace std;


bool CheckAdmin()
{
	ifstream fin;

	//fin.exceptions(ifstream::badbit | istream::failbit);

	try
	{
		string path = Admin::filename;

		fin.open(path);

		if (!fin.is_open())
		{
			throw ExceptionAdmin("Файл конфигурации не найден. ", 1);
		}

		fin.close();

		return true;
	}

	catch (ExceptionAdmin& ex)
	{
		cout << ex.what() << "Код ошибки: " << ex.GetError() << "\n";
		
		cout << "В системе нет зарегистрированного администратора.\n";
		cout << "Пройдите регистрацию" << "\n";
		system("pause");

		return false;
	}


}

//------------------------------------------------

string* MenuItems(string user); // функция, которая возвращает имя пункта меню.

void gotoxy(int, int); //с помощью этой функции вы можете перейти в любую позицию на экране строки cmd.

void ChangeCursorStatus(bool);

void MenuFun1(); // пункт меню 1

void MenuFun2(); //    пункт меню 2

void MenuFun3(); // пункт меню 3

void MenuFun20(); //    пункт меню 2

void MenuFun21(); // пункт меню 3

void ExitOption(); // элемент выхода

void UseMenu(string user, int ItemCount)
{
	ChangeCursorStatus(false);
	
	int MenuChoice = 1; // Эта переменная содержит позицию курсора. 
	char key; // для ввода ключа (стрелка вверх, стрелка вниз и т. д.);

	typedef void (*TMenuOption)(); // typedef for defining a 'pointer to function' type.

	TMenuOption* MenuOption = new TMenuOption[ItemCount];// массив указателей на функции (динамический).
	
	if (user == "admin")
	{
		MenuOption[0] = MenuFun1; // заполнение массива функциями.
		MenuOption[1] = MenuFun2;
		MenuOption[2] = MenuFun3;
		MenuOption[3] = ExitOption;
	}

	if (user == "student")
	{
		MenuOption[0] = MenuFun1; // заполнение массива функциями.
		MenuOption[1] = MenuFun2;
		//MenuOption[2] = MenuFun3;
		MenuOption[2] = ExitOption;
	}

	if (user == "sign")
	{
		MenuOption[0] = MenuFun20; // заполнение массива функциями.
		MenuOption[1] = MenuFun21;
		//MenuOption[2] = MenuFun3;
		MenuOption[2] = ExitOption;
	}
	

	while (1) // этот цикл не прервется, поэтому нам нужна функция выхода.
	{
		for (int i = 0; i < ItemCount; i++) // вывод Меню.
		{
			gotoxy(25, 7 + i);
			MenuChoice == i + 1 ? cout << " -> " : cout << "    ";

			cout << MenuItems(user)[i] << endl; // вывод названий Меню
		}

		key = _getch();

		switch (key) // проверка введенного ключа.
		{
		case '\r': // если введенный ключ - «Enter».
			try
			{
				(*MenuOption[MenuChoice - 1])(); // вызвать функцию индекса 'cursor-1' в
											   //    массив "указатель на функцию".
			}
			catch (...)
			{
			} 

			break;

		case 'P': // 'up arrow' 
			MenuChoice++; 
			if (MenuChoice > ItemCount) 
				MenuChoice = 1; 
			break;

		case 'H': // 'down arrow'
			MenuChoice--;
			if (MenuChoice < 1)
				MenuChoice = ItemCount;
			break;

		case 27:// (Esc)
			try { (*MenuOption[ItemCount - 1])(); } 
			catch (...) {}
			break;
		default:
			if (key >= '1' && key <= char(ItemCount + '0'))
			
			{
				try { (*MenuOption[int(key) - '0' - 1])(); } 
				catch (...) {}
			}
		}
	}

	delete[]MenuOption;
}

//-----------------------------------------------------------------------------------------------
string* MenuItems(string user) // this function returns a pointer to a string.
{
	if (user == "admin")
	{
		string* item = new string[4];
		item[0] = "Menu Option #1.";
		item[1] = "Menu Option #2.";
		item[2] = "Menu Option #3.";
		item[3] = "Exit.";
		return item;
	}
	
	if (user == "student")
	{
		string* item = new string[3];
		item[0] = "Menu Option #1.";
		item[1] = "Menu Option #2.";
		//item[2] = "Menu Option #3.";
		item[2] = "Exit.";
		return item;
	}

	if (user == "sign")
	{
		string* item = new string[3];
		item[0] = "Вход c помощью почты";
		item[1] = "Регистрация";
		//item[2] = "Menu Option #3.";
		item[2] = "Выход";
		return item;
	}

	
}
//-----------------------------------------------------------------------------------------------
void gotoxy(int xpos, int ypos)  // just take this function as it is.
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}
//-----------------------------------------------------------------------------------------------
void MenuFun1()
{
	system("cls"); //clear the screen.
	gotoxy(25, 10);
	cout << "You have selected menu option (#1)" << endl;
	_getch(); // we can use getch like pause (whitout assigning the return value to a variable).
	system("cls");
}
//-----------------------------------------------------------------------------------------------
void MenuFun2()
{
	system("cls");
	gotoxy(25, 10);
	cout << "You have selected menu option (#2)" << endl;
	_getch();
	system("cls");
}
//-----------------------------------------------------------------------------------------------
void MenuFun3()
{
	system("cls");
	gotoxy(25, 10);
	cout << "You have selected menu option (#3)" << endl;
	_getch();
	system("cls");
}


//-----------------------------------------------------------------------------------------------
void MenuFun20()
{
	system("cls"); //clear the screen.
	gotoxy(25, 10);
	
	

	_getch(); // we can use getch like pause (whitout assigning the return value to a variable).
	system("cls");
}
//-----------------------------------------------------------------------------------------------
void MenuFun21()
{
	system("cls");
	gotoxy(25, 10);
	unique_ptr<Student> st(new Student);

	st->Registration();
	gotoxy(25, 12);
	cout << "Регистрация завершена. Нажмите любую кнопку." << endl;
	_getch();
	system("cls");
}

//-----------------------------------------------------------------------------------------------
void ExitOption()
{
	gotoxy(30, 15);
	cout << "Exitting..." << endl;
	_getch();
	exit(0);
}
//-----------------------------------------------------------------------------------------------
void ChangeCursorStatus(bool Visible)
{
	CONSOLE_CURSOR_INFO* c = new CONSOLE_CURSOR_INFO;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	if (Visible)
	{
		c->bVisible = TRUE;
		c->dwSize = 0;//(0) is invild so the default value is set
	}
	else
	{
		c->bVisible = FALSE;
		c->dwSize = 1;//it must be assigned to a valid value
	}

	SetConsoleCursorInfo(h, c);
}
//-----------------------------------------------------------------------------------------------