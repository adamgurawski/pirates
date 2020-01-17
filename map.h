// File map.h, contains TMap responsible for all of map-related actions.

#ifndef _MAP_H
#define _MAP_H

#include "coordinates.h"

#include <map>

class IShip;

class TMap
{
public:
	TMap(unsigned int width, unsigned int height);

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
	// Nullify ship pointer at current ship's position and assign ship to target coordinates.
	void Move(const IShip* ship, const TCoordinates& target);
	// Nullify coordinates (ship destroyed / changed position).
	void Remove(const TCoordinates& coordinates);
	// Tell if there is no ship on these coordinates.
	bool IsEmpty(TCoordinates coordinates) const;
	// Return if there are any empty coordinates.
	bool HasEmptyCoordinates() const;

private:
	// Create map of coordinates with nullptr assigned.
	void CreateEmptyMap();

	std::map<TCoordinates, const IShip*, TCoordinatesComparer> Map;

	unsigned int Width;
	unsigned int Height;
};

#endif // _MAP_H