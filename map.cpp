#include "map.h"

#include <random> // rand()
#include <cassert> // assert

TCoordinates TMap::RollCivilianPosition() const
{
	TCoordinates result = { 0,0 };

	switch (rand() % 4 + 1)
	{
	case 1:
		// X = 0. Roll Y.
		result.Y = rand() % Dimensions.Y;
		break;
	case 2:
		// Y = 0. Roll X.
		result.X = rand() % Dimensions.X;
		break;
	case 3:
		// X = max. Roll Y.
		result.X = Dimensions.X;
		result.Y = rand() % Dimensions.Y;
		break;
	case 4:
		// Y = max. Roll X.
		result.Y = Dimensions.Y;
		result.X = rand() % Dimensions.X;
		break;
	default:
		assert(false && "How did we get here?");
	} // switch

	return result;
}

TCoordinates TMap::RollPiratesPosition() const
{
	TCoordinates result;
	result.X = rand() % Dimensions.X;
	result.Y = rand() % Dimensions.Y;
	return result;
}