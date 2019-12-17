#ifndef _MAP_H
#define _MAP_H

#include "ship.h" // debug
class IShip;

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
	bool operator()(TCoordinates lhs, TCoordinates rhs) const
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
			std::cout << y << " ";
			for (unsigned int x = 0; x < Width; ++x)
			{
				if (IsEmpty({ x, static_cast<unsigned int>(y) }))
					std::cout << " .";
				else
				{
					// Debug.
					if (dynamic_cast<const TPirate*>(Map.at({ x, static_cast<unsigned int>(y) })))
						std::cout << " O";
					else
						std::cout << " X";
				}
			}
			std::cout << std::endl;
		}
		std::cout << "   ";
		for (int j = 0; j < Width; j++)
			std::cout << j << " ";
		std::cout << std::endl;
	}

	// Returns true if new ship was added, false when the field was not nullptr.
	bool PlaceOnMap(TCoordinates coordinates, const IShip* ship);

private:
	bool IsEmpty(TCoordinates coordinates) const
	{
		return (Map.at(coordinates) == nullptr);
	}

	void CreateEmptyMap();

	std::map<TCoordinates, const IShip*, TCoordinatesComparer> Map;

	unsigned int Width;
	unsigned int Height;
};

#endif // _MAP_H