namespace renderer
{
	struct Mesh
	{
		void* memory;
		
		u32 id;
		u32 vao;
		u32 vbo;
		u32 ibo;

		i32 draw_flags;
		i32 vertex_count;
		i32 index_count;
					
		Vec3* positions;
		Vec2* uvs;
		Vec2* uvs2;
		RGBA* colors;
		Vec3* normals;
		u16* indices;
	};

	struct Texture
	{
		u32 id;
		i32 format;
		i32 bit_depth;
		i32 up_filter, down_filter;
		i32 wrap_x, wrap_y;
		i32 min_mipmap, max_mipmap;
		u32 width, height;
		u8* pixels;
	};

	struct PixelBuffer
	{
		i32 width;
		i32 height;
		i32 pixels_per_world_unit;
		i32 bytes_per_pixel;
		i32 pitch;
		RGB8* pixels;
	};

	


	/*
	fn void
    image(PixelBuffer* buffer, Bitmap* bitmap, f32 x, f32 y)
    {
    	auto i_min_x = round_to_i32(x * buffer->pixels_per_world_unit);
		auto i_min_y = round_to_i32(y * buffer->pixels_per_world_unit);
		auto i_max_x = i_min_x + bitmap->width;
		auto i_max_y = i_min_y + bitmap->height;

		// Clip to screen
		if(i_min_x < 0) i_min_x = 0;
		if(i_min_y < 0) i_min_y = 0;
		if(i_max_x > buffer->width) i_max_x = buffer->width;
		if(i_max_y > buffer->height) i_max_y = buffer->height;
		
		// Flip y axis

		i_min_y = buffer->height - i_min_y;
		i_max_y = buffer->height - i_max_y;

		auto x_offset = i_min_x * buffer->bytes_per_pixel;
		auto y_offset = i_max_y * buffer->pitch;
		
		auto src_row = bitmap->pixels;
		auto dest_row = (u8*)(buffer->memory) + x_offset + y_offset;


		for(int y = i_max_y; y < i_min_y; ++y)
		{
			auto src = src_row;
			auto dest = (u32*)dest_row;

			for(int x = i_min_x; x < i_max_x; ++x)
			{
				// unpack rgba values from the bitmaps ui32
				f32 a  = (f32)((*src >> 24) & 0xFF) / 255.0f;
				f32 sr = (f32)((*src >> 16) & 0xFF);
				f32 sg = (f32)((*src >> 8)  & 0xFF);
				f32 sb = (f32)((*src >> 0)  & 0xFF);

				// same for the buffers current value
				f32 dr = (f32)((*dest >> 16) & 0xFF);
				f32 dg = (f32)((*dest >> 8)  & 0xFF);
				f32 db = (f32)((*dest >> 0)  & 0xFF);				

				// linear blend the bitmaps values onto the buffers value
				f32 r = (1.0f - a) * dr + a * sr;
				f32 g = (1.0f - a) * dg + a * sg;
				f32 b = (1.0f - a) * db + a * sb; 

				// convert back into a ui32
				*dest = (((u32)(r + 0.5f) << 16) | 
						 ((u32)(g + 0.5f) <<  8) | 
						 ((u32)(b + 0.5f) <<  0));

				++src;
				++dest;
			}

			src_row += bitmap->width;
			dest_row += buffer->pitch;
		}

    }
    */

    /*
	fn void
	line(PixelBuffer* buffer, Vec2 start, Vec2 end, RGBA rgba)
	{
		auto x0 = round_to_i32(start.x * buffer->pixels_per_world_unit);
		auto x1 = round_to_i32(end.x * buffer->pixels_per_world_unit);
		auto y0 = buffer->height - round_to_i32(start.y * buffer->pixels_per_world_unit);
		auto y1 = buffer->height - round_to_i32(end.y * buffer->pixels_per_world_unit);

		auto color = to_u32(rgba);

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
			pixel(buffer, x0, y0, color);

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
	wire_rect(PixelBuffer* buffer, Rect rect, RGBA rgba)
	{
		line(buffer, {rect.min.x, rect.max.y}, {rect.max.x, rect.max.y}, rgba);
		line(buffer, {rect.max.x, rect.max.y}, {rect.max.x, rect.min.y}, rgba);
		line(buffer, {rect.max.x, rect.min.y}, {rect.min.x, rect.min.y}, rgba);
		line(buffer, {rect.min.x, rect.min.y}, {rect.min.x, rect.max.y}, rgba);
	}

	fn void
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