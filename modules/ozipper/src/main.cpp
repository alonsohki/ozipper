#include "exception.h"
#include "languagetpl.h"

int main(int argc, char *argv[], char *env[])
{
  try
  {
    LanguageTemplate tpl("es");
  }
  catch (Exception &e)
  {
    fprintf(stderr, "Exception caught at %s(%d): %s\n", e.GetFile(), e.GetLine(), e.GetErr());
  }
  return 0;
}
