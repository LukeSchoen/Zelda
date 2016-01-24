#ifndef Entity_h__
#define Entity_h__

#include "String.h"
#include "Platform.h"

struct _Component;

struct _Entity : Platform::Entity
{
  _Entity(char* name);

  void AddComponent(Platform::Component* component);

  str m_name;
  _Component **m_components;
  int m_componentCount;
};
#endif // Entity_h__