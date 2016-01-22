#ifndef String_h__
#define String_h__

struct str
{
  str(char* string = "");
  void copy(char* string);
  int length(char* string);
  char* m_buffer;
  int m_length;
};
#endif // String_h__