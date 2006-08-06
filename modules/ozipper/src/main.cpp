#include "exception.h"
#include "ship.h"
#include "shipfactory.h"
#include "xmloutput.h"
#include "languagetpl.h"

int main(int argc, char *argv[], char *env[])
{
  try
  {
  }
  catch (Exception &e)
  {
    fprintf(stderr, "Exception caught at %s(%d): %s\n", e.GetFile(), e.GetLine(), e.GetErr());
  }
  return 0;
}
