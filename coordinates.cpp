#include "coordinates.h"

bool TCoordinatesComparer::operator()(const TCoordinates& lhs,
	const TCoordinates& rhs) const
{
	if (lhs.X == rhs.X)
		return lhs.Y < rhs.Y;
	else if (lhs.Y == rhs.Y)
		return lhs.X < rhs.X;
	else
		return (lhs.X < rhs.X);
}

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