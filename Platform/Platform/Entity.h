#ifndef Entity_h__
#define Entity_h__

#include "String.h"
#include "Platform.h"

using namespace Platform;

struct _Component;

struct _Entity : Entity
{
  _Entity(char* name);

  void AddComponent(Component* component);

  str m_name;
  _Component **m_components;
  int m_componentCount;
};
#endif // Entity_h__