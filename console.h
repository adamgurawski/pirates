// File console.h, contains functions used for console-input validation.

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <map>
#include <string>
#include <vector>

namespace game
{
  struct TShip;
}

namespace console
{
	bool HandleShipList(size_t& idx, std::vector<std::string>& input, std::vector<game::TShip>& ships);

	/* Validates parameters. When wrong parameters (wrong values, wrong order) were passed or
		 "-h" present, returns false. Otherwise returns true. */
	bool ValidateInput(int argc, const char** argv, std::map<std::string, int>& params,
		std::vector<game::TShip>& ships);

	void DisplayHelp();
}

#endif // _CONSOLE_H
