#include <glm.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtc/matrix_transform.hpp>
#include "SDL_keycode.h"

static glm::vec3 camPos(1000.0, -800.0, -500);
static glm::vec2 camAng(0.0, 1.6);

void MouseControls(int mouseX, int mouseY)
{
  float TurnSpeed = 0.002;
  camAng.x += mouseY * TurnSpeed;
  camAng.y += mouseX * TurnSpeed;
}

glm::vec3 CameraKeyboardControls2D(const unsigned char *keyboard)
{
  float MoveSpeed = 5.0f;
  glm::vec3 mov(0.0, 0.0, 1.0);
  glm::vec3 finalMov(0.0, 0.0, 0.0);

  // movement speed
  if (keyboard[SDL_SCANCODE_LSHIFT]) MoveSpeed *= 5;
  if (keyboard[SDL_SCANCODE_LCTRL]) MoveSpeed /= 5;


  // forward backward
  //mov = glm::rotate(mov, 0 - camAng.x, glm::vec3(1.0, 0, 0));
  mov = glm::rotate(mov, 0 - camAng.y, glm::vec3(0, 1.0, 0));

  if (keyboard[SDL_SCANCODE_W]) finalMov += mov;
  if (keyboard[SDL_SCANCODE_S]) finalMov -= mov;


  // left right
  mov = glm::vec3(1.0, 0.0, 0.0);
  //mov = glm::rotate(mov, 0 - camAng.x, glm::vec3(1.0, 0, 0));
  mov = glm::rotate(mov, 0 - camAng.y, glm::vec3(0, 1.0, 0));

  if (keyboard[SDL_SCANCODE_A]) finalMov += mov;
  if (keyboard[SDL_SCANCODE_D]) finalMov -= mov;

  // create and return velocity vector
  if (glm::length(finalMov) >= 0.05)
    finalMov = glm::normalize(finalMov) * MoveSpeed;
  return finalMov;
}

glm::vec3 CameraKeyboardControls(const unsigned char *keyboard)
{
  float MoveSpeed = 10.0f;
  glm::vec3 mov(0.0, 0.0, 1.0);
  glm::vec3 finalMov(0.0, 0.0, 0.0);

  // movement speed
  if (keyboard[SDL_SCANCODE_LSHIFT]) MoveSpeed *= 5;
  if (keyboard[SDL_SCANCODE_LCTRL]) MoveSpeed /= 5;


  // forward backward
  mov = glm::rotate(mov, 0 - camAng.x, glm::vec3(1.0, 0, 0));
  mov = glm::rotate(mov, 0 - camAng.y, glm::vec3(0, 1.0, 0));

  if (keyboard[SDL_SCANCODE_W]) finalMov += mov;
  if (keyboard[SDL_SCANCODE_S]) finalMov -= mov;


  // left right
  mov = glm::vec3(1.0, 0.0, 0.0);
  mov = glm::rotate(mov, 0 - camAng.x, glm::vec3(1.0, 0, 0));
  mov = glm::rotate(mov, 0 - camAng.y, glm::vec3(0, 1.0, 0));

  if (keyboard[SDL_SCANCODE_A]) finalMov += mov;
  if (keyboard[SDL_SCANCODE_D]) finalMov -= mov;


  // up down
  if (keyboard[SDL_SCANCODE_E]) finalMov -= glm::vec3(0.0, 1.0, 0.0);
  if (keyboard[SDL_SCANCODE_X]) finalMov += glm::vec3(0.0, 1.0, 0.0);


  // create and return velocity vector
  if (glm::length(finalMov) >= 0.05)
    finalMov = glm::normalize(finalMov) * MoveSpeed;
  return finalMov;
}

glm::mat4 getCamera()
{
  glm::mat4 cam;
  cam = glm::rotate(cam, camAng.x, glm::vec3(1.0, 0, 0));
  cam = glm::rotate(cam, camAng.y, glm::vec3(0, 1.0, 0));
  cam = glm::translate(cam, camPos);
  return cam;
}