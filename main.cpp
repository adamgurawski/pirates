#include "options.h"
#include "game.h"
#include "map.h"

#include <iostream>

// TODO: change velocity into float (TOptions)

int main(int argc, const char** argv)
{
	options::TOptions options(argc, argv);

	TGame game(options);
	//game.Run();

	std::cin.get();
	return 0;
}


/* The first version of validating console input:

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
	std::cout << "Map size: " << parameters.at("length") << " x " << parameters.at("width")
		<< std::endl;
	std::cout << "Time of simulation (in seconds): " << parameters.at("time") << std::endl;
	game::TGame game;
	ok = game.Run();
}
*/