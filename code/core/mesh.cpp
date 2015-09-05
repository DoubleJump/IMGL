namespace renderer
{
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
		enum VertexAttribute
		{
			POSITION = 0,
			NORMAL = 1,
			UV = 2,
			UV2 = 4,
			COLOR = 16
		};
	}

	struct Mesh
	{
		AssetID asset_id;
		
		void* memory;
		
		u32 vao;
		u32 vbo;
		u32 ibo;

		mesh::DrawType draw_type;
		mesh::UpdateMode update_mode;

		i32 vertex_count;
		i32 index_count;
		i32 attribute_mask;
					
		Vec3* positions;
		Vec3* normals;
		Vec2* uvs;
		Vec2* uvs2;
		Color* colors;
		u16* indices;

		memsize vertex_array_size;
		memsize index_array_size;

		b32 linked;
		b32 dirty;
	};

	namespace mesh
	{
		fn void 
		set_data(Mesh* m, f32* vertices, u16* indices)
		{
			ASSERT(m->vertex_array_size > 0, "Memory for mesh not allocated");
			memory::copy((void*)vertices, m->memory, m->vertex_array_size);
			memory::copy((void*)indices, (void*)m->indices, m->index_array_size);
		}

		fn void
		set_data(Mesh* m, void* data)
		{
			auto size = m->vertex_array_size + m->index_array_size;
			memory::copy(m->memory, data, size);
		}

		fn void 
		set_positions(Mesh* m, Vec3* data)
		{
			auto size = m->vertex_count * sizeof(f32) * 3;
			memory::copy(m->positions, (void*)data, size);
		}

		fn void
		set_normals(Mesh* m, Vec3* data)
		{
			auto size = m->vertex_count * sizeof(f32) * 3;
			memory::copy((void*)m->normals, (void*)data, size);
		}

		fn void
		set_uvs(Mesh* m, Vec2* data, i32 layer)
		{
			auto size = m->vertex_count * sizeof(f32) * 2;
			if(layer == 1)
				memory::copy((void*)m->uvs, (void*)data, size);
			else if(layer == 2)
				memory::copy((void*)m->uvs2, (void*)data, size);
		}

		fn void 
		set_colors(Mesh* m, Color* data)
		{
			auto size = m->vertex_count * sizeof(f32) * 4;
			memory::copy((void*)m->colors, (void*)data, size);
		}

		fn AABB
		get_bounds(Mesh* m)
		{
			Vec3 min = m->positions[0];
			Vec3 max = m->positions[0];
			auto n = m->vertex_count;
			for(i32 i = 1; i < n; ++i)
			{
				Vec3 p = m->positions[i];
				if(p.x < min.x) min.x = p.x;
				if(p.y < min.y) min.y = p.y;
				if(p.z < min.z) min.z = p.z;

				if(p.x > max.x) max.x = p.x;
				if(p.y > max.y) max.y = p.y;
				if(p.z > max.z) max.z = p.z;
			}
			return { min, max };
		}

		fn Mesh*
		alloc_array_buffers(Mesh* m, memory::Block* storage)
		{
			m->positions = 0;
			m->normals 	 = 0;
			m->uvs 		 = 0;
			m->uvs2 	 = 0;
			m->colors 	 = 0;
			m->indices	 = 0;

			memsize position = 0;
			memsize normal = 0;
			memsize uv = 0;
			memsize uv2 = 0;
			memsize color = 0;
			memsize vertex_bytes = 0;

			auto mask = m->attribute_mask;

			if(FLAG_SET(mask, VertexAttribute::POSITION))
			{
				position = vertex_bytes;
				vertex_bytes += sizeof(f32) * 3 * m->vertex_count;
			}
			if(FLAG_SET(mask, VertexAttribute::NORMAL))
			{
				normal = vertex_bytes;
				vertex_bytes += sizeof(f32) * 3 * m->vertex_count;
			}
			if(FLAG_SET(mask, VertexAttribute::UV))
			{
				uv = vertex_bytes;
				vertex_bytes += sizeof(f32) * 2 * m->vertex_count;
			}
			if(FLAG_SET(mask, VertexAttribute::UV2))
			{
				uv2 = vertex_bytes;
				vertex_bytes += sizeof(f32) * 2 * m->vertex_count;
			}
			if(FLAG_SET(mask, VertexAttribute::COLOR))
			{
				color = vertex_bytes;
				vertex_bytes += sizeof(f32) * 4 * m->vertex_count;
			}

			memsize index_bytes = sizeof(u16) * m->index_count;

			m->memory = memory::push_bytes(storage, vertex_bytes + index_bytes);
			m->vertex_array_size = vertex_bytes;
			m->index_array_size = index_bytes;

			auto vertex_memory = (u8*)m->memory;
			m->positions = (Vec3*)(vertex_memory + position);
			m->normals 	 = (Vec3*)(vertex_memory + normal);
			m->uvs 		 = (Vec2*)(vertex_memory + uv);
			if(uv2) m->uvs2 = (Vec2*)(vertex_memory + uv2);
			m->colors 	 = (Color*)(vertex_memory + color);
			m->indices	 = (u16*)(vertex_memory + vertex_bytes);

			return m;
		}

		fn Mesh*
		quad(memory::Block* storage, f32 width, f32 height, f32 depth)
		{
			auto w = width * 0.5f;
			auto h = height * 0.5f;
			auto d = depth * 0.5f;
			float verts[] = 
			{ 
				-w, -h, d,
				 w, -h, d,
				-w,  h, d,
				 w,  h, d,

				0.0f, 1.0f,
				1.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 0.0f
			};

			u16 indices[] = 
			{
				0,1,3,0,3,2
			};

			auto mask = VertexAttribute::POSITION |
						VertexAttribute::UV;

			auto m = push_struct(storage, Mesh);
			m->vertex_count = 4;
			m->index_count = 6;
			m->draw_type = mesh::DrawType::TRIANGLES;
			m->update_mode = mesh::UpdateMode::STATIC;
			m->attribute_mask = VertexAttribute::POSITION |
								VertexAttribute::UV;

			mesh::alloc_array_buffers(m, storage);
			mesh::set_data(m, &verts[0], &indices[0]);

			m->linked = false;
			m->dirty = true;

			return m;
		}
	}
}