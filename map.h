#ifndef _MAP_H
#define _MAP_H

struct TCoordinates
{
	unsigned int X = 0;
	unsigned int Y = 0;
};

class TMap
{
public:
	TMap(TCoordinates dimensions) : Dimensions(dimensions)
		{}

	TCoordinates GetMapDimensions() const
	{
		return Dimensions;
	}

	TCoordinates RollCivilianPosition() const;

	TCoordinates RollPiratesPosition() const;

private:
	TCoordinates Dimensions;
};

#endif // _MAP_H