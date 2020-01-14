#include "coordinates.h"

TCoordinates operator+(const TCoordinates& lhs, const TCoordinates& rhs)
{
	TCoordinates result;
	result.X = lhs.X + rhs.X;
	result.Y = lhs.Y + rhs.Y;
	return result;
}

TCoordinates operator-(const TCoordinates& lhs, const TCoordinates& rhs)
{
	TCoordinates result;
	result.X = lhs.X - rhs.X;
	result.Y = lhs.Y - rhs.Y;
	return result;
}

bool operator==(const TCoordinates& lhs, const TCoordinates& rhs)
{
	return (lhs.X == rhs.X) && (lhs.Y == rhs.Y);
}

std::ostream& operator<<(std::ostream& stream, const TCoordinates& coordinates)
{
	stream << "[" << coordinates.X << " " << coordinates.Y << "]";
	return stream;
}