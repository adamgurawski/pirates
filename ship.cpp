#include "ship.h"

#include <iostream> // debugging purposes

ACivilian::ACivilian(TCoordinates position, TCoordinates mapDimensions, 
	unsigned vulnerability) : Vulnerability(vulnerability)
{
	Position = position;
	SetStartingDestination(mapDimensions);
	std::cout << "My vulnerability: " << Vulnerability << std::endl;
}

void ACivilian::SetStartingDestination(TCoordinates mapDimensions)
{
	Destination = Position;

	/* Get opposite side of map. 
		 Not using else because of these cases:
		 - for x = 0, y = 0 we want to set x = max, y = max.
		 - for x = max, y = max we want to set x = 0, y = 0. */
	if (Position.X == 0)
		Destination.X = mapDimensions.X;
	if (Position.Y == 0)
		Destination.Y = mapDimensions.Y;
	if (Position.X == mapDimensions.X)
		Destination.X = 0;
	if (Position.Y == mapDimensions.Y)
		Position.Y = 0;
}