#ifndef __SHIP_H
#define __SHIP_H

class Ship
{
public:
  Ship(int metal, int crystal, int deuterium);
  Ship(const Ship& ship);

  const int metal;
  const int crystal;
  const int deuterium;
};

#endif /* #ifndef __SHIP_H */
