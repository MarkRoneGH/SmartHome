#include "prototype.h"

using smart_system::SmartHomeInteraction;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try {
		SmartHomeInteraction::showEntryMenu();
	}
	catch (std::exception& e)
	{
		cout << "Â ןנמדנאללו מרטבךא:" << e.what() << endl;
	}

	return 0;
}