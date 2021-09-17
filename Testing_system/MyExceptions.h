#pragma once
#include <iostream>
#include <string>
#include <exception>

using namespace std;

class ExceptionAdmin: public exception
{
public:
	ExceptionAdmin(const char *mesg, int error): exception (mesg) { this->error = error; }
	
	int GetError() { return error; }

private:
	int error;
};