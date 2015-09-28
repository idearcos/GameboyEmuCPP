// GBEmu.cpp : Defines the entry point for the console application.
//

#include <thread>
#include <iostream>
#include "GameBoy.h"

int main(int /*argc*/, char* /*argv*/[])
{
	try
	{
		GameBoy gb;

		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
	catch (std::exception &e)
	{
		std::cout << "Exception caught: " << e.what();
	}

	return 0;
}
