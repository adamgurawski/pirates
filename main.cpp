#include "options.h"
#include "game.h"
#include "map.h"

#include <iostream>

// TODO: !! implement TGame::Move (find a way to calculate distance to move the pirate).
// TODO: !! TSimpleBrain and TPirate's move mechanism.

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