#include "map.h"
#include "ship.h"

#include <iomanip> // std::setw
#include <iostream>
#include <random> // std::rand
#include <cassert> // assert

namespace
{
// If one wishes to change the name, must do it in "pirate.cpp" as well.
#define PIRATE_NAME "The Green Oyster"
}

TMap::TMap(unsigned int width, unsigned int height) : Width(width), Height(height)
{
	CreateEmptyMap();
}

void TMap::CorrectModulo(unsigned& x, unsigned& y) const
{
	unsigned xMax = Width - 1;
	unsigned yMax = Height - 1;

	if (xMax == 0)
		x = 1;
	else if (xMax == 1)
		x = 2;

	if (yMax == 0)
		y = 1;
	else if (yMax == 1)
		y = 2;
}

TCoordinates TMap::RollCivilianPosition() const
{
	if (!HasEmptyCoordinates())
		throw std::logic_error(
			"Error: There is no place on the map available for generation of new ship.");

	TCoordinates result = { 0,0 };
	unsigned xMax = Width - 1;
	unsigned yMax = Height - 1;
	unsigned moduloX = xMax;
	unsigned moduloY = yMax;
	CorrectModulo(moduloX, moduloY);

	switch (rand() % 4 + 1)
	{
	case 1:
		// X = 0. Roll Y.
		result.Y = rand() % moduloY;
		break;
	case 2:
		// Y = 0. Roll X.
		result.X = rand() % moduloX;
		break;
	case 3:
		// X = max. Roll Y.
		result.X = xMax;
		result.Y = rand() % moduloY;
		break;
	case 4:
		// Y = max. Roll X.
		result.Y = yMax;
		result.X = rand() % moduloX;
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
	unsigned moduloX = Width - 1;
	unsigned moduloY = Height - 1;
	CorrectModulo(moduloX, moduloY);
	result.X = rand() % moduloX;
	result.Y = rand() % moduloY;

	if (!IsEmpty({ result.X, result.Y }))
		return RollPiratesPosition();
	else
		return result;
}

void TMap::Display() const
{
	for (int y = Height - 1; y >= 0; y--)
	{ // Print map from top to down (descending Y).
		std::cout << std::setw(2) << y << " ";

		for (unsigned int x = 0; x < Width; ++x)
		{
			if (IsEmpty({ x, static_cast<unsigned int>(y) }))
				std::cout << " . ";
			else
			{
				if (Map.at({ x, static_cast<unsigned int>(y) })->GetName() == PIRATE_NAME)
				{ // Print X for the pirate and O for civilians.
					std::cout << " X ";
				}
				else
					std::cout << " O ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "Y/X";

	for (unsigned int j = 0; j < Width; j++)
	{ // Print X coordinates.
		std::cout << std::setw(2) << j << " ";
	}

	std::cout << std::endl;
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
{ 
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
	int smallestDistance = (maxY > maxX) ? maxY : maxX;

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
		throw std::logic_error("Error: Calculation of the closest exit failed.");
	}
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
{
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

