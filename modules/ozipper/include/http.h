#ifndef __HTTP_H
#define __HTTP_H

#include <map>
#include <string>

struct Http_String_Compare
{
  bool operator()(const std::string& a, const std::string& b);
};

class Http
{
public:
  Http(const char *p_env[]);

  const std::string& HTTP(const std::string& key);
  const std::string& GET(const std::string& key);
  const std::string& POST(const std::string& key);
  const std::string& SERVER(const std::string& key);
  const std::string& ENV(const std::string& key);

private:
  const std::string& FindData(std::map<const std::string, const std::string, Http_String_Compare>& where, const std::string& key);

  std::map<const std::string, const std::string, Http_String_Compare> env;
  std::map<const std::string, const std::string, Http_String_Compare> http;
  std::map<const std::string, const std::string, Http_String_Compare> get;
  std::map<const std::string, const std::string, Http_String_Compare> post;
  std::map<const std::string, const std::string, Http_String_Compare> server;
};

#endif /* #ifndef __HTTP_H */
