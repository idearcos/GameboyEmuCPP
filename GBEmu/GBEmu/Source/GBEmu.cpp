// GBEmu.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "GameBoy.h"

int main(int /*argc*/, char* /*argv*/[])
{
	try
	{
		GameBoy gb;
		gb.Run();
	}
	catch (std::exception &e)
	{
		std::cout << "Exception caught: " << e.what();
	}

	return 0;
}
