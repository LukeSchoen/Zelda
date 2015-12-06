#pragma region Build Options

#define GLEW_STATIC

#pragma endregion

#pragma region includes

#include <stdio.h>
#include <string>
#include <tchar.h>

#include "Bitmap Loader.h"
#include "OBJ Loader.h"
#include "Shader Loader.h"

#include "Controls.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "SDL_opengles2.h"

#pragma endregion

#pragma region strucs

struct VertexObject
{
  GLint textureID = 0;
  GLuint vertPosDataGLPtr = 0;
  GLuint texPosDataGLPtr = 0;
  uint32_t verticeCount = 0;
  GLfloat *vertPosData;
  GLfloat *texPosData;
};

struct RenderObject
{
  glm::mat4 modelMatrix;
  VertexObject *vertexObjects;
  uint32_t vertexObjectCount = 0;
};

#pragma endregion

#pragma region globals

//Program name
const char* PROGRAM_NAME = "ZELDA";
//Window resolution
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const bool FULL_SCEEN = true;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Graphics program
GLuint gProgramID = 0;

//Uniform Locations
GLuint gLocMVP = -1;
GLint gLocVertexPos4D = -1;
GLuint gLocTexture = -1;

//Model References
OBJ kakrikoOBJ;
MTL kakrikoMTL;
RenderObject kakrikoRenderObject;

OBJ windMillOBJ;
MTL windMillMTL;
RenderObject windMillRenderObject;

#pragma endregion

#pragma region prototypes

bool init();

bool initGL();

void render();

void close();

void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);

#pragma endregion

void RenderRenderObject(RenderObject &RenObj)
{
  for (int VertexObjectID = 0; VertexObjectID < RenObj.vertexObjectCount; VertexObjectID++)
  {
    glBindBuffer(GL_ARRAY_BUFFER, RenObj.vertexObjects[VertexObjectID].vertPosDataGLPtr);
    glVertexAttribPointer(
      0,                  // attribute number. must match the layout in the shader.
      3,                  // count
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

    glBindBuffer(GL_ARRAY_BUFFER, RenObj.vertexObjects[VertexObjectID].texPosDataGLPtr);
    glVertexAttribPointer(
      1,                  // attribute number. must match the layout in the shader.
      2,                  // count
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

    glBindTexture(GL_TEXTURE_2D, RenObj.vertexObjects[VertexObjectID].textureID);
    glDrawArrays(GL_TRIANGLES, 0, RenObj.vertexObjects[VertexObjectID].verticeCount);
  }
}

void InitializeMaterialLibrary(MTL &mtl, char * basePath)
{
  SDL_Surface *tex;
  char fullMatPath [1024];
  memcpy(fullMatPath, basePath, strlen(basePath) + 1); // initialize the base path

  for (int matID = 0; matID < mtl.materialCount ; matID++)
  {
    memcpy(fullMatPath + strlen(basePath), mtl.materials[matID].path, strlen(mtl.materials[matID].path) + 1); // append the local material path

    if ((tex = SDL_LoadBMP(fullMatPath)))
    {
      glGenTextures(1, &(mtl.materials[matID].textureID));
      glBindTexture(GL_TEXTURE_2D, mtl.materials[matID].textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, 3, tex->w, tex->h, 0, GL_BGR, GL_UNSIGNED_BYTE, tex->pixels);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
  }

  if (tex)
    SDL_FreeSurface(tex);
}

bool init()
{
  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("Error: SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  //Use OpenGL 3.1 core
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  //Enable AA
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

  //Create window
  gWindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (gWindow == NULL)
  {
    printf("Error: Window could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  //Create context
  gContext = SDL_GL_CreateContext(gWindow);
  if (gContext == NULL)
  {
    printf("Error: OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  //Initialize GLEW
  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK)
  {
    printf("Error: Unable to initialize GLEW! %s\n", glewGetErrorString(glewError));
    return false;
  }

  //Enable Vsync
  if (SDL_GL_SetSwapInterval(1) < 0)
  {
    printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
  }

  //Initialize OpenGL
  if (!initGL())
  {
    printf("Error: Unable to initialize OpenGL!\n");
    return false;
  }

  //Successful init
  return true;
}

bool initGL()
{
  //Depth Testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //Multi sampling
  glEnable(GL_MULTISAMPLE);

  //Generate shader program
  gProgramID = glCreateProgram();

  //Create vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

  //Get vertex source
  char *vertShader[1];
  vertShader[0] = LoadShader("..\\..\\..\\game\\Assets\\Shaders\\Vert\\texture.vert");

  //Set vertex source
  glShaderSource(vertexShader, 1, vertShader, NULL);

  //Compile vertex source
  glCompileShader(vertexShader);

  //Check vertex shader for errors
  GLint vShaderCompiled = GL_FALSE;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
  if (vShaderCompiled != GL_TRUE)
  {
    printf("Unable to compile vertex shader %d!\n", vertexShader);
    printShaderLog(vertexShader);
    return false;
  }

  //Attach vertex shader to program
  glAttachShader(gProgramID, vertexShader);

  //Create fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  //Get fragment source
  char *fragShader[1];
  fragShader[0] = LoadShader("..\\..\\..\\Game\\Assets\\Shaders\\frag\\texture.frag");
  
  //Set fragment source
  glShaderSource(fragmentShader, 1, fragShader, NULL);

  //Compile fragment source
  glCompileShader(fragmentShader);

  //Check fragment shader for errors
  GLint fShaderCompiled = GL_FALSE;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
  if (fShaderCompiled != GL_TRUE)
  {
    printf("Unable to compile fragment shader %d!\n", fragmentShader);
    printShaderLog(fragmentShader);
    return false;
  }

  //Attach fragment shader to program
  glAttachShader(gProgramID, fragmentShader);

  //Link program
  glLinkProgram(gProgramID);

  //Check for errors
  GLint programSuccess = GL_TRUE;
  glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
  if (programSuccess != GL_TRUE)
  {
    printf("Error linking program %d!\n", gProgramID);
    printProgramLog(gProgramID);
    return false;
  }

  //Get vertex attribute locations
  gLocVertexPos4D = glGetAttribLocation(gProgramID, "vert");
  if (gLocVertexPos4D == -1)
  {
    printf("vert is not a valid glsl program variable!\n");
    return false;
  }

  //Model View Projection Shader parameter
  gLocMVP = glGetUniformLocation(gProgramID, "MVP");

  //Texture sampler
  gLocTexture = glGetUniformLocation(gProgramID, "TextureSampler");

  //Initialize clear color
  glClearColor(0.f, 0.75, 1.0, 1.f);
  
  //success
  return true;
}

RenderObject GenerateRenderObject(OBJ &model, MTL &materials)
{
  RenderObject object;
  object.modelMatrix = glm::mat4();

  object.vertexObjectCount = materials.materialCount;
  object.vertexObjects = new VertexObject[materials.materialCount];

  for (int objID = 0; objID < materials.materialCount; objID++)
  {
    //Count faces for this material
    uint32_t texID = materials.materials[objID].textureID;
    uint32_t faceCount = 0;
    for (int faceID = 0; faceID < model.faceCount ; faceID++)
      if (texID == model.polys[faceID].textureID) faceCount++;

    //create vertexObject for this material
    object.vertexObjects[objID].textureID = texID;
    object.vertexObjects[objID].vertPosData = new GLfloat[faceCount * 9];
    object.vertexObjects[objID].texPosData = new GLfloat[faceCount * 6];
    uint32_t currentFace = 0;
    for (int faceID = 0; faceID < model.faceCount; faceID++)
    {
      if (texID == model.polys[faceID].textureID)
      {
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 0] = model.polys[faceID].verticies[0].x;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 1] = model.polys[faceID].verticies[0].y;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 2] = model.polys[faceID].verticies[0].z;

        object.vertexObjects[objID].texPosData[currentFace * 6 + 0] = model.polys[faceID].verticies[0].u;
        object.vertexObjects[objID].texPosData[currentFace * 6 + 1] = model.polys[faceID].verticies[0].v;

        object.vertexObjects[objID].vertPosData[currentFace * 9 + 3] = model.polys[faceID].verticies[1].x;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 4] = model.polys[faceID].verticies[1].y;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 5] = model.polys[faceID].verticies[1].z;

        object.vertexObjects[objID].texPosData[currentFace * 6 + 2] = model.polys[faceID].verticies[1].u;
        object.vertexObjects[objID].texPosData[currentFace * 6 + 3] = model.polys[faceID].verticies[1].v;
        
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 6] = model.polys[faceID].verticies[2].x;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 7] = model.polys[faceID].verticies[2].y;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 8] = model.polys[faceID].verticies[2].z;

        object.vertexObjects[objID].texPosData[currentFace * 6 + 4] = model.polys[faceID].verticies[2].u;
        object.vertexObjects[objID].texPosData[currentFace * 6 + 5] = model.polys[faceID].verticies[2].v;
        object.vertexObjects[objID].verticeCount+=3;
        currentFace++;
      }
    }

    //upload vertexObject to OpenGL for this material
    glGenBuffers(1, &object.vertexObjects[objID].vertPosDataGLPtr);
    glBindBuffer(GL_ARRAY_BUFFER, object.vertexObjects[objID].vertPosDataGLPtr);
    glBufferData(GL_ARRAY_BUFFER, faceCount * 9 * sizeof(GLfloat), object.vertexObjects[objID].vertPosData, GL_STATIC_DRAW);

    glGenBuffers(1, &object.vertexObjects[objID].texPosDataGLPtr);
    glBindBuffer(GL_ARRAY_BUFFER, object.vertexObjects[objID].texPosDataGLPtr);
    glBufferData(GL_ARRAY_BUFFER, faceCount * 6 * sizeof(GLfloat), object.vertexObjects[objID].texPosData, GL_STATIC_DRAW);

  }

  return object;
}

void render()
{
  //Clear color buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind shader program
  glUseProgram(gProgramID);

  // Projection matrix (Field of View, Aspect Ratio, NearPlane, FarPlane)
  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 5000.0f);

  // Camera matrix
  glm::mat4 View = getCamera();

  // Model Matrix
  glm::mat4 Model = glm::mat4(1.0f);
  Model = glm::translate(Model, glm::vec3(0.0, -20.0, 0.0));

  static float rot = 0.0;
  rot = rot - 0.00333;

  // Model View Projection Matrix
  glm::mat4 MVP = Projection * View * Model; // I should handle this properly for each render object!!
  
  //Bind Vert Shader Attributes
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glUniformMatrix4fv(gLocMVP, 1, GL_FALSE, &MVP[0][0]);
  RenderRenderObject(kakrikoRenderObject);

  Model = glm::translate(Model, glm::vec3(1130.0, 1000.0, 530.0));
  Model = glm::rotate(Model, rot, glm::vec3(1.0, 0.0, 0.0));
  MVP = Projection * View * Model;
  glUniformMatrix4fv(gLocMVP, 1, GL_FALSE, &MVP[0][0]);
  RenderRenderObject(windMillRenderObject);

  //Unbind Vert Shader Attributes
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  //Unbind program
  glUseProgram(NULL);

}

void close()
{
  //Deallocate program
  glDeleteProgram(gProgramID);

  //Destroy window	
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  //Quit SDL
  SDL_Quit();
}

void printProgramLog(GLuint program)
{
  //Make sure name is shader
  if (glIsProgram(program))
  {
    //Program log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    //Get info string length
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    //Allocate string
    char* infoLog = new char[maxLength];

    //Get info log
    glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0)
    {
      //Print Log
      printf("%s\n", infoLog);
    }

    //Deallocate string
    delete[] infoLog;
  }
  else
  {
    printf("Name %d is not a program\n", program);
  }
}

void printShaderLog(GLuint shader)
{
  //Make sure name is shader
  if (glIsShader(shader))
  {
    //Shader log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    //Get info string length
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    //Allocate string
    char* infoLog = new char[maxLength];

    //Get info log
    glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0)
    {
      //Print Log
      printf("%s\n", infoLog);
    }

    //Deallocate string
    delete[] infoLog;
  }
  else
  {
    printf("Name %d is not a shader\n", shader);
  }
}

int wmain(int argc, char* argv[])
{
  //Start SDL & Create Window
  if (!init())
  {
    printf("Failed to initialize!\n");
    getchar();
    return 1;
  }

  kakrikoMTL = LoadMTL("..\\..\\..\\game\\Assets\\Kakriko\\Kakriko.mtl");
  InitializeMaterialLibrary(kakrikoMTL, "..\\..\\..\\game\\Assets\\Kakriko\\");
  kakrikoOBJ = LoadOBJ("..\\..\\..\\game\\Assets\\Kakriko\\Kakriko.obj", kakrikoMTL);
  kakrikoRenderObject = GenerateRenderObject(kakrikoOBJ, kakrikoMTL);

  windMillMTL = LoadMTL("..\\..\\..\\game\\Assets\\Kakriko\\WindMill.mtl");
  InitializeMaterialLibrary(windMillMTL, "..\\..\\..\\game\\Assets\\Kakriko\\");
  windMillOBJ = LoadOBJ("..\\..\\..\\game\\Assets\\Kakriko\\WindMill.obj", windMillMTL);
  windMillRenderObject = GenerateRenderObject(windMillOBJ, windMillMTL);

  //Take control of the cursor
  SDL_SetRelativeMouseMode(SDL_TRUE);

  //Go Full screen?
  if ( FULL_SCEEN ) SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN);

  //Main loop flag
  bool running = true;

  //Event handler
  SDL_Event e;

  //Enable text input
  //SDL_StartTextInput();

  while (running)
  {
    //Handle SDL queue
    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_QUIT) // User closed window?
        running = false;
      if (e.type == SDL_KEYDOWN) // User pressed esc?
        if (e.key.keysym.sym == SDLK_ESCAPE)
          running = false;

      // mouse was moved
      int x = 0, y = 0;
      if (e.type == SDL_MOUSEMOTION)
      {
        x = e.motion.xrel;
        y = e.motion.yrel;
        MouseControls(x, y);
      }
    }

    //Handle keyboard input
    const static unsigned char *keyboard = SDL_GetKeyboardState(NULL);
    KeyboardControls(keyboard);

    //Render
    render();

    //Update screen
    SDL_GL_SwapWindow(gWindow);

    //Just in case Vsync fails!
    Sleep(15);

  }

  //Free engine resources
  close();

  return 0; // return success!
}
