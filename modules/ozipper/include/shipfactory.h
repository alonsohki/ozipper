#ifndef __SHIPFACTORY_H
#define __SHIPFACTORY_H

#include <map>
#include <string>
#include "ship.h"
#include "exception.h"

class ShipFactory
{
public:
  void CreateShip(const std::string& alias, const std::string& name, int metal, int crystal, int deuterium);
  const Ship& GetShip(const std::string& alias) throw(Exception);

  static ShipFactory * getInstance();

private:
  std::map<const std::string, Ship> m_ships;
};

#endif /* #ifndef __SHIPFACTORY_H */
