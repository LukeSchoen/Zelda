#ifndef String_h__
#define String_h__


struct str
{
  str(char* string = "");
  void copy(char* string);
  static int length(char* string);
  static bool compare(char* string1, char* string2);
  char* m_buffer;
  int m_length;
};
#endif // String_h__