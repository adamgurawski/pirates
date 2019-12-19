
#include "options.h"
#include "game.h"

#include <iostream>
#include <exception>
#include <string>
#include <vector>

options::TOptions::TOptions(int argc, const char** argv) : ArgCount(argc), Args(argv)
{
	if (!ValidateInput())
	{ 
		DisplayHelp();
		// TODO: return the nice way.
		exit(-1);
	}
}

bool options::TOptions::InvalidFormatOfInput() const
{
	std::cerr << "Error: Invalid format of input." << std::endl;
	return false;
}

bool options::TOptions::InvalidNumberOfArguments() const
{
	std::cerr << "Error: Invalid number of arguments." << std::endl;
	return false;
}

void options::TOptions::DisplayHelp() const
{ // TODO: implement.
	std::cout << "Help called, to be implemented." << std::endl;
}

bool options::TOptions::ValidateInput()
{
	if (ArgCount < RequiredArgs)
		return InvalidNumberOfArguments();	

	try
	{
		for (int i = 1; i < ArgCount;)
		{
			const std::string parameter = Args[i];

			if (parameter == "-s")
			{ // Ship list.
				HandleShipList(++i);
			}
			else if (parameter == "-m")
			{ // Map dimenions.
				HandleMapDimensions(++i);
			}
			else if (parameter == "-t")
			{ // Simulation time.
				HandleSimulationTime(++i);
			}
			else
			{
				++i;
			}
		}
	}
	catch (std::invalid_argument)
	{ // std::stoi failed.
		return InvalidFormatOfInput();
	}
	catch (std::out_of_range)
	{ // std::vector::at failed.
		return InvalidFormatOfInput();
	}
	catch (std::logic_error logic)
	{ // time/speed negative
		std::cerr << logic.what() << std::endl;
		return false;
	}

	return true;
}

bool options::TOptions::IsNotSwitch(int idx) const
{
	std::string arg = Args[idx];

	if (arg == "-s")
		return false;
	else if (arg == "-m")
		return false;
	else if (arg == "-t")
		return false;
	else
		return true;
}

void options::TOptions::HandleShipList(int& idx)
{
	for (; idx + 1 < ArgCount && IsNotSwitch(idx);)
	{
		std::string name = Args[idx++];
		int velocity = std::stoi(Args[idx++]);
		int time = std::stoi(Args[idx++]);

		if (velocity <= 0)
			throw std::logic_error("Velocity value shall be positive.");
		else if (time < 0)
			throw std::logic_error("Time value cannot be negative.");

		TShipInfo ship({ name, velocity, time });
		Ships.push_back(ship);
	}
}

void options::TOptions::HandleMapDimensions(int& widthIdx)
{
	int width = std::stoi(Args[widthIdx++]);
	int height = std::stoi(Args[widthIdx++]);

	if (width < 0 || height < 0)
		throw std::logic_error("Map width/height value shall be positive.");

	MapWidth = width;
	MapHeight = height;
}

void options::TOptions::HandleSimulationTime(int& timeIdx)
{
	int time = std::stoi(Args[timeIdx++]);

	if (time < 0)
		throw std::logic_error("Time value cannot be negative.");

	SimulationTime = time;
}