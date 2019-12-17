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
	TMap(TCoordinates dimensions) : Dimensions(dimensions)
	{
		CreateEmptyMap();
	}

	TCoordinates GetMapDimensions() const
	{
		return Dimensions;
	}

	TCoordinates RollCivilianPosition() const;

	TCoordinates RollPiratesPosition() const;

	void debug_PrintMap() const
	{
		// TODO: correct condition.
		for (unsigned int x = 0; x <= Dimensions.X; ++x)
		{
			for (unsigned int y = 0; y <= Dimensions.Y; ++y)
			{
				if (IsEmpty({ x,y }))
					std::cout << " .";
				else
				{
					// Debug.
					if (dynamic_cast<const TPirate*>(Map.at({ x,y })))
						std::cout << " O";
					else
						std::cout << " X";
				}
			}
			std::cout << std::endl;
		}
	}

	// Returns true if new ship was added, false when the field was not nullptr.
	bool PlaceShipOnMap(TCoordinates coordinates, const IShip* ship)
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

private:
	bool IsEmpty(TCoordinates coordinates) const
	{
		return (Map.at(coordinates) == nullptr);
	}

	void CreateEmptyMap();

	std::map<TCoordinates, const IShip*, TCoordinatesComparer> Map;

	// TODO: verify.
	// In fact this field represent maximum possible coordinates, so 
	// width = dimensions.X + 1; height = dimensions.Y + 1;
	TCoordinates Dimensions;
};

#endif // _MAP_H