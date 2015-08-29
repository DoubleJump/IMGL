struct Entity
{
	i32 id;
	b32 active;
	b32 dirty;
	Vec2 position;
	Vec2 scale;
	f32 rotation;
	Mat3 local_matrix;
	Mat3 world_matrix;
	i32 parent;
	i32 children[8];
};

struct Scene
{
	Entity* entities;
	i32 num_entities;
};


namespace scene
{
	global_var Scene* ctx;

	fn void
	set_context(Scene& context)
	{
		scene::ctx = &context;
	}

	fn Entity&
	new_entity()
	{
		i32 num_entities = scene::ctx->num_entities;
		auto& e = scene::ctx->entities[num_entities];
		e.id = num_entities;
		e.parent = 0;
		e.active = true;
		e.dirty = true;
		e.position = {0,0};
		e.scale = {1,1};
		e.rotation = 0;

		for(i32 i = 0; i < 8; ++i)
		{
			e.children[i] = -1;
		}

		scene::ctx->entities[e.id] = e;
		scene::ctx->num_entities++;
		return e;
	}

	fn void 
	set_transform(Entity& e, Vec2 position, Vec2 scale, f32 rotation)
	{
		e.position = position;
		e.scale = scale;
		e.rotation = rotation;
		e.dirty = true;
	}

	fn void
	set_parent(Scene& scene, Entity& entity, Entity& parent)
	{
		if(parent.id == entity.parent) return;
		if(entity.parent != 0) //remove from children
		{
			auto& current_parent = scene.entities[entity.parent];
			for(i32 i = 0; i < 8; ++i)
			{
				auto& child = current_parent.children[i];
				if(child == entity.id)
				{
					child = -1;
					break;
				}
			}
		}

		entity.parent = parent.id;
		for(i32 i = 0; i < 8; ++i)
		{
			auto& child = parent.children[i];
			if(child == -1)
			{
				child = entity.id;
				break;
			}
		}
	}

	fn void
	clear_parent(Entity& e)
	{
		if(e.parent == 0) return;
		auto& current_parent = scene::ctx->entities[e.parent];
		for(i32 i = 0; i < 8; ++i)
		{
			auto& child = current_parent.children[i];
			if(child == e.id)
			{
				child = -1;
				break;
			}
		}
		e.parent = 0;
	}

	fn void
	update_entity(Entity& e)
	{
		if(!e.active) return;
		auto& parent = scene::ctx->entities[e.parent];

		if(e.dirty)
		{
			e.local_matrix = mat3::compose(e.position, e.scale, e.rotation);
		}
		e.world_matrix = parent.world_matrix * e.local_matrix;
		
		for(i32 i = 0; i < 8; ++i)
		{
			auto id = e.children[i];
			if(id != -1)
			{
				auto& child = scene::ctx->entities[id];
				update_entity(child);
			}
		}
		e.dirty = false;
	}
	

	fn void
	init(memory::Block& storage, i32 count)
	{
		scene::ctx->num_entities = 0;
		scene::ctx->entities = alloc_array(storage, Entity, count);
		scene::ctx->entities[0] = scene::new_entity();
	}

	fn void
	update()
	{
		auto& root = scene::ctx->entities[0];
		if(root.dirty)
		{
			root.world_matrix = mat3::compose(root.position, root.scale, root.rotation);
		}
		for(i32 i = 1; i < scene::ctx->num_entities; ++i)
		{
			update_entity(scene::ctx->entities[i]);
		}
	}

	/*
	world_to_screen: function(r, camera, world, view)
    {
    	var wp = gb.vec3.tmp(); 
        mat4::mul_projection(wp, camera.view_projection, world);
        r[0] = ((wp[0] + 1.0) / 2.0) * view.width;
        r[1] = ((1.0 - wp[1]) / 2.0) * view.height;
    },

    screen_to_view: function(r, point, view)
    {
        r.x = point.x / view.width;
        r.y = 1.0f - (point.y / view.height);
        r.z = point.z;
        return r;
    },

    screen_to_world: function(r, camera, point, view)
    {
        Vec3 t;
        t.x = 2.0f * point.x / view.width - 1.0f;
        t.y = -2.0f * point.y / view.height + 1.0f;
        t.z = point.z;
            
        auto inverse = mat4::inverse(camera.view_projection);
        return mat4::mul_projection(inv, t);
    },
	*/
}