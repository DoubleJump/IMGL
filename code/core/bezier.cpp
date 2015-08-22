namespace bezier
{
	global_var Bezier linear = {{0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}};
	global_var Bezier ping_pong = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};

	fn f32
	evaluate(Bezier b, f32 t)
	{
		f32 u = 1.0f - t;
		f32 tt = t * t;
		f32 uu = u * u;
		f32 uuu = uu * u;
		f32 ttt = tt * t;

		Vec2 r = b.a * uuu;
		r += b.b * 3.0f * uu * t; 
		r += b.c * 3.0f * u * tt; 
		r += b.d * ttt; 

		return r.y;
	}
}