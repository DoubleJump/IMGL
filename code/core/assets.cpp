namespace asset
{
	using namespace renderer;

	struct Group
	{
		u32 num_shaders;
		u32 num_textures;
		u32 num_meshes;

		Shader* shaders;
		Texture* textures;
		Mesh* meshes;
	};

	fn Shader*
	shader(Group* group, char* name)
	{
		auto hash = math::hash(name);
		auto s = &(group->shaders[0]);
		b32 found = false;
		for(u32 i = 0; i < group->num_shaders; ++i)
		{
			if(hash == s->asset_id)
			{
				found = true;
				break;
			}
			s++;
		}
		#if DEBUG
			if(!found) 
			{
				FAIL("Could not find shader: %s \n", name);
			}
		#endif
		return s;
	}

	fn Group
	load(memory::Block* storage, char* path)
	{
		Group assets = {};

		auto asset_file = file::open(path);

		assets.num_shaders = file::read_i32(&asset_file);
		assets.num_meshes = file::read_i32(&asset_file);
		assets.num_textures = file::read_i32(&asset_file);

		assets.shaders = alloc_array(storage, Shader, assets.num_shaders);
		assets.meshes = alloc_array(storage, Mesh, assets.num_meshes);
		assets.textures = alloc_array(storage, Texture, assets.num_textures);

		auto s = &(assets.shaders[0]);
		for(u32 i = 0; i < assets.num_shaders; ++i)
		{
			auto name = file::read_string(&asset_file);

			s->asset_id = math::hash(name.data, name.length);
			s->vertex_src = file::read_string(&asset_file);
			s->fragment_src = file::read_string(&asset_file);
			s->linked = false;
			s++;
		}
		auto m = &(assets.meshes[0]);
		for(u32 i = 0; i < assets.num_meshes; ++i)
		{
			auto name = file::read_string(&asset_file);

			m->asset_id = math::hash(name.data, name.length);
			m->draw_type = mesh::DrawType::TRIANGLES;
			m->update_mode = mesh::UpdateMode::STATIC;
			m->vertex_count = file::read_i32(&asset_file);
			m->index_count = file::read_i32(&asset_file);
			m->attribute_mask = file::read_i32(&asset_file);

			mesh::alloc_array_buffers(storage, m);

			m->linked = false;
			m->dirty = true;
			m++;
		}
		/*
		Texture* t = &assets->textures[0];
		for(i32 i = 0; i < assets.num_textures; ++i)
		{
			auto name = file::read_string(&asset_file);

			t->asset_id = math::hash(name.data, name.length);
			//
			//
			t->linked = false;
			t->dirty = true;

			t++;
		}
		*/
		return assets;
	}
}