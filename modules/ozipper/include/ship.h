#ifndef __SHIP_H
#define __SHIP_H

#include <string>

struct Costs
{
  unsigned long long metal;
  unsigned long long crystal;
  unsigned long long deuterium;
};

class Ship
{
public:
  Ship(const std::string& name, unsigned int metal, unsigned int crystal, unsigned int deuterium);
  Ship(const Ship& ship);

  const std::string name;
  const unsigned int metal;
  const unsigned int crystal;
  const unsigned int deuterium;
};

#endif /* #ifndef __SHIP_H */
