#include "glext.h"
#include "wglext.h"

namespace opengl
{
	#define GL_FUNC(type, name) name = (PFN##type##PROC)wglGetProcAddress(#name);

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

	// SHADER
	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLDELETESHADERPROC glDeleteShader;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLGETSHADERIVPROC glGetShaderiv;

	// PROGRAM
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
	PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;


	// TEXTURE
	PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
	//PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;

	// OTHER
	PFNGLBUFFERSUBDATAPROC glBufferSubData;

	fn void
	load_extensions()
	{
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
		GL_FUNC(GLBINDRENDERBUFFER, glBindRenderbuffer);

		//TEXTURE
		GL_FUNC(GLGENERATEMIPMAP, glGenerateMipmap);
		//GL_FUNC(GLTEXSUBIMAGE2D, glTexSubImage2D);

		//OTHER
		GL_FUNC(GLBUFFERSUBDATA, glBufferSubData);
	}
}