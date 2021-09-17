#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include"MyExceptions.h"
#include"Menu.h"
//#include"Function.h"

using namespace std;

void gotoxy(int, int);

class User
{
public:
	User() { login = "Зарегистрируйтесь, или войдите"; }
	User(string login, string pass) : login(login), pass(pass) {}
	
	//virtual void Registration() = 0;
	
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

inline void Admin::RegistrationAdmin()
{
	
	cout << "Регистрация администратора системы тестирования\n";	

	ofstream fout;

	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		string path = Admin::filename;

		fout.open(path);

		cout << "Введите логин (e-mail): ";
		string login;
		cin >> login;
		fout << login <<"\n";
		
		cout << "Введите пароль: ";
		string pass;
		cin >> pass;
		fout << pass;

		fout.close();		
	}

	catch (const ifstream::failure& ex)
	{
		cout << ex.what() << "Код ошибки: " << ex.code()<< "\n";
		system("pause");
	}
}

void Student::Registration()
{
	cout << "Регистрация студента\n";

	gotoxy(25, 11);
	cout << "Введите логин (e-mail): ";

	cin >> this->login;

	ofstream fout;

	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		fout.open(this->folder + "/" + this->login + ".txt");
		fout << this->login << "\n";
		gotoxy(25, 12);
		cout << "Введите пароль: ";
		string pass;
		cin >> pass;
		fout << pass;

		fout.close();
	}

	catch (const ifstream::failure& ex)
	{
		gotoxy(25, 13);
		cout << ex.what() << "\nКод ошибки: " << ex.code() << "\n";
		system("pause");
	}
}