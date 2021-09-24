#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
//#include <filesystem>



#include"MyExceptions.h"
#include"Menu.h"
#include"Encryption.h"
#include"md5.h"
//#include"Function.h"


using namespace std;



void gotoxy(int, int);

class User
{
public:
	User() { login = "Зарегистрируйтесь, или войдите"; }
	User(string login, string pass) : login(login), pass(pass) {}
	
	//virtual void Registration() = 0;

	string GetLogin() { return this->login; }

	void UserSignIn();
	
	~User() {};

protected:
	string login;
	string pass;

};

class Admin: public User
{
public:
	Admin() {};

	~Admin() {};

	string GetFilename() { return filename; }

	friend bool CheckAdmin();

	void RegistrationAdmin();
	
private:
	
	static string filename;
};

string Admin::filename = "admin.txt";

class Student : public User
{
public:
	Student() {};
	~Student() {};

	void Registration();

private:
	string filename;
	static string folder; //имя папки для сохранения регистрационных данных студентов
};

string Student::folder = "StudentLogin";

void User::UserSignIn()
{
	cout << "Вход в систему\n";

	gotoxy(25, 11);
	cout << "Введите логин (e-mail): ";
	cin >> this->login ;

	unique_ptr<Admin> ad(new Admin);

	string path = ad->GetFilename();

	if (this->login + ".txt" == path)
	{
		system("cls");
		gotoxy(25, 11);
		cout << "Введите логин (e-mail) администратора системы: ";
		string adminlogin, md5adminlogin;
		cin >> this->login;

		cout << "Введите пароль администратора системы: ";
		string adminpass, md5adminpass;
		cin >> this->pass;

		md5adminlogin = md5(this->login);
		md5adminpass = md5(this->pass);

		ifstream afin;

		afin.exceptions(ifstream::badbit | ifstream::failbit);

		try
		{
			afin.open(path);
			//afin >> adminlogin;
			int count = 0;
			while (!afin.eof())
			{				
				if (count)
				getline(afin, adminpass);
				else
				getline(afin, adminlogin);
				count++;
			}	

			afin.close();
		}
		catch (const ifstream::failure& ex)
		{
			gotoxy(25, 13);
			cout << ex.what() << "\nКод ошибки: " << ex.code() << "\n";
			system("pause");
		}

		if (md5adminlogin == adminlogin && md5adminpass == adminpass)
		{
			unique_ptr<Menu> menu;
			shared_ptr<Factory> main_menu;
			shared_ptr<AdminFactory> ma(new AdminFactory);
			main_menu = ma;
			menu = main_menu->runMenu();
			menu->printMenu();
		}

		else
		{
			cout << "Ошибка входа";
			//this->login = "sign";
		}	
	}
	
}

inline void Admin::RegistrationAdmin()
{	
	cout << "Регистрация администратора системы тестирования\n";	

	ofstream fout;

	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		string path = Admin::filename;

		fout.open(path);
		
		cout << "Введите логин администратора: ";
		string login;
		cin >> login;
		string md5login;
		md5login = md5(login);
		fout << md5login << "\n";

		cout << "Введите пароль администратора: ";
		string pass;
		cin >> pass;
		string md5pass;
		md5pass = md5(pass);
		fout << md5pass;

		fout.close();		
	}

	catch (const ifstream::failure& ex)
	{
		cout << ex.what() << "Код ошибки: " << ex.code()<< "\n";
		system("pause");
	}

	catch (...)
	{
		cout << "Catch Admin::RegistrationAdmin()\n";
	}
}

void Student::Registration()
{
	cout << "Регистрация студента\n";

	gotoxy(25, 11);
	cout << "Введите логин (e-mail): ";
	cin >> this->login;

	string path = this->folder + "/" + md5(this->login) + ".txt";
	ifstream fin(path);

	//проверяем есть ли пользователь, если нет продолжаем регистрацию
	bool ckfile = fin.fail();
	
	//проверяем что бы введенный логин не совпадал с логином админа
	unique_ptr<Admin> admin(new Admin);
	path = admin->GetFilename();

	try
	{		
		//проверка на логин администратора системы

		if (this->login + ".txt" == path)
		{
			throw ExceptionUser("Регистрация с веденным логином запрещена", 2);
		}

		//проверяем есть ли пользователь с таким логином
		if (ckfile)
		{
			ofstream fout;
			fout.exceptions(ofstream::badbit | ofstream::failbit);			
			fout.open(this->folder + "/" + md5(this->login) + ".txt");

			string hash = md5(this->login);
			fout << hash << "\n";

			gotoxy(25, 12);
			cout << "Введите пароль: ";
			string pass;
			cin >> pass;				

			//сохранение зашифрованного пароля
			hash = md5(pass);
			fout << hash;

			gotoxy(25, 13);
			cout << "Регистрация завершена. Нажмите любую кнопку." << endl;
			fout.close();			
		}

		else
		{
			throw ExceptionUser("Пользователь с таким логином существует. \nВойдите в систему используя указанный логин. \nИли пройдите регистрацию используя другой e-mail" , 3);
		}
	}

	catch (const ofstream::failure& ex)
	{
		gotoxy(25, 13);
		cout << ex.what() << "\nКод ошибки: " << ex.code() << "\n";
		system("pause");
	}

	catch (ExceptionUser& ex)
	{
		gotoxy(25, 13);
		cout << ex.what() << "\nКод ошибки: " << ex.GetError() << "\n";
		system("pause");
	}

	catch (...)
	{	
		gotoxy(25, 13);
		cout << "Ошибка в Student::Registration()";
		system("pause");
	}
}