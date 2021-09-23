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

	if (!check)
	{		
		unique_ptr<Admin> ad(new Admin);
		 
		ad->RegistrationAdmin();
		system("cls");
		/*unique_ptr<Menu> menu;
		Factory* main_menu;

		AminFactory ma;

		main_menu = &ma;

		menu = main_menu->runMenu();

		menu->printMenu();*/
		
		
		////Меню Админа
		//unique_ptr<Menu> menu;
		//
		//shared_ptr<Factory> main_menu;

		//shared_ptr<AdminFactory> ma (new AdminFactory);

		//main_menu = ma;

		//menu = main_menu->runMenu();

		//menu->printMenu();
	}

	
		unique_ptr<Menu> menu;
		
		shared_ptr<Factory> main_menu;

		shared_ptr<SignFactory> ma (new SignFactory);

		main_menu = ma;

		menu = main_menu->runMenu();

		menu->printMenu();
	
	
	return 0;
}
