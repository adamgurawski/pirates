#include "map.h"

#include <random> // rand()
#include <cassert> // assert

TCoordinates TMap::RollCivilianPosition() const
{
	TCoordinates result = { 0,0 };
	unsigned int xMax = Width - 1;
	unsigned int yMax = Height - 1;

	switch (rand() % 4 + 1)
	{
	case 1:
		// X = 0. Roll Y.
		result.Y = rand() % xMax;
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

	if (!IsEmpty({ result.X, result.Y }))
		return RollCivilianPosition();
	else
		return result;
}

TCoordinates TMap::RollPiratesPosition() const
{
	TCoordinates result;
	result.X = rand() % (Width - 1);
	result.Y = rand() % (Height - 1);

	if (!IsEmpty({ result.X, result.Y }))
		return RollPiratesPosition();
	else
		return result;
}

bool TMap::PlaceOnMap(TCoordinates coordinates, const IShip* ship)
{
	if (IsEmpty(coordinates))
	{
		auto it = Map.find(coordinates);
		it->second = ship;
		return true;
	}
	else
	{
		return false;
	}
}

void TMap::CreateEmptyMap()
{
	for (unsigned int x = 0; x < Width; ++x)
		for (unsigned int y = 0; y < Height; ++y)
			Map.insert(std::pair<TCoordinates, const IShip*>({ x,y }, nullptr));
}

