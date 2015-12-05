#include <stdio.h>
#include <cstdint>

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
};

struct OBJ
{
  uint32_t faceCount = 0;
  Vertex *verts;
  Triangle *polys;
};

OBJ* loadOBJ(char* path)
{
  //Variables
  uint32_t fileSize = 0;
  uint32_t lineCount = 1;
  uint32_t vertCount = 0;
  uint32_t faceCount = 0;

  //Read File Into Memory
  FILE *file = fopen(path, "rb");
  fseek(file, 0, SEEK_END);
  fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);
  char* fileData = new char[fileSize];
  fread(fileData, 1, fileSize, file);


  //Do A Quick Parse Over The File To Determine How Much Temporary Memory We Need To Load It
  bool newLine = true;
  int lineType = 0;
  for (int fptr = 0; fptr < fileSize ; fptr++)
  {
    if (newLine) //Determine This Lines type
    {
      if (fileData[fptr] == '#' & fileData[fptr + 1] == ' ')
        lineType = 1; // Comment

      if (fileData[fptr] == 'g' & fileData[fptr + 1] == ' ')
        lineType = 2; // Group

      if (fileData[fptr] == 'v' & fileData[fptr + 1] == ' ')
        lineType = 3; // Vertex X Y Z

      if (fileData[fptr] == 'v' & fileData[fptr + 1] == 't' & fileData[fptr + 2] == ' ')
        lineType = 4; // Vertex U V

      if (fileData[fptr] == 'f' & fileData[fptr + 1] == ' ')
        lineType = 5; // Face

      newLine = false;
    }
    if (fileData[fptr] == '\n') // Count occurrences
    {
      if (lineType == 3) vertCount++;
      if (lineType == 5) faceCount++;
      newLine = true;
      lineType = 0;
      lineCount++;
    }
  }

  //Create OBJ
  OBJ *modelData = new OBJ;
  OBJ &model = *modelData;
  Vertex *OBJ_Verts = new Vertex[vertCount];
  model.polys = new Triangle[faceCount];

  //Parse File And Load Data
  int vertItr = 0;
  int texcItr = 0;
  int faceItr = 0;
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
        sscanf((const char*)(fileData+fptr + 2), "%f %f %f", &(OBJ_Verts[vertItr].x), &(OBJ_Verts[vertItr].y), &(OBJ_Verts[vertItr].z));
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
        sscanf((const char*)(fileData + fptr + 2), "%s %s %s%", d1, d2, d3);
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
        model.polys[faceItr].verticies[0] = OBJ_Verts[v1-1];
        model.polys[faceItr].verticies[1] = OBJ_Verts[v2 - 1];
        model.polys[faceItr].verticies[2] = OBJ_Verts[v3 - 1];
        //Store Faces Vertex UV Data
        model.polys[faceItr].verticies[0].u = OBJ_Verts[t1 - 1].u;
        model.polys[faceItr].verticies[0].v = OBJ_Verts[t1 - 1].v;
        model.polys[faceItr].verticies[1].u = OBJ_Verts[t2 - 1].u;
        model.polys[faceItr].verticies[1].v = OBJ_Verts[t2 - 1].v;
        model.polys[faceItr].verticies[2].u = OBJ_Verts[t3 - 1].u;
        model.polys[faceItr].verticies[2].v = OBJ_Verts[t3 - 1].v;
        model.faceCount++;
        faceItr++;
      }

      newLine = false;
    }
    if (fileData[fptr] == '\n') // End of line
    {
      newLine = true;
      lineType = 0;
    }
  }
  delete[] OBJ_Verts;
  return modelData;
}