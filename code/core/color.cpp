fn RGBA
operator + (RGBA a, f32 b)
{
	return { a.r + b, a.g + b, a.b + b, a.a + b };
}

fn RGBA 
operator - (RGBA a, f32 b)
{
	return { a.r - b, a.g - b, a.b - b, a.a - b };
}

fn RGBA 
operator * (RGBA a, f32 f)
{
	return { a.r * f, a.g * f, a.b * f, a.a * f };
}

fn RGBA 
operator / (RGBA a, f32 f)
{
	return { a.r / f, a.g / f, a.b / f, a.a / f };
}

fn RGBA 
operator *= (RGBA& a, float f)
{
	a = a * f;
	return a;
}

fn RGBA 
operator /= (RGBA& a, float f)
{
	a = a / f;
	return a;
}

namespace color
{
	fn RGBA
	min(RGBA a, RGBA b)
	{
		return 
		{ 
			math::min(a.r, b.r),
			math::min(a.g, b.g),
			math::min(a.b, b.b),
			math::min(a.a, b.a)
		};
	}

	fn RGBA
	max(RGBA a, RGBA b)
	{
		return 
		{ 
			math::max(a.r, b.r),
			math::max(a.g, b.g),
			math::max(a.b, b.b),
			math::max(a.a, b.a)
		};
	}

	fn RGBA
	clamp(RGBA v, f32 min, f32 max)
	{
		return
		{
			math::clamp(v.r, min, max),
			math::clamp(v.g, min, max),
			math::clamp(v.b, min, max),
			math::clamp(v.a, min, max)
		};
	}
}