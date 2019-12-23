#include "game.h"

#include <cassert>
#include <iostream> 
#include <time.h> // srand

TGame::TGame(options::TOptions& options)
	: Map({ options.GetMapWidth(), options.GetMapHeight() }),
	SimulationTime(options.GetSimulationTime())
{
	// Srand must be called before Map.RollPiratesPosition.
	srand(static_cast<unsigned int>(time(NULL)));

	// Create pirate and place it on the map.
	Pirate = TPirate(0, 5, Map.RollPiratesPosition(), { 0,0 });
	Map.PlaceOnMap(Pirate.GetPosition(), &Pirate);

	std::vector<options::TShipInfo> shipInfoVector = options.GetShipInfoVector();
	
	for (options::TShipInfo shipInfo: shipInfoVector)
	{
		TCoordinates startingPosition = Map.RollCivilianPosition();
		TCoordinates destination = SetCivilianStartingDestination(startingPosition);
		std::unique_ptr<IShip> ship;

		// TODO: how to set visibility?
		float visibility = 5.0f;

		int generate = rand() % 3 + 1;
		switch (generate)
		{
		case 1:
			ship = std::make_unique<TPassenger>(TPassenger(shipInfo.Name, shipInfo.Velocity, visibility,
				startingPosition, destination));
			break;
		case 2:
			ship = std::make_unique<TBulkCarrier>(TBulkCarrier(shipInfo.Name, shipInfo.Velocity, visibility,
				startingPosition, destination));
			break;
		case 3:
			ship = std::make_unique<TTanker>(TTanker(shipInfo.Name, shipInfo.Velocity, visibility,
				startingPosition, destination));
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

void TGame::CorrectMaxVelocity(float newVelocity)
{
	if (newVelocity > CurrentMaxVelocity)
	{
		CurrentMaxVelocity = newVelocity;
		Pirate.ModifyVelocity(CurrentMaxVelocity);
	}
}