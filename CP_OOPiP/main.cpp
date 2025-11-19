#include "prototype.h"

using smart_system::SmartHomeInteraction;

int main()
{
	system("color f0");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	try {
		SmartHomeInteraction::showEntryMenu();
	}
	catch (std::exception& e)
	{
		cout << "Â ןנמדנאללו מרטבךא:" << e.what() << endl;
	}
	//system("pause");
	//system("cls");
	return 0;
}