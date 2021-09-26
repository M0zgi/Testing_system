#pragma once

#include <iostream>
#include <string>
#include <exception>
#include"MyExceptions.h"
#include"Menu.h"
#include"User.h"

using namespace std;

//ф-кция проверки есть ли администратор системы тестирования
bool CheckAdmin()
{
	ifstream fin;

	fin.exceptions(ifstream::badbit | istream::failbit);

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

	catch (const ifstream::failure& ex)
	{
		gotoxy(25, 13);
		cout << ex.what() << "\nКод ошибки: " << ex.code() << "\n";
		system("pause");
	}

	catch (ExceptionAdmin& ex)
	{
		cout << ex.what() << "Код ошибки: " << ex.GetError() << "\n";

		cout << "В системе нет зарегистрированного администратора.\n";
		cout << "Пройдите регистрацию" << "\n";
		system("pause");

		return false;
	}

	catch (...)
	{
		cout << "Ошибка в функции bool CheckAdmin()";
	}
}

//------------------------------------------------

string* MenuItems(string user); // функция, которая возвращает имя пункта меню.

void gotoxy(int, int); //с помощью этой функции вы можете перейти в любую позицию на экране строки cmd.

void ChangeCursorStatus(bool);

void AdminFun1(); // пункт меню Админ 1

void AdminFun2(); // пункт меню Админ 2

void AdminFun3(); // пункт меню Админ 3

void MenuFun20(); //  пункт меню входа пользователя в систему

void MenuFun21(); // пункт меню регистрации пользователя в системе

void StudentFun1();
void StudentFun2();
void StudentFun3();

void ExitOption(); // элемент выхода


//Отображение текущего имени пользователя
class UserName
{
public:
	
	UserName() {};
	UserName(string user, int ItemCount) : user(user), ItemCount(ItemCount) {}
	~UserName() {};

	void SetUser(string user) { this->user = user; }
	void SetItemCount(int ItemCount) { this->ItemCount = ItemCount; }

	string  GetUser() { return this->user; }
	//void SetItemCount(int ItemCount) { this->ItemCount = ItemCount; }
	//friend class User;

private:

	string user = "Зарегистрируйтесь, или войдите";
	int ItemCount = 3;
}; 

unique_ptr<UserName> userName (new UserName);


void UseMenu(string user, int ItemCount)
{
	ChangeCursorStatus(false);
	//cout << fm->GetUser();
	//unique_ptr<User> us(new User);
	//user = us->GetLogin();

	int MenuChoice = 1; // Эта переменная содержит позицию курсора. 
	char key; // для ввода ключа (стрелка вверх, стрелка вниз и т. д.);

	typedef void (*TMenuOption)(); // typedef for defining a 'pointer to function' type.

	TMenuOption* MenuOption = new TMenuOption[ItemCount];// массив указателей на функции (динамический).

	if (user == "admin")
	{
		userName->SetUser("admin");
		gotoxy(0, 0);
		cout << userName->GetUser();
		MenuOption[0] = MenuFun21; // заполнение массива функциями.
		MenuOption[1] = AdminFun1;
		MenuOption[2] = AdminFun2;
		MenuOption[3] = AdminFun3;
		MenuOption[4] = ExitOption;
	}

	else if (user == "sign")
	{
		gotoxy(0, 0);
		cout << userName->GetUser();
		MenuOption[0] = MenuFun20;
		MenuOption[1] = MenuFun21;
		//MenuOption[2] = MenuFun3;
		MenuOption[2] = ExitOption;
	}

	else 
	{
		gotoxy(0, 0);
		cout << userName->GetUser();		
		MenuOption[0] = StudentFun1;
		MenuOption[1] = StudentFun2;
		MenuOption[2] = StudentFun3;
		MenuOption[3] = ExitOption;		
	}


	while (1) // этот цикл не прервется, поэтому нам нужна функция выхода.
	{
		
		for (int i = 0; i < ItemCount; i++) // вывод Меню.
		{
			
			gotoxy(25, 7 + i);
			MenuChoice == i + 1 ? cout << " -> " : cout << "    ";

			cout << MenuItems(user)[i] << endl; // вывод названий Меню
		}

		gotoxy(0, 0);
		cout << userName->GetUser();

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
string* MenuItems(string user)
{
	if (user == "admin")
	{
		string* item = new string[5];
		item[0] = "#1. Создание, удаление, модификация пользователей.";
		item[1] = "#2. Просмотр статистики.";
		item[2] = "#3. Управление тестированием.";
		item[3] = "#4. Управление пользователями";
		item[4] = "#5 Exit.";
		return item;
	}	

	else if (user == "sign" )
	{		
		string* item = new string[3];
		item[0] = "Вход c помощью почты";
		item[1] = "Регистрация";
		//item[2] = "Menu Option #3.";
		item[2] = "Выход";
		return item;
	}

	else
	{
		string* item = new string[4];
		item[0] = "student Option #1.";
		item[1] = "student Option #2.";
		item[2] = "student Option #3.";
		item[3] = "Exit.";
		return item;
	}
}
//-----------------------------------------------------------------------------------------------
void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}
//-----------------------------------------------------------------------------------------------
void AdminFun1()
{
	system("cls");

	gotoxy(0, 0);
	cout << userName->GetUser();

	gotoxy(25, 10);
	cout << "You have selected menu option (#1)" << endl;
	_getch();
	system("cls");
}
//-----------------------------------------------------------------------------------------------
void AdminFun2()
{
	system("cls");

	gotoxy(0, 0);
	cout << userName->GetUser();

	gotoxy(25, 10);
	cout << "You have selected menu option (#2)" << endl;
	_getch();
	system("cls");
}
//-----------------------------------------------------------------------------------------------
void AdminFun3()
{
	system("cls");

	gotoxy(0, 0);
	cout << userName->GetUser();

	gotoxy(25, 10);
	cout << "You have selected menu option (#3)" << endl;
	_getch();
	system("cls");
}


//-----------------------------------------------------------------------------------------------
void MenuFun20() //вход пользователя в систему
{
	system("cls");

	gotoxy(0, 0);
	cout << userName->GetUser();
	gotoxy(25, 10);
	
	//Вход через почту
	gotoxy(25, 11);
	unique_ptr<Student> user(new Student);

	string name;

	name = user->UserSignIn();

	userName->SetUser(name);

	if (userName->GetUser() == "admin")
	{
		unique_ptr<Menu> menu;
		shared_ptr<Factory> main_menu;
		shared_ptr<AdminFactory> ma(new AdminFactory);
		main_menu = ma;
		menu = main_menu->runMenu();
		menu->printMenu();
	}

	else if (userName->GetUser() == "Зарегистрируйтесь, или войдите")
	{
		cout << "Вход в систему не удался";
	}

	else
	{
		unique_ptr<Menu> menu;
		shared_ptr<Factory> main_menu;
		shared_ptr<StudentFactory> ma(new StudentFactory);
		main_menu = ma;
		menu = main_menu->runMenu();
		menu->printMenu();
	}

	_getch();
	system("cls");
}
//-----------------------------------------------------------------------------------------------
void MenuFun21() // регистрация пользователя
{
	system("cls");

	gotoxy(0, 0);
	cout << userName->GetUser();

	gotoxy(25, 10);
	unique_ptr<Student> st(new Student);
	st->Registration();

	_getch();
	system("cls");
}

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void StudentFun1() // регистрация пользователя
{
	system("cls");

	gotoxy(0, 0);
	cout << userName->GetUser();

	gotoxy(25, 10);

	_getch();
	system("cls");
}

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void StudentFun2() // регистрация пользователя
{
	system("cls");

	gotoxy(0, 0);
	cout << userName->GetUser();

	gotoxy(25, 10);

	_getch();
	system("cls");
}

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void StudentFun3() // регистрация пользователя
{
	system("cls");

	gotoxy(0, 0);
	cout << userName->GetUser();

	gotoxy(25, 10);

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
		c->dwSize = 0;
	}
	else
	{
		c->bVisible = FALSE;
		c->dwSize = 1;
	}

	SetConsoleCursorInfo(h, c);
}
//-----------------------------------------------------------------------------------------------
















//#pragma once
//
//#include <iostream>
//#include <string>
//#include <exception>
//#include"MyExceptions.h"
//#include"Menu.h"
//#include"User.h"
//
//using namespace std;
//
//
//bool CheckAdmin()
//{
//	ifstream fin;
//
//	//fin.exceptions(ifstream::badbit | istream::failbit);
//
//	try
//	{
//		string path = Admin::filename;
//
//		fin.open(path);
//
//		if (!fin.is_open())
//		{
//			throw ExceptionAdmin("Файл конфигурации не найден. ", 1);
//		}
//
//		fin.close();
//
//		return true;
//	}
//
//	catch (ExceptionAdmin& ex)
//	{
//		cout << ex.what() << "Код ошибки: " << ex.GetError() << "\n";
//		
//		cout << "В системе нет зарегистрированного администратора.\n";
//		cout << "Пройдите регистрацию" << "\n";
//		system("pause");
//
//		return false;
//	}
//
//
//}
//
////------------------------------------------------
//
//string* MenuItems(string user); // функция, которая возвращает имя пункта меню.
//
//void gotoxy(int, int); //с помощью этой функции вы можете перейти в любую позицию на экране строки cmd.
//
//void ChangeCursorStatus(bool);
//
//void MenuFun1(); // пункт меню 1
//
//void MenuFun2(); //    пункт меню 2
//
//void MenuFun3(); // пункт меню 3
//
//void MenuFun20(); //    пункт меню 2
//
//void MenuFun21(); // пункт меню 3
//
//void ExitOption(); // элемент выхода
//
//void UseMenu(string user, int ItemCount)
//{
//	ChangeCursorStatus(false);
//	
//	//unique_ptr<User> us(new User);
//	//user = us->GetLogin();
//
//	int MenuChoice = 1; // Эта переменная содержит позицию курсора. 
//	char key; // для ввода ключа (стрелка вверх, стрелка вниз и т. д.);
//
//	typedef void (*TMenuOption)(); // typedef for defining a 'pointer to function' type.
//
//	TMenuOption* MenuOption = new TMenuOption[ItemCount];// массив указателей на функции (динамический).
//	
//	if (user == "admin")
//	{
//		MenuOption[0] = MenuFun1; // заполнение массива функциями.
//		MenuOption[1] = MenuFun2;
//		MenuOption[2] = MenuFun3;
//		MenuOption[3] = ExitOption;
//	}
//
//	if (user == "student")
//	{
//		MenuOption[0] = MenuFun1; // заполнение массива функциями.
//		MenuOption[1] = MenuFun2;
//		//MenuOption[2] = MenuFun3;
//		MenuOption[2] = ExitOption;
//	}
//
//	if (user == "sign")
//	{
//		MenuOption[0] = MenuFun20; // заполнение массива функциями.
//		MenuOption[1] = MenuFun21;
//		//MenuOption[2] = MenuFun3;
//		MenuOption[2] = ExitOption;
//	}
//	
//
//	while (1) // этот цикл не прервется, поэтому нам нужна функция выхода.
//	{
//		for (int i = 0; i < ItemCount; i++) // вывод Меню.
//		{
//			gotoxy(25, 7 + i);
//			MenuChoice == i + 1 ? cout << " -> " : cout << "    ";
//
//			cout << MenuItems(user)[i] << endl; // вывод названий Меню
//		}
//
//		key = _getch();
//
//		switch (key) // проверка введенного ключа.
//		{
//		case '\r': // если введенный ключ - «Enter».
//			try
//			{
//				(*MenuOption[MenuChoice - 1])(); // вызвать функцию индекса 'cursor-1' в
//											   //    массив "указатель на функцию".
//			}
//			catch (...)
//			{
//			} 
//
//			break;
//
//		case 'P': // 'up arrow' 
//			MenuChoice++; 
//			if (MenuChoice > ItemCount) 
//				MenuChoice = 1; 
//			break;
//
//		case 'H': // 'down arrow'
//			MenuChoice--;
//			if (MenuChoice < 1)
//				MenuChoice = ItemCount;
//			break;
//
//		case 27:// (Esc)
//			try { (*MenuOption[ItemCount - 1])(); } 
//			catch (...) {}
//			break;
//		default:
//			if (key >= '1' && key <= char(ItemCount + '0'))
//			
//			{
//				try { (*MenuOption[int(key) - '0' - 1])(); } 
//				catch (...) {}
//			}
//		}
//	}
//
//	delete[]MenuOption;
//}
//
////-----------------------------------------------------------------------------------------------
//string* MenuItems(string user) 
//{
//	if (user == "admin")
//	{
//		string* item = new string[4];
//		item[0] = "Админ #1.";
//		item[1] = "Menu Option #2.";
//		item[2] = "Menu Option #3.";
//		item[3] = "Exit.";
//		return item;
//	}
//	
//	if (user == "student")
//	{
//		string* item = new string[3];
//		item[0] = "Menu Option #1.";
//		item[1] = "Menu Option #2.";
//		//item[2] = "Menu Option #3.";
//		item[2] = "Exit.";
//		return item;
//	}
//
//	if (user == "sign")
//	{
//		string* item = new string[3];
//		item[0] = "Вход c помощью почты";
//		item[1] = "Регистрация";
//		//item[2] = "Menu Option #3.";
//		item[2] = "Выход";
//		return item;
//	}
//
//	
//}
////-----------------------------------------------------------------------------------------------
//void gotoxy(int xpos, int ypos)  
//{
//	COORD scrn;
//	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
//	scrn.X = xpos; scrn.Y = ypos;
//	SetConsoleCursorPosition(hOuput, scrn);
//}
////-----------------------------------------------------------------------------------------------
//void MenuFun1()
//{
//	system("cls"); 
//	gotoxy(25, 10);
//	cout << "You have selected menu option (#1)" << endl;
//	_getch();
//	system("cls");
//}
////-----------------------------------------------------------------------------------------------
//void MenuFun2()
//{
//	system("cls");
//	gotoxy(25, 10);
//	cout << "You have selected menu option (#2)" << endl;
//	_getch();
//	system("cls");
//}
////-----------------------------------------------------------------------------------------------
//void MenuFun3()
//{
//	system("cls");
//	gotoxy(25, 10);
//	cout << "You have selected menu option (#3)" << endl;
//	_getch();
//	system("cls");
//}
//
//
////-----------------------------------------------------------------------------------------------
//void MenuFun20()
//{
//	system("cls"); 
//	gotoxy(25, 10);
//	
//	//Вход через почту
//	
//	gotoxy(25, 11);
//	unique_ptr<User> user(new User);
//
//	user->UserSignIn();
//
//	_getch(); 
//	system("cls");
//}
////-----------------------------------------------------------------------------------------------
//void MenuFun21()
//{
//	system("cls");
//	gotoxy(25, 10);
//	unique_ptr<Student> st(new Student);
//
//	st->Registration();
//	
//	_getch();
//	system("cls");
//}
//
////-----------------------------------------------------------------------------------------------
//void ExitOption()
//{
//	gotoxy(30, 15);
//	cout << "Exitting..." << endl;
//	_getch();
//	exit(0);
//}
////-----------------------------------------------------------------------------------------------
//void ChangeCursorStatus(bool Visible)
//{
//	CONSOLE_CURSOR_INFO* c = new CONSOLE_CURSOR_INFO;
//	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
//
//	if (Visible)
//	{
//		c->bVisible = TRUE;
//		c->dwSize = 0;
//	}
//	else
//	{
//		c->bVisible = FALSE;
//		c->dwSize = 1;
//	}
//
//	SetConsoleCursorInfo(h, c);
//}
////-----------------------------------------------------------------------------------------------