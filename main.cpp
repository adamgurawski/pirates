#include "console.h"
#include "game.h"
#include "map.h"

#define DEBUG 1

#include <iostream>
#include <string>
#include <time.h>
#include <vector>

// TODO: use boost?
// TODO: where to put srand?

int main(int argc, const char** argv)
{
	/*
	std::map<std::string, int> parameters;
	std::vector<game::TShip> ships;
	bool ok = console::ValidateInput(argc, argv, parameters, ships);

	if (!ok)
	{
		// To be moved to ValidateInput?
		console::DisplayHelp();
		return -1;
	}
	else
	{ // Run the game.
		game::TShip ship1 = ships.at(0);
		std::cout << "Ship: " << ship1.Name << "." << std::endl;
		std::cout << "Velocity: " << ship1.Speed << " knots." << std::endl;
		std::cout << "Time to be generated: " << ship1.TimeToGeneration << "." << std::endl;
		std::cout << "Map size: " << parameters.at("length") << " x " << parameters.at("width") << std::endl;
		std::cout << "Time of simulation (in seconds): " << parameters.at("time") << std::endl;
		game::TGame game;
		ok = game.Run();
	}
	*/

	srand(time(NULL));

#if DEBUG
	for (int i = 0; i < 100; ++i)
	{
#endif
	TCoordinates rollTest = getBorderCoordinates(10, 20);
	std::cout << "Rolled coordinates: " << rollTest.X << " " << rollTest.Y <<
		"." << std::endl;
#if DEBUG
	}
#endif

	std::cin.get();
	return 0;
}