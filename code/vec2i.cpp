Vec2i 
operator - (Vec2i a)
{
	return { -a.x, -a.y };
}

Vec2i 
operator + (Vec2i a, Vec2i b)
{
	return { a.x + b.x, a.y + b.y };
}

Vec2i 
operator - (Vec2i a, Vec2i b)
{
	return { a.x - b.x, a.y - b.y };
}

Vec2i 
operator * (Vec2i a, i32 b)
{
	return { a.x * b, a.y * b };
}

Vec2i 
operator / (Vec2i a, i32 b)
{
	return { a.x / b, a.y / b };
}

Vec2i 
operator += (Vec2i& a, Vec2i b)
{
	a = a + b;
	return a;
}

Vec2i 
operator -= (Vec2i& a, Vec2i b)
{
	a = a - b;
	return a;
}

Vec2i 
operator *= (Vec2i& a, i32 b)
{
	a = a * b;
	return a;
}

Vec2i 
operator /= (Vec2i& a, i32 b)
{
	a = a / b;
	return a;
}

