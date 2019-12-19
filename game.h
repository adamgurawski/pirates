// File game.h, contains TGame responsible for managing the game.

#ifndef _GAME_H
#define _GAME_H

#include "map.h"
#include "options.h"
#include "ship.h"

#include <memory>
#include <string>
#include <list>

class TGame
{
public:
	TGame(options::TOptions& options);

	~TGame()
	{}

	bool Run();

private:
	TCoordinates SetCivilianStartingDestination(TCoordinates position) const;

	// Save max velocity of the fastest ship and modify pirate's velocity.
	void CorrectMaxVelocity(float newVelocity);

private:
	TMap Map;

	unsigned int SimulationTime;

	std::list<std::unique_ptr<IShip>> Ships;

	// Number of pirate's attack attempts.
	unsigned int Attempts = 0;

	// Number of successful pirate's attack attempts.
	unsigned int SuccessfulAttempts = 0;

	// The speed of the fastest ship on map.
	float CurrentMaxVelocity = 0;

	// The pirate.
	TPirate Pirate;
};

#endif // _GAME_H