namespace draw
{
	struct Context
	{
		renderer::PixelBuffer* buffer;
		RGB8 color;
		Mat3 transform;
	};

	global_var Context* ctx;

	fn Context new_context(renderer::PixelBuffer& buffer)
	{
		Context c;
		c.buffer = &buffer;
		c.color = { 0,0,0 };
		c.transform = mat3::identity;
		return c;
	}

	fn void set_context(Context& context)
	{
		draw::ctx = &context;
	}

	fn void clear(RGB8 color)
	{
		auto buffer = draw::ctx->buffer;
		memsize count = buffer->width * buffer->height;
		for(memsize i = 0; i < count; ++i)
		{
			buffer->pixels[i] = color;
		}
	}

	fn void set_color(RGB8 color)
	{
		draw::ctx->color = color;
	}

	/*
	fn void
	rect(Rect r)
	{
		auto buffer = draw::ctx->buffer;
		auto rgb = draw::ctx->color;

		auto min = rect::min(r);
		auto max = rect::max(r);
		auto i_min_x = math::round_to_i32(min.x);
		auto i_min_y = math::round_to_i32(min.y);
		auto i_max_x = math::round_to_i32(max.x);
		auto i_max_y = math::round_to_i32(max.y);

		// Clip to screen
		if(i_min_x < 0) i_min_x = 0;
		if(i_max_x < 0) i_max_x = 0;

		if(i_min_y < 0) i_min_y = 0;
		if(i_max_y < 0) i_max_y = 0;

		if(i_min_x > buffer->width)  i_min_x = buffer->width;
		if(i_min_y > buffer->height) i_min_y = buffer->height;
		if(i_max_x > buffer->width)  i_max_x = buffer->width;
		if(i_max_y > buffer->height) i_max_y = buffer->height;


		auto x_offset = i_min_x * buffer->bytes_per_pixel;
		auto y_offset = i_min_y * buffer->pitch;
		auto row = (u8*)(buffer->pixels) + x_offset + y_offset;

		for(int y = i_min_y; y < i_max_y; ++y)
		{
			auto pixel = (RGB8*)row;
			for(int x = i_min_x; x < i_max_x; ++x)
			{
				*pixel++ = rgb;
			}

			row += buffer->pitch;
		}
	}
	*/

	fn void
	rect(f32 x, f32 y, Mat3 m)
	{
		auto buffer = draw::ctx->buffer;
		auto rgb = draw::ctx->color;

		Vec2 origin = { m[2], m[5] };
		Vec2 radii = { x * 0.5f, y * 0.5f };
		
		auto a = mat3::mul_point(m, -radii);
		auto b = mat3::mul_point(m, { -radii.x, radii.y });
		auto c = mat3::mul_point(m, radii);
		auto d = mat3::mul_point(m, { radii.x, -radii.y });

		auto min_x = a.x;
		if(b.x < min_x) min_x = b.x;
		if(c.x < min_x) min_x = c.x;
		if(d.x < min_x) min_x = d.x;

		auto max_x = a.x;	
		if(b.x > max_x) max_x = b.x;
		if(c.x > max_x) max_x = c.x;
		if(d.x > max_x) max_x = d.x;

		auto min_y = a.y;
		if(b.y < min_y) min_y = b.y;
		if(c.y < min_y) min_y = c.y;
		if(d.y < min_y) min_y = d.y;

		auto max_y = a.y;	
		if(b.y > max_y) max_y = b.y;
		if(c.y > max_y) max_y = c.y;
		if(d.y > max_y) max_y = d.y;

		Vec2i vmin = vec2i::from_vec2f({min_x, min_y});
		Vec2i vmax = vec2i::from_vec2f({max_x, max_y});		

		/*
		auto min_x = a.x;
		auto min_y = a.y;
		auto max_x = a.x;
		auto max_y = a.y;
		if(c.x < min_x) min_x = c.x;
		if(c.x > max_x) max_x = c.x; 
		if(c.y < min_y) min_y = c.y; 
		if(c.y > min_y) min_y = c.y; 

		Vec2i vmin = vec2i::from_vec2f({min_x, min_y});
		Vec2i vmax = vec2i::from_vec2f({max_x, max_y});	
		*/

		//Vec2 x_axis = { m[0], m[3] };
		//Vec2 y_axis = { m[1], m[4] };

		/*
		auto i_min_x = math::round_to_i32(min.x);
		auto i_min_y = math::round_to_i32(min.y);
		auto i_max_x = math::round_to_i32(max.x);
		auto i_max_y = math::round_to_i32(max.y);
		*/

		// Clip to screen
		/*
		if(i_min_x < 0) i_min_x = 0;
		if(i_max_x < 0) i_max_x = 0;

		if(i_min_y < 0) i_min_y = 0;
		if(i_max_y < 0) i_max_y = 0;

		if(i_min_x > buffer->width)  i_min_x = buffer->width;
		if(i_min_y > buffer->height) i_min_y = buffer->height;
		if(i_max_x > buffer->width)  i_max_x = buffer->width;
		if(i_max_y > buffer->height) i_max_y = buffer->height;
		*/


		auto x_offset = vmin.x * buffer->bytes_per_pixel;
		auto y_offset = vmin.y * buffer->pitch;
		auto row = (u8*)(buffer->pixels) + x_offset + y_offset;

		for(int y = vmin.y; y < vmax.y; ++y)
		{
			auto pixel = (RGB8*)row;
			for(int x = vmin.x; x < vmax.x; ++x)
			{
				Vec2 pos = {(f32)x - vmin.x, (f32)y - vmin.y};
				f32 edge0 = vec2::dot(pos, {0,-1});
				f32 edge1 = vec2::dot(pos, {0,1});
				f32 edge2 = vec2::dot(pos, {0,0});
				f32 edge3 = vec2::dot(pos, {1,0});

				/*
				if((edge0 < 0) && (edge1 < 0) && (edge2 < 0) && (edge3 < 0))
				{
					*pixel = rgb;
				}
				*/
				if((edge0 < 0) && (edge1 < 0))
				{
					*pixel = rgb;
				}
				//*pixel = rgb;
				++pixel;
			}

			row += buffer->pitch;
		}
	}
}