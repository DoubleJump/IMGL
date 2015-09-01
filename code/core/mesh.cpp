namespace mesh
{
	enum DrawType
	{
		TRIANGLES = 0,
		LINES = 1
	};
	enum UpdateMode
	{
		STATIC = 0,
		DYNAMIC = 1,
		STREAM = 2
	};
}

namespace renderer
{
	struct Mesh
	{
		void* memory;
		
		u32 id;
		u32 vao;
		u32 vbo;
		u32 ibo;

		mesh::DrawType draw_type;
		mesh::UpdateMode update_mode;

		i32 vertex_count;
		i32 index_count;
					
		Vec3* positions;
		Vec2* uvs;
		Vec2* uvs2;
		Color* colors;
		Vec3* normals;
		u16* indices;

		b32 dirty;
	};

	/*
	fn Mesh*
	new_mesh(i32 vertex_count, i32 index_count, i32 attributes)
	{
		auto mesh = alloc_struct(storage, Mesh);
		mesh->vertex_count = ARRAY_COUNT(verts);
		mesh->index_count = ARRAY_COUNT(indices);

		memsize pos_bytes = sizeof(Vec3) * mesh->vertex_count;
		memsize uv_bytes = sizeof(Vec2) * mesh->vertex_count;
		//memsize col_bytes = sizeof(Color) * state->mesh->vertex_count;
		memsize index_bytes = sizeof(u16) * mesh->index_count;
		memsize vertex_bytes = pos_bytes + uv_bytes;// + col_bytes;

		state.mesh->memory = memory::alloc(storage, vertex_bytes);
		u8* offset = (u8*)mesh->memory;

		// POSITIONS 

		mesh->positions = (Vec3*)offset;
		memory::copy(verts, mesh->positions, pos_bytes);
		offset += pos_bytes;

		// UV1

		mesh->uvs = (Vec2*)offset;
		memory::copy(uvs, state.mesh->uvs, uv_bytes);
		offset += uv_bytes;
		
		// UV2
		
		// COLOR
		mesh->colors = (app::Color*)offset;
		app::mem_copy(colors, mesh->colors, col_bytes);
		offset += col_bytes;
				
		// NORMALS
		
		// INDICES

		mesh->indices = (u16*)offset;
		memory::copy(indices, mesh->indices, index_bytes);
	}
	*/
}
