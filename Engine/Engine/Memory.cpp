char memory[64 * 1024 * 1024];
int memptr = 0;

char* memalloc(int length)
{
  return memory + (memptr += length) - length;
}

void memcopy(void *dst, void *src, int len)
{
  for (int i = 0; i < len; i++)
    *((char*)dst + i) = *((char*)src + i);
}
