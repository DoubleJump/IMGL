internal Mat3
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

internal Mat3
operator *= (Mat3& a, Mat3 b)
{
	a = a * b;
	return a;
}

namespace mat3
{
	internal Mat3 identity = 
	{ 
		1,0,0,
		0,1,0,
		0,0,1
	};

	internal f32
	determinant(Mat3 m)
	{
		return m[0] * (m[4] * m[8] - m[5] * m[7]) -
          	   m[1] * (m[3] * m[8] - m[5] * m[6]) +
          	   m[2] * (m[3] * m[7] - m[4] * m[6]);
	}

	internal Mat3
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
	    if(abs(det) <= F32_EPSILON)
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

	internal Mat3
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

	internal Mat3
	set_rotation(Mat3 m, Quat q)
	{
		Mat3 r;

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

		r.m[0] = 1 - (yy + zz);
		r.m[1] = xy + wz;
		r.m[2] = xz - wy;
		r.m[3] = xy - wz;
		r.m[4] = 1 - (xx + zz);
		r.m[5] = yz + wx;
		r.m[6] = xz + wy;
		r.m[7] = yz - wx;
		r.m[8] = 1 - (xx + yy);

		return r;
	}
}