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
	try
	{
		string path = Admin::filename;
		ifstream fin(path);

		if (fin.fail())
		{
			throw ExceptionAdmin("Файл конфигурации не найден. ", 1);
		}

		fin.close();
		return true;
	}

	catch (ExceptionAdmin& ex)
	{		
		gotoxy(25, 7);
		cout << ex.what() << "Код ошибки: " << ex.GetError() << "\n";
		gotoxy(25, 8);
		SetColor(Red, Black);
		cout << "В системе нет зарегистрированного администратора.\n";
		SetColor(15, Black);
		gotoxy(25, 9);
		cout << "Пройдите регистрацию" << "\n";

		return false;
	}

	catch (...)
	{
		gotoxy(25, 16);
		cout << "Ошибка в функции bool CheckAdmin()";
	}
}

//------------------------------------------------

string* MenuItems(string user); // функция, которая возвращает имя пункта меню.

void gotoxy(int, int); //с помощью этой функции вы можете перейти в любую позицию на экране строки cmd.

void ChangeCursorStatus(bool);

void AdminFun1(); // #1. Создание, удаление, модификация пользователей.
void AdminFun2(); // #2. Просмотр статистики."
void AdminFun3(); // #3. Управление тестированием.";
void AdminFun4(); // #4. Изменение логина и пароля администратора";

void MenuFun20(); //  пункт меню входа пользователя в систему
void MenuFun21(); // пункт меню регистрации пользователя в системе

void StudentFun1(); //#1. Сдать новое тестирование.";
void StudentFun2(); //#2. Продолжить тестирование
void StudentFun3(); //#3 Просмотреть список пройденных тестов

void ExitOption(); // элемент выхода


//Отображение текущего имени пользователя
class UserName
{
public:
	
	UserName() {};
	UserName(string user, string fio, int ItemCount) : user(user), fio(fio), ItemCount(ItemCount) {}
	~UserName() {};

	void SetUser(string user) { this->user = user; }
	void SetItemCount(int ItemCount) { this->ItemCount = ItemCount; }
	void SetFIO(string fio) { this->fio = fio; }

	string GetUser() { return this->user; }
	string GetFIO() 
	{
		unique_ptr<Student> student(new Student);

		string path = student->GetUserLogins() + "/" + md5(user) + ".txt";
		
		ifstream sfin;
		sfin.exceptions(ifstream::badbit | ifstream::failbit);

		if (user != "Зарегистрируйтесь, или войдите" && user != "admin")
		{
			try
			{
				sfin.open(path);
				int count = -3;
				while (count)
				{					
					getline(sfin, fio);					
					count++;
				}

				sfin.close();

			}
			catch (const ifstream::failure& ex)
			{
				gotoxy(25, 13);
				cout << ex.what() << "\nКод ошибки: " << ex.code() << "\n";
				system("pause");
			}
		}		

		return fio;
	};

private:

	string user = "Зарегистрируйтесь, или войдите";
	string fio = "Sign in / Sign up";
	int ItemCount = 3;
}; 

unique_ptr<UserName> userName (new UserName);


void UseMenu(string user, int ItemCount)
{
	ChangeCursorStatus(false);

	int MenuChoice = 1; // Эта переменная содержит позицию курсора. 
	char key; // для ввода ключа (стрелка вверх, стрелка вниз и т. д.);

	typedef void (*TMenuOption)(); // typedef for defining a 'pointer to function' type.

	TMenuOption* MenuOption = new TMenuOption[ItemCount];// массив указателей на функции (динамический).

	if (user == "admin")
	{		
		MenuOption[0] = AdminFun1; // заполнение массива функциями.
		MenuOption[1] = AdminFun2;
		MenuOption[2] = AdminFun3;
		MenuOption[3] = AdminFun4;
		MenuOption[4] = ExitOption;
	}

	else if (user == "sign")
	{		
		MenuOption[0] = MenuFun20;
		MenuOption[1] = MenuFun21;
		MenuOption[2] = ExitOption;
	}

	else 
	{		
		MenuOption[0] = StudentFun1;
		MenuOption[1] = StudentFun2;
	    MenuOption[2] = StudentFun3;
		MenuOption[3] = ExitOption;		
	}


	while (1) // этот цикл не прервется, поэтому нам нужна функция выхода.
	{
		
		for (int i = 0; i < ItemCount; i++) // вывод Меню.
		{
			
			gotoxy(23, 7 + i);
			MenuChoice == i + 1 ? cout << " -> " : cout << "    ";

			cout << MenuItems(user)[i] << endl; // вывод названий Меню
		}

		gotoxy(0, 0);
		cout << "Пользователь: " << userName->GetUser() << " ("<< userName->GetFIO() << ")";

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
				cout << "catch (...) Function.h";
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
		item[3] = "#4. Изменение логина и пароля администратора";
		item[4] = "#5. Выход.";
		return item;
	}	

	else if (user == "sign" )
	{		
		string* item = new string[3];
		item[0] = "#1. Вход c помощью почты.";
		item[1] = "#2. Регистрация.";		
		item[2] = "#3. Выход.";
		return item;
	}

	else
	{
		string* item = new string[4];
		item[0] = "#1. Пройти новое тестирование.";
		item[1] = "#2. Продолжить незавершенное тестирование.";
		item[2] = "#3. Просмотреть результаты тестирования.";
		item[3] = "#4. Выход.";
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
	char key = '1'; // для ввода ключа (стрелка вверх, стрелка вниз и т. д.);
	//char key = _getch();
	do 
	{
		system("cls");
		gotoxy(0, 0);
		cout << "Пользователь: " << userName->GetUser() << " (" << userName->GetFIO() << ")";
		int gotx = 6;

		gotoxy(25, ++gotx);

		cout << "Создание, удаление, модификация пользователей." << endl;
		gotoxy(25, ++gotx);
		cout << "#1. Создание";
		gotoxy(25, ++gotx);
		cout << "#2. Удаление";
		gotoxy(25, ++gotx);
		cout << "#3. Модификация";
		gotoxy(25, ++gotx);
		cout << "#4. Вывод всех пользователей";
		gotoxy(25, ++gotx);
		cout << "#0. Выход";

		gotoxy(25, ++gotx);
		cout << "Укажите пункт меню: ";
		
		cin >> key;

		switch (key) // проверка введенного ключа.
		{
		case '1': // Создание.			
		{			
			unique_ptr<Student> st(new Student);
			st->Registration();
			//_getch();
			break;
		}		

		case '2': // Удаление
		{			
			unique_ptr<Admin> ad(new Admin);
			ad->DeleteUser();			
			break;
		}			

		case '3': // Модификация
		{
			unique_ptr<Admin> ad(new Admin);
			ad->Modification();			
			break;
		}	

		case '4': // Вывод на экран списка зарегистрированных пользователей
		{
			system("cls"); 
			unique_ptr<Admin> ad(new Admin);
			ad->PrintStudentsFull();

			_getch();
			break;
		}

		case '0': // Выход
		{			
			break;
		}
			
		default:
			break;
		}
	} while (key!='0');


	//_getch();
	system("cls");
}
//-----------------------------------------------------------------------------------------------
void AdminFun2()
{
	
}
//-----------------------------------------------------------------------------------------------
void AdminFun3()
{
	char key = '1'; 
	//key = _getch();
	do
	{
		system("cls");
		gotoxy(0, 0);
		cout << "Пользователь: " << userName->GetUser() << " (" << userName->GetFIO() << ")";

		gotoxy(25, 7);

		cout << "Управление тестированием." << endl;
		gotoxy(25, 8);
		cout << "#1. Создание категорий тестирования";
		gotoxy(25, 9);
		cout << "#2. Редактирование категорий тестирования";
		gotoxy(25, 10);
		cout << "#3. Создание тестов";
		gotoxy(25, 11);
		cout << "#4. Создание названий тестов в категориях";
		gotoxy(25, 12);
		cout << "#0. Выход";

		gotoxy(25, 13);
		cout << "Укажите пункт меню: ";
		
		cin >> key;

		switch (key) // проверка введенного ключа.
		{
		case '1': // Создание разделов			
		{
			unique_ptr<Admin> ad(new Admin);
			ad->AddCategories();
			break;
		}

		case '2': // Редактирование разделов	
		{
			unique_ptr<Admin> ad(new Admin);
			ad->EditCategories();
			break;
		}

		case '3': // Создание тестов
		{
			unique_ptr<Admin> ad(new Admin);
			ad->AddTests();
			break;
		}

		case '4': // Редактирование тестов
		{
			unique_ptr<Admin> ad(new Admin);

			ad->AddTestsName();

			break;
		}

		case '0': // Выход
		{
			break;
		}

		default:
			break;
		}
	} while (key != '0');


	//_getch();
	system("cls");
}

void AdminFun4()
{
	system("cls");

	gotoxy(0, 0);
	cout << "Пользователь: " << userName->GetUser() << " (" << userName->GetFIO() << ")";;

	unique_ptr<Admin>admin (new Admin);

	admin->ChangeLogin();
	
	//_getch();
	system("cls");
}


//-----------------------------------------------------------------------------------------------
void MenuFun20() //вход пользователя в систему
{
	system("cls");

	gotoxy(0, 0);
	cout << "Пользователь: " << userName->GetUser() << " (" << userName->GetFIO() << ")";;
	gotoxy(25, 7);
	
	//Вход через почту
	unique_ptr<Student> user(new Student);

	string name;

	name = user->UserSignIn();

	if (name == "admin")
	{
		userName->SetUser("admin");
		userName->SetFIO("Администратор системы");
		unique_ptr<Menu> menu;
		shared_ptr<Factory> main_menu;
		shared_ptr<AdminFactory> ma(new AdminFactory);
		main_menu = ma;
		menu = main_menu->runMenu();
		menu->printMenu();
	}

	else if (name == userName->GetUser())
	{
		gotoxy(25, 12);
		cout << "Вход в систему не удался";
	}

	else
	{
		userName->SetUser(name);
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
	cout << "Пользователь: " << userName->GetUser() << " (" << userName->GetFIO() << ")";;

	gotoxy(25, 7);
	unique_ptr<Student> st(new Student);
	st->Registration();

	//_getch();
	system("cls");
}

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void StudentFun1() // регистрация пользователя
{
	system("cls");

	gotoxy(0, 0);
	cout << "Пользователь: " << userName->GetUser() << " (" << userName->GetFIO() << ")";

	unique_ptr<Student> student(new Student);
	student->SetLogin(userName->GetUser());

	student->NewTest();

	gotoxy(25, 7);
	//cin.ignore();
	//_getch();
	system("cls");
}

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void StudentFun2() // 
{
	system("cls");

	gotoxy(0, 0);
	cout << "Пользователь: " << userName->GetUser() << " (" << userName->GetFIO() << ")";;

	gotoxy(25, 7);
	unique_ptr<Student> student(new Student);
	student->SetLogin(userName->GetUser());
	student->ContinueTest();

	//_getch();
	system("cls");
}

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void StudentFun3() // 
{
	system("cls");

	gotoxy(0, 0);
	cout << "Пользователь: " << userName->GetUser() << " (" << userName->GetFIO() << ")";;

	unique_ptr<Student> student(new Student);
	student->SetLogin(userName->GetUser());
	student->ShowUserGrade();

	//gotoxy(25, 7);

	//_getch();
	system("cls");
}

//-----------------------------------------------------------------------------------------------
void ExitOption()
{
	gotoxy(30, 15);
	cout << "Выход..." << endl;
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