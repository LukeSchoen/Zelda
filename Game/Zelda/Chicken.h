#include "Platform.h"

void Update_Chicken(Actor& chicken)
{
  chicken.m_entities[0].m_components[0].m_type; // should return type mesh
  chicken.m_entities[1].m_components[0].m_type; // should return type sound

  //calculate movement

  //perform collision

  //render actor
}

Actor Create_Chicken()
{
  Actor chicken("Chicken");

  Entity& chickenVisual = chicken.AddEntity("Chicken Visual");
  Component& KakricoChickenModel = chickenVisual.AddComponent("Kakrico Chicken Model");
  KakricoChickenModel.LoadMesh("meshes/kakrico/chicken.obj");

  Entity& chickenAudial = chicken.AddEntity("Chicken Audial");
  Component& KakricoChickenSound = chickenAudial.AddComponent("Kakrico Chicken Sound");
  KakricoChickenSound.LoadSound("sounds/kakrico/chicken.mp3");

  chicken.update = &Update_Chicken;

  return chicken;
}