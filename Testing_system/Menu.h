#pragma once

void UseMenu(string name, int ItemCount);

//Запуск нужных элементов Меню реализуется используя паттерн Фабричный метод (Factory method)	
//Определяет интерфейс для создания объекта, но оставляет подклассам решение о том, какой класс инстанцировать.

// UseMenu(string name, int ItemCount) - в эту функцию передается количество элементов меню и кто ее вызвал, user или admin

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

class MenuStudent : public Menu
{
	void printMenu() override;
};

class MenuSign : public Menu
{
public:
	void printMenu() override;
};

class Factory
{
public:

	virtual unique_ptr<Menu> runMenu() = 0;

private:

};

class AdminFactory : public Factory
{
public:

	unique_ptr<Menu>runMenu() override
	{
		return make_unique<MenuAdmin>();
	}

private:

};

class SignFactory : public Factory
{
public:

	unique_ptr<Menu>runMenu() override
	{
		return make_unique<MenuSign>();
	}

private:

};

class StudentFactory : public Factory
{
public:

	unique_ptr<Menu>runMenu() override
	{
		return make_unique<MenuStudent>();
	}

private:

};

void MenuAdmin::printMenu()
{	
	system("cls");
	int ItemCount = 5; // Эта переменная содержит количество пунктов меню.	
	UseMenu("admin", ItemCount);
}


void MenuSign::printMenu()
{
	int ItemCount = 3; // Эта переменная содержит количество пунктов меню.	
	UseMenu("sign", ItemCount);
}

void MenuStudent::printMenu()
{
	system("cls");
	int ItemCount = 4; // Эта переменная содержит количество пунктов меню.	
	UseMenu("student", ItemCount);
}