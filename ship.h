#ifndef _SHIP_H
#define _SHIP_H

#include <iostream> // debugging purposes
#include <memory>
#include <string>

#include "coordinates.h"

// Disable "... inherits via dominance" warning.
#pragma warning (disable: 4250)

// Disable "this kind of function may not throw".
#pragma warning (disable: 26439)

/*
		IShip             AShip (template deriving from IShip or ICivilian)
			|              |      |
	ICivilian	     TPirate    ACivilian
	                          |   |    |
													concrete civilian classes
*/

class IShip
{
public:
	virtual ~IShip() = default;

	virtual float GetVelocity() const = 0;
	virtual float GetRangeOfView() const = 0;
	virtual TCoordinates GetPosition() const = 0;
	virtual void Move(const TCoordinates& coordinates) = 0;
	virtual void debug_IntroduceYourself() const = 0;
	virtual bool debug_IsPirate() const = 0;

protected:
	IShip() = default;
};

// Interface used by TGame (TGame keeps list of std::unique_ptr<ICivilian>).
class ICivilian : public IShip
{
public:
	virtual ~ICivilian() = default;

	virtual bool WasAttacked() const = 0;
	virtual void SetAttacked() = 0;
	virtual bool IsRunningAway() const = 0;
	virtual void SetRunningAway() = 0;
	virtual TCoordinates GetDestination() const = 0;
	virtual float GetVulnerability() const = 0;
	virtual void ChangeDestination(TCoordinates coordinates) = 0;

protected:
	ICivilian() = default;
};

// Common base class for all ships (pirate and civilians).
template <class Interface>
class AShip : public Interface
{
public:
	virtual ~AShip() = default;

	virtual float GetVelocity() const override
	{
		return Velocity;
	}

	virtual float GetRangeOfView() const override
	{
		return Visibility;
	}

	virtual TCoordinates GetPosition() const override
	{
		return Position;
	}

	virtual void Move(const TCoordinates& coordinates) override
	{
		Position = coordinates;
	}

	virtual void debug_IntroduceYourself() const override
	{
		std::cout << "Name: " << Name << std::endl << "Velocity: " <<
			Velocity << std::endl << "Visibility: " << Visibility << std::endl <<
			"Position: [" << Position.X << " " << Position.Y << "]" << std::endl <<
			"Destination: [" << Destination.X << " " << Destination.Y << "]" << std::endl;
	}

protected:
	// Needed by TGame's constructor in order to construct empty TPirate.
	// Is it anymore after changing inheritance to template?
	AShip() = default;

	AShip(const std::string& name, float velocity, float visibility, TCoordinates position,
		TCoordinates destination) : Name(name), Velocity(velocity), Visibility(visibility),
		Position(position), Destination(destination)
	{}

	// Move constructor.
	AShip(AShip&& rhs) : Name(std::move(rhs.Name)),
		Velocity(std::move(rhs.Velocity)),
		Visibility(std::move(rhs.Visibility)),
		Position(std::move(rhs.Position)),
		Destination(std::move(rhs.Destination))
	{}

	// Move assignment operator.
	AShip& operator=(AShip&& rhs)
	{
		Name = std::move(rhs.Name);
		Velocity = std::move(rhs.Velocity);
		Visibility = std::move(rhs.Visibility);
		Position = std::move(rhs.Position);
		Destination = std::move(rhs.Destination);
		return *this;
	}

protected:
	// Name. Separate identificator to be added? Is it neccessary?
	std::string Name;

	// Speed.
	float Velocity;

	// Range of view.
	float Visibility;

	// Position on a map.
	TCoordinates Position;

	// Position the ship is travelling to.
	TCoordinates Destination;
};

// Common base class for all of civilian ships (potential pirate targets).
class ACivilian : public AShip<ICivilian>
{
public:
	virtual ~ACivilian() = default;

	virtual bool WasAttacked() const override;
	virtual void SetAttacked() override;
	virtual bool IsRunningAway() const override;
	virtual void SetRunningAway() override;
	virtual TCoordinates GetDestination() const override;
	virtual float GetVulnerability() const override;
	virtual void ChangeDestination(TCoordinates coordinates) override;

	virtual void debug_IntroduceYourself() const override
	{
		AShip::debug_IntroduceYourself();
		std::cout << "Vulnerability: " << Vulnerability << std::endl <<
			"Attacked: " << std::boolalpha << Attacked << std::endl;
	}

	virtual bool debug_IsPirate() const override
	{
		return false;
	}

protected:
	ACivilian(const std::string& name, float velocity, float visibility, TCoordinates position,
		TCoordinates destination, float vulnerability = 1.0f) :
		AShip(name, velocity, visibility, position, destination), Vulnerability(vulnerability),
		RunningAway(false), Attacked(false)
	{}


protected:
	// Chance to be attacked (percents?).
	const float Vulnerability;

	/* Set to true when the ship was unsuccessfully attacked.
		 If it was successfully attacked - it sinks and gets deleted
		 from the map, so there is no need to set Attacked = true in this case. */
	bool Attacked;

	// True if ship has spotted the pirates and is trying to leave the map.
	bool RunningAway;
};

/* Vulnerability = 60%. */
class TBulkCarrier : public ACivilian
{
public:
	TBulkCarrier(const std::string& name, float velocity, TCoordinates position,
		TCoordinates destination);

	virtual ~TBulkCarrier() = default;
};

/* Vulnerability = 80%.
   Tankers transport milions worth of oil, so it's a lucrative target for the pirates.*/
class TTanker : public ACivilian
{
public:
	TTanker(const std::string& name, float velocity, TCoordinates position, TCoordinates destination);

	virtual ~TTanker() = default;
};

/* Vulnerability = 30%.
   It's hard to get away with attacking passenger ships, so the pirates prefer cargo ships.*/
class TPassenger : public ACivilian
{
public:
	TPassenger(const std::string& name, float velocity, TCoordinates position,
		TCoordinates destination);

	virtual ~TPassenger() = default;
};

#endif // _SHIP_H