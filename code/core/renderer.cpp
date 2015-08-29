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
		RGBA8* pixels;
	};
}