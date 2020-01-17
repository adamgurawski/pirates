#include "options.h"
#include "game.h"
#include "map.h"

#include <iostream>

int main(int argc, const char** argv)
{
	options::TOptions options(argc, argv);

	if (!options.IsValid())
		return EXIT_FAILURE;

	TGame game(options);

	bool ok = game.Run();

	if (!ok)
		return EXIT_FAILURE;

	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();
	return 0;
}