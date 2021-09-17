#pragma once


void UseMenu(string name, int ItemCount);


//���� ����������� ��������� ������� ��������� ����� (Factory method)	
//���������� ��������� ��� �������� �������, �� ��������� ���������� ������� � ���, ����� ����� ��������������.

// UseMenu(string name, int ItemCount) - � ��� ������� ���������� ���������� ��������� ���� � ��� �� ������, user ��� admin

class Menu
{
public:
	Menu() {};
	~Menu() {};
	virtual void printMenu() = 0;

private:

};

class MenuAdmin: public Menu
{
public:
	void printMenu() override;
};

class MenuUser : public Menu
{

};

class Factory
{
public:

	virtual unique_ptr<Menu> runMenu() = 0;

private:

};

class AminFactory : public Factory
{
public:

	unique_ptr<Menu>runMenu() override
	{
		return make_unique<MenuAdmin>();
	}

private:

};

void MenuAdmin::printMenu()
{	
	int ItemCount = 4; // ��� ���������� �������� ���������� ������� ����.	
	UseMenu("admin", ItemCount);
}


