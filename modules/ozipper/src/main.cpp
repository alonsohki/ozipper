#include "exception.h"
#include "languagetpl.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[], char *env[])
{
  try
  {
/*    char tmp[512];
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
    tpl.Parse(out);*/
    char tmp[2048];
    std::cin.read(tmp, 2047);
    
    printf("Content-type: text/html; charset=UTF-8\r\n\r\n");
    for (int i = 0; env[i]; i++)
    {
      printf("%s\n", env[i]);
    }
    printf("POST: %s\n", tmp);
    printf("Test: %s\n", argv[1]);
  }
  catch (Exception &e)
  {
    fprintf(stderr, "Exception caught at %s(%d): %s\n", e.GetFile(), e.GetLine(), e.GetErr());
  }
  return 0;
}
