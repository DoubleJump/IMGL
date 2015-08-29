fn Vec2i 
operator - (Vec2i a)
{
	return { -a.x, -a.y };
}

fn Vec2i 
operator + (Vec2i a, Vec2i b)
{
	return { a.x + b.x, a.y + b.y };
}

fn Vec2i 
operator - (Vec2i a, Vec2i b)
{
	return { a.x - b.x, a.y - b.y };
}

fn Vec2i 
operator * (Vec2i a, i32 b)
{
	return { a.x * b, a.y * b };
}

fn Vec2i 
operator / (Vec2i a, i32 b)
{
	return { a.x / b, a.y / b };
}

fn Vec2i 
operator += (Vec2i& a, Vec2i b)
{
	a = a + b;
	return a;
}

fn Vec2i 
operator -= (Vec2i& a, Vec2i b)
{
	a = a - b;
	return a;
}

fn Vec2i 
operator *= (Vec2i& a, i32 b)
{
	a = a * b;
	return a;
}

fn Vec2i 
operator /= (Vec2i& a, i32 b)
{
	a = a / b;
	return a;
}

namespace vec2i
{
	fn Vec2i
	from_vec2f(Vec2 v)
	{
		return { math::round_to_i32(v.x), math::round_to_i32(v.y) };
	}

	fn i32
	dot(Vec2i a, Vec2i b)
	{
		return a.x * b.x + a.y * b.y;
	}

	fn i32
	sqr_length(Vec2i a)
	{
		return dot(a, a);
	}

	fn f32
	length(Vec2i a)
	{
		return sqrt((f32)dot(a, a));
	}

	fn f32
	angle(Vec2i a, Vec2i b)
	{
		f32 l = length(a) * length(b);
		if(l < EPSILON) l = EPSILON;
		f32 f = dot(a,b) / l;

		if(f > 1.0f) return acos(1.0f);
		else if(f < 1.0f) return acos(-1.0f);
		else return acos(f);
	}

	fn Vec2i
	min(Vec2i a, Vec2i b)
	{
		return 
		{ 
			math::min(a.x, b.x),
			math::min(a.y, b.y),
		};
	}

	fn Vec2i
	max(Vec2i a, Vec2i b)
	{
		return 
		{ 
			math::max(a.x, b.x),
			math::max(a.y, b.y),
		};
	}
}
