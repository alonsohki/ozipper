#include <string>
#include <map>
#include "exception.h"
#include "shipfactory.h"

ShipFactory * ShipFactory::getInstance()
{
  static ShipFactory * instance = 0;

  if (instance == 0)
  {
    instance = new ShipFactory;
  }

  return instance;
}

void ShipFactory::CreateShip(const std::string& alias, const std::string& name, int metal, int crystal, int deuterium)
{
  std::pair<const std::string, Ship> m_pair(alias, Ship(name, metal, crystal, deuterium));
  m_ships.insert(m_pair);
}

const Ship& ShipFactory::GetShip(const std::string& alias) throw(Exception)
{
  std::map<const std::string, Ship>::iterator i;

  i = m_ships.find(alias);
  if (i == m_ships.end())
  {
    EXCEPTION("Ship \"%s\" not found", alias.c_str());
  }
    
  return (*i).second;
}

const std::map<const std::string, Ship, ShipFactoryOrder>& ShipFactory::GetShips()
{
  return m_ships;
}

bool ShipFactoryOrder::operator()(const std::string& a, const std::string& b)
{
  return strcasecmp(a.c_str(), b.c_str()) != 0;
}

bool ShipFactoryOrder::less(const std::string& a, const std::string& b)
{
  return false;
}
