#ifndef __INTL_H
#define __INTL_H

#include <libintl.h>

enum StrID
{
  EMPTY_REPORT_OR_LANG_VARS,
  UNABLE_TO_FIND_REQUIRED_TEMPLATE,
  NO_INFORMATION_REQUIRED,
  UNKNOWN_REPORT_FORMAT,
  INTERNAL_ERROR,
  COULDNT_INIT_ROLES,
  COULDNT_INIT_SHIPS,
  COULDNT_INIT_REGEXP,
  COULDNT_OPEN_TRANSLATION,
  PLAYER_NOT_FOUND,
  SHIP_NOT_FOUND,

  STRID_MAX
};

class Intl
{
public:
  static const char * _(StrID id);
  static void InitLocale(const char *lang);
  static void EndLocale();
};

#endif /* #ifndef __INTL_H */
