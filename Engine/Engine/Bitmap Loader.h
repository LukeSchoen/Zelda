#include <stdio.h>

struct BMP
{
  unsigned char* data = nullptr;
  int width = 0;
  int height = 0;
};

BMP loadBMP(const char * imagepath)
{
  // Data read from the header of the BMP file
  unsigned char header[54]; // Each BMP file begins by a 54-bytes header
  unsigned int dataPos;     // Position in the file where the actual data begins
  unsigned int width, height;
  unsigned int imageSize;   // = width*height*3

  BMP bitmap;

  // Actual RGB data
  unsigned char * data;

  // Open the file
  FILE * file = fopen(imagepath, "rb");
  if (!file)
  {
    printf("Image could not be opened\n");
    return bitmap;
  }

  if (fread(header, 1, 54, file) != 54)
  { // If not 54 bytes read : problem
    printf("Not a correct BMP file\n");
    return bitmap;
  }

  if (header[0] != 'B' || header[1] != 'M')
  {
    printf("Not a correct BMP file\n");
    return bitmap;
  }

  // Read ints from the byte array
  dataPos = *(int*)&(header[0x0A]);
  imageSize = *(int*)&(header[0x22]);
  width = *(int*)&(header[0x12]);
  height = *(int*)&(header[0x16]);

  // Some BMP files are badly formatted, may have to guess missing information
  if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
  if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

  // Create a buffer
  data = new unsigned char[imageSize];

  // Read the actual data from the file into the buffer
  fread(data, 1, imageSize, file);

  //Everything is in memory now, the file can be closed
  fclose(file);

  // Create a buffer
  data = new unsigned char[imageSize];

  // Read the actual data from the file into the buffer
  fread(data, 1, imageSize, file);

  //Everything is in memory now, the file can be closed
  fclose(file);

  bitmap.data = data;
  bitmap.width = width;
  bitmap.height = height;

  return bitmap;
}