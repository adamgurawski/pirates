// File game.h, contains TGame responsible for managing the game.

#ifndef _GAME_H
#define _GAME_H

#include "map.h"
#include "ship.h"

#include <memory>
#include <string>
#include <vector>

class TGame
{
public:
	TGame(unsigned int width, unsigned int height);

	~TGame()
	{}

	bool Run();

private:
	TMap Map;

	std::vector<std::unique_ptr<IShip*>> Ships;

	// Number of pirate's attack attempts.
	unsigned int Attempts = 0;

	// Number of successful pirate's attack attempts.
	unsigned int SuccessfulAttempts = 0;

	// The speed of the fastest ship on map.
	float CurrentMaxVelocity = 0;
};

#endif // _GAME_H