#include "string.h"
#include "Memory.h"

#define NULL 0

str::str(char* string)
  : m_buffer("")
  , m_length(0)
{
  copy(string);
}

void str::copy(char* string)
{
  int newlen = length(string);
  if (newlen > m_length)
    m_buffer = memalloc(newlen);
  m_length = newlen;
  memcopy(m_buffer, string, m_length);
}

int str::length(char* string)
{
  if (string == nullptr)
    return 0;
  for (int i = 0; true; i++)
    if (string[i] == NULL)
      return i+1;
}

bool str::compare(char* string1, char* string2)
{
  if (string1 == nullptr || string2 == nullptr)
    return false;
  for (int i = 0; true; i++)
  {
    if (string1[i] != string2[i])
      return false;
    if (string1[i] == NULL && string2[i] == NULL)
      return true;
  }

}
