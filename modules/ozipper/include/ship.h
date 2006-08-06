#ifndef __SHIP_H
#define __SHIP_H

#include <string>

class Ship
{
public:
  Ship(const std::string& name, int metal, int crystal, int deuterium);
  Ship(const Ship& ship);

  const std::string name;
  const int metal;
  const int crystal;
  const int deuterium;
};

#endif /* #ifndef __SHIP_H */
