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

void KeyboardControls(const unsigned char *keyboard)
{
  float MoveSpeed = 1.0f;

  // forward backward
  if (keyboard[SDL_SCANCODE_LSHIFT]) MoveSpeed *= 5;
  if (keyboard[SDL_SCANCODE_LCTRL]) MoveSpeed /= 5;

  glm::vec3 mov(0.0, 0.0, MoveSpeed);
  mov = glm::rotate(mov, 0 - camAng.x, glm::vec3(1.0, 0, 0));
  mov = glm::rotate(mov, 0 - camAng.y, glm::vec3(0, 1.0, 0));

  if (keyboard[SDL_SCANCODE_W]) camPos += mov;
  if (keyboard[SDL_SCANCODE_S]) camPos -= mov;


  // left right
  mov = glm::vec3(MoveSpeed, 0.0, 0.0);
  mov = glm::rotate(mov, 0 - camAng.x, glm::vec3(1.0, 0, 0));
  mov = glm::rotate(mov, 0 - camAng.y, glm::vec3(0, 1.0, 0));

  if (keyboard[SDL_SCANCODE_A]) camPos += mov;
  if (keyboard[SDL_SCANCODE_D]) camPos -= mov;

  // up down

  if (keyboard[SDL_SCANCODE_E]) camPos -= glm::vec3(0.0, MoveSpeed, 0.0);
  if (keyboard[SDL_SCANCODE_X]) camPos += glm::vec3(0.0, MoveSpeed, 0.0);
}

glm::mat4 getCamera()
{
  glm::mat4 cam;
  cam = glm::rotate(cam, camAng.x, glm::vec3(1.0, 0, 0));
  cam = glm::rotate(cam, camAng.y, glm::vec3(0, 1.0, 0));
  cam = glm::translate(cam, camPos);
  return cam;
}