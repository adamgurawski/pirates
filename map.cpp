#include "map.h"

#include <random> // rand()
#include <cassert> // assert

TCoordinates getBorderCoordinates(unsigned int xMax, unsigned int yMax)
{
	TCoordinates result = { 0,0 };

	switch (rand() % 4 + 1)
	{
	case 1:
		// X = 0. Roll Y.
		result.Y = rand() % yMax;
		break;
	case 2:
		// Y = 0. Roll X.
		result.X = rand() % xMax;
		break;
	case 3:
		// X = max. Roll Y.
		result.X = xMax;
		result.Y = rand() % yMax;
		break;
	case 4:
		// Y = max. Roll X.
		result.Y = yMax;
		result.X = rand() % xMax;
		break;
	default:
		assert(false && "How did we get here?");
	} // switch

	return result;
}