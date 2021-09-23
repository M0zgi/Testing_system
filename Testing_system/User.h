#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
//#include <filesystem>



#include"MyExceptions.h"
#include"Menu.h"
#include"Encryption.h"

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
		string adminlogin;
		cin >> this->login;

		cout << "Введите пароль администратора системы: ";
		string adminpass;
		cin >> this->pass;

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

		//Процесс расшифровывания логина администратора системы
		
		unique_ptr<Decrypt> en1(new Decrypt(adminlogin));
		en1->Hash(adminlogin);
		en1->SetMycode(adminpass);
		en1->Hash(adminpass);

		if (this->login == adminlogin && this->pass == adminpass)

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

		
		cout << "Введите логин (e-mail): ";
		string login;
		cin >> login;

		//Процесс шифрования
		
		unique_ptr<Encrypt> cryptlogin(new Encrypt(login));
		cryptlogin->Hash(login);

		fout << login <<"\n";

		cout << "Введите пароль: ";
		string pass;
		cin >> pass;

		//Процесс шифрования
		unique_ptr<Encrypt> cryptpass(new Encrypt(pass));
		cryptpass->Hash(pass);

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

	string path = this->folder + "/" + this->login + ".txt";
	ifstream fin(path);

	//проверяем есть ли пользователь, если нет продолжаем регистрацию
	bool ckfile = fin.fail();
	
	//проверяем что бы введенный логин не совпадал с логином админа
	unique_ptr<Admin> admin(new Admin);
	path = admin->GetFilename();
	string adminlogin;
	ifstream afin;
	afin.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		afin.open(path);
		afin >> adminlogin;	

		//Процесс расшифровывания логина администратора системы
		
		unique_ptr<Decrypt> en1(new Decrypt(adminlogin));		
		en1->Hash(adminlogin);
		afin.close();

		//проверяем есть ли пользователь с таким логином
		//вторая проверка на уникальность логина администратора системы

		if (ckfile && this->login != adminlogin)
		{
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

				//Процесс шифрования
				
				unique_ptr<Encrypt> crypt(new Encrypt (pass));
				crypt->Hash(pass);

				//сохранение зашифрованного пароля
				fout << pass;

				gotoxy(25, 13);
				cout << "Регистрация завершена. Нажмите любую кнопку." << endl;
				fout.close();

				/*afin.open(this->folder + "/" + this->login + ".txt");
				
				string add;

				while (!afin.eof())
				{
					int i = -1;

					do
					{
						getline(afin, add);

						i++;

						if (i)
						{
							shared_ptr<Decrypt> en1(new Decrypt(add));
							en = en1;

							en->Hash(add);

							cout << add << "\n";
						}						

					} while (!i);					
				}				
				
				afin.close();*/

			}

			catch (const ofstream::failure& ex)
			{
				gotoxy(25, 13);
				cout << ex.what() << "\nКод ошибки: " << ex.code() << "\n";
				system("pause");
			}
		}

		else
		{
			cout << "Пользователь с таким логином существует.\n";
			cout << "Войдите в систему используя указанный логин: " << this->login << "\n";
			cout << "Или пройдите регистрацию используя другой e-mail";
		}

	}
	catch (const ifstream::failure& ex)
	{
		gotoxy(25, 13);
		cout << ex.what() << "\nКод ошибки: " << ex.code() << "\n";
		system("pause");
	}
}