#ifndef _SHIP_H
#define _SHIP_H

/* The problem is that I want three separate interfaces (IShip, ICivilian, IPirate), but
	 TPirate and civilians have many common fields, so they should inherit from common class
	 (f.e. AShip as it was before). */

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
	virtual ~IShip() = default;

	virtual float GetVelocity() const = 0;
	virtual float GetRangeOfView() const = 0;
	virtual TCoordinates GetPosition() const = 0;
	virtual void debug_IntroduceYourself() const = 0;
	virtual void Move(TCoordinates coordinates) = 0;

protected:
	IShip() = default;
};

class ICivilian : public IShip
{
public:
	virtual ~ICivilian() = default;

	virtual bool IsRunningAway() const = 0;
	virtual void SetRunningAway() = 0;
	virtual TCoordinates GetDestination() const = 0;
	virtual void ChangeDestination(TCoordinates coordinates) = 0;

protected:
	ICivilian() = default;
};

class AShip : public ICivilian
{
public:
	virtual ~AShip() = default;

	virtual void debug_IntroduceYourself() const;

	virtual bool IsRunningAway() const override;
	virtual void SetRunningAway() override;
	virtual float GetVelocity() const override;
	virtual float GetRangeOfView() const override;
	virtual TCoordinates GetPosition() const override;
	virtual TCoordinates GetDestination() const override;

	virtual void ChangeDestination(TCoordinates coordinates) override;

protected:
	// Needed by TGame's constructor in order to construct empty TPirate.
	AShip() = default;

	AShip(const std::string& name, float velocity, float visibility, TCoordinates position,
		TCoordinates destination) : Name(name), Velocity(velocity), Visibility(visibility),
		Position(position), Destination(destination), RunningAway(false)
	{}

	virtual void Move(TCoordinates coordinates) override
	{
		Position = coordinates;
	}

protected:
	// Name. Separate identificator to be added? Is it neccessary?
	std::string Name;

	// True if ship has spotted the pirates and changed its destination.
	bool RunningAway;

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
class ACivilian : public AShip
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
		AShip::debug_IntroduceYourself();
		std::cout << "Vulnerability: " << Vulnerability << std::endl <<
			"Attacked: " << std::boolalpha << Attacked << std::endl;
	}

protected:
	ACivilian(const std::string& name, float velocity, float visibility, TCoordinates position,
		TCoordinates destination, unsigned vulnerability = 100) :
		AShip(name, velocity, visibility, position, destination), Vulnerability(vulnerability)
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
	TBulkCarrier(const std::string& name, float velocity, float visibility, TCoordinates position,
		TCoordinates destination) : ACivilian(name, velocity, visibility, position, destination, 60)
	{}

	virtual ~TBulkCarrier() = default;
};

/* Vulnerability = 80%.
   Tankers transport milions worth of oil, so it's a lucrative target for the pirates.*/
class TTanker : public ACivilian
{
public:
	TTanker(const std::string& name, float velocity, float visibility, TCoordinates position,
		TCoordinates destination) : ACivilian(name, velocity, visibility, position, destination, 80)
	{}

	virtual ~TTanker() = default;
};

/* Vulnerability = 30%.
   It's hard to get away with attacking passenger ships, so the pirates prefer cargo ships.*/
class TPassenger : public ACivilian
{
public:
	TPassenger(const std::string& name, float velocity, float visibility, TCoordinates position,
		TCoordinates destination) : ACivilian(name, velocity, visibility, position, destination, 30)
	{}

	virtual ~TPassenger() = default;
};

class TPirate final : public IShip
{
public:
	TPirate(float velocity, float visibility, TCoordinates position, TCoordinates destination) : 
		Name("The Green Oyster"), Velocity(velocity), Visibility(visibility), Position(position), 
		Destination(destination), Target(nullptr)
	{}

	// Needed by TGame's constructor.
	TPirate() = default;

	// Do not call "delete" on Target, pirate does not own this pointer.
	~TPirate() = default;
	
	// IShip overrides:
	virtual float GetVelocity() const override;
	virtual float GetRangeOfView() const override;
	virtual TCoordinates GetPosition() const override;

	void ModifyVelocity(float baseVelocity);
	void ChangeTarget(const IShip* target);
	const IShip* GetTarget() const;

	virtual void Move(TCoordinates coordinates) override
	{
		Position = coordinates;
	}

	virtual void debug_IntroduceYourself() const override;

private:
	// Const pointer to a ship the pirate is following. Can be null (no target).
	const IShip* Target;

	// Common fields for Pirate and civilians.
	std::string Name;
	float Velocity;
	float Visibility;
	TCoordinates Position;
	TCoordinates Destination;
};

#endif // _SHIP_H