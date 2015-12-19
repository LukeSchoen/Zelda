#include <SDL_opengl.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


struct VertexObject
{
  uint32_t verticeCount = 0;
  GLint textureID = 0;
  GLuint vertPosDataGLPtr = 0;
  GLuint texPosDataGLPtr = 0;
  GLuint normPosDataGLPtr = 0;
  GLfloat *vertPosData;
  GLfloat *texPosData;
  GLfloat *normPosData;

};

struct RenderObject
{
  glm::mat4 modelMatrix;
  VertexObject *vertexObjects;
  uint32_t vertexObjectCount = 0;
};

void InitializeMaterialLibrary(MTL &mtl, char * basePath)
{
  SDL_Surface *tex;
  char fullMatPath[1024];
  memcpy(fullMatPath, basePath, strlen(basePath) + 1); // initialize the base path

  for (int matID = 0; matID < mtl.materialCount; matID++)
  {
    memcpy(fullMatPath + strlen(basePath), mtl.materials[matID].path, strlen(mtl.materials[matID].path) + 1); // append the local material path

    if ((tex = SDL_LoadBMP(fullMatPath)))
    {
      glGenTextures(1, &(mtl.materials[matID].textureID));
      glBindTexture(GL_TEXTURE_2D, mtl.materials[matID].textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, 3, tex->w, tex->h, 0, GL_BGR, GL_UNSIGNED_BYTE, tex->pixels);

      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
  }

  if (tex)
    SDL_FreeSurface(tex);
}

void RenderRenderObject(RenderObject &RenObj)
{
  for (int VertexObjectID = 0; VertexObjectID < RenObj.vertexObjectCount; VertexObjectID++)
  {
    //vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, RenObj.vertexObjects[VertexObjectID].vertPosDataGLPtr);
    glVertexAttribPointer(
      0,                  // attribute number. must match the layout in the shader.
      3,                  // count
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

    //texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, RenObj.vertexObjects[VertexObjectID].texPosDataGLPtr);
    glVertexAttribPointer(
      1,                  // attribute number. must match the layout in the shader.
      2,                  // count
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

    //polygon normals
    glBindBuffer(GL_ARRAY_BUFFER, RenObj.vertexObjects[VertexObjectID].normPosDataGLPtr);
    glVertexAttribPointer(
      2,                  // attribute number. must match the layout in the shader.
      3,                  // count
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

    glBindTexture(GL_TEXTURE_2D, RenObj.vertexObjects[VertexObjectID].textureID);
    glDrawArrays(GL_TRIANGLES, 0, RenObj.vertexObjects[VertexObjectID].verticeCount);
  }
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
    for (int faceID = 0; faceID < model.faceCount; faceID++)
      if (texID == model.polys[faceID].textureID) faceCount++;

    //create vertexObject for this material
    object.vertexObjects[objID].textureID = texID;
    object.vertexObjects[objID].vertPosData = new GLfloat[faceCount * 9];
    object.vertexObjects[objID].texPosData = new GLfloat[faceCount * 6];
    object.vertexObjects[objID].normPosData = new GLfloat[faceCount * 9];

    uint32_t currentFace = 0;
    for (int faceID = 0; faceID < model.faceCount; faceID++)
    {
      if (texID == model.polys[faceID].textureID)
      {
        //Positions
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 0] = model.polys[faceID].verticies[0].x;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 1] = model.polys[faceID].verticies[0].y;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 2] = model.polys[faceID].verticies[0].z;

        object.vertexObjects[objID].vertPosData[currentFace * 9 + 3] = model.polys[faceID].verticies[1].x;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 4] = model.polys[faceID].verticies[1].y;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 5] = model.polys[faceID].verticies[1].z;

        object.vertexObjects[objID].vertPosData[currentFace * 9 + 6] = model.polys[faceID].verticies[2].x;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 7] = model.polys[faceID].verticies[2].y;
        object.vertexObjects[objID].vertPosData[currentFace * 9 + 8] = model.polys[faceID].verticies[2].z;

        //UVs
        object.vertexObjects[objID].texPosData[currentFace * 6 + 0] = model.polys[faceID].verticies[0].u;
        object.vertexObjects[objID].texPosData[currentFace * 6 + 1] = model.polys[faceID].verticies[0].v;

        object.vertexObjects[objID].texPosData[currentFace * 6 + 2] = model.polys[faceID].verticies[1].u;
        object.vertexObjects[objID].texPosData[currentFace * 6 + 3] = model.polys[faceID].verticies[1].v;

        object.vertexObjects[objID].texPosData[currentFace * 6 + 4] = model.polys[faceID].verticies[2].u;
        object.vertexObjects[objID].texPosData[currentFace * 6 + 5] = model.polys[faceID].verticies[2].v;

        ////Normals
        //glm::vec3 V1 = glm::vec3(model.polys[faceID].verticies[0].x, model.polys[faceID].verticies[0].y, model.polys[faceID].verticies[0].z);
        //glm::vec3 V2 = glm::vec3(model.polys[faceID].verticies[1].x, model.polys[faceID].verticies[1].y, model.polys[faceID].verticies[1].z);
        //glm::vec3 V3 = glm::vec3(model.polys[faceID].verticies[2].x, model.polys[faceID].verticies[2].y, model.polys[faceID].verticies[2].z);
        //glm::vec3 Vert1 = V2 - V1;
        //glm::vec3 Vert2 = V3 - V1;
        //glm::vec3 Normal = glm::normalize(glm::cross(Vert1, Vert2));

        object.vertexObjects[objID].normPosData[currentFace * 9 + 0] = model.polys[faceID].verticies[0].nx;
        object.vertexObjects[objID].normPosData[currentFace * 9 + 1] = model.polys[faceID].verticies[0].ny;
        object.vertexObjects[objID].normPosData[currentFace * 9 + 2] = model.polys[faceID].verticies[0].nz;

        object.vertexObjects[objID].normPosData[currentFace * 9 + 3] = model.polys[faceID].verticies[1].nx;
        object.vertexObjects[objID].normPosData[currentFace * 9 + 4] = model.polys[faceID].verticies[1].ny;
        object.vertexObjects[objID].normPosData[currentFace * 9 + 5] = model.polys[faceID].verticies[1].nz;

        object.vertexObjects[objID].normPosData[currentFace * 9 + 6] = model.polys[faceID].verticies[2].nx;
        object.vertexObjects[objID].normPosData[currentFace * 9 + 7] = model.polys[faceID].verticies[2].ny;
        object.vertexObjects[objID].normPosData[currentFace * 9 + 8] = model.polys[faceID].verticies[2].nz;

        object.vertexObjects[objID].verticeCount += 3;
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

    glGenBuffers(1, &object.vertexObjects[objID].normPosDataGLPtr);
    glBindBuffer(GL_ARRAY_BUFFER, object.vertexObjects[objID].normPosDataGLPtr);
    glBufferData(GL_ARRAY_BUFFER, faceCount * 9 * sizeof(GLfloat), object.vertexObjects[objID].normPosData, GL_STATIC_DRAW);

  }
  return object;
}

RenderObject GenerateRenderObject(char* modelPath)
{
  //Determine Paths
  int pathLen = strlen(modelPath);
  char * basePath;
  for (int c = pathLen - 1; c >= 0; c--)
  {
    if (modelPath[c] == '\\' | modelPath[c] == '/')
    {
      basePath = new char[c + 2];
      memcpy(basePath, modelPath, c + 1);
      basePath[c + 1] = NULL;
      break;
    }
  }
  char* mtlName = ExtractMTLName(modelPath);
  char * mtlPath = new char[strlen(basePath) + strlen(mtlName) + 1];
  memcpy(mtlPath, basePath, strlen(basePath));
  memcpy(mtlPath + strlen(basePath), mtlName, strlen(mtlName) + 1);
  mtlPath = mtlPath;
  //Load Data
  MTL MODELMTL = LoadMTL(mtlPath);
  InitializeMaterialLibrary(MODELMTL, basePath);
  OBJ MODELOBJ = LoadOBJ(modelPath, MODELMTL);
  return GenerateRenderObject(MODELOBJ, MODELMTL);
}
