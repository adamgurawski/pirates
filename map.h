#ifndef _MAP_H
#define _MAP_H

#include "coordinates.h"
#include "ship.h" // debug

#include <iomanip> // debug
#include <iostream> // debug
#include <map>
#include <memory>

class IShip;

class TMap
{
public:
	TMap(unsigned int width, unsigned int height) : Width(width), Height(height)
	{
		CreateEmptyMap();
	}

	unsigned int GetWidth() const
		{ return Width; }
	unsigned int GetHeight() const
		{ return Height; }

	// Correct divider for rand().
	void CorrectModulo(unsigned& x, unsigned& y) const;
	// Roll position at map's border.
	TCoordinates RollCivilianPosition() const;
	// Roll position anywhere.
	TCoordinates RollPiratesPosition() const;
	// Print map (called when -g was passed).
	void Display() const;
	// Return true if new ship was added, false when the field was not nullptr.
	bool PlaceOnMap(TCoordinates coordinates, const IShip* ship);
	// Return the closest point to leave the map.
	TCoordinates CalculateClosestExit(TCoordinates coordinates) const;

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