namespace draw
{
	enum BlendMode
	{
		DEFAULT = 0,
		LINEAR = 1
	};

	struct Context
	{
		renderer::State* render_state;
		RGBA8 color;
		Mat3 transform;
		BlendMode blend_mode;
	};

	global_var Context* ctx;

	fn Context* 
	new_context(memory::Block* storage, renderer::State* render_state)
	{
		auto c = alloc_struct(storage, Context);
		c->render_state = render_state;
		c->color = { 0,0,0,255 };
		c->transform = mat3::identity;
		c->blend_mode = BlendMode::DEFAULT;
		return c;
	}

	fn void 
	set_context(Context* context)
	{
		draw::ctx = context;
	}

	fn void 
	clear(RGBA8 color)
	{
		auto buffer = draw::ctx->render_state->texture;
		memsize count = buffer->width * buffer->height;
		auto pixels = (RGBA8*)buffer->pixels;
		for(memsize i = 0; i < count; ++i)
		{
			*pixels++ = color;
		}
	}

	fn void 
	set_color(RGBA8 color)
	{
		draw::ctx->color = color;
	}


	fn RGBA8
	linear_blend(RGBA8 src, RGBA8 dest)
	{
		f32 dest_r = (f32)dest.r;
		f32 dest_g = (f32)dest.g;
		f32 dest_b = (f32)dest.b;

		f32 src_r = (f32)src.r;
		f32 src_g = (f32)src.g;
		f32 src_b = (f32)src.b;
		f32 t = src.a / 255.0f;

		//(1-t) * B + t * A;
		u8 r = (u8)((1.0f - t) * dest.r + t * src.r);
		u8 g = (u8)((1.0f - t) * dest.g + t * src.g);
		u8 b = (u8)((1.0f - t) * dest.b + t * src.b);

		return {r, g, b, dest.a};
	}

	fn void
	fill_pixel(RGBA8* pixel)
	{
		switch(draw::ctx->blend_mode)
		{
		case BlendMode::DEFAULT:
			*pixel = draw::ctx->color;
		break;	
		case BlendMode::LINEAR:
			*pixel = linear_blend(draw::ctx->color, *pixel);
		break;
		}
	}

	fn void
	fill_points(Vec2* points, i32 count, Mat3 m)
	{
		auto buffer = draw::ctx->render_state->texture;

		// these bounds can be calculated elsewhere and cached
		auto min = mat3::mul_point(m, points[0]);
		auto max = mat3::mul_point(m, points[0]);
		for(i32 i = 1; i < count; ++i)
		{
			auto p = mat3::mul_point(m, points[i]);
			if(p.x < min.x) min.x = p.x;
			if(p.x > max.x) max.x = p.x;
			if(p.y < min.y) min.y = p.y;
			if(p.y > max.y) max.y = p.y;
		}

		Vec2i vmin = vec2i::from_vec2f(min);
		Vec2i vmax = vec2i::from_vec2f(max);		

		// Clip to screen
		if(vmin.x < 0) vmin.x = 0;
		if(vmin.y < 0) vmin.y = 0;
		if(vmax.x > buffer->width)  vmax.x = buffer->width;
		if(vmax.y > buffer->height) vmax.y = buffer->height;

		auto x_offset = vmin.x * buffer->bytes_per_pixel;
		auto y_offset = vmin.y * buffer->pitch;
		auto row = (u8*)(buffer->pixels) + x_offset + y_offset;

		for(i32 y = vmin.y; y < vmax.y; ++y)
		{
			auto pixel = (RGBA8*)row;
			f32 fy = (f32)y;

			for(i32 x = vmin.x; x < vmax.x; ++x)
			{
				Vec2 pos = {(f32)x,fy};

				Vec2 a = mat3::mul_point(m, points[0]);
				b32 test = true;
				for(i32 z = 1; z < count; ++z)
				{
					Vec2 b = mat3::mul_point(m, points[z]);
					if(vec2::dot(pos - a, b-a) < 0)
					{
						test = false;
						break;
					}
					a = b;
				}
				if(test)
				{
					*pixel = linear_blend(draw::ctx->color, *pixel);
				}
				
				++pixel;
			}

			row += buffer->pitch;
		}
	}

	fn void
	fill_rect(f32 x, f32 y, f32 w, f32 h)
	{
		auto buffer = draw::ctx->render_state->texture;

		auto min_x = math::round_to_i32(x);
		auto min_y = math::round_to_i32(y);
		auto max_x = min_x + math::round_to_i32(w);
		auto max_y = min_y + math::round_to_i32(h);

		// Clip to screen
		if(min_x < 0) min_x = 0;
		if(min_y < 0) min_y = 0;
		if(max_x > buffer->width)  max_x = buffer->width;
		if(max_y > buffer->height) max_y = buffer->height;

		auto x_offset = min_x * buffer->bytes_per_pixel;
		auto y_offset = min_y * buffer->pitch;
		auto row = (u8*)(buffer->pixels) + x_offset + y_offset;

		for(int py = min_y; py < max_y; ++py)
		{
			auto pixel = (RGBA8*)row;
			for(int px = min_x; px < max_x; ++px)
			{
				fill_pixel(pixel);
				++pixel;
			}

			row += buffer->pitch;
		}
	}

	fn void
	fill_rect(Rect r)
	{
		fill_rect(r.x, r.y, r.width, r.height);
	}

	
	/*
	fn void
	line(Vec2 start, Vec2 end)
	{
		auto buffer = draw::ctx->render_state->pixel_buffer;
		auto color = draw::ctx->color;

		auto x0 = math::round_to_i32(start.x);
		auto x1 = math::round_to_i32(end.x);
		auto y0 = math::round_to_i32(start.y);
		auto y1 = math::round_to_i32(end.y);

		x0 = math::clamp(x0, 0, buffer->width);
		x1 = math::clamp(x1, 0, buffer->width);
		y0 = math::clamp(y0, 0, buffer->height);
		y1 = math::clamp(y1, 0, buffer->height);
		
		i32 dx = abs(x1 - x0);		
		i32 sx = -1;
		if(x0 < x1)
		{
			sx = 1;
		}
   		i32 dy = -abs(y1 - y0);
   		i32 sy = -1;
   		if(y0 < y1)
   		{
   			sy = 1;
   		} 
   		i32 err = dx + dy;
   		i32 e2;
 
		for(;;)
		{
			auto index = x0 + (y0 * buffer->width); 
			auto pixel = (RGBA8*)(buffer->pixels) + index;
			*pixel = linear_blend(color, *pixel);

			if (x0 == x1 && y0 == y1) break;
			e2 = 2 * err;
			if (e2 >= dy) 
			{ 
				err += dy; 
				x0 += sx; 
			}
			if (e2 <= dx) 
			{ 
				err += dx; 
				y0 += sy; 
			}
		}
	}

	fn void
	wire_points(Vec2* points, i32 count, Mat3 m)
	{
		auto buffer = draw::ctx->render_state->pixel_buffer;
		auto color = draw::ctx->color;

		Vec2 a = mat3::mul_point(m, points[0]);
		for(i32 i = 1; i < count; ++i)
		{
			Vec2 b = mat3::mul_point(m, points[i]);
			draw::line(a,b);
			a = b;
		}
	}
	*/

	/*
	internal void
	wire_rect(PixelBuffer* buffer, Rect rect, RGBA rgba)
	{
		line(buffer, {rect.min.x, rect.max.y}, {rect.max.x, rect.max.y}, rgba);
		line(buffer, {rect.max.x, rect.max.y}, {rect.max.x, rect.min.y}, rgba);
		line(buffer, {rect.max.x, rect.min.y}, {rect.min.x, rect.min.y}, rgba);
		line(buffer, {rect.min.x, rect.min.y}, {rect.min.x, rect.max.y}, rgba);
	}

	internal void
	circle(PixelBuffer* buffer, Vec2 pos, f32 radius, RGBA rgba)
	{
		auto color = to_u32(rgba);

		i32 ix = round_to_i32(pos.x);
		i32 iy = round_to_i32(pos.y);
		i32 r = round_to_i32(radius);
		i32 x = -r;
		i32 y = 0;
		i32 err = 2 - 2 * r; 

		while(x < 0)
		{
			pixel(buffer, ix - x, iy + y, color); 
      		pixel(buffer, ix - y, iy - x, color); 
      		pixel(buffer, ix + x, iy - y, color); 
      		pixel(buffer, ix + y, iy + x, color);

      		r = err;
      		if(r <= y) 
      		{
      			err += ++y * 2 + 1;
      		}           
      		if(r > x || err > y)
      		{
      			err += ++x * 2 + 1;
      		}
		}
    }
    */
}