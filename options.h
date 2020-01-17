// File options.h, contains TOptions responsible for console-input validation.

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <string>
#include <utility>
#include <set>

/*	Required arguments:
	0:	(default) program name
	1:	- s (ship)
	2:	ship's name
	3:	ship's speed
	4:	ship's time to generation
	5:	-m (map size)
	6:	map width
	7:	map height
	8:	-t (time)
	9:	time in "time units"
	10: (optional) -g (graphical representation of current situation on map)
	11: (optional) -v (verbose) used to print messages related to changing
			ships' position, if not passed, only "relevant" actions get printed, f.e.
			ship left the map, ship got successfully/unsuccessfully attacked. */

struct TShipInfo
{
	std::string Name;
	float				Velocity;
	int					TimeToGeneration;
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
		{ return SimulationTime; }
	unsigned int GetMapWidth() const
		{ return MapWidth; }
	unsigned int GetMapHeight() const
		{ return MapHeight; }
	bool IsGraphical() const
		{	return Graphical; }
	bool IsVerbose() const
		{ return Verbose; }
	bool IsValid() const
		{ return ValidParams; }

	TShipInfoSet&& StealShipInfo()
		{ return std::move(Ships); }

private:
	// Check if exactly one -t/-m is present and minimum of one -s.
	// Verify that one (-t), two (-m), or three (-s) next arguments
	// are not switches.
	bool PreValidate() const;
	// Validate parameters. When wrong parameters (wrong values, wrong order) were passed or
	// "-h" present, return false. Otherwise return true.
	bool ValidateInput();
	// Tell if idx parameter is not a switch.
	bool IsNotSwitch(int idx) const;
	// Take first ship's name's idx as parameter, add its info to the vector.
	void HandleShip(int& idx);
	// Make sure given map dimensions are valid and set MapWidth and MapHeight.
	void SetMapDimensions(int& widthIdx);
	// Make sure given time is valid (positive) and set simulation time.
	void SetSimulationTime(int& timeIdx);

	// Errors. Used to describe exceptions thrown by f.e. std::stoi the human way.
	bool InvalidFormatOfInput() const;
	bool InvalidNumberOfArguments() const;

	// Display tips on how to use the program.
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

	// Display options.
	bool Graphical;
	bool Verbose;

	// Based on data stored in this vector, the ships will be created by TGame.
	// It's a multiset in order to allow sorted inserting.
	TShipInfoSet Ships;
};

} // namespace options
#endif // _OPTIONS_H
