#ifndef VEC_TYPES_H
#define VEC_TYPES_H

struct Vec2
{
	f32 x, y;
	f32 operator[](int i) 
	{
		ASSERT(i == 0 || i == 1); 
		return (&x)[i]; 
	}
};

struct Vec2i
{
	i32 x, y;
	i32 operator[](int i) 
	{
		ASSERT(i == 0 || i == 1);
		return (&x)[i]; 
	}
};

struct Vec3
{
	f32 x, y, z;
	f32 operator[](int i) 
	{ 
		ASSERT(i > -1 && i < 4); 
		return (&x)[i]; 
	}
};

struct AngleAxis
{
	f32 angle;
	Vec3 axis;
};

struct Rect
{
	Vec2 min;
	Vec2 max;
};

struct Quat
{
	f32 x, y, z, w;
	f32 operator[](int i) 
	{
		ASSERT(i > -1 && i < 5);  
		return (&x)[i]; 
	}
};

struct Bezier
{
	Vec2 a, b, c, d;
};

struct Mat3
{
	f32 m[9];
	f32 operator[](int i) 
	{
		return m[i]; 
	}
};

struct Mat4
{
	f32 m[16];
	f32 operator[](int i) 
	{
		return m[i]; 
	}
};

struct Ray
{
	Vec3 origin;
	Vec3 dir;
};

struct AABB
{
	Vec3 min;
	Vec3 max;
};

struct Color
{
	f32 r, g, b, a;
};

struct RGB
{
	u8 r, g, b;
};


#endif