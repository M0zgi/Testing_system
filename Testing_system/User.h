#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include"MyExceptions.h"
#include"Menu.h"

using namespace std;

class User
{
public:
	User() { login = "�����������������, ��� �������"; }
	User(string login) : login(login) {}
	//~User();

protected:
	string login;
	string name;

};

class Admin: public User
{
public:
	Admin() {};

	~Admin();

	friend bool CheckAdmin();
	
private:
	
	static string filename;
};

string Admin::filename = "admin.txt";

//inline void Admin::CheckAdmin()
//{
//	ifstream fin;	
//
//	//fin.exceptions(ifstream::badbit | istream::failbit);
//
//	try
//	{
//		fin.open(filename);
//
//		if (!fin.is_open())
//		{
//			throw ExceptionAdmin("���� ������������ �� ������", 1);
//		}
//		
//		fin.close();
//	}	
//
//	catch (ExceptionAdmin& ex)
//	{
//	
//		cout << ex.what() << ex.GetError() << "\n";
//		cout << "� ������� ��� ������������������� ��������������.\n";
//		cout << "�������� �����������";
//		system("pause");
//	}
//		
//}

Admin::~Admin()
{
}