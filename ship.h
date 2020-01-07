#ifndef _SHIP_H
#define _SHIP_H

// Disable "... inherits via dominance" warning.
#pragma warning (disable: 4250)

#include <iostream> // debugging purposes
#include <memory>
#include <string>

struct TCoordinates
{
	unsigned int X = 0;
	unsigned int Y = 0;
};

/*
					 IShip
					/     \
		AShip       ICivilian
		 |    \     /
		 |		 ACivilian
		TPirate
*/

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

// Common base class for all ships (pirate and civilians).
class AShip : virtual public IShip
{
public:
	virtual ~AShip() = default;

	virtual float GetVelocity() const override;
	virtual float GetRangeOfView() const override;
	virtual TCoordinates GetPosition() const override;
	virtual void debug_IntroduceYourself() const;
	virtual void Move(TCoordinates coordinates) override;

protected:
	// Needed by TGame's constructor in order to construct empty TPirate.
	AShip() = default;

	AShip(const std::string& name, float velocity, float visibility, TCoordinates position,
		TCoordinates destination) : Name(name), Velocity(velocity), Visibility(visibility),
		Position(position), Destination(destination)
	{}

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

// Interface used by TGame (TGame keeps list of std::unique_ptr<ICivilian>).
class ICivilian : virtual public IShip
{
public:
	virtual ~ICivilian() = default;

	virtual bool WasAttacked() const = 0;
	virtual void SetAttacked() = 0;
	virtual bool IsRunningAway() const = 0;
	virtual void SetRunningAway() = 0;
	virtual TCoordinates GetDestination() const = 0;
	virtual void ChangeDestination(TCoordinates coordinates) = 0;

protected:
	ICivilian() = default;
};

// Common base class for all of civilian ships (potential pirate targets).
class ACivilian : public AShip, public ICivilian
{
public:
	virtual ~ACivilian() = default;

	virtual bool WasAttacked() const override;
	virtual void SetAttacked() override;
	virtual bool IsRunningAway() const override;
	virtual void SetRunningAway() override;
	virtual TCoordinates GetDestination() const override;
	virtual void ChangeDestination(TCoordinates coordinates) override;

	virtual void debug_IntroduceYourself() const override
	{
		AShip::debug_IntroduceYourself();
		std::cout << "Vulnerability: " << Vulnerability << std::endl <<
			"Attacked: " << std::boolalpha << Attacked << std::endl;
	}

protected:
	ACivilian(const std::string& name, float velocity, float visibility, TCoordinates position,
		TCoordinates destination, unsigned vulnerability = 100) :
		AShip(name, velocity, visibility, position, destination), ICivilian(), 
		Vulnerability(vulnerability), RunningAway(false)
	{}


protected:
	// Chance to be attacked (percents?).
	const unsigned int Vulnerability;

	/* Set to true when the ship was unsuccessfully attacked.
		 If it was successfully attacked - it sinks and gets deleted
		 from the map, so there is no need to set Attacked = true in this case. */
	bool Attacked = false;

	// True if ship has spotted the pirates and is trying to leave the map.
	bool RunningAway;
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

class TPirate final : public AShip
{
public:
	TPirate(float velocity, float visibility, TCoordinates position, TCoordinates destination) : 
		AShip("The Green Oyster", velocity, visibility, position, destination), Target(nullptr)
	{}

	// Needed by TGame's constructor.
	TPirate() = default;

	// Do not call "delete" on Target, pirate does not own this pointer.
	~TPirate() = default;
  
	// Methods below are meant to be called by TGame, because it is the only entity that holds
	// a TPirate object:

	// Sets pirate's velocity as base * 1.25.
	void ModifyVelocity(float baseVelocity);

	void ChangeTarget(const IShip* target);
	const IShip* GetTarget() const;

	virtual void debug_IntroduceYourself() const override;

private:
	// Const pointer to a ship the pirate is following. Can be null (no target).
	const IShip* Target;
};

#endif // _SHIP_H