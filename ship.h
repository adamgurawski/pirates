#ifndef _SHIP_H
#define _SHIP_H

struct TCoordinates
{
	unsigned int X = 0;
	unsigned int Y = 0;
};

#include <iostream> // debugging purposes
#include <memory>
#include <string>

class IShip
{
public:
	// Needed by TGame's constructor.
	IShip() = default;

	virtual ~IShip() = default;

	TCoordinates GetPosition() const
	{
		return Position;
	}

	virtual void debug_IntroduceYourself() const
	{
		std::cout << "Name: " << Name << std::endl << "Velocity: " <<
			Velocity << std::endl << "Visibility: " << Visibility << std::endl <<
			"Position: [" << Position.X << " " << Position.Y << "]" << std::endl <<
			"Destination: " << Destination.X << " " << Destination.Y << std::endl;
	}

protected:
	IShip(const std::string& name, float velocity, int visibility, TCoordinates position,
		TCoordinates destination) : Name(name), Velocity(velocity), Visibility(visibility),
		Position(position), Destination(destination)
	{}


protected:
	// Name. Separate identificator to be added? Is it neccessary?
	std::string Name;

	// Speed.
	float Velocity;

	// Range of view.
	int Visibility;

	// Position on a map.
	TCoordinates Position;

	// Position the ship is travelling to.
	TCoordinates Destination;
};

// Common base class for all of civilian ships (potential pirate targets).
class ACivilian : public IShip
{
public:
	virtual ~ACivilian() = default;

	bool WasAttacked() const
	{
		return Attacked;
	}

	void SetAttacked()
	{
		Attacked = true;
	}

	virtual void debug_IntroduceYourself() const override
	{
		IShip::debug_IntroduceYourself();
		std::cout << "Vulnerability: " << Vulnerability << std::endl <<
			"Attacked: " << std::boolalpha << Attacked << std::endl;
	}


protected:
	ACivilian(const std::string& name, float velocity, int visibility, TCoordinates position,
		TCoordinates destination, unsigned vulnerability = 100) :
		IShip(name, velocity, visibility, position, destination), Vulnerability(vulnerability)
	{}


protected:
	// Chance to be attacked (percents?).
	const unsigned int Vulnerability;

	/* Set to true when the ship was unsuccessfully attacked.
		 In case when it was successfully attacked - it sinks and gets deleted
		 from the map, so there is no need to set Attacked to true. */
	bool Attacked = false;
};

/* Vulnerability = 60%. */
class TBulkCarrier : public ACivilian
{
public:
	TBulkCarrier(const std::string& name, float velocity, int visibility, TCoordinates position,
		TCoordinates destination) : ACivilian(name, velocity, visibility, position, destination, 60)
	{}

	virtual ~TBulkCarrier() = default;
};

/* Vulnerability = 80%.
   Tankers transport milions worth of oil, so it's a lucrative target for the pirates.*/
class TTanker : public ACivilian
{
public:
	TTanker(const std::string& name, float velocity, int visibility, TCoordinates position,
		TCoordinates destination) : ACivilian(name, velocity, visibility, position, destination, 80)
	{}

	virtual ~TTanker() = default;
};

/* Vulnerability = 30%.
   It's hard to get away with attacking passenger ships, so the pirates prefer cargo ships.*/
class TPassenger : public ACivilian
{
public:
	TPassenger(const std::string& name, float velocity, int visibility, TCoordinates position,
		TCoordinates destination) : ACivilian(name, velocity, visibility, position, destination, 30)
	{}

	virtual ~TPassenger() = default;
};

class TPirate : public IShip
{
public:
	TPirate(float velocity, float visibility,
		TCoordinates position, TCoordinates destination) : IShip("The Green Oyster",
			velocity, visibility, position, destination), Target(nullptr)
	{}

	// Needed by TGame's constructor.
	TPirate() = default;

	// Do not call "delete" on Target, pirate does not own this pointer.
	virtual ~TPirate() = default;
	
	void ModifyVelocity(float baseVelocity)
	{
		Velocity = baseVelocity * 1.25;
	}

	virtual void debug_IntroduceYourself() const override
	{
		std::cout << "Argghh!" << std::endl;
		IShip::debug_IntroduceYourself();
	}

private:
	// Const pointer to a ship the pirate is following. Can be null (no target).
	const IShip* Target;
};

#endif // _SHIP_H