#include "Entity.h"
#include "Component.h"
#include "Memory.h"

_Entity::_Entity(char* name)
  : m_name(name)
  , m_components(nullptr)
  , m_componentCount(0)
{
  m_components = memalloctypecount(_Component*, 16);
}

void _Entity::AddComponent(Component* component)
{
  m_components[m_componentCount++] = (_Component*)component;
}
