#include <GL/GL.h>
#include "../win32/opengl_extensions.cpp"

using namespace renderer;

namespace opengl
{
	// SHADER

	fn void
	link(memory::Block* storage, Shader* s)
	{
		GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER); 
		glShaderSource(vert_shader, 1, (GLchar**)&(s->vertex_src.data), 0);
		glCompileShader(vert_shader);

		#if DEBUG
		{
			GLint status;
			glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
			if(status == GL_FALSE)
			{
				GLint log_length = 0;
				glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &log_length);
				auto error_log = push_array(storage, GLchar, log_length);
				glGetShaderInfoLog(vert_shader, log_length, &log_length, error_log);
				printf(error_log);
				FAIL("Vertex shader compile error");
			}
		}
		#endif

		GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag_shader, 1, (GLchar**)&(s->fragment_src.data), 0);
		glCompileShader(frag_shader);

		#if DEBUG
		{
			GLint status;
			glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
			if(status == GL_FALSE)
			{
				GLint log_length = 0;
				glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &log_length);
				auto error_log = push_array(storage, GLchar, log_length);
				glGetShaderInfoLog(frag_shader, log_length, &log_length, error_log);
				printf(error_log);
				FAIL("Fragment shader compile error");
			}
		}
		#endif

		GLuint program = glCreateProgram();
		glAttachShader(program, vert_shader);
		glAttachShader(program, frag_shader);
		glLinkProgram(program);

		#if DEBUG
		{
			GLint status;
			glGetProgramiv(program, GL_LINK_STATUS, &status);
			if(status == GL_FALSE)
			{
				GLint log_length = 0;
				glGetProgramiv(frag_shader, GL_INFO_LOG_LENGTH, &log_length);
				auto error_log = push_array(storage, GLchar, log_length);
				glGetProgramInfoLog(program, log_length, &log_length, error_log);
				printf(error_log);
				FAIL("Shader link error");
			}
		}
		#endif
	
		glDetachShader(program, vert_shader);
		glDetachShader(program, frag_shader);
		glDeleteShader(vert_shader);
		glDeleteShader(frag_shader);

		s->id = program;


		// Loop through the shader and build an array of uniforms that caches all the lookups

		//glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &s->num_attributes);
	    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &s->num_uniforms);

	    //printf("Shader Attributes: %i \n", s->num_attributes);  
	    //printf("Shader Uniforms: %i \n", s->num_uniforms);

	    //s->attributes = push_array(storage, shader::Attribute, s->num_attributes);
	    s->uniforms = push_array(storage, shader::Uniform, s->num_uniforms);

	    char name[256];
	    GLsizei name_len;
		GLint size;
		GLenum type;

		shader::Uniform* uniform = &s->uniforms[0];
	    for(i32 i = 0; i < s->num_uniforms; ++i)
	    {
	    	glGetActiveUniform(program, i, 256, &name_len, &size, &type, &name[0]);

	    	uniform->name = math::hash(&name[0], name_len);
	    	uniform->location = i;
	    	uniform->dirty = false;
	    	uniform->value = push_bytes(storage, size * 4);

	    	//TODO: add the rest of the data types
	    	switch(type)
	    	{
	    		case GL_FLOAT: uniform->type = shader::DataType::F32; break;
	    		case GL_FLOAT_VEC2: uniform->type = shader::DataType::VEC2; break;	
	    		case GL_FLOAT_VEC3: uniform->type = shader::DataType::VEC3; break; 
	    		case GL_FLOAT_VEC4: uniform->type = shader::DataType::VEC4; break;
	    		case GL_SAMPLER_2D: uniform->type = shader::DataType::TEXTURE; break;	
	    	}

	    	uniform++;
	    	//puts(name);
	    }

	    s->linked = true;
	}

	fn void
	use(Shader* s)
	{
		ASSERT(s->id != 0 && s->linked, "Shader being used is not linked");
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
		ASSERT(error == GL_FRAMEBUFFER_COMPLETE, "Invalid frame buffer");
	}

	fn void
	link(Target* rt)
	{
		glGenFramebuffers(1, &rt->frame_buffer);
		rt->linked = true;
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
			ASSERT(rt->frame_buffer != 0 && rt->linked, "Render target is not linked");
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
			/*
			if(clear)
			{
			}
			*/
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

		ASSERT(up_filter != 0, "Up filter not set");
		ASSERT(down_filter != 0, "Down filter not set");
		ASSERT(wrap_x != 0, "Wrap x not set");
		ASSERT(wrap_y != 0, "Wrap y not set");

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
		t->dirty = false;
	}

	fn void
	update(Texture* t, Rect r)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)r.x, (GLint)r.y, (GLint)r.width, (GLint)r.height, get_texture_format(t->format), GL_UNSIGNED_BYTE, t->pixels);
		t->dirty = false;
	}

	fn void
	link(Texture* t)
	{
		glGenTextures(1, &t->id);
		glBindTexture(GL_TEXTURE_2D, t->id);

		update(t);
		set_sampler(t->sampler);

		t->linked = true;
	}

	fn void
	use(Texture* t, i32 slot)
	{
		ASSERT(t->id != 0 && t->linked, "Texture not linked");
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

		ASSERT(m->vao != 0, "Could create vertex array object");
		ASSERT(m->vbo != 0, "Could create vertex buffer object");
		ASSERT(m->ibo != 0, "Could create index buffer object");

		m->linked = true;
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
		memsize vertex_bytes = m->vertex_array_size;

		GLuint attribute_pointer = 0;
		memsize offset = 0;

		auto update_mode = get_mesh_update_mode(m);

		glBindVertexArray(m->vao);

		glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
		glBufferData(GL_ARRAY_BUFFER, m->vertex_array_size, m->memory, update_mode);

		if(FLAG_SET(m->attribute_mask, mesh::VertexAttribute::POSITION))
		{
			glVertexAttribPointer(attribute_pointer, 3, GL_FLOAT, GL_FALSE, 0, (const void*)offset);
			glEnableVertexAttribArray(attribute_pointer);
			offset += sizeof(f32) * 3 * m->vertex_count;
			attribute_pointer++;
		}
		if(FLAG_SET(m->attribute_mask, mesh::VertexAttribute::NORMAL))
		{
			glVertexAttribPointer(attribute_pointer, 3, GL_FLOAT, GL_FALSE, 0, (const void*)offset);
			glEnableVertexAttribArray(attribute_pointer);
			offset += sizeof(f32) * 3 * m->vertex_count;
			attribute_pointer++;
		}
		if(FLAG_SET(m->attribute_mask, mesh::VertexAttribute::UV))
		{
			glVertexAttribPointer(attribute_pointer, 2, GL_FLOAT, GL_FALSE, 0, (const void*)offset);
			glEnableVertexAttribArray(attribute_pointer);
			offset += sizeof(f32) * 2 * m->vertex_count;
			attribute_pointer++;
		}
		if(FLAG_SET(m->attribute_mask, mesh::VertexAttribute::UV2))
		{
			glVertexAttribPointer(attribute_pointer, 2, GL_FLOAT, GL_FALSE, 0, (const void*)offset);
			glEnableVertexAttribArray(attribute_pointer);
			offset += sizeof(f32) * 2 * m->vertex_count;
			attribute_pointer++;
		}
		if(FLAG_SET(m->attribute_mask, mesh::VertexAttribute::COLOR))
		{
			glVertexAttribPointer(attribute_pointer, 4, GL_FLOAT, GL_TRUE, 0, (const void*)offset);
			glEnableVertexAttribArray(attribute_pointer);
			offset += sizeof(f32) * 4 * m->vertex_count;
			attribute_pointer++;
		}
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->index_array_size, m->indices, update_mode);
		
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
	create_context(State* state, HDC device_context)
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
		ASSERT(pixel_format, "Could not choose pixel format");
		
		if(!SetPixelFormat(device_context, pixel_format, &pfd))
		{
			FAIL("Could not set pixel format");
		}

		// windows only returns the old opengl 1.2 context
		auto gl11 = wglCreateContext(device_context); 
		ASSERT(gl11, "Could not create opengl 1.1 context");
		
		if(!wglMakeCurrent(device_context, gl11))
		{
			FAIL("Could not set opengl 1.1 context");
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
				FAIL("Incorrect opengl major version");
			}

			if(minor < minor_version)
			{
				FAIL("Incorrect opengl minor version");
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
		ASSERT(gl, "Could not create ARB context");
		
		// remove the context we had to create initially
		wglMakeCurrent(0,0); 
		wglDeleteContext(gl11);

		// bind the up to date opengl context
		if(!wglMakeCurrent(device_context, gl))
		{
			FAIL("Could not make device");
		}

		set_viewport(state->view);

		return gl;

		
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
		
		// SCREEN MESH
		//state->mesh = mesh::quad(storage, 2.0f, 2.0f, 0.0f);
		//link(state->mesh);
		//update(state->mesh);

		// FRAME BUFFER TEXTURE

		/*
		state->render_target = push_struct(storage, Target);
		auto color_target = texture::create_pixel_buffer(storage, (i32)state->view.width, (i32)state->view.height);

		link(color_target);
		link(state->render_target);
		set_render_target_attachment(state->render_target, target::Type::COLOR, color_target);
		*/

		// FRAME BUFFER SHADER
		/*
		state->render_target->shader = push_struct(storage, Shader);
		{
			auto vert_file = file::open(state->thread, "..\\data\\frame.vert");
			auto vert_source = (const char*)vert_file.contents;

			auto frag_file = file::open(state->thread, "..\\data\\frame.frag");
			auto frag_source = (const char*)frag_file.contents;
			link(storage, state->render_target->shader, vert_source, frag_source);
		}
		*/

		//set_viewport(state->view);
		//glClearColor(0.05f, 0.1f, 0.2f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

			
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
		if(!state->shader->linked) link(state->storage, state->shader);
		if(!state->screen_mesh->linked) link(state->screen_mesh);
		if(state->screen_mesh->dirty) update(state->screen_mesh);
		
		if(!state->texture->linked) link(state->texture);
		/*
		if(!state->target->linked)
		{
			link(state->target);
			set_render_target_attachment(state->target, target::Type::COLOR, state->texture);
		}
		*/

		auto s = state->shader;
		use(s);

		auto info = &s->uniforms[0];
		for(i32 i = 0; i < s->num_uniforms; ++i)
		{
			if(info->dirty)
			{
				switch(info->type)
				{
					case shader::DataType::F32:
					{
						glUniform1f(info->location, *((f32*)info->value)); //not sure we have to cast?
						break;
					}
					default: break;
				}
				info->dirty = false;
			}
			info++;
		}

		//use(state->target->color, 0);
		//update(state->target->color, state->view);

		use(state->texture, 0);
		update(state->texture);
		draw_mesh_elements(state->screen_mesh);
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