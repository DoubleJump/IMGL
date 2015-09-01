fn Color
operator + (Color a, f32 b)
{
	return { a.r + b, a.g + b, a.b + b, a.a + b };
}

fn Color 
operator - (Color a, f32 b)
{
	return { a.r - b, a.g - b, a.b - b, a.a - b };
}

fn Color 
operator * (Color a, f32 f)
{
	return { a.r * f, a.g * f, a.b * f, a.a * f };
}

fn Color 
operator / (Color a, f32 f)
{
	return { a.r / f, a.g / f, a.b / f, a.a / f };
}

fn Color 
operator *= (Color& a, float f)
{
	a = a * f;
	return a;
}

fn Color 
operator /= (Color& a, float f)
{
	a = a / f;
	return a;
}

namespace color
{
	fn Color
	min(Color a, Color b)
	{
		return 
		{ 
			math::min(a.r, b.r),
			math::min(a.g, b.g),
			math::min(a.b, b.b),
			math::min(a.a, b.a)
		};
	}

	fn Color
	max(Color a, Color b)
	{
		return 
		{ 
			math::max(a.r, b.r),
			math::max(a.g, b.g),
			math::max(a.b, b.b),
			math::max(a.a, b.a)
		};
	}

	fn Color
	clamp(Color v, f32 min, f32 max)
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