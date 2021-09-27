#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include<map>
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
	
	virtual void Registration() = 0;
	
	string GetLogin() { return this->login; }
	string GetUserFolder() { return this->folder; } //геттер папки для хранения регистрационных данных пользователей
	string UserSignIn();
	
	~User() {};

protected:

	string login;
	string pass;
	static string folder; //имя папки для сохранения регистрационных данных студентов
};

string User::folder = "StudentLogin";

class Admin: public User
{
public:
	Admin() {};
	~Admin() {};

	string GetFilename() { return filename; } //геттер названия файла с логином и паролем администратора системы
	string GetUsersFolder() { return UsersFolder; }
	string GetStudentsFile() { return StudentsFile; }
	string GetTestFolder() { return TestFolder; }
	string GetCategoriesFile() { return CategoriesFile; }

	void ChangeLogin(); //изменение логина и пароля администратора  системы
	void DeleteUser();//удаление пользовтаеля
	void Modification();//модификация пользовтаеля

	void AddCategories();//добавленеи категорий для тестов
	void EditCategories();//редкатирование категорий для тестов

	friend bool CheckAdmin();

	void Registration() override;
	
private:	
	static string filename;
	string UsersFolder = "UsersFolder"; // папка для хранения реузльтатов тестирования студентов
	string StudentsFile = "StudentsFile.txt"; // файл в котором хранятся логины и ФИО студентов
	string TestFolder = "TestFolder"; // папка для хранения вопросов тестирования
	string CategoriesFile = "CategoriesFile.txt"; // файл в котором хранятся категории с тестами
};

string Admin::filename = "admin.txt";

class Student : public User
{
public:
	Student() {};
	~Student() {};
	string GetStudentName() { return this->name; }

	void Registration() override;

private:
	string filename;
	string name;
	string telephone;
	string adress;
};

string User::UserSignIn()
{
	cout << "Вход в систему\n";
	unique_ptr<Student> student(new Student);

	gotoxy(25, 7);
	cout << "Введите логин (e-mail): ";
	cin >> student->login;	

	unique_ptr<Admin> ad(new Admin);

	string path = ad->GetFilename();

	if (student->login + ".txt" == path)
	{
		system("cls");
		gotoxy(25, 7);
		cout << "Введите логин (e-mail) администратора системы: ";
		string adminlogin, md5adminlogin;
		cin >> ad->login;
		gotoxy(25, 8);
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
			gotoxy(25, 10);
			cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
			system("pause");
		}

		if (md5adminlogin == adminlogin && md5adminpass == adminpass)
		{
			return "admin";
		}

		else
		{
			gotoxy(25, 11);
			cout << "Ошибка входа";
			return this->login;
		}	
	}

	else 
	{		
		path = student->folder + "/" + md5(student->login) + ".txt";

		try
		{
			ifstream ufin(path);
			ufin.exceptions(ifstream::badbit | ifstream::failbit);			
		}
		catch (...)
		{			
			
			gotoxy(25, 8);
			cout << "Пользователь с таким логином не найден.";
			gotoxy(25, 10);
			system("pause");
			return this->login;
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
				gotoxy(25, 8); 
				cout << "Введите пароль: ";
				string userpass, md5userpass;
				cin >> student->pass;

				md5userpass = md5(student->pass);

				int count = -2;
				while (count)
				{					
					getline(ufin, userpass);					
					count++;
				}

				if (md5userpass == userpass)
				{
					gotoxy(25, 13);
					cout << "Вы успешно вошли в систему тестирования. Нажмите любую кнопку." << endl;
					ufin.close();
					return student->GetLogin();
				}

				else
				{
					throw ExceptionUser("Вы ввели неверный пароль пользователя", 4);
					return this->login;
				}				
			}
		}

		catch (const ofstream::failure& ex)
		{
			gotoxy(25, 13);
			cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
			system("pause");
		}

		catch (ExceptionUser& ex)
		{
			gotoxy(25, 13);
			cout << ex.what() << " Код ошибки: " << ex.GetError() << "\n";
			system("pause");
		}

		catch (...)
		{
			gotoxy(25, 13);
			cout << "Ошибка в string User::UserSignIn()";
			system("pause");
		}
		
		ufin.close();
		return this->login;;
	}	
}

inline void Admin::Registration()
{		
	gotoxy(25, 11);
	cout << "Регистрация администратора системы тестирования\n";	

	ofstream fout;
	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		string path = Admin::filename;

		fout.open(path);
		
		gotoxy(25, 12);
		cout << "Логин администратора: admin";
		string login = "admin";
		//cin >> login;
		string md5login;
		md5login = md5(login);
		fout << md5login << "\n";
		gotoxy(25, 13);
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
		gotoxy(25, 14);
		cout << ex.what() << " Код ошибки: " << ex.code()<< "\n";
		system("pause");
	}

	catch (...)
	{
		gotoxy(25, 14);
		cout << "Catch Admin::RegistrationAdmin()\n";
	}
}

inline void Admin::ChangeLogin()
{
	ofstream fout;
	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{		
		gotoxy(25, 7);
		cout << "Изменение логина и пароля администратора системы тестирования\n";
		string path = Admin::filename;

		fout.open(path);

		gotoxy(25, 8);
		cout << "Введите новый логин администратора: ";
		string login;
		cin >> login;
		string md5login;
		md5login = md5(login);
		fout << md5login << "\n";
		gotoxy(25, 9);
		cout << "Введите новый пароль администратора: ";
		string pass;
		cin >> pass;
		string md5pass;
		md5pass = md5(pass);
		fout << md5pass;

		gotoxy(25, 10);
		cout << "Операция успешна." << endl;
		system("pause");
		fout.close();
	}

	catch (const ifstream::failure& ex)
	{
		gotoxy(25, 12);
		cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
		system("pause");
	}

	catch (...)
	{
		gotoxy(25, 12);
		cout << "Catch Admin::ChangeLogin()\n";
	}
}

inline void Admin::DeleteUser()
{
	unique_ptr<Admin> admin(new Admin);	
	map <string, string> mp;
	string path = admin->GetUsersFolder() + "/" + admin->GetStudentsFile();
	string key, data;	
	ifstream ifs;

	try
	{		
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, 13); 
			cout << "Ошибка открытия файла Admin::DeleteUser()";
		}

		while (!ifs.eof())
		{
			key = "";
			data = "";
			getline(ifs, key);
			if (key != "")
			getline(ifs, data);

			if(key != "")
			mp[key] = data;
		} 

		ifs.close();

		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			//gotoxy(25, 13);
			cout << "Список зарегистрированных студентов в базе (номмер телефона, ФИО, логин)\n";
			cout << "Номер телефона: " << it->first << " : " << it->second << "\n";
		}

		cout << "Введите мобильный номер студента для удаления: ";
		cin >> key;

		auto it = mp.find(key);
		
		if (it != mp.end())
		{
			cout << it->first << " : " << it->second << endl;

			cout << "Введите логин студента для удаления: ";
			string login;
			cin >> data;
			login = admin->GetUserFolder() + "/" + md5(data) + ".txt";

			if (remove(login.c_str()) != 0)
				perror("Ошибка удаления файла с логином студента");
			else
				puts("Оперция удаления студента успешно завершена");

			if (remove(path.c_str()) != 0)
				perror("Ошибка удаления файла с базой студентов");
			else
				puts("Оперция удаления старого файла базы студентов успешно завершена");

			mp.erase(key);

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
		}
		
		else
		{
			gotoxy(25, 16);
			cout << "Cтудент с указанным номером телефона не найден в базе.";
		}		
	}

	catch (...)
	{		
		cout << "Catch Admin::DeleteUser()";
		system("pause");
	}
}

inline void Admin::Modification()
{
	unique_ptr<Admin> admin(new Admin);
	map <string, string> mp;
	string path = admin->GetUsersFolder() + "/" + admin->GetStudentsFile();
	string key, data;
	ifstream ifs;

	try
	{
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, 13);
			cout << "Ошибка открытия файла Admin::Modification()";
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

		cout << "Введите мобильный номер студента для модификации: ";
		cin >> key;

		auto it = mp.find(key);

		if (it != mp.end())
		{
			cout << it->first << " : " << it->second << endl;

			cout << "Введите логин студента для модификации: ";
			string login;
			cin >> data;
			login = admin->GetUserFolder() + "/" + md5(data) + ".txt";

			if (remove(login.c_str()) != 0)
				perror("Ошибка удаления файла с логином студента");
			else
				puts("Оперция удаления студента успешно завершена");

			if (remove(path.c_str()) != 0)
				perror("Ошибка удаления файла с базой студентов");
			else
				puts("Оперция удаления старого файла базы студентов успешно завершена");

			ofstream ofs;

			ofs.open(admin->GetUserFolder() + "/" + md5(data) + ".txt");

			cout << "Логин студента (e-mail): " << data << "\n";
			ofs << md5(data) << "\n";
			cout << "Введите пароль: ";
			string pass;
			cin >> pass;
			//сохранение зашифрованного пароля
			string hash = md5(pass);
			ofs << hash << "\n";
			cin.ignore();
		
			cout << "Введите ФИО: ";
			string name;
			getline(cin, name);
			ofs << name << "\n";

			cout << "Введите номер мобильного телефона: +38";
			string telephone;
			getline(cin, telephone);
			ofs << telephone << "\n";

			cout << "Введите домашний адрес: ";
			string adress;
			getline(cin, adress);
			ofs << adress << "\n";

			cout << "Изменение данных завершено. \n";

			ofs.close();

			mp.erase(key);

			mp.emplace(telephone, name + " (логин: " + data + ")");

			cout << endl;

			for (auto it = mp.begin(); it != mp.end(); ++it)
			{
				cout << it->first << " : " << it->second << endl;
			}

			ofs.open(path, ostream::app);

			for (auto it = mp.begin(); it != mp.end(); ++it)
			{
				ofs << it->first << "\n" << it->second << "\n";
			}

			ofs.close();

		}

		else
		{
			gotoxy(25, 16);
			cout << "Cтудент с указанным номером телефона не найден в базе.";
		}

		system("pause");
	}

	catch (...)
	{
		cout << "Catch Admin::Modification()";
		system("pause");
	}
}

inline void Admin::AddCategories()
{
	system("cls");	
	int gotx = 6;
	unique_ptr<Admin> admin(new Admin);
	map <string, string> mp;
	string path = admin->GetTestFolder() + "/" + admin->GetCategoriesFile();
	string key, category;

	int count = 0; // счетчик кол-ва разделов

	ifstream ifs;

	try
	{
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, gotx);
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
	unique_ptr<Admin> admin(new Admin);
	map <string, string> mp;
	string path = admin->GetTestFolder() + "/" + admin->GetCategoriesFile();
	string key, category;

	int count = 0; // счетчик кол-ва разделов

	ifstream ifs;

	try
	{
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, gotx);
			cout << "В системе еще нет созданных разделов тестирвоания\n";
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

void Student::Registration()
{	
	system("cls");
	gotoxy(25, 7);
	cout << "Регистрация студента\n";

	gotoxy(25, 8);
	cout << "Введите логин (e-mail): ";
	cin >> this->login;

	string path = this->folder + "/" + md5(this->login) + ".txt";
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
			ofstream sfout, afout;
			sfout.exceptions(ofstream::badbit | ofstream::failbit);		
			afout.exceptions(ofstream::badbit | ofstream::failbit);

			path = admin->GetUsersFolder() + "/" + admin->GetStudentsFile();

			afout.open(path, ofstream::app);

			sfout.open(this->folder + "/" + md5(this->login) + ".txt");

			string hash = md5(this->login);
			sfout << hash << "\n";

			gotoxy(25, 9);
			cout << "Введите пароль: ";
			string pass;
			cin >> pass;				
			//сохранение зашифрованного пароля
			hash = md5(pass);
			sfout << hash << "\n";
			cin.ignore();
			gotoxy(25, 10);
			cout << "Введите ФИО: ";
			string name;
			getline(cin, name);
			sfout << name << "\n";

			

			gotoxy(25, 11);
			cout << "Введите номер мобильного телефона: +38";
			string telephone;
			getline(cin, telephone);
			sfout << telephone << "\n";

			afout << telephone << "\n" << name << " (логин: " << this->login << ")" << "\n";
			afout.close();

			gotoxy(25, 12);
			cout << "Введите домашний адрес: ";
			string adress;
			getline(cin, adress);			
			sfout << adress << "\n";

			gotoxy(25, 13);
			cout << "Регистрация завершена. Нажмите любую кнопку." << endl;
			sfout.close();			
		}

		else
		{
			throw ExceptionUser("Пользователь с таким логином существует. Войдите в систему используя указанный логин. \nИли пройдите регистрацию используя другой логин (e-mail)" , 3);
		}
	}

	catch (const ofstream::failure& ex)
	{
		gotoxy(25, 15);
		cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
		system("pause");
	}

	catch (ExceptionUser& ex)
	{
		gotoxy(0, 11);
		cout << ex.what() << " Код ошибки: " << ex.GetError() << "\n";
		
		gotoxy(25, 14);
		system("pause");
	}

	catch (...)
	{	
		gotoxy(25, 15);
		cout << "Ошибка в Student::Registration()";
		system("pause");
	}	
}