#ifndef Actor_h__
#define Actor_h__

#include "String.h"
#include <glm.hpp>
#include "Platform.h"

struct _Entity;

using namespace Platform;

struct _Actor : Actor
{
  _Actor::_Actor(char* name, glm::vec3 position = { 0, 0, 0 }, glm::vec3 rotation = { 0, 0, 0 }, glm::vec3 scale = { 0, 0, 0 });

  void AddEntity(Entity* entity);

  str m_name;
  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;
  _Entity **m_entities;
  int m_entityCount;
};
#endif // Actor_h__


