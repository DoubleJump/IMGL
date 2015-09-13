fn Mat4 
operator * (Mat4 a, Mat4 b)
{
	Mat4 r;

	r.m[ 0] = a[ 0] * b[0] + a[ 1] * b[4] + a[ 2] * b[ 8] + a[ 3] * b[12];
	r.m[ 1] = a[ 0] * b[1] + a[ 1] * b[5] + a[ 2] * b[ 9] + a[ 3] * b[13];
	r.m[ 2] = a[ 0] * b[2] + a[ 1] * b[6] + a[ 2] * b[10] + a[ 3] * b[14];
	r.m[ 3] = a[ 0] * b[3] + a[ 1] * b[7] + a[ 2] * b[11] + a[ 3] * b[15];
	r.m[ 4] = a[ 4] * b[0] + a[ 5] * b[4] + a[ 6] * b[ 8] + a[ 7] * b[12];
	r.m[ 5] = a[ 4] * b[1] + a[ 5] * b[5] + a[ 6] * b[ 9] + a[ 7] * b[13];
	r.m[ 6] = a[ 4] * b[2] + a[ 5] * b[6] + a[ 6] * b[10] + a[ 7] * b[14];
	r.m[ 7] = a[ 4] * b[3] + a[ 5] * b[7] + a[ 6] * b[11] + a[ 7] * b[15];	
	r.m[ 8] = a[ 8] * b[0] + a[ 9] * b[4] + a[10] * b[ 8] + a[11] * b[12];
	r.m[ 9] = a[ 8] * b[1] + a[ 9] * b[5] + a[10] * b[ 9] + a[11] * b[13];
	r.m[10] = a[ 8] * b[2] + a[ 9] * b[6] + a[10] * b[10] + a[11] * b[14];
	r.m[11] = a[ 8] * b[3] + a[ 9] * b[7] + a[10] * b[11] + a[11] * b[15];
	r.m[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[ 8] + a[15] * b[12];
	r.m[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[ 9] + a[15] * b[13];
	r.m[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
	r.m[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];

	return r;
}

fn Mat4 
operator *= (Mat4& a, Mat4 b)
{
	a = a * b;
	return a;
}

namespace mat4
{
	fn Mat4 identity = 
	{ 
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	fn f32
	determinant(Mat4 m)
	{
		f32 a0 = m[ 0] * m[ 5] - m[ 1] * m[ 4];
		f32 a1 = m[ 0] * m[ 6] - m[ 2] * m[ 4];
		f32 a2 = m[ 0] * m[ 7] - m[ 3] * m[ 4];
		f32 a3 = m[ 1] * m[ 6] - m[ 2] * m[ 5];
		f32 a4 = m[ 1] * m[ 7] - m[ 3] * m[ 5];
		f32 a5 = m[ 2] * m[ 7] - m[ 3] * m[ 6];
		f32 b0 = m[ 8] * m[13] - m[ 9] * m[12];
		f32 b1 = m[ 8] * m[14] - m[10] * m[12];
		f32 b2 = m[ 8] * m[15] - m[11] * m[12];
		f32 b3 = m[ 9] * m[14] - m[10] * m[13];
		f32 b4 = m[ 9] * m[15] - m[11] * m[13];
		f32 b5 = m[10] * m[15] - m[11] * m[14];

		return a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
	}

	fn Mat4
	transposed(Mat4 m)
	{
		Mat4 r;

		r.m[ 1] = m[ 4]; 
		r.m[ 2] = m[ 8]; 
		r.m[ 3] = m[12];
		r.m[ 4] = m[ 1]; 
		r.m[ 6] = m[ 9]; 
		r.m[ 7] = m[13];
		r.m[ 8] = m[ 2]; 
		r.m[ 9] = m[ 6]; 
		r.m[11] = m[14];
		r.m[12] = m[ 3]; 
		r.m[13] = m[ 7]; 
		r.m[14] = m[11];
		r.m[15] = m[15]; 
		
		return r;
	}

	fn Mat4
	inverse(Mat4 m)
	{
		Mat4 r;

		f32 v0 = m[ 2] * m[ 7] - m[ 6] * m[ 3];
		f32 v1 = m[ 2] * m[11] - m[10] * m[ 3];
		f32 v2 = m[ 2] * m[15] - m[14] * m[ 3];
		f32 v3 = m[ 6] * m[11] - m[10] * m[ 7];
		f32 v4 = m[ 6] * m[15] - m[14] * m[ 7];
		f32 v5 = m[10] * m[15] - m[14] * m[11];

		f32 t0 =   v5 * m[5] - v4 * m[9] + v3 * m[13];
		f32 t1 = -(v5 * m[1] - v2 * m[9] + v1 * m[13]);
		f32 t2 =   v4 * m[1] - v2 * m[5] + v0 * m[13];
		f32 t3 = -(v3 * m[1] - v1 * m[5] + v0 * m[ 9]);

		f32 inv_det = 1.0f / (t0 * m[0] + t1 * m[4] + t2 * m[8] + t3 * m[12]);

		r.m[0] = t0 * inv_det;
		r.m[1] = t1 * inv_det;
		r.m[2] = t2 * inv_det;
		r.m[3] = t3 * inv_det;

		r.m[4] = -(v5 * m[4] - v4 * m[8] + v3 * m[12]) * inv_det;
		r.m[5] =  (v5 * m[0] - v2 * m[8] + v1 * m[12]) * inv_det;
		r.m[6] = -(v4 * m[0] - v2 * m[4] + v0 * m[12]) * inv_det;
		r.m[7] =  (v3 * m[0] - v1 * m[4] + v0 * m[ 8]) * inv_det;

		v0 = m[1] * m[ 7] - m[ 5] * m[3];
		v1 = m[1] * m[11] - m[ 9] * m[3];
		v2 = m[1] * m[15] - m[13] * m[3];
		v3 = m[5] * m[11] - m[ 9] * m[7];
		v4 = m[5] * m[15] - m[13] * m[7];
		v5 = m[9] * m[15] - m[13] * m[11];

		r.m[ 8] =  (v5 * m[4] - v4 * m[8] + v3 * m[12]) * inv_det;
		r.m[ 9] = -(v5 * m[0] - v2 * m[8] + v1 * m[12]) * inv_det;
		r.m[10] =  (v4 * m[0] - v2 * m[4] + v0 * m[12]) * inv_det;
		r.m[11] = -(v3 * m[0] - v1 * m[4] + v0 * m[ 8]) * inv_det;

		v0 = m[ 6] * m[1] - m[ 2] * m[ 5];
		v1 = m[10] * m[1] - m[ 2] * m[ 9];
		v2 = m[14] * m[1] - m[ 2] * m[13];
		v3 = m[10] * m[5] - m[ 6] * m[ 9];
		v4 = m[14] * m[5] - m[ 6] * m[13];
		v5 = m[14] * m[9] - m[10] * m[13];

		r.m[12] = -(v5 * m[4] - v4 * m[8] + v3 * m[12]) * inv_det;
		r.m[13] =  (v5 * m[0] - v2 * m[8] + v1 * m[12]) * inv_det;
		r.m[14] = -(v4 * m[0] - v2 * m[4] + v0 * m[12]) * inv_det;
		r.m[15] =  (v3 * m[0] - v1 * m[4] + v0 * m[ 8]) * inv_det;

		return r;
	}

	fn Mat4
	inverse_affine(Mat4 m)
	{
		f32 t0 = m[10] * m[5] - m[ 6] * m[9];
		f32 t1 = m[ 2] * m[9] - m[10] * m[1];
		f32 t2 = m[ 6] * m[1] - m[ 2] * m[5];

		f32 idet = 1.0f / (m[0] * t0 + m[4] * t1 + m[8] * t2);

		t0 *= idet;
		t1 *= idet;
		t2 *= idet;

		f32 v0 = m[0] * idet;
		f32 v4 = m[4] * idet;
		f32 v8 = m[8] * idet;

		Mat4 r;
		r.m[ 0] = t0; 
		r.m[ 1] = t1; 
		r.m[ 2] = t2;
		r.m[ 3] = 0.0f;
		r.m[ 4] = v8 * m[ 6] - v4 * m[10];
		r.m[ 5] = v0 * m[10] - v8 * m[ 2];
		r.m[ 6] = v4 * m[ 2] - v0 * m[ 6];
		r.m[ 7] = 0.0f;
		r.m[ 8] = v4 * m[9] - v8 * m[5];
		r.m[ 9] = v8 * m[1] - v0 * m[9];
		r.m[10] = v0 * m[5] - v4 * m[1];
		r.m[11] = 0.0f;
		r.m[12] = -(r[0] * m[12] + r[4] * m[13] + r[ 8] * m[14]);
		r.m[13] = -(r[1] * m[12] + r[5] * m[13] + r[ 9] * m[14]);
		r.m[14] = -(r[2] * m[12] + r[6] * m[13] + r[10] * m[14]);		
		r.m[15] = 1.0f;

		return r;
	}

	fn void
	set_position(Mat4& m, Vec3 p)
	{
		m.m[12] = p.x; 
		m.m[13] = p.y; 
		m.m[14] = p.z;
	}

	fn Vec3
	get_position(Mat4 m)
	{
		return { m[12], m[13], m.m[14] };
	}

	fn void 
	set_scale(Mat4& m, Vec3 s)
	{
		m.m[ 0] = s.x; 
		m.m[ 5] = s.y; 
		m.m[10] = s.z;
	}

	fn Vec3
	get_scale(Mat4 m)
	{
		return { m[0], m[5], m[10] };
	}

	fn void
	set_rotation(Mat4& m, Quat q)
	{
		f32 x2 = 2.0f * q.x; 
		f32 y2 = 2.0f * q.y; 
		f32 z2 = 2.0f * q.z;
		f32 xx = q.x * x2; 
		f32 xy = q.x * y2; 
		f32 xz = q.x * z2;
		f32 yy = q.y * y2;
		f32 yz = q.y * z2;
		f32 zz = q.z * z2;
		f32 wx = q.w * x2; 
		f32 wy = q.w * y2;
		f32 wz = q.w * z2;

		m.m[ 0] = 1.0f - (yy + zz);
		m.m[ 1] = xy + wz;
		m.m[ 2] = xz - wy;
		m.m[ 3] = 0.0f;
		m.m[ 4] = xy - wz;
		m.m[ 5] = 1.0f - (xx + zz);
		m.m[ 6] = yz + wx;
		m.m[ 7] = 0.0f;
		m.m[ 8] = xz + wy;
		m.m[ 9] = yz - wx;
		m.m[10] = 1.0f - (xx + yy);
		m.m[11] = 0.0f;
		m.m[12] = 0.0f;
		m.m[13] = 0.0f;
		m.m[14] = 0.0f;
		m.m[15] = 1.0f;
	}

	fn Quat
	get_rotation(Mat4 m)
	{
		Quat q;
		f32 t;

		if(m[10] < 0.0f)
		{
			if(m[0] > m[5])
			{
				t = 1.0f + m[0] - m[5] - m[10];
				q = { t, m[1] + m[4], m[8] + m[2], m[6] - m[9] };
			}
			else
			{
				t = 1.0f - m[0] + m[5] - m[10];
				q = { m[1] + m[4], t, m[6] + m[9], m[8] - m[2] };
			}
		}
		else
		{
			if (m[0] < -m[5])
			{
				t = 1.0f - m[0] - m[5] + m[10];
				q = { m[8] + m[2], m[6] + m[9], t, m[1] - m[4] };
			}
			else
			{
				t = 1.0f + m[0] + m[5] + m[10];
				q = { m[6] - m[9], m[8] - m[2], m[1] - m[4], t };
			}
		}

		q *= 0.5f / sqrtf(t);
	}

	fn Mat4
	compose(Vec3 position, Vec3 scale, Quat rotation)
	{
		Mat4 r; 

		set_rotation(r, rotation);
		set_scale(r, scale);
		set_position(r, position);

		return r;
	}

	
	fn Vec3
	mul_point(Mat4 m, Vec3 p)
	{
		return
		{
			m[0] * p.x + m[4] * p.y + m[ 8] * p.z + m[12],
			m[1] * p.x + m[5] * p.y + m[ 9] * p.z + m[13],
			m[2] * p.x + m[6] * p.y + m[10] * p.z + m[14]
		};
	}

	fn Vec3
	mul_projection(Mat4 m, Vec3 p)
	{
		f32 d = 1.0f / (m[3] * p.x + m[7] * p.y + m[11] * p.z + m[15]);

		return
		{
			(m[0] * p.x + m[4] * p.y + m[ 8] * p.z + m[12]) * d,
			(m[1] * p.x + m[5] * p.y + m[ 9] * p.z + m[13]) * d,
			(m[2] * p.x + m[6] * p.y + m[10] * p.z + m[14]) * d
		};
	}

}