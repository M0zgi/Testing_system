#include <iostream>
#include <string>
#include <windows.h> 
#include <conio.h>
#include <fstream>

#include"MyExceptions.h"
#include"User.h"
#include"Function.h"

using namespace std;



				   
//-----------------------------------------------------------------------------------------------
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//Admin admin;

	//admin.CheckAdmin();	
	
	bool check = CheckAdmin();

	if (check)
	{
		unique_ptr<Menu> menu;
		Factory* main_menu;

		AminFactory ma;

		main_menu = &ma;

		menu = main_menu->runMenu();

		menu->printMenu();
	}

	else
	{		
		cout << "Регатся";	
	}
	
	return 0;
}
