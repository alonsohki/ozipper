#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include <string>

class Exception
{
public:
  Exception(const char *file, int line, const char *errstr) :
            m_file(file), m_line(line), m_errstr(errstr)
  {
  }

  const char *GetFile() { return m_file; }
  int GetLine() { return m_line; }
  const char *GetErr() { return m_errstr.c_str(); }

private:
  const char *m_file;
  int m_line;
  std::string m_errstr;
};

#define EXCEPTION(...) { \
  char __tmp_exception_message[512]; \
  sprintf(__tmp_exception_message, __VA_ARGS__); \
  throw Exception(__FILE__, __LINE__, __tmp_exception_message); \
}

#endif /* #ifndef __EXCEPTION_H */
