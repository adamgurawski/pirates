#ifndef _MAP_H
#define _MAP_H

#include "ship.h" // debug
class IShip;

#include <iomanip> // debug
#include <iostream> // debug
#include <map>
#include <memory>

/*
struct TCoordinates
{
	unsigned int X = 0;
	unsigned int Y = 0;
};
*/

struct TCoordinatesComparer
{
	bool operator()(TCoordinates lhs, TCoordinates rhs) const;
};

class TMap
{
public:
	TMap(unsigned int width, unsigned int height) : Width(width), Height(height)
	{
		CreateEmptyMap();
	}

	unsigned int GetWidth() const
	{
		return Width;
	}

	unsigned int GetHeight() const
	{
		return Height;
	}

	TCoordinates RollCivilianPosition() const;

	TCoordinates RollPiratesPosition() const;

	void debug_PrintMap() const
	{
		for (int y = Height - 1; y >= 0; y--)
		{
			std::cout << std::setw(2) << y << " ";
			for (unsigned int x = 0; x < Width; ++x)
			{
				if (IsEmpty({ x, static_cast<unsigned int>(y) }))
					std::cout << " . ";
				else
				{
					if (Map.at({ x, static_cast<unsigned int>(y) })->debug_IsPirate())
						std::cout << " X ";
					else
						std::cout << " O ";
				}
			}
			std::cout << std::endl;
		}
		std::cout << "   ";
		for (unsigned int j = 0; j < Width; j++)
			std::cout << std::setw(2) << j << " ";
		std::cout << std::endl;
	}

	// Returns true if new ship was added, false when the field was not nullptr.
	bool PlaceOnMap(TCoordinates coordinates, const IShip* ship);

	// Returns the closest point to leave the map.
	TCoordinates CalculateClosestExit(TCoordinates coordinates) const;

	// Check whether target is seen by ship which position is center.
	bool IsInRange(const TCoordinates& center, float visibility, 
		const TCoordinates& target) const;

	void Move(const IShip* ship, const TCoordinates& target);

	// Nullify coordinates (ship destroyed / changed position).
	void Remove(const TCoordinates& coordinates);

	// Tell if there is no ship on these coordinates.
	bool IsEmpty(TCoordinates coordinates) const;

	bool HasEmptyCoordinates() const;

private:
	void CreateEmptyMap();

	std::map<TCoordinates, const IShip*, TCoordinatesComparer> Map;

	unsigned int Width;
	unsigned int Height;
};

#endif // _MAP_H