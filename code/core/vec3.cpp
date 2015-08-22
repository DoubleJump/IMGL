fn Vec3 
operator - (Vec3 a)
{
	return { -a.x, -a.y, -a.z };
}

fn Vec3 
operator + (Vec3 a, Vec3 b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

fn Vec3 
operator + (Vec3 a, f32 b)
{
	return { a.x + b, a.y + b, a.z + b };
}

fn Vec3 
operator - (Vec3 a, Vec3 b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

fn Vec3 
operator - (Vec3 a, f32 b)
{
	return { a.x - b, a.y - b, a.z - b };
}

fn Vec3 
operator * (Vec3 a, f32 f)
{
	return { a.x * f, a.y * f, a.z * f };
}

fn Vec3 
operator / (Vec3 a, f32 f)
{
	return { a.x / f, a.y / f, a.z / f };
}

fn Vec3 
operator += (Vec3& a, Vec3 b)
{
	a = a + b;
	return a;
}

fn Vec3 
operator -= (Vec3& a, Vec3 b)
{
	a = a - b;
	return a;
}

fn Vec3 
operator *= (Vec3& a, float f)
{
	a = a * f;
	return a;
}

fn Vec3 
operator /= (Vec3& a, float f)
{
	a = a / f;
	return a;
}

namespace vec3
{
	fn f32
	dot(Vec3 a, Vec3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	fn f32
	sqr_length(Vec3 a)
	{
		return dot(a, a);
	}

	fn f32
	length(Vec3 a)
	{
		return sqrt(dot(a, a));
	}

	fn Vec3
	normalize(Vec3 a)
	{
		f32 l = length(a);
		if(l > F32_EPSILON)
		{
			a *= (1.0f / l);
		}
		return a;
	}

	fn Vec3
	cross(Vec3 a, Vec3 b)
	{
		return
		{ 
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	fn Vec3
	project(Vec3 a, Vec3 b)
	{
		Vec3 r = a * dot(a, b);
		f32 l = sqr_length(r);
		if(l < 1.0f)
		{
			r /= l; 
		}
		return r;
	}

	fn Vec3
	reflect(Vec3 a, Vec3 n)
	{
		return (a + n) * (-2.0f * dot(a,n)); 
	}

	fn f32
	angle(Vec3 a, Vec3 b)
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

	fn Vec3
	min(Vec3 a, Vec3 b)
	{
		return 
		{ 
			math::min(a.x, b.x),
			math::min(a.y, b.y),
			math::min(a.z, b.z)
		};
	}

	fn Vec3
	max(Vec3 a, Vec3 b)
	{
		return 
		{ 
			math::max(a.x, b.x),
			math::max(a.y, b.y),
			math::max(a.z, b.z)
		};
	}

	fn Vec3
	clamp(Vec3 v, f32 min, f32 max)
	{
		return
		{
			math::clamp(v.x, min, max),
			math::clamp(v.y, min, max),
			math::clamp(v.z, min, max)
		};
	}
}