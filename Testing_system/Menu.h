#pragma once


void UseMenu(string name, int ItemCount);


//Меню реализуется используя паттерн Фабричный метод (Factory method)	
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
	int ItemCount = 4; // Эта переменная содержит количество пунктов меню.	
	UseMenu("admin", ItemCount);
}


