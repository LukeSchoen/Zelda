#include <glm.hpp>

#define NULL 0


char memory[64 * 1024 * 1024];
int memptr = 0;
char* memnew(int length)
{
  return memory + (memptr+=length) - length;
}

void memcopy(void *dst, void *src, int len)
{
  for (int i = 0; i < len; i++)
    *((char*)dst+i) = *((char*)src+i);
}



struct str
{
  str(char* string = "")
  {
    copy(string);
  }
  void copy(char* string = "")
  {
    int newlen = length(string);
    if (newlen > m_length)
      m_buffer = memnew(newlen);
    m_length = newlen;
    memcopy(m_buffer, string, m_length);
  }
  int length(char* string)
  {
    for (int i = 0; true; i++)
      if (string[i] == NULL)
        return i;
  }
  char* m_buffer="";
  int m_length = 0;
};



struct Window
{
  Window(char * title = "Application", int width = 800, int height = 600, bool fullscreen = false)
  {
    m_title.copy(title);
    m_with = width;
    m_height = height;
    m_fullscreen = fullscreen;
  }
  str m_title;
  int m_with;
  int m_height;
  bool m_fullscreen;
};






struct Component
{
  void LoadMesh(char* path)
  {

  }

  void LoadSound(char* path)
  {

  }

  str m_name;
  int m_type;
  int m_length;
  char* m_data;
};


struct Entity
{
  Entity(char* name)
  {
    m_name.copy(name);
  }
  Component& AddComponent(str name)
  {
    //create some space for entities
    if (m_componentCount == 0)
      m_components = (Component*)memnew(sizeof(Component) * 16);
    //add entity
    m_components[m_componentCount].m_name = name;
    return m_components[m_componentCount++];
  }

  str m_name;
  Component *m_components;
  int m_componentCount = 0;
};


struct Actor
{
  Actor(char* name)
  {
    m_name.copy(name);
  }
  Entity& AddEntity(str name)
  {
    //create some space for entities
    if (m_entityCount == 0)
      m_entities = (Entity*)memnew(sizeof(Entity) * 16);
    //add entity
    m_entities[m_entityCount].m_name = name;
    return m_entities[m_entityCount++];
  }

  str m_name;
  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;
  Entity *m_entities;
  void(*update)(Actor& actor);
  int m_entityCount = 0;
};


struct Scene
{
  Scene(char* name)
  {
    m_name.copy(name);
  }
  Actor& AddActor(str name)
  {
    //create some space for actors
    if (m_actorCount == 0)
      m_actors = (Actor*)memnew(sizeof(Actor) * 256);
    //add actor
    m_actors[m_actorCount].m_name = name;
    return m_actors[m_actorCount++];
  }
  str m_name;
  Actor *m_actors;
  int m_actorCount = 0;
};