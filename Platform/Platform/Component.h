#ifndef Component_h__
#define Component_h__

#include "String.h"
#include "Platform.h"
#include <cstdint>

using namespace  Platform;


enum ComponentType
{
  CT_CUSTOM = 0,
};

void whatever(ComponentType a);

struct _Component : Component
{
  _Component(char* name);

  void LoadMesh(char* path);
  void LoadShader(char* path);
  void LoadSound(char* path);
  void LoadImage(char* path);
  void SetNumber(double value);
  void SetText(char* text);
  double GetNumber();
  char* GetText();

  str m_name;
  int m_type;
  int m_length;
  char* m_data = nullptr;
};
#endif // Component_h__
