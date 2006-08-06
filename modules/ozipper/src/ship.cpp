#include "ship.h"

Ship::Ship(int p_metal, int p_crystal, int p_deuterium) :
      metal(p_metal), crystal(p_crystal), deuterium(p_deuterium)
{
}

Ship::Ship(const Ship &ship) :
      metal(ship.metal), crystal(ship.crystal), deuterium(ship.deuterium)
{
}
