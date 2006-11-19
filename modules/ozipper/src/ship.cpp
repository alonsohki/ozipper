#include "ship.h"

Ship::Ship(const std::string& p_name, unsigned int p_metal, unsigned int p_crystal, unsigned int p_deuterium) :
      name(p_name), metal(p_metal), crystal(p_crystal), deuterium(p_deuterium)
{
}

Ship::Ship(const Ship &ship) :
      name(ship.name), metal(ship.metal), crystal(ship.crystal), deuterium(ship.deuterium)
{
}
