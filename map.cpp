#include "map.h"

#include <cmath> // sqrt(), abs()
#include <random> // rand()
#include <cassert> // assert

bool TCoordinatesComparer::operator()(TCoordinates lhs, TCoordinates rhs) const
{
	if (lhs.X == rhs.X)
	{
		return lhs.Y < rhs.Y;
	}
	else if (lhs.Y == rhs.Y)
	{
		return lhs.X < rhs.X;
	}
	else
		return (lhs.X < rhs.X);
}

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

TCoordinates TMap::CalculateClosestExit(TCoordinates coordinates) const
{ // TODO: make it prettier.
	int maxX = Width - 1;
	int maxY = Height - 1;

	int x = coordinates.X;
	int y = coordinates.Y;

	enum class TRoute
	{
		INVALID = 0,
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};

	TRoute route = TRoute::INVALID;

	int smallestDistance;

	if (maxY > maxX)
		smallestDistance = maxY;
	else
		smallestDistance = maxX;

	// TODO: verify that there are no errors (f.e. when current coordinates are in the middle).
	if (maxY - y < smallestDistance)
	{
		smallestDistance = maxY - y;
		route = TRoute::UP;
	}
	if (y < smallestDistance)
	{
		smallestDistance = y;
		route = TRoute::DOWN;
	}
	if (maxX - x < smallestDistance)
	{
		smallestDistance = maxX - x;
		route = TRoute::RIGHT;
	}
	if (x < smallestDistance)
	{
		smallestDistance = x;
		route = TRoute::LEFT;
	}

	switch (route)
	{
	case TRoute::UP:
		return { coordinates.X, static_cast<unsigned>(maxY) };
	case TRoute::DOWN:
		return { coordinates.X, 0 };
	case TRoute::LEFT:
		return { 0, coordinates.Y };
	case TRoute::RIGHT:
		return { static_cast<unsigned>(maxX), coordinates.Y };
	default:
		assert(false && "Invalid route.");
		throw std::logic_error("Something went wrong in calculating closest exit.");
	}
}

bool TMap::IsInRange(const TCoordinates& center, float visibility, 
	const TCoordinates& target) const
{
	int x = std::abs(static_cast<int>(target.X - center.X));
	int y = std::abs(static_cast<int>(target.Y - center.Y));

	float powX = std::pow(x, 2);
	float powY = std::pow(y, 2);
	float powDistance = powX + powY;
	float distance = std::sqrt(powDistance);

	return !(distance > visibility);
}

void TMap::Move(const IShip* ship, const TCoordinates& target)
{ 
	Remove(ship->GetPosition());
	auto it = Map.find(target);
	assert(!it->second && "Must be nullptr.");
	it->second = ship;
}

// Nullify ship pointer at specified coordinates.
void TMap::Remove(const TCoordinates& coordinates)
{
	auto it = Map.find(coordinates);
	it->second = nullptr;
}

bool TMap::IsEmpty(TCoordinates coordinates) const
{
	return (Map.at(coordinates) == nullptr);
}

bool TMap::HasEmptyCoordinates() const
{ // Not so optimal solution, but I'm running out of time.
	for (auto& element : Map)
		if (element.second == nullptr)
			return true;

	return false;
}

void TMap::CreateEmptyMap()
{
	for (unsigned int x = 0; x < Width; ++x)
		for (unsigned int y = 0; y < Height; ++y)
			Map.insert(std::pair<TCoordinates, const IShip*>({ x,y }, nullptr));
}

