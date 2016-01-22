#ifndef Window_h__
#define Window_h__

#include "string.h"

struct Window
{
  Window(char * title = "", int width = 800, int height = 600, bool fullscreen = false);
  str m_title;
  int m_with;
  int m_height;
  bool m_fullscreen;
};
#endif // Window_h__