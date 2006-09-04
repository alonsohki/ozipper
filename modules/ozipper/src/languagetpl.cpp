#include <string>
#include <vector>
#include <pcre.h>
#include <fstream>
#include <sys/stat.h>
#include "exception.h"
#include "languagetpl.h"
#include "shipfactory.h"

int LanguageTemplate::ExecRegex(pcre *re, const std::string& subject, int offset, int *vector, size_t vsize, const char *regexName, bool match_error)
                                throw(Exception)
{
  int rc;

  rc = pcre_exec(re,
                 0,
		 subject.c_str(),
		 subject.length(),
		 offset,
		 0,
		 vector,
		 vsize);

  if (rc < 0)
  {
    switch (rc)
    {
      case PCRE_ERROR_NOMATCH:
	if (match_error)
	{
	  EXCEPTION("pcre_exec error: String didn't match with %s", regexName);
	}
	break;

      default:
	EXCEPTION("pcre_exec error: Error while matching: %d\n", rc);
	break;
    }
  }

  return rc;
}

static int IntRegex(const char *buffer, size_t length)
{
  char tmp[length + 1];
  memcpy(tmp, buffer, length);
  tmp[length] = '\0';

  return atoi(tmp);
}

static inline const char * JumpNull(const char **p)
{
  while (**p != '\0' && (**p == '\r' || **p == '\n' || **p == '\t' || **p == '\f' || **p == ' '))
  {
    *p = *p + 1;
  }

  return *p;
}
static inline const char * FindNull(const char **p)
{
  while (**p != '\0' && **p != '\r' && **p != '\n' && **p != '\t' && **p != '\f' && **p != ' ')
  {
    *p = *p + 1;
  }

  return *p;
}

static inline bool AppendShip(const char *a, const char *b, std::vector<std::string>& ship_names)
{
  std::string name;
  name.append(a, b - a);

  try
  {
    ShipFactory::getInstance()->GetShip(name);
    ship_names.push_back(name);
    return true;
  }
  catch (Exception &e)
  {
    return false;
  }
}

static inline void ParseShips(const char *report,
                              int names_begin,
		              int names_end,
                              int count_begin,
		              int count_end,
		              std::vector<std::string>& ship_names,
		              std::vector<unsigned int>& ship_counts)
{
  const char *p = report + names_begin;
  const char *p2;

  for (p = report + names_begin; JumpNull(&p) <= report + names_end; p++)
  {
    p2 = p;
    do
    {
      if (FindNull(&p2) > report + names_end)
      {
        p2 = report + names_end;
      }
      if (AppendShip(p, p2, ship_names))
      {
        break;;
      }
      p2++;
    } while (p2 < (report + names_end));

    p = p2;
  }

  for (p = report + count_begin; JumpNull(&p) <= report + count_end; p++)
  {
    p2 = p;
    if (FindNull(&p2) > report + count_end)
    {
      p2 = report + count_end;
    }

    ship_counts.push_back(IntRegex(p, p2 - p));
    p = p2;
  }
}

const ReportData& LanguageTemplate::Parse(const std::string& report) throw(Exception)
{
  int rc;
  static int ovector[64];
  int last_end;

  /* Obtenemos la fecha */
  rc = ExecRegex(m_re_date, report, 0, ovector, 64, "date");
  m_result.date.tm_mon  = IntRegex(report.c_str() + ovector[2],  ovector[3] - ovector[2]) - 1;
  m_result.date.tm_mday = IntRegex(report.c_str() + ovector[4],  ovector[5] - ovector[4]);
  m_result.date.tm_hour = IntRegex(report.c_str() + ovector[6],  ovector[7] - ovector[6]);
  m_result.date.tm_min  = IntRegex(report.c_str() + ovector[8],  ovector[9] - ovector[8]);
  m_result.date.tm_sec  = IntRegex(report.c_str() + ovector[10], ovector[11] - ovector[10]);
  last_end = ovector[1];

  /* Obtenemos la presentación de las flotas */
  do
  {
    rc = ExecRegex(m_re_fleets, report, last_end, ovector, 64, "fleets", false);

    if (rc > 0)
    {
      int x;

      last_end = ovector[1];

      /* Extraemos los datos del jugador */
      std::string name;
      std::string role;
      name.append(report.c_str() + ovector[6], ovector[7] - ovector[6]);
      role.append(report.c_str() + ovector[4], ovector[5] - ovector[4]);
      
      Player *player = (Player *)&Player::GetPlayer(
            name,									/* name */
	    role == m_roles[0] ? "attacker" : "defender",                               /* role */
	    std::string().append(report.c_str() + ovector[8], ovector[9] - ovector[8]), /* coords */
	    IntRegex(report.c_str() + ovector[12], ovector[13] - ovector[12]),		/* weapons */
	    IntRegex(report.c_str() + ovector[14], ovector[15] - ovector[14]),		/* shield */
	    IntRegex(report.c_str() + ovector[16], ovector[17] - ovector[16]),		/* armour */
	    true);

      /* Si no tiene flota ni defensa, rc == 9 */
      if (rc > 9)
      {
	std::vector<std::string> ship_names;
	std::vector<unsigned int> ship_counts;
	
        ParseShips(report.c_str(), ovector[24], ovector[25], ovector[28], ovector[29], ship_names, ship_counts);
        x = 0;
        for (std::vector<std::string>::const_iterator i = ship_names.begin(); i != ship_names.end(); i++)
        {
	  player->CreateShip(*i, ship_counts[x], 0);
	  x++;
        }
      }
    }
  } while (rc > 0);

  /* Construímos el vector de jugadores.
   * TODO: Eliminar esta función y utilizar el método estático de Player.
   */
  std::map<const std::string, Player>::const_iterator i;
  for (i = Player::GetPlayers("attacker").begin(); i != Player::GetPlayers("attacker").end(); i++)
  {
    const Player &p = (*i).second;
    m_result.players.push_back((Player *)&p);
  }
  for (i = Player::GetPlayers("defender").begin(); i != Player::GetPlayers("defender").end(); i++)
  {
    const Player &p = (*i).second;
    m_result.players.push_back((Player *)&p);
  }

  /* Buscamos la última ronda */
  m_result.rounds = 0;
  do
  {
    rc = ExecRegex(m_re_startround, report, last_end, ovector, 64, "start_round", false);

    if (rc > 0)
    {
      last_end = ovector[1];
      m_result.rounds++;
    }
  } while (rc > 0);

  do
  {
    rc = ExecRegex(m_re_round, report, last_end, ovector, 64, "round", false);

    if (rc > 0)
    {
      last_end = ovector[1];
      if (rc > 5)
      {
        int x;
        std::vector<std::string> ship_names;
        std::vector<unsigned int> ship_counts;
	std::string role;
	role.append(report.c_str() + ovector[4], ovector[5] - ovector[4]);

        Player * player = (Player *)&Player::GetPlayer(
	      std::string().append(report.c_str() + ovector[6], ovector[7] - ovector[6]),  /* name */
	      role == m_roles[0] ? "attacker" : "defender"); /* role */

        ParseShips(report.c_str(), ovector[14], ovector[15], ovector[18], ovector[19], ship_names, ship_counts);
        x = 0;
        for (std::vector<std::string>::const_iterator i = ship_names.begin(); i != ship_names.end(); i++)
        {
	  player->CreateShip(*i, ship_counts[x], 1);
	  x++;
        }
      }
    }
  } while (rc > 0);

  /* Resultado de la batalla */
  rc = ExecRegex(m_re_result, report, last_end, ovector, 64, "result");

  if (rc > 0)
  {
    last_end = ovector[1];
    std::string winner;
    winner.append(report.c_str() + ovector[6], ovector[7] - ovector[6]);
    
    if (winner != "")
    {
      m_result.winner = ((strcasecmp(winner.c_str(), m_roles[0].c_str())) ? "defender" : "attacker");
    }
    else
    {
      m_result.winner = "nobody";
    }
    m_result.captures.metal     = IntRegex(report.c_str() + ovector[14], ovector[15] - ovector[14]);
    m_result.captures.crystal   = IntRegex(report.c_str() + ovector[16], ovector[17] - ovector[16]);
    m_result.captures.deuterium = IntRegex(report.c_str() + ovector[18], ovector[19] - ovector[18]);
    m_result.losses.attacker    = IntRegex(report.c_str() + ovector[22], ovector[23] - ovector[22]);
    m_result.losses.defender    = IntRegex(report.c_str() + ovector[26], ovector[27] - ovector[26]);
    m_result.debris.metal       = IntRegex(report.c_str() + ovector[30], ovector[31] - ovector[30]);
    m_result.debris.crystal     = IntRegex(report.c_str() + ovector[32], ovector[33] - ovector[32]);
  }

  /* Verificamos si hay luna */
  rc = ExecRegex(m_re_moon, report, last_end, ovector, 64, "moon", false);

  if (rc > 0)
  {
    m_result.moonchance = IntRegex(report.c_str() + ovector[4], ovector[5] - ovector[4]);
    if (rc > 4)
    {
      m_result.moon = true;
    }
    else
    {
      m_result.moon = false;
    }
  }
  else
  {
    m_result.moon = false;
    m_result.moonchance = 0;
  }

  return m_result;
}

LanguageTemplate::~LanguageTemplate()
{
  if (m_re_date)
  {
    free(m_re_date);
  }
  if (m_re_fleets)
  {
    free(m_re_fleets);
  }
  if (m_re_startround)
  {
    free(m_re_startround);
  }
  if (m_re_round)
  {
    free(m_re_round);
  }
  if (m_re_result)
  {
    free(m_re_result);
  }
  if (m_re_moon)
  {
    free(m_re_moon);
  }
}

LanguageTemplate::LanguageTemplate(const std::string& lang) throw(Exception) :
                                   m_re_date(0), m_re_fleets(0), m_re_startround(0),
				   m_re_round(0), m_re_result(0), m_re_moon(0),
				   m_language(lang)
{
  InitRoles();
  InitShips();
  InitRegex();
}

void LanguageTemplate::InitRoles()
{
  std::ifstream file;
  std::string path("lang/" + m_language + "/roles");
  static char line[512];

  file.open(path.c_str());
  if (!file.good())
  {
    EXCEPTION("Can't open file %s", path.c_str());
  }

  file.getline(line, 512);
  m_roles[0] = line;
  file.getline(line, 512);
  m_roles[1] = line;

  file.close();
}

void LanguageTemplate::InitShips()
{
  std::ifstream file;
  std::string path("lang/" + m_language + "/ships");
  static char line[512];
  static char alias[512];
  static char name[512];
  int metal;
  int crystal;
  int deuterium;
  char * p;
  char * s;
  off_t offset;
  
  file.open(path.c_str());
  if (!file.good())
  {
    EXCEPTION("Can't open file %s", path.c_str());
  }

  while (!file.eof())
  {
    file.getline(line, 512);
    if (file.eof())
    {
      break;
    }

    p = strchr(line, ':');
    offset = (off_t)(p - line);
    memcpy(alias, line, offset);
    alias[offset] = '\0';
    s = p + 1;

    p = strchr(s, ':');
    offset = (off_t)(p - s);
    memcpy(name, s, offset);
    name[offset] = '\0';
    s = p + 1;
    
    sscanf(s, "%d %d %d", &metal, &crystal, &deuterium);

    ShipFactory::getInstance()->CreateShip(alias, name, metal, crystal, deuterium);
  }
  
  file.close();
}

void LanguageTemplate::ReadRegex(const std::string& path, std::string& dest, pcre **re) throw (Exception)
{
  std::ifstream file;
  char *tmp;
  const char *error;
  int erroffset;
  struct stat estado;

  stat(path.c_str(), &estado);
  file.open(path.c_str());
  if (!file.is_open())
  {
    EXCEPTION("Can't open file %s", path.c_str());
  }

  tmp = new char[estado.st_size + 1];
  file.read(tmp, estado.st_size);
  tmp[estado.st_size] = '\0';
  dest = tmp;
  delete[] tmp;

  file.close();

  *re = pcre_compile(dest.c_str(),
                     0,
		     &error,
		     &erroffset,
		     0);

  if (!*re)
  {
    EXCEPTION("pcre_compile failed (offset: %d), %s\n", erroffset, error);
  }
}

void LanguageTemplate::InitRegex() throw (Exception)
{
  ReadRegex("lang/" + m_language + "/regex_date", m_re_date_str, &m_re_date);
  ReadRegex("lang/" + m_language + "/regex_fleets", m_re_fleets_str, &m_re_fleets);
  ReadRegex("lang/" + m_language + "/regex_startround", m_re_startround_str, &m_re_startround);
  ReadRegex("lang/" + m_language + "/regex_round", m_re_round_str, &m_re_round);
  ReadRegex("lang/" + m_language + "/regex_result", m_re_result_str, &m_re_result);
  ReadRegex("lang/" + m_language + "/regex_moon", m_re_moon_str, &m_re_moon);
}
