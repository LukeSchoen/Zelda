#ifndef Scene_h__
#define Scene_h__

#include "String.h"
#include "Platform.h"

struct _Actor;

struct _Scene : Platform::Scene
{
  _Scene(char* name);

  void AddActor(Platform::Actor* actor);

  str m_name;
  _Actor **m_actors;
  int m_actorCount;
};
#endif // Scene_h__