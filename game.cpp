#include "game.h"

#include <iostream> 
#include <time.h> // srand

#define SHIP_COUNT 3

TGame::TGame(unsigned int mapWidth, unsigned int mapHeight)
	: Map({ mapWidth, mapHeight })
{
	srand(time(NULL));

	// Srand must be called before Map.RollPiratesPosition.
	Pirate = TPirate(0, 5, Map.RollPiratesPosition(), { 0,0 });
	
	Map.PlaceOnMap(Pirate.GetPosition(), &Pirate);

	for (int i = 0; i < SHIP_COUNT; ++i)
	{
		TCoordinates startingPosition = Map.RollCivilianPosition();
		TCoordinates destination = SetCivilianStartingDestination(startingPosition);
		std::unique_ptr<IShip> ship;

		int generated = rand() % 3 + 1;
		switch (generated)
		{
			case 1:
				ship = std::make_unique<TPassenger>(TPassenger("HMS Meraviglia", generated, 3, 
					startingPosition, destination));
				break;
			case 2:
				generated *= 2;
				ship = std::make_unique<TBulkCarrier>(TBulkCarrier("MV Maersk Alabama", generated, 5,
					startingPosition, destination));
				break;
			case 3:
				generated *= 3;
				ship = std::make_unique<TTanker>(TTanker("MV Sirius Star", generated, 3, 
					startingPosition, destination));
				break;
		}

		ModifyMaxVelocity(generated);
		// Add ship to list.
		Ships.push_back(std::move(ship));

		IShip* shipInList = std::next(Ships.begin(), i)->get();
		// Starting position's Y=10 generates erorrs.
		Map.PlaceOnMap(shipInList->GetPosition(), shipInList);

		// Debug.
		shipInList->debug_IntroduceYourself();

	} // for
	Pirate.debug_IntroduceYourself();
	Map.debug_PrintMap();
}

bool TGame::Run()
{ // TODO: implement Run().
	return true;
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

void TGame::ModifyMaxVelocity(float newVelocity)
{
	if (newVelocity > CurrentMaxVelocity)
	{
		CurrentMaxVelocity = newVelocity;
		Pirate.ModifyVelocity(CurrentMaxVelocity);
	}
}