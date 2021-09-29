#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include<map>
#include <experimental/filesystem>
#include <ctime>
#include<windows.h>
//#include <filesystem>

#include"MyExceptions.h"
#include"Menu.h"
#include"Encryption.h"
#include"md5.h"
//#include"Function.h"

using namespace std;

namespace fs = std::experimental::filesystem;

void gotoxy(int, int);
const string currentDateTime();

enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};



HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(int text, int background)
{	
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void SetColor(int text, ConsoleColor background)
{	
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

class User
{
public:
	
	User() { login = "Зарегистрируйтесь, или войдите"; }
	User(string login, string pass) : login(login), pass(pass) {}
	
	virtual void Registration() = 0;
	
	void SetLogin(string login) { this->login = login; }

	string GetLogin() { return this->login; }
	string GetUserLogins() { return this->folder; } //геттер папки для хранения регистрационных данных пользователей
	string UserSignIn();
	string GetUsersFolder() { return this->UsersFolder; }
	string GetUsersInfo() { return this->usersinfo; }
	string GetTestFolder() { return TestFolder; }
	string GetTestsName() { return TestsName; }
	string GetCategoriesFile() { return CategoriesFile; }
	string GetStudentsFile() { return StudentsFile; }

	~User() {};

protected:


	string login;
	string pass;
	string usersinfo = "UsersInfo.txt";
	string UsersFolder = "UsersFolder"; // папка для хранения реузльтатов тестирования студентов
	static string folder; //имя папки для сохранения регистрационных данных студентов
	string TestFolder = "TestFolder"; // папка для хранения вопросов тестирования
	string TestsName = "TestsName.txt"; // файл в котором хранятся названия тестов в категории
	string CategoriesFile = "CategoriesFile.txt"; // файл в котором хранятся категории с тестами
	string StudentsFile = "StudentsFile.txt"; // файл в котором хранятся логины и ФИО студентов	
};

string User::folder = "StudentLogin";

class Admin: public User
{
public:
	Admin() {};
	~Admin() {};

	string GetFilename() { return filename; } //геттер названия файла с логином и паролем администратора системы
	
	

	void ChangeLogin(); //изменение логина и пароля администратора  системы
	void DeleteUser();//удаление пользовтаеля
	void Modification();//модификация пользовтаеля
	void PrintStudents();//вывод всех зарегистрированных пользователей системы тестирования кратко
	void PrintStudentsFull();//вывод всех анктетных данных пользователей системы тестирования

	void AddCategories();//добавление категорий для тестов
	void EditCategories();//редкатирование категорий для тестов
	void AddTestsName();//добавление названий тестов
	void AddTests();//добавление тестов

	friend bool CheckAdmin();

	void Registration() override;
	
private:	
	static string filename;
	
};

string Admin::filename = "admin.txt";

class Student : public User
{
public:
	Student() {};
	~Student() {};
	string GetStudentName() { return this->name; }

	void Registration() override;

	void NewTest(); // запуск нового тестирования

private:
	string filename;
	string name;
	string telephone;
	string adress;
};

string User::UserSignIn()
{
	cout << "Вход в систему\n";
	//unique_ptr<Student> student(new Student);

	int gotx = 6;
	gotoxy(25, ++gotx);
	cout << "Введите логин (e-mail): ";
	cin >> this->login;	

	unique_ptr<Admin> ad(new Admin);

	string path = ad->GetFilename();

	if (this->login + ".txt" == path)
	{
		system("cls");
		gotoxy(25, ++gotx);
		cout << "Введите логин (e-mail) администратора системы: ";
		string adminlogin, md5adminlogin;
		cin >> ad->login;
		gotoxy(25, ++gotx);
		cout << "Введите пароль администратора системы: ";
		string adminpass, md5adminpass;
		cin >> ad->pass;

		md5adminlogin = md5(ad->login);
		md5adminpass = md5(ad->pass);

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
			gotoxy(25, ++gotx);
			cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
			system("pause");
		}

		if (md5adminlogin == adminlogin && md5adminpass == adminpass)
		{
			return "admin";
		}

		else
		{
			gotoxy(25, ++gotx);
			cout << "Ошибка входа";
			return this->login;
		}	
	}

	else 
	{		
		path = this->folder + "/" + md5(this->login) + ".txt";

		try
		{
			ifstream ufin(path);
			ufin.exceptions(ifstream::badbit | ifstream::failbit);			
		}
		catch (...)
		{			
			
			gotoxy(25, ++gotx);
			cout << "Пользователь с таким логином не найден.";
			gotoxy(25, ++gotx);
			system("pause");
			//return this->login;
			return "Зарегистрируйтесь, или войдите";
		}

		ifstream ufin(path);
		ufin.exceptions(ifstream::badbit | ifstream::failbit);

		//проверяем есть ли пользовательский файл с веденным логином

		bool ckfile = ufin.is_open();

		//bool ckfile = ufin.open(path);

		try
		{
			//проверяем есть ли пользователь с таким логином
			if (ckfile)
			{				
				gotoxy(25, ++gotx);
				cout << "Введите пароль: ";
				string userpass, md5userpass;
				cin >> this->pass;

				md5userpass = md5(this->pass);

				int count = -2;
				while (count)
				{					
					getline(ufin, userpass);					
					count++;
				}

				if (md5userpass == userpass)
				{
					gotoxy(25, ++gotx);
					cout << "Вы успешно вошли в систему тестирования." << endl;
					gotoxy(25, ++gotx);
					system("pause");
					cin.ignore();
					ufin.close();
					return this->GetLogin();
				}

				else
				{
					throw ExceptionUser("Вы ввели неверный пароль пользователя", 4);
					//return this->login;
				}				
			}
		}

		catch (const ofstream::failure& ex)
		{
			gotoxy(25, ++gotx);
			cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
			gotoxy(25, ++gotx);
			system("pause");
		}

		catch (ExceptionUser& ex)
		{
			gotoxy(25, ++gotx);
			cout << ex.what() << " Код ошибки: " << ex.GetError() << "\n";
			gotoxy(25, ++gotx);
			system("pause");
			
			return "Зарегистрируйтесь, или войдите";
		}

		catch (...)
		{
			gotoxy(25, ++gotx);
			cout << "Ошибка в string User::UserSignIn()";
			gotoxy(25, ++gotx);
			system("pause");
		}
		
		ufin.close();
		return this->login;;
	}	
}

inline void Admin::Registration()
{		

	int gotx = 10;
	gotoxy(25, ++gotx);
	cout << "Регистрация администратора системы тестирования";	

	ofstream fout;
	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		string path = Admin::filename;

		
		
		gotoxy(25, ++gotx);
		cout << "Логин администратора:";
		SetColor(Red, Black); 
		cout<< "admin";
		SetColor(15, Black);
		string login = "admin";
		//cin >> login;
		string md5login;
		md5login = md5(login);
		
		gotoxy(25, ++gotx);
		cout << "Введите пароль администратора: ";
		string pass;
		cin >> pass;
		string md5pass;
		md5pass = md5(pass);
		
		fout.open(path);
		fout << md5login << "\n";
		fout << md5pass;
		fout.close();		
	}

	catch (const ifstream::failure& ex)
	{		
		gotoxy(25, ++gotx);
		cout << ex.what() << " Код ошибки: " << ex.code()<< "\n";
		system("pause");
	}

	catch (...)
	{
		gotoxy(25, ++gotx);
		cout << "Catch Admin::RegistrationAdmin()\n";
	}

	gotx++;
	gotoxy(25, ++gotx);
	cout << "Администратор успешно зарегистрирован. Можете войти в систему с веденными данными";
	gotx++;
	gotoxy(25, ++gotx);
	system("pause");
}

inline void Admin::ChangeLogin()
{
	int gotx = 6;
	
	ofstream fout;
	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{		
		gotoxy(25, ++gotx);
		cout << "Изменение логина и пароля администратора системы тестирования\n";
		string path = Admin::filename;		

		gotoxy(25, ++gotx);
		cout << "Введите новый логин администратора: ";
		string login;
		cin >> login;
		string md5login;
		md5login = md5(login);
		
		gotoxy(25, ++gotx);
		cout << "Введите новый пароль администратора: ";
		string pass;
		cin >> pass;
		string md5pass;
		md5pass = md5(pass);
		
		//сохраняем в файл введенный логи и пароль
		fout.open(path);
		fout << md5login << "\n";
		fout << md5pass;
		fout.close();

		gotoxy(25, ++gotx);
		cout << "Операция успешна." << endl;
		gotoxy(25, ++gotx);
		system("pause");
		
	}

	catch (const ifstream::failure& ex)
	{
		gotoxy(25, ++gotx);
		cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
		gotoxy(25, ++gotx);
		system("pause");
	}

	catch (...)
	{
		gotoxy(25, ++gotx);
		cout << "Catch Admin::ChangeLogin()\n";
	}
}

inline void Admin::DeleteUser()
{
	//unique_ptr<Admin> admin(new Admin);	
	map <string, string> mp;
	map <string, string> mp_users;
	string path = this->GetUsersFolder() + "/" + this->GetStudentsFile();

	//папка для хранения личных данных в одном файле по каждому пользоввателю
	string path2 = this->GetUsersFolder() + "/" + this->GetUsersInfo();

	string key, data, key_u, data_u;
	ifstream ifs;
	int gotx = 6;
	system("cls");

	gotoxy(25, ++gotx);
	cout << "Удаление пользователей системы тестирования";
	try
	{				
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			cout << "Ошибка открытия файла Admin::DeleteUser()";
		}

		while (!ifs.eof())
		{
			key = "";
			data = "";
			getline(ifs, key);
			if (key != "")
				getline(ifs, data);

			if (key != "")
				mp[key] = data;
		}
		ifs.close();

		ifs.open(path2);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			cout << "Ошибка открытия файла Admin::DeleteUser() path2";
		}

		while (!ifs.eof())
		{
			key_u = "";
			data_u = "";
			getline(ifs, key_u);
			if (key_u != "")
				getline(ifs, data_u);

			if (key_u != "")
				mp_users[key_u] = data_u;
		}
		ifs.close();

		this->PrintStudents();

		gotoxy(25, ++gotx);
		cout << "Введите мобильный номер студента для удаления: ";
		cin >> key;	

		auto it = mp.find(key);
		
		if (it != mp.end())
		{
			system("cls");	
			int gotx = 6;
			gotoxy(25, ++gotx);
			cout << "Удаление пользователей системы тестирования";
					
			gotoxy(25, ++gotx);
			cout << "Студент найден в базе: " << it->first << " " << it->second << endl;
			gotoxy(25, ++gotx);
			cout << "Подтвердите логин студента для удаления: ";
			string login;
			cin >> data;
			login = this->GetUserLogins() + "/" + md5(data) + ".txt";

			string userfolder = this->GetUsersFolder() + "/" + data + "/";

			if (remove(login.c_str()) != 0)
			{
				gotoxy(25, ++gotx);
				perror("Ошибка удаления файла с логином студента");
			}
				
			else
			{
				gotoxy(25, ++gotx);
				puts("Оперция удаления студента успешно завершена");

				if (remove(path.c_str()) != 0)
				{
					gotoxy(25, ++gotx);
					perror("Ошибка удаления файла с базой студентов");
				}


				else
				{
					gotoxy(25, ++gotx);
					puts("Оперция удаления старого файла базы студентов успешно завершена");
					mp.erase(key);
					mp_users.erase(key);
				}		

				std::uintmax_t n = fs::remove_all(userfolder.c_str());


			}		



			/*cout << endl;

			for (auto it = mp.begin(); it != mp.end(); ++it)
			{
				cout << it->first << " : " << it->second << endl;
			}*/

			ofstream ofs;
			ofs.open(path, ostream::app);

			for (auto it = mp.begin(); it != mp.end(); ++it)
			{
				ofs << it->first << "\n" << it->second << "\n";
			}

			ofs.close();	

			ofs.open(path2);

			for (auto it = mp_users.begin(); it != mp_users.end(); ++it)
			{
				ofs << it->first << "\n" << it->second << "\n";
			}

			ofs.close();

			gotoxy(25, ++gotx);
			system("pause");
		}
		
		else
		{
			system("cls");
			gotoxy(25, ++gotx);
			cout << "Cтудент с указанным номером телефона не найден в базе.";
			gotoxy(25, ++gotx);
			system("pause");
		}		
	}

	catch (...)
	{		
		system("cls");
		gotoxy(25, ++gotx); 
		cout << "Catch Admin::DeleteUser()";
		gotoxy(25, ++gotx);
		system("pause");
	}
}

inline void Admin::PrintStudents()
{
	//unique_ptr<Admin> admin(new Admin);
	map <string, string> mp;
	string path = this->GetUsersFolder() + "/" + this->GetStudentsFile();
	string key, data;
	ifstream ifs;
	int gotx = 10;

	ifs.open(path);

	if (!ifs.is_open())
	{
		gotoxy(25, ++gotx);
		cout << "Ошибка открытия файла Admin::PrintStudents()";
	}

	while (!ifs.eof())
	{
		key = "";
		data = "";
		getline(ifs, key);
		if (key != "")
			getline(ifs, data);

		if (key != "")
			mp[key] = data;
	}
	ifs.close();

	gotoxy(25, ++gotx);
	cout << "Список зарегистрированных студентов в базе (номмер телефона, ФИО, логин)\n";

	for (auto it = mp.begin(); it != mp.end(); ++it)
	{
		gotoxy(25, ++gotx);
		cout << "Номер телефона: " << it->first << " " << it->second << "\n";
	}
}

inline void Admin::PrintStudentsFull()
{
	system("cls");
	string path = this->GetUsersFolder() + "/" + this->GetUsersInfo();

	ifstream ifs;
	int gotx = 6;

	ifs.open(path);

	if (!ifs.is_open())
	{
		gotoxy(25, ++gotx);
		cout << "В системе нет зарегистрированных пользователей";
	}

	else
	{
		gotoxy(25, ++gotx);
		cout << "Список зарегистрированных студентов в базе (ФИО, номмер телефона, домашний адрес)\n";

		string str;
		int count = 0;

		while (!ifs.eof())
		{
			str = "";
			getline(ifs, str);
			cout << str;		
		}
	}

	ifs.close();
}

inline void Admin::Modification()
{
	//unique_ptr<Admin> admin(new Admin);
	map <string, string> mp_users;
	map <string, string> mp;
	string path = this->GetUsersFolder() + "/" + this->GetStudentsFile();

	//папка для хранения личных данных в одном файле по каждому пользоввателю
	string path2 = this->GetUsersFolder() + "/" + this->GetUsersInfo();

	string key, data, key_u, data_u;
	ifstream ifs;
	int gotx = 6;
	system("cls");
	gotoxy(25, ++gotx);
	cout << "Изменение регистрационных данных пользователей системы тестирования";
	try
	{		
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			cout << "Ошибка открытия файла Admin::DeleteUser()";
		}

		while (!ifs.eof())
		{
			key = "";
			data = "";
			getline(ifs, key);
			if (key != "")
				getline(ifs, data);

			if (key != "")
				mp[key] = data;
		}
		ifs.close();

		ifs.open(path2);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			cout << "Ошибка открытия файла Admin::DeleteUser() path2";
		}

		while (!ifs.eof())
		{
			key_u = "";
			data_u = "";
			getline(ifs, key_u);
			if (key_u != "")
				getline(ifs, data_u);

			if (key_u != "")
				mp_users[key_u] = data_u;
		}
		ifs.close();

		this->PrintStudents();

		gotoxy(25, ++gotx);
		cout << "Введите мобильный номер студента для модификации: ";
		cin >> key;

		auto it = mp.find(key);

		if (it != mp.end())
		{

			int gotx = 6;
		
			system("cls");
			gotoxy(25, ++gotx);
			cout << "Изменение регистрационных данных пользователей системы тестирования";
			
			gotoxy(25, ++gotx);
			cout << "Студент найден в базе: " << it->first << " " << it->second << endl;
			gotoxy(25, ++gotx);
			cout << "Введите логин студента для модификации: ";
			string login;
			cin >> data;
			login = this->GetUserLogins() + "/" + md5(data) + ".txt";

			if (remove(login.c_str()) != 0)
			{
				gotoxy(25, ++gotx);
				perror("Ошибка удаления файла с логином студента");
			}
				
			else
			{
				gotoxy(25, ++gotx); 
				puts("Оперция удаления студента успешно завершена");
			}				

			if (remove(path.c_str()) != 0)
			{
				gotoxy(25, ++gotx);
				perror("Ошибка удаления файла с базой студентов");
			}
				
			else
			{
				gotoxy(25, ++gotx);
				puts("Оперция удаления старого файла базы студентов успешно завершена");
			}
			

			ofstream ofs;

			ofs.open(this->GetUserLogins() + "/" + md5(data) + ".txt");
			gotoxy(25, ++gotx);
			cout << "Логин студента (e-mail): " << data << "\n";
			ofs << md5(data) << "\n";
			gotoxy(25, ++gotx); 
			cout << "Введите пароль: ";
			string pass;
			cin >> pass;
			//сохранение зашифрованного пароля
			string hash = md5(pass);
			ofs << hash << "\n";
			cin.ignore();
			gotoxy(25, ++gotx);
			cout << "Введите ФИО: ";
			string name;
			getline(cin, name);
			//ofs << name << "\n";
			gotoxy(25, ++gotx);
			cout << "Введите номер мобильного телефона: +38";
			string telephone;
			getline(cin, telephone);
			//ofs << telephone << "\n";
			gotoxy(25, ++gotx);
			cout << "Введите домашний адрес: ";
			string adress;
			getline(cin, adress);
			//ofs << adress << "\n";
			system("cls");
			

			ofs.close();

			mp.erase(key);

			mp_users.erase(key);
			mp_users.emplace(telephone, "ФИО: " + name + ". Домашний адрес: " + adress);
			
			mp.emplace(telephone, name + " (логин: " + data + ")");

			cout << endl;

			/*for (auto it = mp.begin(); it != mp.end(); ++it)
			{
				cout << it->first << " : " << it->second << endl;
			}*/

			ofs.open(path, ostream::app);

			for (auto it = mp.begin(); it != mp.end(); ++it)
			{
				ofs << it->first << "\n" << it->second << "\n";
			}

			ofs.close();

			ofs.open(path2);

			for (auto it = mp_users.begin(); it != mp_users.end(); ++it)
			{
				ofs << it->first << "\n" << it->second << "\n";
			}

			ofs.close();
			
		}

		else
		{
			system("cls"); 
			gotoxy(25, ++gotx);
			cout << "Cтудент с указанным номером телефона не найден в базе.";
		}

		gotoxy(25, ++gotx);
		cout << "Изменение данных завершено. \n";
		gotoxy(25, ++gotx);
		system("pause");
	}

	catch (...)
	{
		system("cls");
		gotoxy(25, ++gotx);
		cout << "Catch Admin::Modification()";
		system("pause");
	}
}

inline void Admin::AddCategories()
{
	system("cls");	
	int gotx = 6;
	//unique_ptr<Admin> admin(new Admin);
	map <string, string> mp;
	string path = this->GetTestFolder() + "/" + this->GetCategoriesFile();
	string key, category;

	int count = 0; // счетчик кол-ва разделов

	ifstream ifs;

	try
	{
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			cout << "В системе еще нет созданных разделов тестирвоания\n";
		}

		else
		{
			while (!ifs.eof())
			{					
				getline(ifs, key);
				getline(ifs, category);

				if(key!="")
				mp[key] = category;	
				count++;
			}
			count--;
			gotoxy(25, ++gotx);
			cout << "В системе есть следующие разделы:\n";
		}

		ifs.close();			

		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			gotoxy(25, ++gotx);
			cout << it->first << ". " << it->second << "\n";
		}

		ofstream ofs;

		ofs.open(path , ofstream::app);
		cin.ignore();
		gotoxy(25, ++gotx);
		cout << "Введите название нового раздела: ";
		getline(cin, category);
		ofs << ++count << "\n";
		ofs << category << "\n";
		gotoxy(25, ++gotx);
		cout << "Изменение данных завершено. \n";

		ofs.close();

	}
	catch (...)
	{
		cout << "Ошибка в Admin::AddCategories()";
	}
	gotoxy(25, ++gotx);
	system("pause");
}

inline void Admin::EditCategories()
{	
	system("cls");
	int gotx = 6;
	//unique_ptr<Admin> admin(new Admin);
	map <string, string> mp;
	string path = this->GetTestFolder() + "/" + this->GetCategoriesFile();
	string key, category;

	int count = 0; // счетчик кол-ва разделов

	ifstream ifs;

	try
	{
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			cout << "В системе еще нет созданных разделов тестирования\n";
		}

		else
		{
			while (!ifs.eof())
			{
				getline(ifs, key);
				getline(ifs, category);

				if (key != "")
					mp[key] = category;
				count++;
			}
			count--;
			gotoxy(25, ++gotx);
			cout << "В системе есть следующие разделы:\n";
		}

		ifs.close();

		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			gotoxy(25, ++gotx);
			cout << it->first << ". " << it->second << "\n";
		}

		cin.ignore();
		gotoxy(25, ++gotx);
		cout << "Введите номер раздела для редактирования: ";
		string keyedit;
		getline(cin, keyedit);

		gotoxy(25, ++gotx);
		cout << "Введите новое название раздела: ";
		getline(cin, category);

		auto it = mp.find(keyedit);

		if (it != mp.end())
		{
			mp[keyedit] = category;
		}		

		if (remove(path.c_str()) != 0)
		{
			gotoxy(25, ++gotx);
			perror("Ошибка удаления файла с разделами тестирования");
		}
			
		else
		{
			gotoxy(25, ++gotx);
			puts("Оперция удаления старого файла с разделами тестирования успешно завершена\n");
		}			

		ofstream ofs;
		ofs.open(path, ostream::app);

		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			ofs << it->first << "\n" << it->second << "\n";
		}

		gotoxy(25, ++gotx);
		cout << "Изменение данных завершено. \n";
		ofs.close();
	}
	catch (...)
	{
		cout << "Ошибка в Admin::EditCategories()";
	}
	gotoxy(25, ++gotx);
	system("pause");
}

inline void Admin::AddTestsName()
{
	
	system("cls");
	int gotx = 6;
	//unique_ptr<Admin> admin(new Admin);
	map <string, string> mp;
	string path = this->GetTestFolder() + "/" + this->GetCategoriesFile();
	string key, category;

	int count = 0; // счетчик кол-ва разделов

	ifstream ifs;

	try
	{
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			cout << "В системе еще нет созданных разделов тестирования";
			gotoxy(25, ++gotx);
			cout << "Для создания тестов создайте хотя бы один раздел тестирования";
		}

		else
		{
			while (!ifs.eof())
			{
				getline(ifs, key);
				getline(ifs, category);

				if (key != "")
					mp[key] = category;
				count++;
			}

			ifs.close();

			count--;
			gotoxy(25, ++gotx);

			cout << "В системе есть следующие разделы:\n";

			for (auto it = mp.begin(); it != mp.end(); ++it)
			{
				gotoxy(25, ++gotx);
				cout << it->first << ". " << it->second << "\n";
			}

			cin.ignore();
			gotoxy(25, ++gotx);
			cout << "Введите номер категории для добавления теста: ";
			string keyedit;
			getline(cin, keyedit);
			auto it = mp.find(keyedit);

			if (it != mp.end())
			{
				gotoxy(25, ++gotx);
				cout << "Введите название теста: ";
				string testname;
				getline(cin, testname);

				fs::create_directories(this->GetTestFolder() + "/" + it->second + "/" + testname);

				path = this->GetTestFolder() + "/" + it->second + "/" + this->GetTestsName();

				count = 1;

				mp.clear(); //очищаем map с категориями разделов, и заполняем его названиями существующих тестов

				ifs.open(path);

				bool ckfile = ifs.is_open();

				if (ckfile)
				{						
					while (!ifs.eof())
					{
						getline(ifs, key);
						getline(ifs, category);

						if (key != "")
							mp[key] = category;
						count++;
					}
					count--;
				}
				
				ifs.close();				
				
				ofstream ofs;
				ofs.open(path, ofstream::app);

				ofs << count << "\n";
				ofs << testname << "\n";

				gotoxy(25, ++gotx);
				cout << "Новый тест добавлен в категорию. \n";

				ofs.close();
			}
		}	
	}
	catch (...)
	{
		cout << "Ошибка в Admin::AddCategories()";
	}
}

inline void Admin::AddTests()
{
	system("cls");
	int gotx = 6;
	//unique_ptr<Admin> admin(new Admin);
	map <string, string> mp;
	string path = this->GetTestFolder() + "/" + this->GetCategoriesFile();
	string key, category, folder;

	int count = 0; // счетчик кол-ва разделов

	ifstream ifs;

	try
	{
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			cout << "В системе еще нет созданных разделов тестирования";
		}

		else
		{
			while (!ifs.eof())
			{
				getline(ifs, key);
				getline(ifs, category);

				if (key != "")
					mp[key] = category;
				count++;
			}
			count--;
			gotoxy(25, ++gotx);
			cout << "В системе есть следующие разделы:";
		}

		ifs.close();


		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			gotoxy(25, ++gotx);
			cout << it->first << ". " << it->second << "\n";
		}

		if (mp.size())
		{
			cin.ignore();
			gotoxy(25, ++gotx);
			cout << "Введите номер раздела для добавления тестов: ";
			string keyedit;
			getline(cin, keyedit);

			auto it = mp.find(keyedit);

			if (it != mp.end())
			{
				fs::create_directories(this->GetTestFolder() + "/" + it->second);

				path = this->GetTestFolder() + "/" + it->second + "/" + this->GetTestsName();

				ifs.open(path);

				if (!ifs.is_open())
				{
					gotoxy(25, ++gotx);
					cout << "В этой категории еще нет созданных тестов";
					gotoxy(25, ++gotx);
					cout << "Желаете создать тест? (Да(1)/Нет(0): ";
					int answ;
					cin >> answ;

					if (answ)
					{
						this->AddTestsName();
					}
				}

				else
				{
					map <string, string> map_test;
					count = 0;

					while (!ifs.eof())
					{
						getline(ifs, key);
						getline(ifs, category);

						if (key != "")
							map_test[key] = category;
						count++;
					}
					count--;				

					ifs.close();

					gotoxy(25, ++gotx);
					cout << "В этой категории есть следующие тесты";


					for (auto it = map_test.begin(); it != map_test.end(); ++it)
					{
						gotoxy(25, ++gotx);
						cout << it->first << ". " << it->second << "\n";
					}

					gotoxy(25, ++gotx);
					cout << "Введите номер Теста для создания к нему вопросов: ";
					getline(cin, keyedit);

					auto it_test = map_test.find(keyedit);

					if (it_test != map_test.end())
					{
						path = this->GetTestFolder() + "/" + it->second + "/" + it_test->second + "/" + "quantity.txt"; // количество вопросов в тесте
					
						count = 1;
						int q_key = 0, a_key = 0;
						string question, answer;

						map<int, string> mquestion;
						map<int, string> manswer;

						system("cls");

						ifs.open(path);

						if (!ifs.is_open())
						{
							gotoxy(25, ++gotx);
							cout << "В системе еще нет тестов в разделе: " << it->second << " -> " << it_test->second;
						}

						else
						{
							while (!ifs.eof())
							{
								char* buff = new char[5];
								ifs.getline(buff, 5);
								count = atoi(buff);
								delete[] buff;
							}
							gotoxy(25, ++gotx);
							cout << "В системе есть " << count << " вопросов в разделе " << it->second;
							count++;
						}

						ifs.close();

						gotoxy(25, ++gotx);
						cout << "Добавление теста в раздел: ";
						cout << it->first << ". " << it->second << " -> " << it_test->second;
					
						gotoxy(25, ++gotx);
						cout << "Введите вопрос: ";
						getline(cin, question);

						q_key = count;
						mquestion.emplace(q_key, question);
						int n;
						gotoxy(25, ++gotx);
						cout << "Введите количество ответов на вопрос: ";
						cin >> n;
						gotoxy(25, ++gotx);
						cout << "Введите ответы на вопрос: ";

						cin.ignore();

						for (size_t i = 1; i < n + 1; i++)
						{
							gotoxy(25, ++gotx);
							cout << i << ". ";
							getline(cin, answer);
							//cin.ignore();
							manswer.emplace(++a_key, answer);
						}

						gotoxy(25, ++gotx);
						cout << "Введите номер правильного ответа на вопрос: ";
						cin >> n;

						ofstream ofs;

						ofs.open(path);
						ofs << q_key;
						ofs.close();
						path = this->GetTestFolder() + "/" + it->second + "/" + it_test->second + "/" + to_string(q_key) + ".txt";
						ofs.open(path);
						ofs << n << "\n";

						for (auto it = mquestion.begin(); it != mquestion.end(); ++it)
						{
							ofs << it->second << "\n";
						}

						for (auto it = manswer.begin(); it != manswer.end(); ++it)
						{
							ofs << it->first << ". " << it->second << "\n";
						}

						ofs.close();

						gotoxy(25, ++gotx);
						cout << "Изменение данных завершено.";
					}					
				}
			}						

			else
			{
				gotoxy(25, ++gotx);
				cout << "Вы ввели неверный номер раздела";
			}			
		}
	}

	catch (...)
	{
		cout << "Ошибка в Admin::AddTests()";
	}
	gotoxy(25, ++gotx);
	system("pause");

}

void Student::Registration()
{	
	system("cls");
	
	int gotx = 6;
	gotoxy(25, ++gotx);
	cout << "Регистрация студента\n";

	gotoxy(25, ++gotx);
	cout << "Введите логин (e-mail): ";
	cin >> this->login;

	//папка для хранения регистрационных данных отдельно по каждому пользоввателю
	string path = this->folder + "/" + md5(this->login) + ".txt"; 

	//папка для хранения личных данных в одном файле по каждому пользоввателю
	string path2 = this->GetUsersFolder() + "/" + this->GetUsersInfo();

	ifstream fin(path);

	//проверяем есть ли пользователь, если нет продолжаем регистрацию
	bool ckfile = fin.fail();
	fin.close();
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
			ofstream sfout, afout, ufout;
			sfout.exceptions(ofstream::badbit | ofstream::failbit);		
			afout.exceptions(ofstream::badbit | ofstream::failbit);
			ufout.exceptions(ofstream::badbit | ofstream::failbit);

			path = this->GetUsersFolder() + "/" + this->GetStudentsFile();
			
			sfout.open(this->folder + "/" + md5(this->login) + ".txt");
			
			afout.open(path, ofstream::app);
			
			ufout.open(path2, ofstream::app);


			string hash = md5(this->login);
			sfout << hash << "\n";

			gotoxy(25, ++gotx);
			cout << "Введите пароль: ";
			string pass;
			cin >> pass;				
			//сохранение зашифрованного пароля
			hash = md5(pass);
			sfout << hash << "\n";
			cin.ignore();
			gotoxy(25, ++gotx);
			cout << "Введите ФИО: ";
			string name;
			getline(cin, name);
			//sfout << name << "\n";
			//ufout << name << "\n";
			

			gotoxy(25, ++gotx);
			cout << "Введите номер мобильного телефона: +38";
			string telephone;
			getline(cin, telephone);
			//sfout << telephone << "\n";			

			afout << telephone << "\n" << name << " (логин: " << this->login << ")" << "\n";
			afout.close();

			gotoxy(25, ++gotx);
			cout << "Введите домашний адрес: ";
			string adress;
			getline(cin, adress);			
			//sfout << adress << "\n";

			ufout << telephone << "\n" << "ФИО: " << name << ". Домашний адрес: " <<  adress << "\n";

			gotoxy(25, ++gotx);
			cout << "Регистрация завершена." << endl;
			sfout.close();		
			ufout.close();
			fs::create_directories(this->GetUsersFolder() + "/" + this->login);
			gotoxy(25, ++gotx);
			system("pause");
		}

		else
		{
			throw ExceptionUser("Пользователь с таким логином существует. Войдите в систему используя указанный логин. \nИли пройдите регистрацию используя другой логин (e-mail)" , 3);
		}
	}

	catch (const ofstream::failure& ex)
	{
		gotoxy(25, ++gotx);
		cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
		system("pause");
	}

	catch (ExceptionUser& ex)
	{
		gotoxy(25, ++gotx);
		cout << ex.what() << " Код ошибки: " << ex.GetError() << "\n";
		
		gotoxy(25, ++gotx);
		system("pause");
	}

	catch (...)
	{	
		gotoxy(25, ++gotx);
		cout << "Ошибка в Student::Registration()";
		system("pause");
	}	
}

inline void Student::NewTest()
{
	//system("cls");
	
	int gotx = 6;
	//unique_ptr<Admin> admin(new Admin);
	map <string, string> mp;
	string path = this->GetTestFolder() + "/" + this->GetCategoriesFile();
	string key, category, folder;

	int count = 0; // счетчик кол-ва разделов

	ifstream ifs;

	try
	{
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			cout << "В системе еще нет созданных разделов тестирования";
		}

		else
		{
			while (!ifs.eof())
			{
				getline(ifs, key);
				getline(ifs, category);

				if (key != "")
					mp[key] = category;
				count++;
			}
			count--;
			gotoxy(25, ++gotx);
			cout << "В системе есть следующие разделы:";
		}

		ifs.close();


		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			gotoxy(25, ++gotx);
			cout << it->first << ". " << it->second << "\n";
		}

		
		gotoxy(25, ++gotx);
		cout << "Введите номер категории для выбора теста: ";
		string keyedit;
		getline(cin, keyedit);
		
		auto it = mp.find(keyedit);

		if (it != mp.end())
		{
			gotoxy(25, ++gotx);


			path = this->GetTestFolder() + "/" + it->second + "/" + this->GetTestsName();
			
			string path_questions;
			string path_quantity = path_questions = this->GetTestFolder() + "/" + it->second;
			
			count = 1;

			mp.clear(); //очищаем map с категориями разделов, и заполняем его названиями существующих тестов

			ifs.open(path);

			bool ckfile = ifs.is_open();

			if (ckfile)
			{
				while (!ifs.eof())
				{
					getline(ifs, key);
					getline(ifs, category);

					if (key != "")
						mp[key] = category;
					count++;
				}
				count--;

				for (auto it = mp.begin(); it != mp.end(); ++it)
				{
					gotoxy(25, ++gotx);
					cout << it->first << ". " << it->second << "\n";
				}

				gotoxy(25, ++gotx);
				cout << "Введите номер теста: ";
				keyedit;
				getline(cin, keyedit);

				it = mp.find(keyedit);

				if (it != mp.end())
				{
					path_quantity += "/" + it->second + "/" + "quantity.txt";

					ifstream ifn;
					int n = 0;

					ifn.open(path_quantity);

					if (!ifn.is_open())
					{
						gotoxy(25, ++gotx);
						cout << "В системе еще нет созданных разделов тестирования";
					}

					else
					{
						while (!ifn.eof())
						{
							char* buff = new char[5];
							ifn.getline(buff, 5);
							n = atoi(buff);
							delete[] buff;
						}
						gotoxy(25, ++gotx);
						cout << "Во время тестирования вам нужно будет ответить на " << n << " вопроса(ов) в выбранной категории " << it->second;
					}

					ifn.close();
					
					string questions, answer, path_helper;
					path_helper = path_questions += "/" + it->second;

					string test_time = currentDateTime();

					//сохранение в файл кол-во пройденных ответов по конкретному тесту
					string path_users_test = this->GetUsersFolder() + "/" + this->login + "/" + it->second + " (" + test_time + ")" + ".txt";
					
					//сохранение в файл названий запущеных тестов студентом
					string path_users_name_test = this->GetUsersFolder() + "/" + this->login + "/" + "student_name_tests.txt";

					int un; // номер ответа на вопрос теста введенного пользователем
					int ball_count = 0; // подсчет правильных ответов
					int test_count = 0; // подсчет кол-ва пройденных ответов на выбранный тест

					ofstream users_name_test;
					users_name_test.exceptions(ofstream::badbit | ofstream::failbit);
					try
					{
						users_name_test.open(path_users_test);

						users_name_test << test_count;

						users_name_test.close();

					}
					catch (const ofstream::failure& ex)
					{
						gotoxy(25, 4);
						cout << ex.what() << "\nКод ошибки: " << ex.code() << "\n";
						system("pause");
					}

					ofstream out_test_count;
					out_test_count.exceptions(ofstream::badbit | ofstream::failbit);

					try
					{
						out_test_count.open(path_users_name_test, ofstream::app);
						out_test_count << it->second + " (" + test_time + ")" << "\n";
						out_test_count.close();
					}
					catch (const ofstream::failure& ex)
					{
						gotoxy(25, 4);
						cout << ex.what() << "\nКод ошибки: " << ex.code() << "\n";
						system("pause");
					}

					for (size_t i = 1; i < n + 1; i++)
					{		
						cout << "Вопрос № " << i << ".\n";
						
						path_questions = path_helper + "/" + to_string(i) + ".txt";

						ifn.open(path_questions);

						getline(ifn, answer);

						while (!ifn.eof())
						{ 
							questions = "";
							getline(ifn, questions);
							cout << questions << "\n";
						}
						ifn.close();

						cout << "Вопрос № " << i << ". Введите номер ответа: ";
						cin >> un;

						if (to_string(un) == answer)
							ball_count++;

						test_count++;

						try
						{
							users_name_test.open(path_users_test);

							users_name_test << test_count;

							users_name_test.close();

						}
						catch (const ofstream::failure& ex)
						{
							gotoxy(25, 4);
							cout << ex.what() << "\nКод ошибки: " << ex.code() << "\n";
							system("pause");
						}
					}			
					
				}
			}
			ifs.close();			
			
			if (!mp.size())
			{
				gotoxy(25, ++gotx);
				cout << "В выбранной категории еще нет тестов ";
			}			
		}

		else
		{
			gotx++;
			gotoxy(25, ++gotx);
			cout << "Вы указали несуществующий раздел";
		}
	}

	catch (...)
	{
		gotoxy(25, ++gotx);
		cout << "Ошибка в Student::NewTest()";
		gotoxy(25, ++gotx); 
		system("pause");
	}
	
	gotoxy(25, ++gotx);
	system("pause");
}

const string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d_%H.%M.%S", &tstruct);

	return buf;
}