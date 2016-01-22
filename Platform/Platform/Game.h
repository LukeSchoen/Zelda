#ifndef Game_h__
#define Game_h__

#include "String.h"
#include "Window.h"
#include "platform.h"

using namespace Platform;

struct _Scene;

struct _Game : Game
{
  _Game(char* name);

  void SetDisplayMode(char * title = nullptr, int width = 800, int height = 600, bool fullscreen = false);

  void AddScene(Scene* scene);

  void BeginGame();

  str m_name;
  Window m_window;
  _Scene **m_scenes;
  int m_scenecount;
};
#endif // Game_h__