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
	

	while (1) // этот цикл не прервется, поэтому нам нужна функция выхода.
	{
		for (int i = 0; i < ItemCount; i++) // вывод Меню.
		{
			gotoxy(25, 7 + i);
			MenuChoice == i + 1 ? cout << " -> " : cout << "    ";

			cout << MenuItems(user)[i] << endl; // вывод названий Меню
		}

		key = _getch(); //get the key.

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
			}  // we have to use try and catch coz if we did'nt fill that index with a function.
			   //                     a runtime error will appear.

			break;

		case 'P': // if the entered key is the 'up arrow' notice that its equal to 'P' (capital)
			MenuChoice++; //then we will increment the cursor by one.
			if (MenuChoice > ItemCount) // if the cursor value is more than the items count.
				MenuChoice = 1;         //    then it will return back to the first item.
			break;

		case 'H': // same but with 'down arrow' and decrement the cursor.
			MenuChoice--;
			if (MenuChoice < 1)
				MenuChoice = ItemCount;
			break;

		case 27: // 27 is the asscii to the escape key (Esc)
			try { (*MenuOption[ItemCount - 1])(); } // useually when the 'Esc' key is pressed the last
												//     item will be called (executed). but you can
												//     change it to whatever you want.
			catch (...) {}
			break;
		default:// any another key.
			if (key >= '1' && key <= char(ItemCount + '0'))//check if the pressed key is in the range
													  //    of (1,2,3,...,#of items) [all char(s)]
			{
				try { (*MenuOption[int(key) - '0' - 1])(); } //call the function of the pressed number.
					 //  you can make the cursor move to that item instead of calling (executing)
					 //  it by replacing all the code between 'if (bla){' and '}' with this
					 //  statement MenuChooice=int(key)-'0'
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