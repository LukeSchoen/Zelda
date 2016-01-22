#include "Actor.h"
#include "Memory.h"
#include "Entity.h"

_Actor::_Actor(char* name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
  : m_name(name)
  , m_entities(nullptr)
  , m_entityCount(0)
  , m_position(position)
  , m_rotation(rotation)
  , m_scale(scale)
{
  m_entities = memalloctypecount(_Entity*, 16);
}

void _Actor::AddEntity(Entity* entity)
{
  m_entities[m_entityCount++] = (_Entity*)entity;
}