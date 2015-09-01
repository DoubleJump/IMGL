#include <GL/GL.h>
#include "../win32/opengl_extensions.cpp"

using namespace renderer;

namespace opengl
{
	// SHADER

	fn void
	link(Shader* s, const char* vert_source, const char* frag_source)
	{
		GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER); 
		
		glShaderSource(vert_shader, 1, (GLchar**)&vert_source, 0);
		glCompileShader(vert_shader);
		GLint status;
		glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);

		ASSERT(status != GL_FALSE && "Vertex shader compile error");

		GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag_shader, 1, (GLchar**)&frag_source, 0);
		glCompileShader(frag_shader);
		glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);

		ASSERT(status != GL_FALSE && "Fragment shader compile error");

		GLuint program = glCreateProgram();
		glAttachShader(program, vert_shader);
		glAttachShader(program, frag_shader);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &status);
	
		ASSERT(status != GL_FALSE && "Shader link error");
	
		glDetachShader(program, vert_shader);
		glDetachShader(program, frag_shader);
		glDeleteShader(vert_shader);
		glDeleteShader(frag_shader);

		s->id = program;
		//shader->num_attributes = glGetProgramParameter(program, GL_ACTIVE_ATTRIBUTES);
	    //shader->num_uniforms = glGetProgramParameter(program, GL_ACTIVE_UNIFORMS); 
	}

	fn void
	use(Shader* s)
	{
		glUseProgram(s->id);
	}


	

	// RENDER TARGET

	fn void
	set_render_target_attachment(Target* rt, target::Type tt, Texture* t)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rt->frame_buffer);
		glBindTexture(GL_TEXTURE_2D, t->id);

		switch(tt)
		{
		case target::Type::COLOR:
		{
			rt->color = t;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t->id, 0);
			break;
		}
		case target::Type::DEPTH:
		{
			rt->depth = t;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, t->id, 0);
			break;
		}
		}

		GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		ASSERT(error == GL_FRAMEBUFFER_COMPLETE && "Invalid frame buffer");
	}

	fn void
	link(Target* rt)
	{
		glGenFramebuffers(1, &rt->frame_buffer);
	}

	fn void
	use(Target* rt, b32 clear)
	{
		if(!rt)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			
			//set_viewport(_t.view);
			if(clear)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, rt->frame_buffer);
			if(rt->depth)
			{
				glEnable(GL_DEPTH_TEST);
			}
			if(rt->render_buffer)
			{
				glBindRenderbuffer(GL_RENDERBUFFER, rt->render_buffer);
			}

			//_t.set_viewport(rt.bounds);
			if(clear)
			{
			}
		}
	}

	fn void
	set_viewport(Rect view)
	{
		glViewport((GLsizei)view.x, (GLsizei)view.y, (GLsizei)view.width, (GLsizei)view.height);
	}


	
	// TEXTURE

	fn GLuint
	get_filter_mode(texture::Filter filter)
	{
		switch(filter)
		{
			case texture::Filter::LINEAR:
				return GL_LINEAR;
			case texture::Filter::NEAREST:
				return GL_NEAREST;
			case texture::Filter::MIP_LINEAR:
				return GL_LINEAR_MIPMAP_LINEAR;
			case texture::Filter::MIP_NEAREST:
				return GL_NEAREST_MIPMAP_NEAREST;			
			default: return 0;
		}
	}

	fn GLuint
	get_wrap_mode(texture::WrapMode mode)
	{
		switch(mode)
		{
			case texture::WrapMode::CLAMP:
				return GL_CLAMP_TO_EDGE;
			case texture::WrapMode::REPEAT:
				return GL_REPEAT;
			case texture::WrapMode::MIRROR:
				return GL_MIRRORED_REPEAT;			
			default: return 0;
		}
	}

	fn void
	set_sampler(Sampler s)
	{
		auto up_filter = get_filter_mode(s.up_filter);
		auto down_filter = get_filter_mode(s.up_filter);
		auto wrap_x = get_wrap_mode(s.wrap_x);
		auto wrap_y = get_wrap_mode(s.wrap_y);

		ASSERT(up_filter != 0);
		ASSERT(down_filter != 0);
		ASSERT(wrap_x != 0);
		ASSERT(wrap_y != 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, s.min_mip_level);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,  s.max_mip_level);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, up_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, down_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 	  wrap_x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 	  wrap_y);
	}

	fn GLuint
	get_texture_format(texture::Format format)
	{
		switch(format)
		{
			case texture::Format::RGB:
				return GL_RGB;
			case texture::Format::RGBA:
				return GL_RGBA;
			default: return 0;
		}
	}

	fn void
	update(Texture* t)
	{
		auto format = get_texture_format(t->format);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->width, t->height, 0, format, GL_UNSIGNED_BYTE, t->pixels);
		
		auto down_filter = t->sampler.down_filter;
		if(down_filter != texture::Filter::LINEAR && down_filter != texture::Filter::NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	fn void
	update(Texture* t, Rect r)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)r.x, (GLint)r.y, (GLint)r.width, (GLint)r.height, get_texture_format(t->format), GL_UNSIGNED_BYTE, t->pixels);
	}

	fn void
	link(Texture* t)
	{
		glGenTextures(1, &t->id);
		glBindTexture(GL_TEXTURE_2D, t->id);

		update(t);
		set_sampler(t->sampler);
	}

	fn void
	use(Texture* t, i32 slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, t->id);
	}

	// MESH

	fn void 
	link(Mesh* m)
	{
		glGenVertexArrays(1, &m->vao);
		glGenBuffers(1, &m->vbo);
		glGenBuffers(1, &m->ibo);

		ASSERT(m->vao != 0);
		ASSERT(m->vbo != 0);
		ASSERT(m->ibo != 0);
	}

	fn GLuint 
	get_mesh_draw_type(Mesh* m)
	{
		switch(m->draw_type)
		{
			case mesh::DrawType::TRIANGLES:
			return GL_TRIANGLES;
			case mesh::DrawType::LINES:
			return GL_LINES;
			default: return 0;
		}
	}

	fn GLuint 
	get_mesh_update_mode(Mesh* m)
	{
		switch(m->update_mode)
		{
			case mesh::UpdateMode::STATIC:
			return GL_STATIC_DRAW;
			case mesh::UpdateMode::DYNAMIC:
			return GL_DYNAMIC_DRAW;
			case mesh::UpdateMode::STREAM:
			return GL_STREAM_DRAW;
			default: return 0;
		}
	}

	fn void 
	update(Mesh* m)
	{
		memsize pos_bytes 	 = sizeof(Vec3)  * m->vertex_count;
		memsize uv_bytes 	 = sizeof(Vec2)  * m->vertex_count;
		memsize col_bytes 	 = sizeof(Color) * m->vertex_count;
		memsize index_bytes  = sizeof(u16)   * m->index_count;
		memsize vertex_bytes = pos_bytes + uv_bytes + col_bytes;
		memsize offset = 0;

		auto update_mode = get_mesh_update_mode(m);

		glBindVertexArray(m->vao);

		glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
		glBufferData(GL_ARRAY_BUFFER, vertex_bytes, m->memory, update_mode);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)offset);
		glEnableVertexAttribArray(0);
		offset += pos_bytes;
		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const void*)offset);
		glEnableVertexAttribArray(1);
		offset += uv_bytes;

		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, mesh->uvs2);
		//glEnableVertexAttribArray(2);
		//offset += uv_bytes;

		//glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, 0, (const void*)offset);
		//glEnableVertexAttribArray(3);
		//offset += col_bytes;

		//glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, 0, mesh->normals);
		//glEnableVertexAttribArray(4);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_bytes, m->indices, update_mode);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	fn void
	draw_mesh_elements(Mesh* m)
	{
		auto draw_type = get_mesh_draw_type(m);
		glBindVertexArray(m->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo);
		glDrawElements(draw_type, m->index_count, GL_UNSIGNED_SHORT, 0);
	}


	fn HGLRC
	create_context(memory::Block* storage, State* state, HDC device_context)
	{
		PIXELFORMATDESCRIPTOR pfd = {};

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.cStencilBits = 8;

		auto pixel_format = ChoosePixelFormat(device_context, &pfd); 
		ASSERT(pixel_format && "Could not choose pixel format");
		
		if(!SetPixelFormat(device_context, pixel_format, &pfd))
		{
			ASSERT(!"Could not set pixel format");
		}

		// windows only returns the old opengl 1.2 context
		auto gl11 = wglCreateContext(device_context); 
		ASSERT(gl11 && "Could not create opengl 1.1 context");
		
		if(!wglMakeCurrent(device_context, gl11))
		{
			ASSERT(!"Could not set temp context");
		}

		opengl::load_extensions();
		
		
		int major_version = 3; int minor_version = 3;
		int major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major); //only works for 3.x and up
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		if(major <= major_version)
		{
			if(major < major_version)
			{
				ASSERT(!"Incorrect opengl major version");
			}

			if(minor < minor_version)
			{
				ASSERT(!"Incorrect opengl minor version");
			}
		}
		
		int attributes[] = 
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		    WGL_CONTEXT_MINOR_VERSION_ARB, minor, 
		    WGL_CONTEXT_FLAGS_ARB, 
		    0,0
		};

		HGLRC gl = wglCreateContextAttribsARB(device_context, 0, attributes);
		ASSERT(gl && "Could not create ARB context");
		

		// remove the context we had to create initially
		wglMakeCurrent(0,0); 
		wglDeleteContext(gl11);

		// bind the up to date opengl context
		if(!wglMakeCurrent(device_context, gl))
		{
			ASSERT(!"Could not make device")
		}
		
		// Depth
		
		/*
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		//glDepthMask(GL_TRUE);
		glClearDepth(1.0f);

		// Stencil
		//glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glStencilMask(0xFF);

		// Face Culling
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		// Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		*/
		
		// MESH DATA

		
		Vec3 verts[] = 
		{ 
			{ -1.0f, -1.0f, 0.0f },
			{  1.0f, -1.0f, 0.0f },
			{ -1.0f,  1.0f, 0.0f },
			{  1.0f,  1.0f, 0.0f }
		};
		Vec2 uvs[] = 
		{
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f }
		};
		u16 indices[] = 
		{
			0,1,3,0,3,2
		};

		state->mesh = alloc_struct(storage, Mesh);
		state->mesh->draw_type = mesh::DrawType::TRIANGLES;
		state->mesh->update_mode = mesh::UpdateMode::STATIC;
		state->mesh->vertex_count = ARRAY_COUNT(verts);
		state->mesh->index_count = ARRAY_COUNT(indices);

		memsize pos_bytes = sizeof(Vec3) * state->mesh->vertex_count;
		memsize uv_bytes = sizeof(Vec2) * state->mesh->vertex_count;
		//memsize col_bytes = sizeof(Color) * state->mesh->vertex_count;
		memsize index_bytes = sizeof(u16) * state->mesh->index_count;
		memsize vertex_bytes = pos_bytes + uv_bytes;// + col_bytes;

		state->mesh->memory = memory::alloc(storage, vertex_bytes);
		u8* offset = (u8*)state->mesh->memory;

		// POSITIONS 

		state->mesh->positions = (Vec3*)offset;
		memory::copy(verts, state->mesh->positions, pos_bytes);
		offset += pos_bytes;

		// UV1

		state->mesh->uvs = (Vec2*)offset;
		memory::copy(uvs, state->mesh->uvs, uv_bytes);
		offset += uv_bytes;
		
		// UV2
		
		// COLOR
		/*
		mesh->colors = (app::Color*)offset;
		app::mem_copy(colors, mesh->colors, col_bytes);
		offset += col_bytes;
		*/
				
		// NORMALS
		
		// INDICES

		state->mesh->indices = (u16*)offset;
		memory::copy(indices, state->mesh->indices, index_bytes);

		// BIND MESH

		link(state->mesh);
		update(state->mesh);

		// FRAME BUFFER TEXTURE

		state->render_target = alloc_struct(storage, Target);
		auto color_target = texture::create_pixel_buffer(storage, (i32)state->view.width, (i32)state->view.height);

		link(color_target);
		link(state->render_target);
		set_render_target_attachment(state->render_target, target::Type::COLOR, color_target);

		// FRAME BUFFER SHADER
		state->render_target->shader = shader::create(storage);
		{
			auto vert_file = DEBUG_read_file(state->thread, "..\\data\\frame.vert");
			auto vert_source = (const char*)vert_file.contents;

			auto frag_file = DEBUG_read_file(state->thread, "..\\data\\frame.frag");
			auto frag_source = (const char*)frag_file.contents;
			link(state->render_target->shader, vert_source, frag_source);
		}

		set_viewport(state->view);
		//glClearColor(0.05f, 0.1f, 0.2f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

			
		return gl;
	}

	fn void
	reset_gl_state()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	fn void
	render(State* state, HDC device_context)
	{
		use(state->render_target->shader);
		use(state->render_target->color, 0);
		update(state->render_target->color, state->view);

		draw_mesh_elements(state->mesh);

		reset_gl_state();
		
		SwapBuffers(device_context);
	}

	/*

	fn void
	quit(HDC device_context, HGLRC gl_context)
	{
		wglMakeCurrent(device_context, 0); 
		wglDeleteContext(gl_context); 
	}
	*/
}