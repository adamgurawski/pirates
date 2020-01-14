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
	bool operator()(TCoordinates lhs, TCoordinates rhs) const;
};

TCoordinates operator+(const TCoordinates& lhs, const TCoordinates& rhs);
TCoordinates operator-(const TCoordinates& lhs, const TCoordinates& rhs);
bool operator==(const TCoordinates& lhs, const TCoordinates& rhs);

std::ostream& operator<<(std::ostream& stream, const TCoordinates& coordinates);

#endif // _COORDINATES_H