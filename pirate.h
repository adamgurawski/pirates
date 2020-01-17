// File pirate.h, contains TPirate, TTargetWrapper (wrapper to pirate's target),
// and IPirateBrain/TSimpleBrain (pirate's brain) responsible for deciding
// where the pirate wants to go.

#ifndef _PIRATE_H
#define _PIRATE_H

#include "ship.h"

/* This wrapper is necessary because TPirate::Target can be nullptr and TSimpleBrain
	 holds a reference to Target. */
class TTargetWrapper
{
public:
	// Do not delete Target, wrapper does not own this resource.
	~TTargetWrapper() = default;

	// Constructors.
	TTargetWrapper() { Target = nullptr; }
	TTargetWrapper(const IShip* target) { Target = target; }
	TTargetWrapper(const TTargetWrapper& rhs) : Target(rhs.Target) {}
	TTargetWrapper(TTargetWrapper&& rhs) : Target(std::move(rhs.Target)) {}

	// Operators.
	const IShip* operator*() { return Target;	}
	const IShip* const operator*() const { return Target; }
	const IShip* operator->() { return Target; }
	const IShip* const operator->() const { return Target; }

	TTargetWrapper& operator=(const IShip* ship);
	TTargetWrapper& operator=(const TTargetWrapper& rhs);
	TTargetWrapper& operator=(TTargetWrapper&& rhs);

	bool IsEmpty() const { return Target == nullptr; }

private:
	const IShip* Target;
};

class IPirateBrain
{
public:
	virtual ~IPirateBrain() = default;
	virtual TCoordinates GetDesiredDestination(unsigned attempts) = 0;
	virtual void SetMapBorders(unsigned int maxX, unsigned int maxY) = 0;

protected:
	IPirateBrain() = default;
};

class TSimpleBrain : public IPirateBrain
{
public:
	TSimpleBrain(TCoordinates& position, TCoordinates& destination,  float& velocity, 
		TTargetWrapper& target);

	// Move assignment operator.
	TSimpleBrain& operator=(TSimpleBrain&& rhs);

	virtual TCoordinates GetDesiredDestination(unsigned attempts) override;
	virtual void SetMapBorders(unsigned int maxX, unsigned int maxY) override;

private:
	// Return true if coordinates are reachable in this turn.
	bool CanReach(TCoordinates point) const;
	// Return reachable coordinates with distance to target no longer than 1.
	TCoordinates GetPositionNearTarget(unsigned attempts) const;
	// Align with target's X, then follow along Y.
	TCoordinates ChaseTarget(int adjustedVelocity) const;
	// Align with target's X.
	TCoordinates AlignWithX(const TCoordinates& position,
		const TCoordinates& target, unsigned int& velocity) const;
	// Align with target's Y.
	TCoordinates AlignWithY(const TCoordinates& position,
		const TCoordinates& target, unsigned int& velocity) const;
	// Move in zig-zag pattern while there's no target in sight. Note that
	// if map size is (1, x) or (x, 1) zig-zag is impossible therefore
	// exception "Error: coordinates out of range" is thrown.
	TCoordinates ZigZag(int adjustedVelocity);
	
private:
	unsigned int MaxX;
	unsigned int MaxY;
	
	// Used by zig-zag mechanism, can be going left (false) or right (true).
	bool GoingRight;
	// Used by zig-zag mechanism, can be going up (false) or down (true).
	bool GoingDown;

	// References to TPirate's members.
	float& Velocity;
	TCoordinates& Position;
	TCoordinates& Destination;
	TTargetWrapper& Target;
};

class TPirate final : public AShip<IShip>
{
public:
	// Default constructor needed by TGame's constructor.
	TPirate() = default;
	// Do not call "delete" on Target, pirate does not own this pointer.
	~TPirate() = default;
	// 
	TPirate(TCoordinates position);
	// Move constructor.
	TPirate(TPirate&& rhs);
	// Move assignment operator.
	TPirate& operator=(TPirate&& rhs);

	// Methods below are meant to be called by TGame, because it is the only entity that holds
	// a TPirate object.
	
	// Use Brain to return desired destination.
	// (note that Brain changes Destination member)
	TCoordinates GetDesiredDestination(unsigned attempts);
	// Set Velocity as fastestCivilian multiplied by 1.25.
	void ModifyVelocity(float fastestCivilianVelocity);
	// Change destination if target points to a ship (nullptr otherwise).
	void ChangeTarget(const IShip* target);
	// Return target the ship is following.
	const IShip* GetTarget() const;
	// Set Destination = destination.
	void ChangeDestination(const TCoordinates& destination);
	// Pass map borders to Brain.
	void SetMapBorders(unsigned int maxX, unsigned int maxY);
	// Print all of information about pirate.
	virtual void IntroduceYourself() const override;

private:
	// Note that the target can be nullptr (of course not the wrapper itself).
	TTargetWrapper Target;
	// Brain responsible for deciding where to go.
	std::unique_ptr<IPirateBrain> Brain;
};

#endif // _PIRATE_H