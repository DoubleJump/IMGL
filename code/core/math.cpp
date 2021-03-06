#include <math.h>

namespace math
{
	#define E 2.71828182845f
	#define PI 3.14159265359f
	#define TAU 6.28318530718f
	//#define INFINITY 99999999.0f
	#define SQRT_HALF 0.7071067811865475f
	#define EPSILON 0.000001f
	#define F32_MAX 3.4028234e38f
	#define DEG2RAD PI / 180.0f
	#define PI_OVER_360 PI / 360.0f
	#define RAD2DEG 57.5957795f
	#define GOLDEN_ANGLE (PI*(3-sqrt(5.0f)))

	fn f32 
	min(f32 a, f32 b) 
	{
		if(a < b) return a;
		else return b;
	}

	fn i32
	min(i32 a, i32 b) 
	{
		if(a < b) return a;
		else return b;
	}

	fn f32 
	max(f32 a, f32 b) 
	{
		if(a > b) return a;
		else return b;
	}
	fn i32
	max(i32 a, i32 b) 
	{
		if(a > b) return a;
		else return b;
	}

	fn i32 
	clamp(i32 val, i32 min, i32 max)
	{
		if(val < min) return min;
		if(val > max) return max;
		return val;
	}

	fn f32 
	clamp(f32 val, f32 min, f32 max)
	{
		if(val < min) return min;
		if(val > max) return max;
		return val;
	}

	fn f32
	lerp(f32 a, f32 b, f32 t)
	{
		return a + t * (b-a);
	}

	fn f32
	square(f32 f)
	{
		return f * f;
	}

	fn i32
	square(i32 i)
	{
		return i * i;
	}

	fn u32
	safe_truncate_u64(u64 value)
	{
		ASSERT(value <= UINT32_MAX, "Value is greater that UINT32_MAX");
		return (u32)value;
	}

	fn i32
	round_to_i32(f32 val)
	{
		return (i32)roundf(val);
	}

	fn u32
	round_to_u32(f32 val)
	{
		return (u32)roundf(val);
	}

	fn i32
	ceil_to_i32(f32 x)
	{
		return (i32)ceilf(x);
	}

	fn i32
	floor_to_i32(f32 x)
	{
		return (i32)floorf(x);
	}

	fn i32 
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

	fn f32
	inv_sqrt(f32 x)
	{
		f32 half = 0.5f * x;
		int i = *(int*)&x; // store Numbering-point bits in integer
		i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
		x = *(f32*)&i; // convert new bits into Number
		x = x * (1.5f - half * x * x); // One round of Newton's method
		return x;
	}

	fn i32
	sign(i32 x)
	{
		if(x >= 0) return 1;
		else return 0;
	}

	fn u32
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

	fn u32
    hash(char* str, memsize len)
    {
        u32 hash = 5381;
        int c = 0;

        for(memsize i = 0; i < len; ++i)
        {
        	c = *str++;
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }

    
}

struct BitScanResult
{
	b32 found;
	u32 index;
};

fn BitScanResult
find_lowest_bit_set(u32 value)
{
	BitScanResult result = {};

	#if COMPILER_MSVC

		result.found = _BitScanForward((unsigned long*)&result.index, value);

	#else

		for(u32 i; i < 32; ++i)
		{
			if(value & (1 << i))
			{
				result.index = i;
				result.found = true;
				break;
			}
		}

	#endif

	return result;
}