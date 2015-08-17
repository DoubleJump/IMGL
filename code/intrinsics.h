#ifndef INTRINSICS_H
#define INTRINSICS_H

#include <math.h>

namespace math
{
	internal f32
	square(f32 x)
	{
		return x * x;
	}

	internal i32
	square(i32 x)
	{
		return x * x;
	}

	internal u32
	safe_truncate_u64(u64 value)
	{
		ASSERT(value <= UINT32_MAX);
		return (u32)value;
	}

	internal i32
	round_to_i32(f32 val)
	{
		return (i32)roundf(val);
	}

	internal u32
	round_to_u32(f32 val)
	{
		return (u32)roundf(val);
	}

	internal i32
	ceil_to_i32(f32 x)
	{
		return (i32)ceilf(x);
	}

	internal i32
	floor_to_i32(f32 x)
	{
		return (i32)floorf(x);
	}
}

/*
internal f32
abs(f32 x)
{
	
}

internal f32
sin(f32 x)
{

}

internal f32
cos(f32 x)
{

}

internal f32
tan(f32 x)
{

}

internal f32
asin(f32 x)
{

}

internal f32
acos(f32 x)
{

}

internal f32
atan(f32 x)
{

}

internal f32
atan2(f32 y, f32 x)
{

}

internal f32
sqrt(f32 x)
{
	
}
*/

internal f32
inv_sqrt(f32 x)
{
	f32 half = 0.5f * x;
	int i = *(int*)&x; // store Numbering-point bits in integer
	i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
	x = *(f32*)&i; // convert new bits into Number
	x = x * (1.5f - half * x * x); // One round of Newton's method
	return x;
}

internal i32
sign(i32 x)
{
	if(x >= 0) return 1;
	else return 0;
}


struct BitScanResult
{
	b32 found;
	u32 index;
};

internal BitScanResult
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

#endif