#ifndef Game_h__
#define Game_h__

#include "String.h"
#include "Window.h"
#include "platform.h"

struct _Scene;

struct _Game : Platform::Game
{
  _Game(char* name);

  void SetDisplayMode(char * title, int width, int height, bool fullscreen);

  void AddScene(Platform::Scene* scene);

  void BeginGame();

  str m_name;
  Window m_window;
  _Scene **m_scenes;
  int m_scenecount;
};
#endif // Game_h__