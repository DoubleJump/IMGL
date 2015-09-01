namespace renderer
{
	namespace target
	{
		enum Type
		{
			COLOR = 0,
			DEPTH = 1,
			STENCIL = 2
		};
	}

	struct Target
	{
		u32 frame_buffer;
		u32 render_buffer;
		Texture* color;
		Texture* depth;
		Texture* stencil;
		Shader* shader;
	};
}