#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
#include <string>
#include <windows.h> 
#include <conio.h>
#include <fstream>
#include <experimental/filesystem>

#include"MyExceptions.h"
#include"User.h"
#include"Function.h"
#include"md5.h"


using namespace std;

namespace fs = std::experimental::filesystem;


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	//bool check = ;

	if (!CheckAdmin())
	{		
		unique_ptr<Admin> admin(new Admin);		 
		admin->Registration();
		system("cls");		
	}

	unique_ptr<Student> student(new Student);
	fs::create_directories(student->GetUserFolder());
	
	unique_ptr<Menu> menu;		
	shared_ptr<Factory> main_menu;
	shared_ptr<SignFactory> ma (new SignFactory);
	main_menu = ma;
	menu = main_menu->runMenu();
	menu->printMenu();
	
	return 0;
}
