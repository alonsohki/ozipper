#include <string>
#include <vector>
#include <pcre.h>
#include <fstream>
#include "languagetpl.h"
#include "shipfactory.h"

LanguageTemplate::LanguageTemplate(const std::string& lang) : m_language(lang)
{
  InitShips();
  InitRegex();
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

  while (!file.eof())
  {
    file.getline(line, 2048);
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

void LanguageTemplate::InitRegex()
{
  std::ifstream file;
  std::string path("lang/" + m_language + "/regex");
  char tmp[512];
  size_t bytes;

  file.open(path.c_str());

  do
  {
    bytes = file.readsome(tmp, 512);
    tmp[bytes] = '\0';
    m_regex += tmp;
  } while (bytes);

  file.close();

  printf("Regex: %s", m_regex.c_str());
}

