#include <stdio.h>
#include <cstdint>
#include "detail\func_common.hpp"

//////////////////////////////////////////////////////////////////////////
//  STRUCTS
//////////////////////////////////////////////////////////////////////////

struct Vertex
{
  float x;
  float y;
  float z;
  float u;
  float v;
};

struct Triangle
{
  Vertex verticies[3];
  uint32_t textureID = 0;
};

struct Material
{
  char* name;
  char* path;
  uint32_t textureID = 0;
};

struct MTL
{
  Material *materials;
  uint32_t materialCount = 0;
};

struct OBJ
{
  uint32_t faceCount = 0;
  Triangle *polys;
};

//////////////////////////////////////////////////////////////////////////
//  METHODS
//////////////////////////////////////////////////////////////////////////

template <class T> const T& max(const T& a, const T& b) { return (a < b) ? b : a; }

int MaterialNameToTexture(char* materialName, MTL &mtl)
{
  for (int matID = 0; matID < mtl.materialCount ; matID++)
  {
    if (strcmp(mtl.materials[matID].name, materialName) == 0)
    {
      return mtl.materials[matID].textureID;
    }
  }
}

char* ExtractMTLName(char* OBJpath)
{
  //Read File Into Memory
  FILE *file = fopen(OBJpath, "rb");
  fseek(file, 0, SEEK_END);
  int fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);
  char* fileData = new char[fileSize];
  fread(fileData, 1, fileSize, file);
  fclose(file);

  //Do A Quick Parse Over The File To Determine How Much Temporary Memory We Need To Load It
  bool newLine = true;
  for (int fptr = 0; fptr < fileSize; fptr++)
  {
    if (newLine)
    {
      if (fileData[fptr] == 'm' & fileData[fptr + 1] == 't' & fileData[fptr + 2] == 'l' & fileData[fptr + 3] == 'l' & fileData[fptr + 4] == 'i' & fileData[fptr + 5] == 'b' & fileData[fptr + 6] == ' ')
      {
        char* mtlName = new char[256];
        sscanf((const char*)(fileData + fptr + 7), "%s\n", mtlName);
        delete[] fileData;
        return mtlName;
      }
      newLine = false;
    }
    if (fileData[fptr] == '\n') // Count Lines
    {
      newLine = true;
    }
  }
  printf("No material found in file %s \n\n", OBJpath);
  delete[] fileData;
  return nullptr;
}

MTL LoadMTL(char* path)
{

  //Variables
  uint32_t fileSize = 0;
  uint32_t materialCount = 0;
  char materialName[256];
  char materialPath[256];

  //Read File Into Memory
  FILE *file = fopen(path, "rb");
  fseek(file, 0, SEEK_END);
  fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);
  char* fileData = new char[fileSize];
  fread(fileData, 1, fileSize, file);
  fclose(file);

  //Do A Quick Parse Over The File To Determine How Much Temporary Memory We Need To Load It
  bool newLine = true;
  int lineType = 0;
  for (int fptr = 0; fptr < fileSize; fptr++)
  {
    if (newLine) //Determine This Lines type
    {
      if (fileData[fptr] == '#' & fileData[fptr + 1] == ' ')
        lineType = 1; // Comment

      if (fileData[fptr] == 'n' & fileData[fptr + 1] == 'e' & fileData[fptr + 2] == 'w' & fileData[fptr + 3] == 'm' & fileData[fptr + 4] == 't' & fileData[fptr + 5] == 'l' & fileData[fptr + 6] == ' ')
      {
        lineType = 2; // New Material
        materialCount++;
      }

      if (fileData[fptr] == 'm' & fileData[fptr + 1] == 'a' & fileData[fptr + 2] == 'p' & fileData[fptr + 3] == '_' & fileData[fptr + 4] == 'K' & fileData[fptr + 5] == 'd' & fileData[fptr + 6] == ' ')
      {
        lineType = 3; // Diffusive Map
      }

      if (lineType != 0)
        newLine = false;
    }
    if (fileData[fptr] == '\n')
    {
      newLine = true;
      lineType = 0;
    }
  }

  //Create MTL
  MTL MTL;
  MTL.materials = new Material[materialCount];

  //Parse File And Load Data
  newLine = true;
  uint32_t materialItr = 0;
  for (int fptr = 0; fptr < fileSize; fptr++)
  {
    if (newLine) //Determine This Lines type
    {
      if (fileData[fptr] == '#' & fileData[fptr + 1] == ' ')
        lineType = 1; // Comment

      if (fileData[fptr] == 'n' & fileData[fptr + 1] == 'e' & fileData[fptr + 2] == 'w' & fileData[fptr + 3] == 'm' & fileData[fptr + 4] == 't' & fileData[fptr + 5] == 'l' & fileData[fptr + 6] == ' ')
      {
        lineType = 2; // New Material
        sscanf((const char*)(fileData + fptr + 7), "%s\n", materialName);
      }

      if (fileData[fptr] == 'm' & fileData[fptr + 1] == 'a' & fileData[fptr + 2] == 'p' & fileData[fptr + 3] == '_' & fileData[fptr + 4] == 'K' & fileData[fptr + 5] == 'd' & fileData[fptr + 6] == ' ')
      {
        lineType = 3; // Diffusive Map
        //Write Name
        sscanf((const char*)(fileData + fptr + 7), "%s\n", materialPath);
        MTL.materials[materialItr].name = new char[strlen(materialName) + 1];
        memcpy(MTL.materials[materialItr].name, materialName, strlen(materialName));
        MTL.materials[materialItr].name[strlen(materialName)] = NULL;

        //Write Path
        MTL.materials[materialItr].path = new char[strlen(materialPath) + 1];
        memcpy(MTL.materials[materialItr].path, materialPath, strlen(materialPath));
        MTL.materials[materialItr].path[strlen(materialPath)] = NULL;
        materialItr++;
        MTL.materialCount++;
      }

      if (lineType != 0)
        newLine = false;
    }
    if (fileData[fptr] == '\n')
    {
      newLine = true;
      lineType = 0;
    }
  }

  return MTL;
}

OBJ LoadOBJ(char* path, MTL &mtl)
{
  //Variables
  uint32_t fileSize = 0;
  uint32_t lineCount = 1;
  uint32_t vertCount = 0;
  uint32_t UVCount = 0;
  uint32_t faceCount = 0;

  //Read File Into Memory
  FILE *file = fopen(path, "rb");
  fseek(file, 0, SEEK_END);
  fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);
  char* fileData = new char[fileSize];
  fread(fileData, 1, fileSize, file);
  fclose(file);

  //Do A Quick Parse Over The File To Determine How Much Temporary Memory We Need To Load It
  bool newLine = true;
  int lineType = 0;
  for (int fptr = 0; fptr < fileSize; fptr++)
  {
    if (newLine) //Determine This Lines type
    {
      if (fileData[fptr] == '#' & fileData[fptr + 1] == ' ')
        lineType = 1; // Comment

      if (fileData[fptr] == 'g' & fileData[fptr + 1] == ' ')
        lineType = 2; // Group

      if (fileData[fptr] == 'v' & fileData[fptr + 1] == ' ')
      {
        lineType = 3; // Vertex X Y Z
        vertCount++;
      }

      if (fileData[fptr] == 'v' & fileData[fptr + 1] == 't' & fileData[fptr + 2] == ' ')
      {
        lineType = 4; // Vertex U V
        UVCount++;
      }

      if (fileData[fptr] == 'f' & fileData[fptr + 1] == ' ')
      {
        lineType = 5; // Face
        faceCount++;
      }

      if (lineType != 0)
        newLine = false;
    }
    if (fileData[fptr] == '\n') // Count Lines
    {
      newLine = true;
      lineType = 0;
      lineCount++;
    }
  }

  //Create OBJ
  OBJ model;
  Vertex *OBJ_Verts = new Vertex[max(vertCount, UVCount)];
  model.polys = new Triangle[faceCount];

  //Parse File And Load Data
  int vertItr = 0;
  int texcItr = 0;
  int faceItr = 0;
  int actvTex = 0;
  char materialName[256];
  newLine = true;
  for (int fptr = 0; fptr < fileSize; fptr++)
  {
    if (newLine) //Determine This Lines type
    {
      if (fileData[fptr] == '#' & fileData[fptr + 1] == ' ')
        lineType = 1; // Comment
  
      if (fileData[fptr] == 'g' & fileData[fptr + 1] == ' ')
        lineType = 2; // Group

      if (fileData[fptr] == 'v' & fileData[fptr + 1] == ' ')
      {
        lineType = 3; // Vertex X Y Z
        sscanf((const char*)(fileData + fptr + 2), "%f %f %f", &(OBJ_Verts[vertItr].x), &(OBJ_Verts[vertItr].y), &(OBJ_Verts[vertItr].z));
        vertItr++;
      }
  
      if (fileData[fptr] == 'v' & fileData[fptr + 1] == 't' & fileData[fptr + 2] == ' ')
      {
        lineType = 4; // Vertex U V
        sscanf((const char*)(fileData + fptr + 3), "%f %f", &(OBJ_Verts[texcItr].u), &(OBJ_Verts[texcItr].v));
        texcItr++;
      }
  
      if (fileData[fptr] == 'f' & fileData[fptr + 1] == ' ')
      {
        lineType = 5; // Face
        char d1[32];
        char d2[32];
        char d3[32];
        sscanf((const char*)(fileData + fptr + 2), "%s %s %s\n", d1, d2, d3);
        int v1 = 0;
        int v2 = 0;
        int v3 = 0;
        int t1 = 0;
        int t2 = 0;
        int t3 = 0;
        sscanf(d1, "%d/%d/", &v1, &t1);
        sscanf(d2, "%d/%d/", &v2, &t2);
        sscanf(d3, "%d/%d/", &v3, &t3);
        //Store Faces Vertex Position Data
        model.polys[faceItr].verticies[0] = OBJ_Verts[v1 - 1];
        model.polys[faceItr].verticies[1] = OBJ_Verts[v2 - 1];
        model.polys[faceItr].verticies[2] = OBJ_Verts[v3 - 1];
        //Store Faces Vertex UV Data
        model.polys[faceItr].verticies[0].u = OBJ_Verts[t1 - 1].u;
        model.polys[faceItr].verticies[0].v = OBJ_Verts[t1 - 1].v;
        model.polys[faceItr].verticies[1].u = OBJ_Verts[t2 - 1].u;
        model.polys[faceItr].verticies[1].v = OBJ_Verts[t2 - 1].v;
        model.polys[faceItr].verticies[2].u = OBJ_Verts[t3 - 1].u;
        model.polys[faceItr].verticies[2].v = OBJ_Verts[t3 - 1].v;
        model.polys[faceItr].textureID = actvTex;
        model.faceCount++;
        faceItr++;
      }

      if (fileData[fptr] == 'u' & fileData[fptr + 1] == 's' & fileData[fptr + 2] == 'e' & fileData[fptr + 3] == 'm' & fileData[fptr + 4] == 't' & fileData[fptr + 5] == 'l' & fileData[fptr + 6] == ' ')
      {
        lineType = 6; // Using Material
        sscanf((const char*)(fileData + fptr + 7), "%s\n", materialName);
        actvTex = MaterialNameToTexture(materialName, mtl);
        lineType = lineType;
      }
  
      if (lineType != 0)
        newLine = false;
    }
    if (fileData[fptr] == '\n') // End of line
    {
      newLine = true;
      lineType = 0;
    }
  }
  delete[] OBJ_Verts;
  delete[] fileData;
  return model;
}
