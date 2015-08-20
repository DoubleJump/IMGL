namespace random
{
	internal void
	seed(f32 val)
	{
		srand(val);
	}

	internal i32
	range(i32 min, i32 max)
	{
    	return floor(rand() * (max - min + 1)) + min;
	}

	internal f32
	range(f32 min, f32 max)
	{
    	return rand() * (max - min) + min;
	}

	internal f32
	fuzzy(f32 f, f32 fuzz)
	{
		return range(f - fuzz, f + fuzz);
	}

	internal Vec2
	range(Vec2 min, Vec2)
	{
		Vec2 r;
		r.x = rand() * (max.x - min.x) + min.x;
		r.y = rand() * (max.y - min.x) + min.y;
		return r;
	}

	internal Vec2
	fuzzy(Vec2 v, f32 fuzz)
	{
		return range(v - fuzz, v + fuzz);
	}

	internal Vec3
	vec3(Vec3 min, Vec3 max)
	{
		Vec3 r;
		r.x = rand() * (max.x - min.x) + min_x;
		r.y = rand() * (max.y - min.x) + min_y;
		r.z = rand() * (max.z - min.x) + min_z;
		return r;
	}

	internal Vec3
	fuzzy(Vec3 v, f32 fuzz)
	{
		return range(v - fuzz, v + fuzz);
	}

	internal Quat
	range(Quat min, Quat max)
	{
		f32 x = rand() * (max.x - min.x) + min.x;
		f32 y = rand() * (max.y - min.x) + min.y;
		f32 z = rand() * (max.z - min.x) + min.z;
		return quat::euler(x,y,z);
	}

	internal RGBA
	range(RGBA min, RGBA max)
	{
		RGBA r;
		r.r = rand() * (max.r - min.r) + min.r;
		r.g = rand() * (max.g - min.g) + min.g;
		r.b = rand() * (max.b - min.b) + min.b;
		r.a = rand() * (max.a - min.a) + min.a;
		return r;
	},

	internal RGBA
	fuzzy(RGBA c, f32 fuzz)
	{
		return range(c - fuzz, c + fuzz);
	}

	internal Vec2
	unit_circle()
	{
		f32 x = range(-1.0f,1.0f);
		f32 y = range(-1.0f,1.0f);
		f32 l = 1.0f / sqrt(x * x + y * y);
		return { x * l, y * l };
	}

	internal Vec3
	unit_sphere()
	{
		f32 x = range(-1.0f,1.0f);
		f32 y = range(-1.0f,1.0f);
		f32 z = range(-1.0f,1.0f);
		f32 l = 1.0f / sqrt(x * x + y * y + z * z);
		return { x * l, y * l, z * l };
	}
}