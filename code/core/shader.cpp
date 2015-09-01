namespace renderer
{
	namespace shader
	{
		struct Attribute
		{
			u32 name;
			u32 location;
		};

		struct Uniform
		{
			u32 name;
			u32 location;
		};
	}

	struct Shader
	{
		u32 id;
		u32 num_uniforms;
		u32 num_attributes;
	};

	namespace shader
	{
		fn Shader*
		create(memory::Block* storage)
		{
			return alloc_struct(storage, Shader);
		}
	}

}
