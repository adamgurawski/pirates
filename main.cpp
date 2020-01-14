#include "options.h"
#include "game.h"
#include "map.h"

#include <iostream>

// TODO: !! improve civilians' fleeing when the pirate was spotted (seems like 
// it's not working correctly if the closest border position == ship.position)
// TODO: !! add switches allowing to choose between graphical representation of map 
// and textual information.
// TODO: ! add switch -v for verbose textual information (print "x moved to [y,z] only
// if verbose is on.

int main(int argc, const char** argv)
{
	options::TOptions options(argc, argv);

	if (!options.IsValid())
		return -1;

	TGame game(options);

	bool ok = game.Run();

	std::cin.get();
	return 0;
}