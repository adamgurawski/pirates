#include "options.h"
#include "game.h"
#include "map.h"

#include <iostream>

// TODO: BUG improve civilians' fleeing when the pirate was spotted (seems like 
// it's not working correctly if the closest border position == ship.position)

// TODO: BUG modify velocity when removing ships!

// Possibly handled:
// TODO: BUG when pirate's velocity is bigger than map.

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