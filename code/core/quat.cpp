fn Quat 
operator * (Quat a, Quat b)
{
	return
	{
		a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
		a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
		a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
		a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x
	};
}

fn Quat 
operator * (Quat q, Vec3 v)
{
	f32 tx = (q.y * v.z - q.z * v.y) * 2.0f;
	f32 ty = (q.z * v.x - q.x * v.z) * 2.0f;
	f32 tz = (q.x * v.y - q.y * v.x) * 2.0f;

	f32 cx = q.y * tz - q.z * ty;
	f32 cy = q.z * tx - q.x * tz;
	f32 cz = q.x * ty - q.y * tx;

	return
	{
		v.x + q.w * tx + cx,
		v.y + q.w * ty + cy,
		v.z + q.w * tz + cz,
	};
}

fn Quat 
operator * (Quat a, f32 f)
{
	return { a.x * f, a.y * f, a.z * f, a.w * f };
}

fn Quat 
operator / (Quat a, f32 f)
{
	return { a.x / f, a.y / f, a.z / f, a.w / f };
}

fn Quat 
operator *= (Quat& a, Quat b)
{
	a = a * b;
	return a;
}

fn Quat 
operator *= (Quat& a, float f)
{
	a = a * f;
	return a;
}

fn Quat 
operator /= (Quat& a, float f)
{
	a = a / f;
	return a;
}

namespace quat
{
	fn Quat identity = { 0,0,0,1 };

	fn f32
	dot(Quat a, Quat b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	fn f32
	sqr_length(Quat a)
	{
		return dot(a, a);
	}

	fn f32
	length(Quat a)
	{
		return sqrt(dot(a, a));
	}

	fn Quat
	normalize(Quat a)
	{
		f32 l = length(a);
		if(l > EPSILON)
		{
			f32 i = 1.0f / l;
			a *= i;
		}
		return a;
	}

	fn Quat
	conjugate(Quat a) 
	{
		return { -a.x, -a.y, -a.z, a.w };
	}

	fn Quat
	invert(Quat a)
	{
		return normalize(conjugate(a));
	}

	fn Quat
	euler(f32 x, f32 y, f32 z)
	{
		f32 xr = x * DEG2RAD / 2.0f;
		f32 yr = y * DEG2RAD / 2.0f;
		f32 zr = z * DEG2RAD / 2.0f;

		f32 sx = sin(xr);
		f32 sy = sin(yr);
		f32 sz = sin(zr);
		f32 cx = cos(xr);
		f32 cy = cos(yr);
		f32 cz = cos(zr);

		Quat r = 
		{
			sx * cy * cz - cx * sy * sz,
			cx * sy * cz + sx * cy * sz,
			cx * cy * sz - sx * sy * cz,
			cx * cy * cz + sx * sy * sz
		};

		return quat::normalize(r);
	}

	fn Vec3
	to_euler(Quat q)
	{
		Vec3 v;
		f32 tolerance = 0.499f;
		f32 test = q.x * q.y + q.z * q.w;
		if(test > tolerance) // North singluarity
		{ 
			v.x = 2.0f * atan2(q.x, q.w);
			v.y = PI / 2.0f; 
			v.z = 0;
		}
		else if(test < -tolerance) // South singularity
		{ 
			v.x = -2.0f * atan2(q.x, q.w);
			v.y = -PI / 2.0f;
			v.z = 0;
		}
		else
		{
			f32 sqx = q.x * q.x;
		    f32 sqy = q.y * q.y;
		    f32 sqz = q.z * q.z;
		    
		    v.x = atan2(2.0f * q.y * q.w - 2.0f * q.x * q.z , 1.0f - 2.0f * sqy - 2.0f * sqz);
			v.y = asin(2.0f * test);
			v.z = atan2(2.0f * q.x * q.w - 2.0f * q.y * q.z , 1.0f - 2.0f * sqx - 2.0f * sqz);
		}
		return v;
	}

	fn Quat
	angle_axis(f32 angle, Vec3 axis)
	{
		f32 half_angle = 0.5f * (angle * DEG2RAD);
		f32 sin_half_angle = sin(half_angle);
		axis *= sin_half_angle;
		
		return
		{
			cos(half_angle),
			axis.x,
			axis.y,
			axis.z
		};
	}

	fn AngleAxis
	to_angle_axis(Quat q)
	{
		f32 l = sqr_length(q);
		if(l > 0.0f)
		{
			f32 i = 1.0f / sqrt(l);
			f32 angle = 2.0f * acos(q.w) * RAD2DEG;
			Vec3 axis = { q.x, q.y, q.z };

			return { angle, axis * i };
		}
		else
		{
			return { 0, { 1,0,0 } };
		}
	}
}