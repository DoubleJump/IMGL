#include <GL/GL.h>
#include "glext.h"
#include "wglext.h"

using namespace renderer;

namespace opengl
{
	#define GL_FUNC(type, name) name = (PFN##type##PROC)wglGetProcAddress(#name);

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

	// Shader
	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLDELETESHADERPROC glDeleteShader;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLGETSHADERIVPROC glGetShaderiv;

	// Program
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLDETACHSHADERPROC glDetachShader;
	PFNGLLINKPROGRAMPROC glLinkProgram;
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	PFNGLUNIFORM1IPROC glUniform1i;
	PFNGLUNIFORM1IVPROC glUniform1iv;
	PFNGLUNIFORM2IVPROC glUniform2iv;
	PFNGLUNIFORM3IVPROC glUniform3iv;
	PFNGLUNIFORM4IVPROC glUniform4iv;
	PFNGLUNIFORM1FPROC glUniform1f;
	PFNGLUNIFORM1FVPROC glUniform1fv;
	PFNGLUNIFORM2FVPROC glUniform2fv;
	PFNGLUNIFORM3FVPROC glUniform3fv;
	PFNGLUNIFORM4FVPROC glUniform4fv;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
	PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
	PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
	PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
	PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
	PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
	PFNGLACTIVETEXTUREPROC glActiveTexture;

	// VBO
	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

	// FBO
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
	PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
	PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;

	// TEXTURE
	//PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;

	// OTHER
	PFNGLBUFFERSUBDATAPROC glBufferSubData;

	struct State
	{
		ThreadContext* thread;
		HDC device_context;
		renderer::Mesh* mesh;
		GLuint frame_buffer;
		GLuint frame_shader;
		GLuint frame_texture;
	};


	fn GLuint
	create_shader(const char* vert_source, const char* frag_source)
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

		return program;
	}

	fn void 
	bind_mesh(renderer::Mesh* m) //try without ns and *
	{
		glGenVertexArrays(1, &m->vao);
		glGenBuffers(1, &m->vbo);
		glGenBuffers(1, &m->ibo);

		ASSERT(m->vao != 0);
		ASSERT(m->vbo != 0);
		ASSERT(m->ibo != 0);
	}

	fn void 
	update_mesh(renderer::Mesh* m)
	{
		memsize pos_bytes = sizeof(Vec3) * m->vertex_count;
		memsize uv_bytes = sizeof(Vec2) * m->vertex_count;
		memsize col_bytes = sizeof(RGBA) * m->vertex_count;
		memsize index_bytes = sizeof(u16) * m->index_count;
		memsize vertex_bytes = pos_bytes + uv_bytes + col_bytes;
		memsize offset = 0;

		glBindVertexArray(m->vao);

		glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
		glBufferData(GL_ARRAY_BUFFER, vertex_bytes, m->memory, GL_STATIC_DRAW);

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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_bytes, m->indices, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	fn HGLRC
	create_context(memory::Block& storage, State& state, PixelBuffer& pb)
	{
		PIXELFORMATDESCRIPTOR pfd = {};

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.cStencilBits = 8;

		auto pixel_format = ChoosePixelFormat(state.device_context, &pfd); 
		ASSERT(pixel_format && "Could not choose pixel format");
		
		if(!SetPixelFormat(state.device_context, pixel_format, &pfd))
		{
			ASSERT(!"Could not set pixel format");
		}

		// windows only returns the old opengl 1.2 context natively
		auto gl11 = wglCreateContext(state.device_context); 
		ASSERT(gl11 && "Could not create opengl 1.1 context");
		
		if(!wglMakeCurrent(state.device_context, gl11))
		{
			ASSERT(!"Could not set temp context");
		}


		// LOAD IN GL EXTENSION FUNCTIONS
		
		GL_FUNC(WGLCREATECONTEXTATTRIBSARB, wglCreateContextAttribsARB);

		// Program
		GL_FUNC(GLCREATEPROGRAM, glCreateProgram);
		GL_FUNC(GLDELETEPROGRAM, glDeleteProgram);
		GL_FUNC(GLUSEPROGRAM, glUseProgram);
		GL_FUNC(GLATTACHSHADER, glAttachShader);
		GL_FUNC(GLDETACHSHADER, glDetachShader);
		GL_FUNC(GLLINKPROGRAM, glLinkProgram);
		GL_FUNC(GLGETPROGRAMIV, glGetProgramiv);
		GL_FUNC(GLGETSHADERINFOLOG, glGetShaderInfoLog);
		GL_FUNC(GLGETUNIFORMLOCATION, glGetUniformLocation);
		GL_FUNC(GLUNIFORM1I, glUniform1i);
		GL_FUNC(GLUNIFORM1IV, glUniform1iv);
		GL_FUNC(GLUNIFORM2IV, glUniform2iv);
		GL_FUNC(GLUNIFORM3IV, glUniform3iv);
		GL_FUNC(GLUNIFORM4IV, glUniform4iv);
		GL_FUNC(GLUNIFORM1F, glUniform1f);
		GL_FUNC(GLUNIFORM1FV, glUniform1fv);
		GL_FUNC(GLUNIFORM2FV, glUniform2fv);
		GL_FUNC(GLUNIFORM3FV, glUniform3fv);
		GL_FUNC(GLUNIFORM4FV, glUniform4fv);
		GL_FUNC(GLUNIFORMMATRIX4FV, glUniformMatrix4fv);
		GL_FUNC(GLGETATTRIBLOCATION, glGetAttribLocation);
		GL_FUNC(GLVERTEXATTRIB1F, glVertexAttrib1f);
		GL_FUNC(GLVERTEXATTRIB1FV, glVertexAttrib1fv);
		GL_FUNC(GLVERTEXATTRIB2FV, glVertexAttrib2fv);
		GL_FUNC(GLVERTEXATTRIB3FV, glVertexAttrib3fv);
		GL_FUNC(GLVERTEXATTRIB4FV, glVertexAttrib4fv);
		GL_FUNC(GLENABLEVERTEXATTRIBARRAY, glEnableVertexAttribArray);
		GL_FUNC(GLBINDATTRIBLOCATION, glBindAttribLocation);
		GL_FUNC(GLGETACTIVEATTRIB, glGetActiveAttrib);
		GL_FUNC(GLACTIVETEXTURE, glActiveTexture);

		// Shader
		GL_FUNC(GLCREATESHADER, glCreateShader);
		GL_FUNC(GLDELETESHADER, glDeleteShader);
		GL_FUNC(GLSHADERSOURCE, glShaderSource);
		GL_FUNC(GLCOMPILESHADER, glCompileShader);
		GL_FUNC(GLGETSHADERIV, glGetShaderiv);

		// VBO
		GL_FUNC(GLGENBUFFERS, glGenBuffers);
		GL_FUNC(GLBINDBUFFER, glBindBuffer);
		GL_FUNC(GLBUFFERDATA, glBufferData);
		GL_FUNC(GLVERTEXATTRIBPOINTER, glVertexAttribPointer);
		GL_FUNC(GLGENVERTEXARRAYS, glGenVertexArrays);
		GL_FUNC(GLBINDVERTEXARRAY, glBindVertexArray);

		//FBO
		GL_FUNC(GLGENFRAMEBUFFERS, glGenFramebuffers);
		GL_FUNC(GLBINDFRAMEBUFFER, glBindFramebuffer);
		GL_FUNC(GLFRAMEBUFFERTEXTURE2D, glFramebufferTexture2D);
		GL_FUNC(GLCHECKFRAMEBUFFERSTATUS, glCheckFramebufferStatus);

		//TEXTURE
		//GL_FUNC(GLTEXSUBIMAGE2D, glTexSubImage2D);

		//OTHER
		GL_FUNC(GLBUFFERSUBDATA, glBufferSubData);
		
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

		HGLRC gl = wglCreateContextAttribsARB(state.device_context, 0, attributes);
		ASSERT(gl && "Could not create ARB context");
		

		// remove the context we had to create initially
		wglMakeCurrent(0,0); 
		wglDeleteContext(gl11);

		// bind the up to date opengl context
		if(!wglMakeCurrent(state.device_context, gl))
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
		/*
		app::Color colors[] = 
		{
			{ 1.0f, 0.0f, 0.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f },
			{ 0.25f, 0.5f, 0.75f, 1.0f }
		};
		app::Vec3 normals[] = 
		{
			{ 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f }
		};
		*/
		u16 indices[] = 
		{
			0,1,3,0,3,2
		};

		state.mesh = alloc_struct(storage, Mesh);
		state.mesh->vertex_count = ARRAY_COUNT(verts);
		state.mesh->index_count = ARRAY_COUNT(indices);

		memsize pos_bytes = sizeof(Vec3) * state.mesh->vertex_count;
		memsize uv_bytes = sizeof(Vec2) * state.mesh->vertex_count;
		//memsize col_bytes = sizeof(Color) * state->mesh->vertex_count;
		memsize index_bytes = sizeof(u16) * state.mesh->index_count;
		memsize vertex_bytes = pos_bytes + uv_bytes;// + col_bytes;

		state.mesh->memory = memory::alloc(storage, vertex_bytes);
		u8* offset = (u8*)state.mesh->memory;

		// POSITIONS 

		state.mesh->positions = (Vec3*)offset;
		memory::copy(verts, state.mesh->positions, pos_bytes);
		offset += pos_bytes;

		// UV1

		state.mesh->uvs = (Vec2*)offset;
		memory::copy(uvs, state.mesh->uvs, uv_bytes);
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

		state.mesh->indices = (u16*)offset;
		memory::copy(indices, state.mesh->indices, index_bytes);

		// BIND MESH

		bind_mesh(state.mesh);

		// UPDATE MESH

		update_mesh(state.mesh);

		/*
		int width, height, bit_depth;
   		u8* pixels = stbi_load("..\\data\\orange.png", &width, &height, &bit_depth, 0);
   		
   		ASSERT(pixels && "Could not load pixels");
		
   		app::Texture texture = {};
   		texture.width = width;
   		texture.height = height;
   		texture.bit_depth = bit_depth;
   		if(bit_depth == 4) texture.format = GL_RGBA;
   		else if(bit_depth == 3) texture.format = GL_RGB;
   		texture.up_filter = GL_NEAREST;
   		texture.down_filter = GL_NEAREST;
   		texture.min_mipmap = 0;
   		texture.max_mipmap = 0;
   		texture.wrap_x = GL_CLAMP_TO_EDGE;
   		texture.wrap_y = GL_CLAMP_TO_EDGE;
   		texture.pixels = pixels;


   		glGenTextures(1, &texture.id);
   		ASSERT(texture.id != 0);

   		glBindTexture(GL_TEXTURE_2D, texture.id);

   		glTexImage2D(GL_TEXTURE_2D, 0, texture.format, texture.width, texture.height, 0, texture.format, GL_UNSIGNED_BYTE, texture.pixels);

   		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, texture.min_mipmap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, texture.max_mipmap);
		//glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture.up_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture.down_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture.wrap_x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture.wrap_y);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(pixels);
		*/

		// FRAME BUFFER SHADER
		state.frame_shader = 0;
		{
			auto vert_file = DEBUG_read_file(state.thread, "..\\data\\frame.vert");
			auto vert_source = (const char*)vert_file.contents;

			auto frag_file = DEBUG_read_file(state.thread, "..\\data\\frame.frag");
			auto frag_source = (const char*)frag_file.contents;
			state.frame_shader = create_shader(vert_source, frag_source);
		}

		// FRAME BUFFER TEXTURE
	
		state.frame_texture;
		glGenTextures(1, &state.frame_texture);
		glBindTexture(GL_TEXTURE_2D, state.frame_texture);

   		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pb.width, pb.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pb.pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);


		// FRAME BUFFER
		glGenFramebuffers(1, &state.frame_buffer);

		glBindFramebuffer(GL_FRAMEBUFFER, state.frame_buffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, state.frame_texture, 0);

		GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		ASSERT(error == GL_FRAMEBUFFER_COMPLETE)

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	
		glViewport(0, 0, pb.width, pb.height);
		//glClearColor(0.05f, 0.1f, 0.2f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		
		// DRAW SCENE TO FRAME BUFFER
		/*
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glBindVertexArray(mesh->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

		glUseProgram(shader);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, texture.id);

		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		*/
		
		return gl;
	}

	fn void
	render(State& state, PixelBuffer& pb)
	{
		glBindVertexArray(state.mesh->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.mesh->ibo);

		glUseProgram(state.frame_shader);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, state.frame_texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pb.width, pb.height, GL_RGB, GL_UNSIGNED_BYTE, pb.pixels);
		
		glDrawElements(GL_TRIANGLES, state.mesh->index_count, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
		
		SwapBuffers(state.device_context);
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