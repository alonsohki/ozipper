#include <string>
#include <map>
#include "exception.h"
#include "shipfactory.h"
#include "intl.h"

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
  for (std::map<const std::string, Ship>::const_reverse_iterator s = GetShips().rbegin();
       s != GetShips().rend();
       s++)
  {
    if (!strcasecmp((*s).first.c_str(), alias.c_str()))
    {
      return (*s).second;
    }
  }
  EXCEPTION(SHIP_NOT_FOUND, "Ship \"%s\" not found", alias.c_str());
}

const std::map<const std::string, Ship, ShipFactoryOrder>& ShipFactory::GetShips()
{
  return m_ships;
}

bool ShipFactoryOrder::operator()(const std::string& a, const std::string& b)
{
  return strcasecmp(a.c_str(), b.c_str()) != 0;
}
