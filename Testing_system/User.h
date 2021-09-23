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

inline void Admin::RegistrationAdmin()
{
	
	cout << "Регистрация администратора системы тестирования\n";	

	ofstream fout;

	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		string path = Admin::filename;

		fout.open(path);

		//Процесс шифрования
		

		cout << "Введите логин (e-mail): ";
		string login;
		cin >> login;

		shared_ptr<Encryption> en;
		shared_ptr<Encrypt> cryptlogin(new Encrypt(login));
		en = cryptlogin;
		en->Hash(login);

		fout << login <<"\n";

		cout << "Введите пароль: ";
		string pass;
		cin >> pass;

		shared_ptr<Encrypt> cryptpass(new Encrypt(pass));
		en = cryptpass;
		en->Hash(pass);
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

		shared_ptr<Encryption> en;
		shared_ptr<Decrypt> en1(new Decrypt(adminlogin));
		en = en1;
		en->Hash(adminlogin);
		afin.close();

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
				shared_ptr<Encryption> en;
				shared_ptr<Encrypt> crypt(new Encrypt (pass));
				en = crypt;
				en->Hash(pass);

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