namespace random
{
	fn void
	seed(u32 val)
	{
		srand(val);
	}

	fn i32
	range(i32 min, i32 max)
	{
		f32 r = rand() * (f32)(min - max + 1);
    	return math::floor_to_i32(r) + min;
	}

	fn f32
	range(f32 min, f32 max)
	{
    	return rand() * (max - min) + min;
	}

	fn Vec2
	range(Vec2 min, Vec2 max)
	{
		Vec2 r;
		r.x = rand() * (max.x - min.x) + min.x;
		r.y = rand() * (max.y - min.x) + min.y;
		return r;
	}

	fn Vec3
	range(Vec3 min, Vec3 max)
	{
		Vec3 r;
		r.x = rand() * (max.x - min.x) + min.x;
		r.y = rand() * (max.y - min.x) + min.y;
		r.z = rand() * (max.z - min.x) + min.z;
		return r;
	}

	fn Color
	range(Color min, Color max)
	{
		Color r;
		r.r = rand() * (max.r - min.r) + min.r;
		r.g = rand() * (max.g - min.g) + min.g;
		r.b = rand() * (max.b - min.b) + min.b;
		r.a = rand() * (max.a - min.a) + min.a;
		return r;
	}

	fn Quat
	range(Quat min, Quat max)
	{
		f32 x = rand() * (max.x - min.x) + min.x;
		f32 y = rand() * (max.y - min.x) + min.y;
		f32 z = rand() * (max.z - min.x) + min.z;
		return quat::euler(x,y,z);
	}


	fn f32
	fuzzy(f32 f, f32 fuzz)
	{
		return range(f - fuzz, f + fuzz);
	}

	fn Vec2
	fuzzy(Vec2 v, f32 fuzz)
	{
		return range(v - fuzz, v + fuzz);
	}

	fn Vec3
	fuzzy(Vec3 v, f32 fuzz)
	{
		return range(v - fuzz, v + fuzz);
	}

	fn Color
	fuzzy(Color c, f32 fuzz)
	{
		return range(c - fuzz, c + fuzz);
	}


	fn Vec2
	unit_circle()
	{
		f32 x = range(-1.0f,1.0f);
		f32 y = range(-1.0f,1.0f);
		f32 l = 1.0f / sqrtf(x * x + y * y);
		return { x * l, y * l };
	}

	fn Vec3
	unit_sphere()
	{
		f32 x = range(-1.0f,1.0f);
		f32 y = range(-1.0f,1.0f);
		f32 z = range(-1.0f,1.0f);
		f32 l = 1.0f / sqrtf(x * x + y * y + z * z);
		return { x * l, y * l, z * l };
	}
}