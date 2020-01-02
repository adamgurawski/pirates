#include "game.h"

#include <cassert>
#include <iostream> 
#include <time.h> // srand

TGame::TGame(options::TOptions& options)
	: Map({ options.GetMapWidth(), options.GetMapHeight() }),
	SimDuration(options.GetSimulationTime()), CurrentTime(0)
{
	// Srand must be called before Map.RollPiratesPosition.
	srand(static_cast<unsigned int>(time(NULL)));

	// Create pirate and place it on the map.
	Pirate = TPirate(1, 5, Map.RollPiratesPosition(), { 0,0 });
	Map.PlaceOnMap(Pirate.GetPosition(), &Pirate);

	ShipsInfo = options.GetShipInfo();
	GenerateShips();
			
	Pirate.debug_IntroduceYourself();
	Map.debug_PrintMap();
}

bool TGame::Run()
{ // TODO: implement Run().
	for (; CurrentTime < SimDuration; ++CurrentTime)
	{
		GenerateShips();
		RunTurn();
	}

	return true;
}

/* 1. Check whether civilians see the pirate.
	 2. Move the civilians.	
	 3. If one can get out of map, acknowledge it.
	 4. Move the pirate.
	 5. Check whether it has an opportunity to attack other ship.
	 6. Attack ships, acknowledge if destroyed.*/
bool TGame::RunTurn()
{
	// LookForPirates();
	MoveCivilians();


	return true;
}

void TGame::CreateShip(const TShipInfo& shipInfo)
{ // TODO: throw exception when there is no available space on map?
	TCoordinates startingPosition = Map.RollCivilianPosition();
	TCoordinates destination = SetCivilianStartingDestination(startingPosition);
	TShipPtr ship;

	// TODO: how to set visibility?
	float visibility = 5.0f;

	int generate = rand() % 3 + 1;
	switch (generate)
	{
	case 1:
		ship = std::make_unique<TPassenger>(shipInfo.Name, shipInfo.Velocity, visibility, 
			startingPosition, destination);
		break;
	case 2:
		ship = std::make_unique<TBulkCarrier>(shipInfo.Name, shipInfo.Velocity, visibility,
			startingPosition, destination);
		break;
	case 3:
		ship = std::make_unique<TTanker>(shipInfo.Name, shipInfo.Velocity, visibility,
			startingPosition, destination);
		break;
	default:
		assert(false && "How did we get here?");
	}

	// If new ship is the fastest, update max velocity.
	CorrectMaxVelocity(shipInfo.Velocity);

	// Add ship to list.
	Ships.push_back(std::move(ship));

	// Place added ship on a map.
	const IShip* addedShip = Ships.back().get();
	Map.PlaceOnMap(addedShip->GetPosition(), addedShip);

	// Debug.
	addedShip->debug_IntroduceYourself();
}

void TGame::GenerateShips()
{
	TShipInfoSet::iterator it = ShipsInfo.begin();

	while (it != ShipsInfo.end() && it->TimeToGeneration == CurrentTime)
	{
		CreateShip(*it);
		it = ShipsInfo.erase(it);
	}
}

void TGame::Move(TGame::TShipPtr& ship)
{ // TODO: implement
	TCoordinates destination = ship->GetDestination();
	TCoordinates position = ship->GetPosition();
}

void TGame::MoveCivilians()
{
	for (TShipPtr& ship : Ships)
	{
		Move(ship);
	}
}

void TGame::LookForPirates()
{ // TODO: !! implement.

}

TCoordinates TGame::SetCivilianStartingDestination(TCoordinates position) const
{
	TCoordinates destination = position;
	unsigned int xMax = Map.GetWidth() - 1;
	unsigned int yMax = Map.GetHeight() - 1;

	/* Get opposite side of map.
			Not using else because of these cases:
			- for x = 0, y = 0 we want to set x = max, y = max.
			- for x = max, y = max we want to set x = 0, y = 0. */
	if (position.X == 0)
		destination.X = xMax;
	if (position.Y == 0)
		destination.Y = yMax;
	if (position.X == xMax)
		destination.X = 0;
	if (position.Y == yMax)
		destination.Y = 0;
	
	return destination;
}

void TGame::CorrectMaxVelocity(float newVelocity)
{
	if (newVelocity > CurrentMaxVelocity)
	{
		CurrentMaxVelocity = newVelocity;
		Pirate.ModifyVelocity(CurrentMaxVelocity);
	}
}