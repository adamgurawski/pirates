#ifndef _MAP_H
#define _MAP_H

struct TCoordinates
{
	unsigned int X = 0;
	unsigned int Y = 0;
};

class TMap
{

};

// TODO: rename.
TCoordinates getBorderCoordinates(unsigned int xMax,
	unsigned int yMax);

#endif // _MAP_H