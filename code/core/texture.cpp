namespace renderer
{
	namespace texture
	{
		enum Format
		{
			RGBA = 0,
			RGB = 1
		};

		enum WrapMode
		{
			CLAMP = 0,
			REPEAT = 1,
			MIRROR = 2,
		};

		enum Filter
		{
			LINEAR = 0,
			NEAREST = 1,
			MIP_LINEAR = 2,
			MIP_NEAREST = 3
		};
	}


	struct Sampler
	{
		i32 min_mip_level, max_mip_level;
		texture::Filter up_filter, down_filter;
		texture::WrapMode wrap_x, wrap_y;
	};

	struct Texture
	{
		u32 id;
		texture::Format format;
		i32 bytes_per_pixel;
		i32 width, height;
		i32 pitch;
		Sampler sampler;
		u8* pixels;
		b32 dirty;
		b32 linked;
	};

	namespace texture
	{
		fn Texture*
		create(memory::Block* storage, i32 width, i32 height, texture::Format format, Sampler sampler)
		{
			auto t = push_struct(storage, Texture);
			t->width = width;
			t->height = height; 
			t->format = format;
			switch(format)
			{
				case texture::Format::RGBA:
				{
					t->bytes_per_pixel = 4;
					break;
				}
				case texture::Format::RGB:
				{
					t->bytes_per_pixel = 3;
				}
				default: break;
			}
			t->pitch = width * t->bytes_per_pixel;
			t->sampler = sampler;
			t->pixels = 0;
			t->dirty = false;
			return t;
		}

		fn Texture*
		create(memory::Block* storage, i32 width, i32 height, texture::Format format, u8* pixels, Sampler sampler)
		{
			auto t = create(storage, width, height, format, sampler);
			t->pixels = pixels;
			t->dirty = true;
			return t;
		}

		fn Texture*
		create_pixel_buffer(memory::Block* storage, i32 width, i32 height)
		{
			Sampler s;
			s.min_mip_level = 0;
			s.max_mip_level = 0;
			s.up_filter = texture::Filter::LINEAR; 
			s.down_filter = texture::Filter::LINEAR;
			s.wrap_x = texture::WrapMode::CLAMP;
			s.wrap_y = texture::WrapMode::CLAMP;

			auto t = create(storage, width, height, texture::Format::RGBA, s);
			t->pixels = push_array(storage, u8, t->width * t->height * t->bytes_per_pixel);
			return t;
		}
	}
}