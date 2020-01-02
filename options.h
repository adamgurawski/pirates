// File options.h, contains functions used for console-input validation.

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <string>
#include <utility>
#include <set>

/*	Required arguments :
	(default) program name
		- s (ship)
		ship name
		ship speed
		ship time
		-m (map size)
		length
		width
		-t (time)
		time in seconds */

struct TShipInfo
{
	std::string Name;
	float Velocity;
	int TimeToGeneration;
};

// Comparer used by ship info set.
struct TTimeComparer
{
	bool operator()(TShipInfo lhs, TShipInfo rhs) const
	{
		return lhs.TimeToGeneration < rhs.TimeToGeneration;
	}
};

namespace options
{

class TOptions
{
public:
	using TShipInfoSet = std::multiset<TShipInfo, TTimeComparer>;

	TOptions(int argc, const char** argv);

	unsigned int GetSimulationTime() const
	{
		return SimulationTime;
	}

	unsigned int GetMapWidth() const
	{
		return MapWidth;
	}

	unsigned int GetMapHeight() const
	{
		return MapHeight;
	}

	TShipInfoSet&& GetShipInfo()
	{
		return std::move(Ships);
	}

	bool IsValid() const
	{
		return ValidParams;
	}

private:
	// Input validation:

	// Checks if exactly one -t/-m is present and minimum of one -s.
	// Verifies that one (-t), two (-m), or three (-s) next arguments
	// are not switches.
	bool PreValidate() const;

	/* Validates parameters. When wrong parameters (wrong values, wrong order) were passed or
		"-h" present, returns false. Otherwise returns true. */
	bool ValidateInput();

	bool IsNotSwitch(int idx) const;

	// Takes first ship's name's idx as parameter, add its info to the vector.
	void HandleShip(int& idx);

	void HandleMapDimensions(int& widthIdx);

	void HandleSimulationTime(int& timeIdx);

	// Errors.
	bool InvalidFormatOfInput() const;
	bool InvalidNumberOfArguments() const;

	void DisplayHelp() const;

private:
	int ArgCount;
	const char** Args;
	bool ValidParams;

	// Minimal number of args.
	const int RequiredArgs = 10;

	// Game options.
	unsigned int SimulationTime;
	unsigned int MapWidth;
	unsigned int MapHeight;

	// Based on data stored in this vector, the ships will be created by TGame
	// Update: changed into multiset in order to allow sorted inserting.
	// TODO: verify that elements are inserted in desired order.
	TShipInfoSet Ships;
};

} // namespace options
#endif // _OPTIONS_H
