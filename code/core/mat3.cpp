fn Mat3
operator * (Mat3 a, Mat3 b)
{
	Mat3 r;

	r.m[0] = a[0] * b[0] + a[1] * b[3] + a[2] * b[6];
	r.m[1] = a[0] * b[1] + a[1] * b[4] + a[2] * b[7];
	r.m[2] = a[0] * b[2] + a[1] * b[5] + a[2] * b[8];

	r.m[3] = a[3] * b[0] + a[4] * b[3] + a[5] * b[6];
	r.m[4] = a[3] * b[1] + a[4] * b[4] + a[5] * b[7];
	r.m[5] = a[3] * b[2] + a[4] * b[5] + a[5] * b[8];
	
	r.m[6] = a[6] * b[0] + a[7] * b[3] + a[8] * b[6];
	r.m[7] = a[6] * b[1] + a[7] * b[4] + a[8] * b[7];
	r.m[8] = a[6] * b[2] + a[7] * b[5] + a[8] * b[8];

	return r;
}

fn Mat3
operator *= (Mat3& a, Mat3 b)
{
	a = a * b;
	return a;
}

namespace mat3
{
	fn Mat3 identity = 
	{ 
		1,0,0,
		0,1,0,
		0,0,1
	};

	fn Mat3
	from_mat4(Mat4 m)
	{
		Mat3 r;
		r.m[0] = m[0]; 
		r.m[1] = m[1]; 
		r.m[2] = m[2];
		r.m[3] = m[4]; 
		r.m[4] = m[5]; 
		r.m[5] = m[6];
		r.m[6] = m[8]; 
		r.m[7] = m[9]; 
		r.m[8] = m[10];
		return r;
	}

	fn f32
	determinant(Mat3 m)
	{
		return m[0] * (m[4] * m[8] - m[5] * m[7]) -
          	   m[1] * (m[3] * m[8] - m[5] * m[6]) +
          	   m[2] * (m[3] * m[7] - m[4] * m[6]);
	}

	fn Mat3
	inverse(Mat3 m)
	{
		Mat3 r;
   
	    r.m[0] = m[4] * m[8] - m[5] * m[7];
	    r.m[1] = m[2] * m[7] - m[1] * m[8];
	    r.m[2] = m[1] * m[5] - m[2] * m[4];
	    r.m[3] = m[5] * m[6] - m[3] * m[8];
	    r.m[4] = m[0] * m[8] - m[2] * m[6];
	    r.m[5] = m[2] * m[3] - m[0] * m[5];
	    r.m[6] = m[3] * m[7] - m[4] * m[6];
	    r.m[7] = m[1] * m[6] - m[0] * m[7];
	    r.m[8] = m[0] * m[4] - m[1] * m[3];

	    f32 det = m[0] * r[0] + m[1] * r[3] + m[2] * r[6];
	    if(abs(det) <= EPSILON)
	    {
	        return identity;
	    }

	    f32 inv_det = 1.0f / det;
	    r.m[0] *= inv_det;
	    r.m[1] *= inv_det;
	    r.m[2] *= inv_det;
	    r.m[3] *= inv_det;
	    r.m[4] *= inv_det;
	    r.m[5] *= inv_det;
	    r.m[6] *= inv_det;
	    r.m[7] *= inv_det;
	    r.m[8] *= inv_det;

	    return r;
	}

	fn Mat3
	transpose(Mat3 m)
	{
		Mat3 r;
		r.m[1] = m[3]; 
		r.m[2] = m[6]; 
		r.m[3] = m[1];
		r.m[5] = m[7]; 
		r.m[6] = m[2]; 
		r.m[7] = m[5];
		r.m[8] = m[0];
		return r;
	}

	fn Mat3
	set_position(Mat3& m, f32 x, f32 y)
	{
		m.m[2] = x;
		m.m[5] = y;
	}

	fn Vec2
	get_position(Mat3 m)
	{
		return { m[2], m[5] };
	}

	fn Mat3
	set_rotation(Mat3& m, Quat q)
	{
		f32 x2 = 2 * m[0]; 
		f32 y2 = 2 * m[1]; 
		f32 z2 = 2 * m[2];
		f32 xx = m[0] * x2; 
		f32 xy = m[0] * y2; 
		f32 xz = m[0] * z2;
		f32 yy = m[1] * y2;
		f32 yz = m[1] * z2;
		f32 zz = m[2] * z2;
		f32 wx = m[3] * x2; 
		f32 wy = m[3] * y2;
		f32 wz = m[3] * z2;

		m.m[0] = 1 - (yy + zz);
		m.m[1] = xy + wz;
		m.m[2] = xz - wy;
		m.m[3] = xy - wz;
		m.m[4] = 1 - (xx + zz);
		m.m[5] = yz + wx;
		m.m[6] = xz + wy;
		m.m[7] = yz - wx;
		m.m[8] = 1 - (xx + yy);
	}

	fn Mat3
	set_rotation(Mat3& m, f32 r)
	{
		f32 theta = r * DEG2RAD;
		f32 st = sin(theta);
		f32 ct = cos(theta);

		m.m[0] = ct;
		m.m[1] = st;
		m.m[3] = -st;
		m.m[4] = ct;
	}

	fn Mat3
	scale(Mat3& m, f32 sx, f32 sy)
	{
		m.m[0] *= sx;
		m.m[4] *= sy;
	}

	fn Mat3
	compose(f32 x, f32 y, f32 sx, f32 sy, f32 rot)
	{
		f32 theta = rot * DEG2RAD;
		f32 st = sin(theta);
		f32 ct = cos(theta);

		Mat3 r;
		r.m[0] = ct * sx;
		r.m[1] = st * sy;
		r.m[2] = x;

		r.m[3] = -st * sx;
		r.m[4] = ct * sy;
		r.m[5] = y;

		r.m[6] = 0.0f;
		r.m[7] = 0.0f;
		r.m[8] = 1.0f;
		return r;
	}

	fn Mat3
	compose(Vec2 p, Vec2 s, f32 rot)
	{
		f32 theta = rot * DEG2RAD;
		f32 st = sin(theta);
		f32 ct = cos(theta);

		Mat3 r;
		r.m[0] = ct * s.x;
		r.m[1] = st * s.y;
		r.m[2] = p.x;

		r.m[3] = -st * s.x;
		r.m[4] = ct * s.y;
		r.m[5] = p.y;

		r.m[6] = 0.0f;
		r.m[7] = 0.0f;
		r.m[8] = 1.0f;
		return r;
	}

	fn Vec2
	mul_point(Mat3 m, Vec2 p)
	{
		return { m[0] * p.x + m[1] * p.y + m[2], m[3] * p.x + m[4] * p.y + m[5]};
	}
}