#include <string>
#include <map>

#include "exception.h"
#include "player.h"

std::map< const std::string, std::map<const std::string, Player> > Player::players;

const Player& Player::GetPlayer(const std::string& name,
                                const std::string& role,
				const std::string& coords,
				int weapons,
				int shield,
				int armour,
				bool create)
                                throw(Exception)
{
  std::map<const std::string, Player>::iterator i;

  if (players.begin() == players.end())
  {
    /* Inicialización */
    std::map<const std::string, Player> m_empty;
    std::pair<const std::string, std::map<const std::string, Player> > m_pair_attacker("attacker", m_empty);
    std::pair<const std::string, std::map<const std::string, Player> > m_pair_defender("defender", m_empty);

    players.insert(m_pair_attacker);
    players.insert(m_pair_defender);
  }

  i = players[role].find(name);
  if (i == players[role].end())
  {
    if (!create)
    {
      EXCEPTION("Player %s not found", name.c_str());
    }
    else
    {
      std::pair<const std::string, Player> m_pair(name, Player(name, role, coords, weapons, shield, armour));
      players[role].insert(m_pair);
      i = players[role].find(name);
    }
  }

  return (*i).second;
}

const std::map<const std::string, Player> & Player::GetPlayers(const std::string& role)
{
  return players[role];
}

Player::Player(const Player& player) :
               m_name(player.m_name), m_coords(player.m_coords),
	       m_weapons(player.m_weapons), m_shield(player.m_shield),
	       m_armour(player.m_armour), m_role(player.m_role)
{
}

Player::Player(const std::string& name, const std::string& role, const std::string& coords, int weapons, int shield, int armour) :
               m_name(name), m_coords(coords),
	       m_weapons(weapons), m_shield(shield),
	       m_armour(armour), m_role(role)
{
}

void Player::CreateShip(const std::string& name, unsigned int count, int round)
{
  std::map<const std::string, unsigned int>::iterator i;

  i = m_ships[round].find(name);
  if (i == m_ships[round].end())
  {
    std::pair<const std::string, unsigned int> m_pair(name, count);
    m_ships[round].insert(m_pair);
  }
  else
  {
    (*i).second += count;
  }
}

unsigned int Player::GetShipCount(const std::string& name, int round) throw(Exception)
{
  std::map<const std::string, unsigned int>::iterator i;

  i = m_ships[round].find(name);
  if (i == m_ships[round].end())
  {
    EXCEPTION("Ship %s not found", name.c_str());
  }

  return (*i).second;
}

const std::string& Player::GetName() const
{
  return m_name;
}
const std::string& Player::GetRole() const
{
  return m_role;
}
const std::string& Player::GetCoords() const
{
  return m_coords;
}
int Player::GetWeapons() const
{
  return m_weapons;
}
int Player::GetShield() const
{
  return m_shield;
}
int Player::GetArmour() const
{
  return m_armour;
}
const std::map<const std::string, unsigned int>& Player::GetShips(int round) 
{
  return m_ships[round];
}
