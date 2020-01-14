#ifndef _MESSENGER_H
#define _MESSENGER_H

#include "coordinates.h"

class IShip;

class TMessenger
{
public:
	TMessenger() = default;

	void OnLeave(const IShip* ship, const TCoordinates& lastPosition) const;

	void OnAttack(bool successfulAttack, const IShip* target) const;

	void OnEnd(unsigned int attempts, unsigned int successfulAttempts) const;
};

#endif // _MESSENGER_H
