#ifndef __PLAYER_H
#define __PLAYER_H

#include <string>
#include <map>
#include "exception.h"

class Player
{
public:
  Player(const Player& player);

  const std::string& GetName() const;
  const std::string& GetRole() const;
  const std::string& GetCoords() const;
  int GetWeapons() const;
  int GetShield() const;
  int GetArmour() const;
  const std::map<const std::string, unsigned int>& GetShips(int round);

  void CreateShip(const std::string& name, unsigned int count, int round);
  unsigned int GetShipCount(const std::string& name, int round) throw(Exception);

  static const Player& GetPlayer(const std::string& name,
                                 const std::string& role,
				 const std::string& coords = "",
				 int weapons = 0,
				 int shield = 0,
				 int armour = 0,
				 bool create = false)
                                 throw(Exception);
  static const std::map<const std::string, Player> & GetPlayers(const std::string& role);

private:
  Player(const std::string& m_name, const std::string& role, const std::string& coords, int weapons, int shield, int armour);

  static std::map< const std::string, std::map<const std::string, Player> > players;
  std::string m_name;
  std::string m_coords;
  int m_weapons;
  int m_shield;
  int m_armour;
  std::string m_role;
  std::map<const std::string, unsigned int> m_ships[2]; /* Flota inicial y final */
};

#endif /* #ifndef __PLAYER_H */
