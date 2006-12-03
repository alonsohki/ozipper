#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include <string>

class Exception
{
public:
  Exception(const char *file, int line, int errid, const char *errstr) :
            m_file(file), m_line(line), m_errid(errid), m_errstr(errstr)
  {
  }

  const char *GetFile() { return m_file; }
  int GetLine() { return m_line; }
  const char *GetErr() { return m_errstr.c_str(); }
  int GetErrID() { return m_errid; }

private:
  const char *m_file;
  int m_line;
  int m_errid;
  std::string m_errstr;
};

#define EXCEPTION(ID, ...) { \
  char __tmp_exception_message[512]; \
  sprintf(__tmp_exception_message, __VA_ARGS__); \
  throw Exception(__FILE__, __LINE__, ID, __tmp_exception_message); \
}

#endif /* #ifndef __EXCEPTION_H */
