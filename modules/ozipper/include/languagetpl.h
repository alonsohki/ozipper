#ifndef __LANGUAGETPL_H
#define __LANGUAGETPL_H

#include <string>
#include <vector>
#include <pcre.h>
#include <time.h>
#include "player.h"

struct ReportData
{
public:
  tm date;
  std::vector<Player *> players;
  int rounds;
  std::string winner;
  struct
  {
    int metal;
    int crystal;
    int deuterium;
  } captures;
  struct
  {
    int attacker;
    int defender;
  } losses;
  struct
  {
    int metal;
    int crystal;
  } debris;
  bool moon;
  int moonchance;
};

class LanguageTemplate
{
public:
  LanguageTemplate(const std::string& language) throw(Exception);
  ~LanguageTemplate();
  const ReportData& Parse(const std::string& report) throw(Exception);

private:
  void InitRoles();
  void InitShips();
  void ReadRegex(const std::string& path, std::string& dest, pcre **re) throw(Exception);
  inline int ExecRegex(pcre *re, const std::string& subject, int offset, int *vector, size_t vsize, bool match_error = true) throw (Exception);
  void InitRegex() throw(Exception);

  ReportData m_result;
  pcre * m_re_date;
  pcre * m_re_fleets;
  pcre * m_re_startround;
  pcre * m_re_round;
  pcre * m_re_result;
  pcre * m_re_moon;
  std::string m_language;
  std::string m_re_date_str;
  std::string m_re_fleets_str;
  std::string m_re_startround_str;
  std::string m_re_round_str;
  std::string m_re_result_str;
  std::string m_re_moon_str;
  std::string m_roles[2];
};

#endif /* #ifndef __LANGUAGETPL_H */
