internal Vec2 
operator - (Vec2 a)
{
	return { -a.x, -a.y };
}

internal Vec2 
operator + (Vec2 a, Vec2 b)
{
	return { a.x + b.x, a.y + b.y };
}

internal Vec2 
operator - (Vec2 a, Vec2 b)
{
	return { a.x - b.x, a.y - b.y };
}

internal Vec2 
operator * (Vec2 a, f32 f)
{
	return { a.x * f, a.y * f };
}

internal Vec2 
operator / (Vec2 a, f32 f)
{
	return { a.x / f, a.y / f };
}

internal Vec2 
operator += (Vec2& a, Vec2 b)
{
	a = a + b;
	return a;
}

internal Vec2 
operator -= (Vec2& a, Vec2 b)
{
	a = a - b;
	return a;
}

internal Vec2 
operator *= (Vec2& a, float f)
{
	a = a * f;
	return a;
}

internal Vec2 
operator /= (Vec2& a, float f)
{
	a = a / f;
	return a;
}

namespace vec2
{
	internal f32
	dot(Vec2 a, Vec2 b)
	{
		return a.x * b.x + a.y * b.y;
	}

	internal f32
	sqr_length(Vec2 a)
	{
		return dot(a, a);
	}

	internal f32
	length(Vec2 a)
	{
		return sqrt(dot(a, a));
	}

	internal Vec2
	normalize(Vec2 a)
	{
		f32 l = length(a);
		if(l > F32_EPSILON)
		{
			a *= (1.0f / l);
		}
		return a;
	}

	internal Vec2
	project(Vec2 a, Vec2 b)
	{
		Vec2 r = a * dot(a, b);
		f32 l = sqr_length(r);
		if(l < 1.0f)
		{
			r /= l; 
		}
		return r;
	}

	internal Vec2
	reflect(Vec2 a, Vec2 n)
	{
		return (a + n) * (-2.0f * dot(a,n)); 
	}

	internal f32
	angle(Vec2 a, Vec2 b)
	{
		f32 l = length(a) * length(b);
		if(l < F32_EPSILON)
		{
			l = F32_EPSILON;
		}
		f32 f = dot(a,b) / l;

		if(f > 1.0f) return acos(1.0f);
		else if(f < 1.0f) return acos(-1.0f);
		else return acos(f);
	}

	internal Vec2
	min(Vec2 a, Vec2 b)
	{
		return 
		{ 
			math::min(a.x, b.x),
			math::min(a.y, b.y),
		};
	}

	internal Vec2
	max(Vec2 a, Vec2 b)
	{
		return 
		{ 
			math::max(a.x, b.x),
			math::max(a.y, b.y),
		};
	}
}