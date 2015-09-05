struct Vec2
{
	f32 x, y;
	f32 operator[](int i) 
	{
		ASSERT(i == 0 || i == 1, "Invalid Vector2 index"); 
		return (&x)[i]; 
	}
};

struct Vec2i
{
	i32 x, y;
	i32 operator[](int i) 
	{
		ASSERT(i == 0 || i == 1, "Invalid Vec2i index");
		return (&x)[i]; 
	}
};

struct Vec3
{
	f32 x, y, z;
	f32 operator[](int i) 
	{ 
		ASSERT(i > -1 && i < 4, "Invalid Vector3 index"); 
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
	f32 x, y, width, height;
};



struct Quat
{
	f32 x, y, z, w;
	f32 operator[](int i) 
	{
		ASSERT(i > -1 && i < 5, "Invalid Quaternion index");  
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
	Vec3 origin, dir;
};

struct Line
{
	Vec3 start, end;
};

struct AABB
{
	Vec3 min, max;
};

struct Color
{
	f32 r, g, b, a;
};

struct RGB8
{
	u8 r, g, b;
};

struct RGBA8
{
	u8 r,g,b,a;
};