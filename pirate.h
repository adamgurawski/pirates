#ifndef _PIRATE_H
#define _PIRATE_H

#include "ship.h"

class IPirateBrain
{
public:
	virtual ~IPirateBrain() = default;

	virtual TCoordinates SetNewDestination(TCoordinates position, const IShip* target,
		float velocity) = 0;

protected:
	IPirateBrain() = default;
};

class TSimpleBrain : public IPirateBrain
{
public:
	TSimpleBrain(int maxX, int maxY);

	virtual TCoordinates SetNewDestination(TCoordinates position, const IShip* target,
		float velocity) override;

private:
	int MaxX;
	int MaxY;
	TCoordinates LongTermDestination;
};

class TPirate final : public AShip
{
public:
	TPirate(TCoordinates position, unsigned int mapWidth, unsigned int mapHeight);

	// Can't copy unique_ptr.
	TPirate(const TPirate& rhs) = delete;
	TPirate operator=(const TPirate& rhs) = delete;

	// Move assignment operator.
	TPirate(TPirate&& rhs);
	TPirate& operator=(TPirate&& rhs);

	// Needed by TGame's constructor.
	TPirate() = default;

	// Do not call "delete" on Target, pirate does not own this pointer.
	~TPirate() = default;


	// Methods below are meant to be called by TGame, because it is the only entity that holds
	// a TPirate object:
	
	// Moves the pirate to last destination and sets new destination.
	void Move();

	// Sets pirate's velocity as base * 1.25.
	void ModifyVelocity(float fastestCivilianVelocity);

	void ChangeTarget(const IShip* target);
	const IShip* GetTarget() const;

	virtual void debug_IntroduceYourself() const override;

private:
	// To be used on every move.
	void UpdateDestination();

	// Try to move to destination. If successful, return true.
	// It may not be successful if pirate's speed was changed in the turn before.
	bool HeadToDestination();

private:
	// Const pointer to a ship the pirate is following. Can be null (no target).
	const IShip* Target;

	// Brain responsible for deciding where to go.
	std::unique_ptr<IPirateBrain> Brain;
};

#endif // _PIRATE_H