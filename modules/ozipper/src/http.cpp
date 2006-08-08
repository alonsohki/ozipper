#include <map>
#include <string>
#include <iostream>
#include "http.h"

static int hex2dec_table[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /*  00 -  09 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /*  10 -  19 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /*  20 -  29 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /*  30 -  39 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,    /*  40 -  49 */
  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,    /*  50 -  59 */
  0,  0,  0,  0,  0, 10, 11, 12, 13, 14,    /*  60 -  69 */
 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /*  70 -  79 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /*  80 -  89 */
  0,  0,  0,  0,  0,  0,  0, 10, 11, 12,    /*  90 -  99 */
 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,    /* 100 - 109 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 110 - 119 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 120 - 129 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 130 - 139 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 140 - 149 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 150 - 159 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 160 - 169 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 170 - 179 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 180 - 189 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 190 - 199 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 200 - 209 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 210 - 219 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 220 - 229 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 230 - 239 */
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /* 240 - 249 */
  0,  0,  0,  0,  0,  0                     /* 250 - 255 */
};

static const std::string EmptyString("");

bool Http_String_Compare::operator()(const std::string& a, const std::string& b)
{
  return strcasecmp(a.c_str(), b.c_str()) < 0;
}

static inline char GetHexValue(const char *p)
{
  return (hex2dec_table[(int)*p] << 4) | (hex2dec_table[(int)*(p + 1)]);
}

static inline std::string& ParseHexURLData(const std::string& source, std::string& dest)
{
  const char *p;

  dest = "";

  for (p = source.c_str(); *p != '\0'; p++)
  {
    switch (*p)
    {
      case '+':
	dest += ' ';
	break;
      case '%':
	if (isxdigit(p[1]) && isxdigit(p[2]))
	{
	  p++;
	  dest += GetHexValue(p);
	  p++;
	  break;
	}
      default:
	dest += *p;
	break;
    }
  }

  return dest;
}

static inline void ParseQueryString(const std::string& query, std::map<const std::string, const std::string, Http_String_Compare>& dest)
{
  const char *start;
  const char *equal;
  const char *end;
  const char *finish;
  std::string key;
  std::string value;
  std::string tmp1;
  std::string tmp2;

  finish = query.c_str() + query.length();

  for (start = query.c_str(); start <= finish; start++)
  {
    end = strchr(start, '&');
    if (!end)
    {
      end = finish;
    }
    equal = strchr(start, '=');
    if (!equal || equal > end)
    {
      /* Variable sin valor */
      key = "";
      key.append(start, end - start);

      std::pair<const std::string, const std::string> m_pair(ParseHexURLData(key, tmp1), "");
      dest.insert(m_pair);
    }
    else
    {
      /* Variable = valor */
      key = "";
      value = "";
      equal++;
      key.append(start, equal - start - 1);
      value.append(equal, end - equal);

      std::pair<const std::string, const std::string> m_pair(ParseHexURLData(key, tmp1), ParseHexURLData(value, tmp2));
      dest.insert(m_pair);
    }

    start = end;
  }
}

static inline void ParsePostVars(int length, std::map<const std::string, const std::string, Http_String_Compare>& dest)
{
  char buffer[length + 1];

  std::cin.read(buffer, length);
  buffer[length] = '\0';

  ParseQueryString(buffer, dest);
}

Http::Http(const char *p_env[])
{
  int i;
  const char *p;
  std::string key;
  std::string value;

  for (i = 0; p_env[i] != 0; i++)
  {
    key = "";

    p = strchr(p_env[i], '=');
    key.append(p_env[i], p - p_env[i]);
    value = p + 1;

    std::pair<const std::string, const std::string> m_pair_env(key, value);
    env.insert(m_pair_env);

    if (!strncasecmp(key.c_str(), "HTTP_", 5))
    {
      std::pair<const std::string, const std::string> m_pair(key.substr(5), value);
      http.insert(m_pair);
    }

    else if (!strncasecmp(key.c_str(), "SERVER_", 7))
    {
      std::pair<const std::string, const std::string> m_pair(key.substr(7), value);
      server.insert(m_pair);
    }

    else if (!strcasecmp(key.c_str(), "QUERY_STRING"))
    {
      ParseQueryString(value, get);
    }
  }

  if (!strcasecmp(ENV("REQUEST_METHOD").c_str(), "POST"))
  {
    ParsePostVars(atoi(ENV("CONTENT_LENGTH").c_str()), post);
  }
  
  return;
}

const std::string& Http::FindData(std::map<const std::string, const std::string, Http_String_Compare>& where, const std::string& key)
{
  std::map<const std::string, const std::string>::iterator i;

  i = where.find(key);
  if (i == where.end())
  {
    return EmptyString;
  }

  return (*i).second;
}

const std::string& Http::HTTP(const std::string& key)
{
  return FindData(http, key);
}

const std::string& Http::GET(const std::string& key)
{
  return FindData(get, key);
}

const std::string& Http::POST(const std::string& key)
{
  return FindData(post, key);
}

const std::string& Http::SERVER(const std::string& key)
{
  return FindData(server, key);
}

const std::string& Http::ENV(const std::string& key)
{
  return FindData(env, key);
}
