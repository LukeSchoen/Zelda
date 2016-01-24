#include "Component.h"

using namespace Platform;


_Component::_Component(char* name)
  : Component()
  , m_name(name)
  , m_data(nullptr)
  , m_length(0)
  , m_type(CT_CUSTOM)
{

}

void _Component::LoadMesh(char* path)
{

}

void _Component::LoadShader(char* path)
{

}

void _Component::LoadSound(char* path)
{

}

void _Component::LoadImage(char* path)
{

}

void _Component::SetNumber(double value)
{

}

void _Component::SetText(char* text)
{

}

double _Component::GetNumber()
{
  return *(double*)m_data;
}

char* _Component::GetText()
{
  return (char*)m_data;
}
