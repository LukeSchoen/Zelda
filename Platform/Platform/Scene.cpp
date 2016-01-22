#include "Scene.h"
#include "Actor.h"
#include "Memory.h"

_Scene::_Scene(char* name)
  : m_name(name)
  , m_actors(nullptr)
  , m_actorCount(0)
{
  m_actors = memalloctypecount(_Actor*, 16);
}

void _Scene::AddActor(Actor* actor)
{
  m_actors[m_actorCount++] = (_Actor*)actor;
}
