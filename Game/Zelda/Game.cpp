#include "Platform.h"

using namespace Platform;

int wmain()
{

  Game* zelda = CreateGame("zelda");

  ////    OverWorld    ////
  Scene* overworld = CreateScene("overworld");
  Actor* map = CreateActor("map");

  Entity* model = CreateEntity("model");
  Component* mesh = CreateComponent("mesh");
  Component* vertShader = CreateComponent("vertShader");
  Component* fragShader = CreateComponent("fragShader");
  vertShader->LoadShader("shaders/vert/texture.vert");
  fragShader->LoadShader("shaders/frag/texture.frag");
  mesh->LoadMesh("OverWorld/OverWorld.obj");

  model->AddComponent(vertShader);
  model->AddComponent(fragShader);
  model->AddComponent(mesh);
  map->AddEntity(model);
  overworld->AddActor(map);
  zelda->AddScene(overworld);
  ////                  ////

  zelda->SetDisplayMode("zelda application", 800, 600, false);

  zelda->BeginGame();

}