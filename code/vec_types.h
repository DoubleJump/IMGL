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
	f32 x;
	f32 y;
	f32 width;
	f32 height;
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
	Vec3 a, b, c, d;
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

struct Line
{
	Vec3 start;
	Vec3 end;
};

struct AABB
{
	Vec3 min;
	Vec3 max;
};

struct RGBA
{
	f32 r, g, b, a;
};

struct RGB8
{
	u8 r, g, b;
};