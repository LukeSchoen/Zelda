#include "Game.h"
#include "Memory.h"
#include "Engine.h"

_Game::_Game(char* name)
  : m_name(name)
  , m_window(name)
  , m_scenes(nullptr)
  , m_scenecount(0)
{
  m_scenes = memalloctypecount(_Scene*, 16);
}

void _Game::SetDisplayMode(char * title, int width, int height, bool fullscreen)
{
  if (title != nullptr)
    m_window.m_title.copy(title);
  m_window.m_with = width;
  m_window.m_height = height;
  m_window.m_fullscreen = fullscreen;
}

void _Game::AddScene(Scene* scene)
{
  m_scenes[m_scenecount++] = (_Scene*)scene;
}


void _Game::BeginGame()
{
  //Initialize Engine
  // window information
  Engine::Start();

  //Start Engine
  Engine::Start();

  //Enging ends->
  //Cleanup platform
  //Return control
  
}