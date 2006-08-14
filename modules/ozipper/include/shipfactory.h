#ifndef __SHIPFACTORY_H
#define __SHIPFACTORY_H

#include <map>
#include <string>
#include "ship.h"
#include "exception.h"

struct ShipFactoryOrder
{
  bool operator()(const std::string& a, const std::string& b);
  bool less(const std::string& a, const std::string& b);
};

class ShipFactory
{
public:
  void CreateShip(const std::string& alias, const std::string& name, int metal, int crystal, int deuterium);
  const Ship& GetShip(const std::string& alias) throw(Exception);
  const std::map<const std::string, Ship, ShipFactoryOrder>& GetShips();

  static ShipFactory * getInstance();

private:
  std::map<const std::string, Ship, ShipFactoryOrder> m_ships;
};

#endif /* #ifndef __SHIPFACTORY_H */
