namespace renderer
{
	namespace shader
	{
		enum DataType
		{
			INT  = 0,
			F32  = 1,
			VEC2 = 2,
			VEC3 = 3,
			VEC4 = 4,
			MAT3 = 5,
			MAT4 = 6,
			TEXTURE = 7,
		};

		/*
		struct Attribute
		{
			u32 name;
			u32 location;
			i32 size;
			i32 offset;
			DataType type;
			b32 dirty;
		};
		*/

		struct Uniform
		{
			u32 name;
			u32 location;
			DataType type;
			b32 dirty;
			void* value;
		};
	}

	struct Shader
	{
		AssetID asset_id;
		string vertex_src;
		string fragment_src;

		u32 id;
		i32 num_uniforms;
		//i32 num_attributes;

		shader::Uniform* uniforms;
		u8* uniform_data;

		b32 linked;
	};

	namespace shader
	{
		fn Uniform*
		get_uniform(Shader* s, char* name)
		{
			auto name_hash = math::hash(name);
			auto uniform = &s->uniforms[0];
			b32 found = false;
			auto n = s->num_uniforms;
			for(i32 i = 0; i < n; ++i)
			{
				if(name_hash == uniform->name)
				{
					found = true;
					break;
				}
				uniform++;
			}
			ASSERT(found, ("Could not find uniform: %s \n", name));
			return uniform;
		}

		fn void 
		set_value(Shader* s, char* name, f32 value)
		{
			auto uniform = get_uniform(s, name);
			ASSERT(uniform->type == DataType::F32, ("Uniform %s is not a f32", name));
			*((f32*)uniform->value) = value;
			uniform->dirty = true;
		}
	}
}