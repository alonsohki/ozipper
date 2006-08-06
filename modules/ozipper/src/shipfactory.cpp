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

void ShipFactory::CreateShip(const std::string& name, int metal, int crystal, int deuterium)
{
  std::pair<const std::string, Ship> m_pair(name, Ship(metal, crystal, deuterium));
  m_ships.insert(m_pair);
}

const Ship& ShipFactory::GetShip(const std::string& name) throw(Exception)
{
  std::map<const std::string, Ship>::iterator i;

  i = m_ships.find(name);
  if (i == m_ships.end())
  {
    EXCEPTION("Ship \"%s\" not found", name.c_str());
  }
    
  return (*i).second;
}
