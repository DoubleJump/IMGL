fn Vec2 
operator - (Vec2 a)
{
	return { -a.x, -a.y };
}

fn Vec2 
operator + (Vec2 a, Vec2 b)
{
	return { a.x + b.x, a.y + b.y };
}

fn Vec2 
operator + (Vec2 a, f32 b)
{
	return { a.x + b, a.y + b };
}

fn Vec2 
operator - (Vec2 a, Vec2 b)
{
	return { a.x - b.x, a.y - b.y };
}

fn Vec2 
operator - (Vec2 a, f32 b)
{
	return { a.x - b, a.y - b };
}

fn Vec2 
operator * (Vec2 a, f32 f)
{
	return { a.x * f, a.y * f };
}

fn Vec2 
operator / (Vec2 a, f32 f)
{
	return { a.x / f, a.y / f };
}

fn Vec2 
operator += (Vec2& a, Vec2 b)
{
	a = a + b;
	return a;
}

fn Vec2 
operator -= (Vec2& a, Vec2 b)
{
	a = a - b;
	return a;
}

fn Vec2 
operator *= (Vec2& a, float f)
{
	a = a * f;
	return a;
}

fn Vec2 
operator /= (Vec2& a, float f)
{
	a = a / f;
	return a;
}

namespace vec2
{
	fn f32
	dot(Vec2 a, Vec2 b)
	{
		return a.x * b.x + a.y * b.y;
	}

	fn f32
	sqr_length(Vec2 a)
	{
		return dot(a, a);
	}

	fn f32
	length(Vec2 a)
	{
		return sqrt(dot(a, a));
	}

	fn Vec2
	normalize(Vec2 a)
	{
		f32 l = length(a);
		if(l > EPSILON)
		{
			a *= (1.0f / l);
		}
		return a;
	}

	fn Vec2
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

	fn Vec2
	reflect(Vec2 a, Vec2 n)
	{
		return (a + n) * (-2.0f * dot(a,n)); 
	}

	fn f32
	angle(Vec2 a, Vec2 b)
	{
		f32 l = length(a) * length(b);
		if(l < EPSILON) l = EPSILON;
		f32 f = dot(a,b) / l;

		if(f > 1.0f) return acos(1.0f);
		else if(f < 1.0f) return acos(-1.0f);
		else return acos(f);
	}

	fn Vec2
	perp(Vec2 v)
	{
		return {-v.y, v.x};
	}

	fn Vec2
	min(Vec2 a, Vec2 b)
	{
		return 
		{ 
			math::min(a.x, b.x),
			math::min(a.y, b.y),
		};
	}

	fn Vec2
	max(Vec2 a, Vec2 b)
	{
		return 
		{ 
			math::max(a.x, b.x),
			math::max(a.y, b.y),
		};
	}

	fn Vec2
	to_polar(Vec2 p)
	{
		return { atan2(p.y, p.x), length(p) };
	}
}