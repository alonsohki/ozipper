#ifndef __LANGUAGETPL_H
#define __LANGUAGETPL_H

#include <string>
#include <vector>

class LanguageTemplate
{
public:
  LanguageTemplate(const std::string& language);
  const std::vector<std::string>& Parse(const std::string report);

private:
  void InitShips();
  void InitRegex();

  std::string m_language;
  std::string m_regex;
  std::vector<std::string> m_result;
};

#endif /* #ifndef __LANGUAGETPL_H */
