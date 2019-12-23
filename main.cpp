#include "options.h"
#include "game.h"
#include "map.h"

#include <iostream>

int main(int argc, const char** argv)
{
	options::TOptions options(argc, argv);

	if (!options.IsGood())
		return -1;

	TGame game(options);
	
	//game.Run();

	std::cin.get();
	return 0;
}