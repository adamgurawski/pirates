// File coordinates.h, contains struct TCoordinates, definitions of operators
// and TCoordinatesComparer used in STL containers.

#ifndef _COORDINATES_H
#define _COORDINATES_H

#include <iostream>

struct TCoordinates
{
	unsigned int X = 0;
	unsigned int Y = 0;
};

struct TCoordinatesComparer
{
	bool operator()(const TCoordinates& lhs, const TCoordinates& rhs) const;
};

TCoordinates operator+(const TCoordinates& lhs, const TCoordinates& rhs);
TCoordinates operator-(const TCoordinates& lhs, const TCoordinates& rhs);
bool operator==(const TCoordinates& lhs, const TCoordinates& rhs);

std::ostream& operator<<(std::ostream& stream, const TCoordinates& coordinates);

#endif // _COORDINATES_H