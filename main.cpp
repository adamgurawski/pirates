#include "options.h"
#include "game.h"
#include "map.h"

#include <iostream>

// TODO: !! implement AShip move constructor.
// TODO: !! TSimpleBrain and TPirate's move mechanism.
/* TODO: !! improve civilians' fleeing when the pirate was spotted (seems like it's not
			   working correctly if the closest border position == ship.position)
	 TODO: !! how to update pirate's Destination?
*/
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