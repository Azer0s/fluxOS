#include <ctype.h>

bool isspace(char c)
{
   return c == ' ' || c == '\t'; // || whatever other char you consider space
}

bool isdigit(char ch)
{
  return (ch >= '0') && (ch <= '9');
}
