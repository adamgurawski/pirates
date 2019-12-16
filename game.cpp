#include "game.h"

#include <iostream> 
#include <time.h> // srand

TGame::TGame(unsigned int width, unsigned int height)
	: Map({ width, height })
{
	srand(time(NULL));
	TCoordinates startingPosition = Map.RollCivilianPosition();
	Ships.push_back(std::make_unique<IShip*>(new TBulkCarrier(startingPosition,
		{ width, height })));

	std::cout << "Civilian position: " << startingPosition.X << " " << startingPosition.Y <<
		"." << std::endl;
	
	TCoordinates destination = (*Ships[0])->debug_GetDestination();
	std::cout << "Civilian's destinaton: " << destination.X << " " << destination.Y <<
		"." << std::endl;
}

bool TGame::Run()
{ // TODO: implement.

	return true;
}