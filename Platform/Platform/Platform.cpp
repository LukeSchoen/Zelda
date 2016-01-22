#include "Memory.h"
#include "Game.h"
#include "Scene.h"
#include "Actor.h"
#include "Entity.h"
#include "Component.h"
#include "Platform.h"

namespace Platform
{

  Component* CreateComponent(char* name)
  {
    return memnew(_Component)(name);
  }

  Entity* CreateEntity(char* name)
  {
    return memnew(_Entity)(name);
  }

  Actor* CreateActor(char* name)
  {
    return memnew(_Actor)(name);
  }

  Scene* CreateScene(char* name)
  {
    return memnew(_Scene)(name);
  }

  Game* CreateGame(char* name)
  {
    return memnew(_Game)(name);
  }

}
