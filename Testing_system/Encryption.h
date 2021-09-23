#pragma once
#include <iostream>
#include <string>
#include <exception>
using namespace std;

class Encryption
{
public:
	Encryption(string mycode) : mycode(mycode) {}
	~Encryption() {};

	virtual void Hash(string &s) = 0;

protected:
	string mycode;
	int key = 365;
};

class Encrypt: public Encryption
{
public:
	Encrypt(string mycode) : Encryption(mycode) {}
	~Encrypt() {};

	void Hash(string& s) override
	{
		int size = this->mycode.size();

		for (size_t i = 0; i < size; i++)
		{
			this->mycode[i] += pow(key, 2);	
		}

		s = this->mycode;
	}

private:

};

class Decrypt: public Encryption
{
public:
	Decrypt(string mycode) : Encryption(mycode) {}
	~Decrypt() {};

	void Hash(string& s) override
	{
		int size = this->mycode.size();
		
		for (size_t i = 0; i < size; i++)
		{
			this->mycode[i] -= pow(key, 2);
			
		}
		s = this->mycode;
	}

private:

};