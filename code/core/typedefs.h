#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#define internal static
#define local_persist static
#define global_var static
#define fn static

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef i32 b32;

typedef float f32;
typedef double f64;

typedef size_t memsize;

typedef u32 AssetID;

struct string
{
	memsize length;
	char* data;
};

#if DEBUG
#define FAIL(expr, ...) printf(expr, __VA_ARGS__); printf("Error in file: %s line: %d \n ", __FILE__, __LINE__ ); *(int*)0 = 0;
#define ASSERT(expr, msg, ...) if(!(expr)) { FAIL(msg, __VA_ARGS__); } 
#else
#define FAIL(expr)
#define ASSERT(expr, msg)
#endif

#define KILOBYTES(value) ((value)*1024LL)
#define MEGABYTES(value) (KILOBYTES(value)*1024LL)
#define GIGABYTES(value) (MEGABYTES(value)*1024LL)
#define TERABYTES(value) (GIGABYTES(value)*1024LL)
#define ARRAY_COUNT(Array) (sizeof(Array) / sizeof((Array)[0]))

#define FLAG_SET(mask, flag) ((flag & mask) == flag)
#define LOOP(index, count) for(int index = 0; index < count; ++index)

#endif