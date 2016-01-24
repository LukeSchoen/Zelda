#include "Window.h"

Window::Window(char * title /*= ""*/, int width /*= 800*/, int height /*= 600*/, bool fullscreen /*= false*/)
  : m_title(title)
  , m_width(width)
  , m_height(height)
  , m_fullscreen(fullscreen)
{

}
