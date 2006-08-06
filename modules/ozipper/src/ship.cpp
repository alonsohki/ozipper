#include "ship.h"

Ship::Ship(const std::string& p_name, int p_metal, int p_crystal, int p_deuterium) :
      name(p_name), metal(p_metal), crystal(p_crystal), deuterium(p_deuterium)
{
}

Ship::Ship(const Ship &ship) :
      name(ship.name), metal(ship.metal), crystal(ship.crystal), deuterium(ship.deuterium)
{
}
