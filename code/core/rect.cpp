namespace rect
{
	internal f32
	area(Rect r)
	{
		return r.width * r.height;
	}

	internal Vec2
	min(Rect r)
	{
		return { r.x - (r.width * 0.5f), r.y - (r.height * 0.5f) };
	}

	internal Vec2
	max(Rect r)
	{
		return { r.x + (r.width * 0.5f), r.y + (r.height * 0.5f) };
	}

	internal Vec2
	radii(Rect r)
	{
		return { r.width * 0.5f , r.height * 0.5f };
	}

	internal Rect
	combine(Rect a, Rect b)
	{
		Rect r;
		auto min = vec2::min(rect::min(a), rect::min(b));
		auto max = vec2::max(rect::max(a), rect::max(b));
		r.width = max.x - min.x;
		r.height = max.y - min.y;
		r.x = min.x + (r.width * 0.5f);
		r.y = min.y + (r.height * 0.5f);
	}
}