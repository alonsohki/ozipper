#ifndef __LANGUAGETPL_H
#define __LANGUAGETPL_H

#include <string>
#include <vector>

class LanguageTemplate
{
public:
  LanguageTemplate(std::string& language);
  const std::vector<std::string>& Parse(std::string report);

private:
  void InitShips();
  void InitRegex();

  std::string m_language;
  std::vector<std::string> m_result;
};

#endif /* #ifndef __LANGUAGETPL_H */
