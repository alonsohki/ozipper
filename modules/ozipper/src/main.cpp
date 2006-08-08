#include "exception.h"
#include "languagetpl.h"
#include "http.h"
#include <iostream>

int main(int argc, char *argv[], const char *env[])
{
  try
  {
    char tmp[512];
    std::string out;
    std::ifstream file;
    size_t bytes;

    file.open(argv[1]);
    do
    {
      bytes = file.readsome(tmp, 512);
      tmp[bytes] = '\0';
      out += tmp;
    } while (bytes);
    file.close();

    LanguageTemplate tpl("es");
    tpl.Parse(out);
  }
  catch (Exception &e)
  {
    fprintf(stderr, "Exception caught at %s(%d): %s\n", e.GetFile(), e.GetLine(), e.GetErr());
  }

  return 0;
}
