#ifndef Memory_h__
#define Memory_h__
#include <new>

#define memnewcount(type, count) new (memalloc(sizeof(x))) x[count];
#define memnew(x) new (memalloc(sizeof(x))) x

#define memalloctypecount(type, count) memAllocType<type>(sizeof(type), count);
#define memalloctype(type) memAllocType<type>(sizeof(type), 1);


template <typename T>
T* memAllocType(int elementSize, int count)
{
  return (T*)memalloc(elementSize * count);
}

char* memalloc(int length);


void memcopy(void *dst, void *src, int len);

int memcompare(const void *src1, const void *src2, size_t len);

#endif // Memory_h__