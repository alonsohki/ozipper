#include "intl.h"
#include <locale.h>
#include <libintl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _(STR) gettext(STR)

static struct IntlLangAliases
{
  const char *alias;
  const char *local;
} __langAliases[] = {
  { "es", "es_ES" },
  { "en", "en_US" },
  { "de", "de_DE" },
  { 0, 0 }
};

static struct IntlStrs
{
  StrID id;
  const char *str;
} * __strs;

static void AddLocaleField(StrID id, const char *str)
{
  static IntlStrs *p = __strs;

  p->id = id;
  p->str = str;
  p++;
}

static void InitLocaleStruct()
{
  __strs = new IntlStrs[STRID_MAX];
  
  AddLocaleField(EMPTY_REPORT_OR_LANG_VARS,          _("Empty report or lang variables"));
  AddLocaleField(UNABLE_TO_FIND_REQUIRED_TEMPLATE,   _("Unable to find required template"));
  AddLocaleField(NO_INFORMATION_REQUIRED,            _("No information required"));
  AddLocaleField(UNKNOWN_REPORT_FORMAT,              _("Unknown report format"));
  AddLocaleField(INTERNAL_ERROR,                     _("Internal error"));
  AddLocaleField(COULDNT_INIT_ROLES,                 _("Couldn't init roles"));
  AddLocaleField(COULDNT_INIT_SHIPS,                 _("Couldn't init ships"));
  AddLocaleField(COULDNT_INIT_REGEXP,                _("Couldn't init regular expressions"));
  AddLocaleField(COULDNT_OPEN_TRANSLATION,           _("Couldn't open translation"));
  AddLocaleField(PLAYER_NOT_FOUND,                   _("Player not found"));
  AddLocaleField(SHIP_NOT_FOUND,                     _("Ship not found"));
};

static const char * AliasGetLocal(const char *alias)
{
  for (int i = 0; __langAliases[i].alias != 0; i++)
  {
    if (!strcasecmp(alias, __langAliases[i].alias))
    {
      return __langAliases[i].local;
    }
  }

  return "C";
}

#undef _

const char * Intl::_(StrID id)
{
  for (int i = 0; i < STRID_MAX; i++)
  {
    if (id == __strs[i].id)
    {
      return __strs[i].str;
    }
  }

  return 0;
}

void Intl::InitLocale(const char *lang)
{
  bindtextdomain("ozipper", "lang");
  textdomain("ozipper");
  setlocale(LC_ALL, AliasGetLocal(lang));
  InitLocaleStruct();
}
