namespace collision
{
	struct Result
	{
		b32 hit;
		Vec3 point;
		Vec3 normal;
		f32 fraction;
	};

	/*
	internal Result 
	point_line(Vec3 p, Vec3 a, Vec3 b)
	{

	}
	*/

	internal Result
	line_line(Vec3 a, Vec3 b, Vec3 c, Vec3 d)
	{
		f32 aa = b.y - a.y;
		f32 ab = a.x - b.x;
		f32 ac = aa * a.x + ab * a.y;

		f32 ba = d.y - c.y;
		f32 bb = c.x - d.x;
		f32 bc = ab * c.x + bb * c.y;

		f32 delta = aa * bb - ab * ba;
		if(delta == 0)
	    {
	    	return { false };
	    }

	    Result r;
	    r.point = { bb * ac - ab * bc,  aa * bc - ba * ac };
	    r.point /= 2.0f; 
	    r.hit = true;
	    return r;
	}

	internal Result
	point_circle(Vec3 p, Vec3 c, f32 r)
	{
		f32 rr = r * r;
		Vec3 delta = c - p;
		f32 distance = vec3::sqr_length(delta);

		if(distance < rr)
		{
			Vec3 hit_point = p + (vec3::normalize(delta) * r);
			return { true, hit_point };
		}
		return { false };
	}

	internal Result
	circle_circle(Vec3 a, f32 ra, Vec3 b, f32 rb)
	{
		Vec3 delta = b - a;
		f32 distance = vec3::sqr_length(delta);
		f32 rad_sum = math::square(ra) + math::square(rb);

		if(distance < rad_sum)
		{
			Vec3 hit_point = a + (vec3::normalize(delta) * ra);
			return { true, hit_point };
		}
		return { false };
	}

	
	internal Result
	aabb_ray(AABB a, Ray r)
	{
		f32 fx = 1.0f / r.dir.x;
		f32 fy = 1.0f / r.dir.y;
		f32 fz = 1.0f / r.dir.z;

		f32 t1 = (a.min.x - r.origin.x) * fx;
		f32 t2 = (a.max.x - r.origin.x) * fx;
		f32 t3 = (a.min.y - r.origin.y) * fy;
		f32 t4 = (a.max.y - r.origin.y) * fy;
		f32 t5 = (a.min.z - r.origin.z) * fz;
		f32 t6 = (a.max.z - r.origin.z) * fz;

		f32 tmin = math::max(math::max(math::min(t1, t2), math::min(t3, t4)), math::min(t5, t6));
		f32 tmax = math::min(math::min(math::max(t1, t2), math::max(t3, t4)), math::max(t5, t6));

		if(tmax < 0.0f || tmin > tmax)
		{
			return { false };
		}

		Result result;
		result.point = r.origin + (r.dir * tmin);
		result.fraction = tmin;
		return result;
	}

	/*
	internal Result
	point_triangle(Vec3 p, Vec3 a, Vec3 b, Vec3 c)
	{
		var A =  (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y) / 2;
		var sign = A < 0 ? -1 : 1;
		var s = (a.y * c.x - a.x * c.y + (c.y - a.y) * p.x + (a.x - c.x) * p.y) * sign;
		var t = (a.x * b.y - a.y * b.x + (a.y - b.y) * p.x + (b.x - a.x) * p.y) * sign;
		return s > 0 && t > 0 && s + t < 2 * A * sign;
	}

	
	static triangle_ray(a: Vec3, b: Vec3, c: Vec3, r: Ray, result?: HitInfo): boolean
	{
		result = result || HitInfo.tmpA;
		result.clear();

		var e0 = Vec3.tmpA.copy(b).sub(a);
		var e1 = Vec3.tmpB.copy(c).sub(a);
		Vec3.cross(e0, e1, Vec3.tmpC);
		var n = Vec3.tmpC.normalize();

		var t = -(Vec3.dot(n, r.origin) + Vec3.dot(n, a)) / Vec3.dot(n, r.dir);

		var p = Vec3.tmpA.copy(r.origin);
		var d = Vec3.tmpB.copy(r.dir).mulf(t);
		p.add(d);

		if(Intersect.point_triangle(p, a, b, c))
		{
			result.point.copy(p);
			result.t = t;
			result.normal.copy(n);
			return true;
		}
		else return false;
	}
	*/

}