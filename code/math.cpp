#ifndef MATH_H
#define MATH_H

namespace math
{
	#define PI32 3.14159265359f
	#define TAU32 2.0f * PI32
	#define SQRTHALF 0.7071067811865475f
	#define F32_EPSILON 0.000001f
	#define DEG2RAD PI32 / 180.0f
	#define RAD2DEG 57.5957795f

	internal f32 
	min(f32 a, f32 b) 
	{
		if(a < b) return a;
		else return b;
	}
	internal i32
	min(i32 a, i32 b) 
	{
		if(a < b) return a;
		else return b;
	}
	internal f32 
	max(f32 a, f32 b) 
	{
		if(a > b) return a;
		else return b;
	}
	internal i32
	max(i32 a, i32 b) 
	{
		if(a > b) return a;
		else return b;
	}

	internal i32 
	clamp(i32 val, i32 min, i32 max)
	{
		if(val < min) return min;
		if(val > max) return max;
		return val;
	}

	internal f32 
	clamp(f32 val, f32 min, f32 max)
	{
		if(val < min) return min;
		if(val > max) return max;
		return val;
	}

	internal f32
	lerp(f32 a, f32 b, f32 t)
	{
		return a + t * (b-a);
	}

	/*
	internal f32
	square(f32 f)
	{
		return f * f;
	}*/

	internal i32 
	next_power_of_two(i32 v)
	{
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;
		return v;
	}

	internal u32
    hash(char* str)
    {
        u32 hash = 5381;
        int c = 0;

        for(;;)
        {
        	c = *str++;
        	if(!c) break;
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }
}
#endif