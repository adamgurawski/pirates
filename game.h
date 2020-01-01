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
	using TShipPtr = std::unique_ptr<IShip>;

	TGame(options::TOptions& options);
	~TGame() = default;

	bool Run();

	bool RunTurn();

private:
	// Create a ship and place it on the map.
	void CreateShip(const TShipInfo& shipInfo);

	// Create ships which time of generation equals CurrentTime and erase their info
	// from ship info set.
	void GenerateShips();

	// Change ships position and change its coordinates on map.
	void Move(TShipPtr& ship);

	// Move all of civilian ships.
	void MoveCivilians();

	TCoordinates SetCivilianStartingDestination(TCoordinates position) const;

	// Save max velocity of the fastest ship and modify pirate's velocity.
	void CorrectMaxVelocity(float newVelocity);

private:
	TMap Map;

	unsigned int CurrentTime;

	unsigned int SimDuration;

	// Stores information about ships to be generated sorted ascending by time to generation.
	TShipInfoSet ShipsInfo;

	std::list<TShipPtr> Ships;

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