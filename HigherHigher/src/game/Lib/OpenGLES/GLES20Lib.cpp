//#ifndef MAC_OS
#include "pch.h"
#include "GLES20Lib.h"
#include <OpenGLES/ES2/gl.h>
#include "../../GameCore/CGame.h"
//#include "Matrix4x4.h"
const GLbyte GLES20Lib::_vShaderStr[] = ""
"attribute vec4 a_position;                 \n"
"attribute vec4 a_color;                    \n"
"attribute vec2 a_texCoord;                 \n"
"uniform mat4 u_mvpMatrix;                  \n"
"varying vec4 v_color;                      \n"
"varying vec2 v_texCoord;                   \n"
"void main()                                \n"
"{                                          \n"
"   gl_Position = u_mvpMatrix * a_position; \n"
"   v_color = a_color;                      \n"
"   v_texCoord = a_texCoord;                \n"
"}                                          \n";

const GLbyte GLES20Lib::_fShaderStr[] = ""
"precision mediump float;               \n"
"varying vec4 v_color;                  \n"
"varying float v_EnableTexture;         \n"
"varying vec2 v_texCoord;               \n"
"uniform sampler2D s_texture;           \n"
"uniform float s_alpha;                 \n"
"uniform float u_EnableTexture;         \n"
"void main()                            \n"
"{                                      \n"
"       gl_FragColor = (u_EnableTexture>0.0)?v_color*texture2D(s_texture, v_texCoord):v_color; \n"
"}                                      \n";
void GLES20Lib::UsedCurrentPrograme()
{
    glUseProgram(_glprograme);
}
GLuint GLES20Lib::GetCurrentPrograme()
{
    return _glprograme;
}
GLuint GLES20Lib::LoadShader(const char *shaderSrc, GLenum type)
{
    const unsigned int shader = glCreateShader(type);
	if (shader == 0)
		return 0;
    
	glShaderSource(shader, 1, (const GLchar**)&shaderSrc, NULL);
	glCompileShader(shader);
    
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		char errorMsg[2048];
		glGetShaderInfoLog(shader, sizeof(errorMsg), NULL, errorMsg);
		printf("Compile error: %s\n", errorMsg);
		glDeleteShader(shader);
		return 0;
	}
    
    return shader;

}


void GLES20Lib::glClearColor(float a, float r, float g, float b)
{
    ::glClearColor(r,g,b,a);
}
void GLES20Lib::InitGLES20()
{
        
    // Load the vertex/fragment shaders
    GLuint vertexShader = LoadShader((const char*)_vShaderStr,GL_VERTEX_SHADER);
    GLuint fragmentShader = LoadShader((const char*)_fShaderStr,GL_FRAGMENT_SHADER);
    //printf("vertexShader = %d",vertexShader);
    // Create the program object
    _glprograme = glCreateProgram();
    //printf("\n_glprograme  =%d",_glprograme);
    if(_glprograme == 0){
        return;
    }
    glAttachShader(_glprograme, vertexShader);
    glAttachShader(_glprograme, fragmentShader);
    // Link the program
    glLinkProgram(_glprograme);
    // Check the link status
    GLint linked;
    glGetProgramiv(_glprograme, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        glDeleteProgram(_glprograme);
        return;
    }
    m_a_positionHandle = glGetAttribLocation(_glprograme, "a_position");
	m_a_colorHandle = glGetAttribLocation(_glprograme, "a_color");
	m_a_TexCoordHandle = glGetAttribLocation(_glprograme, "a_texCoord");
	m_u_mvpHandle = glGetUniformLocation(_glprograme, "u_mvpMatrix");
    
    m_u_EnableTextureHandle = glGetUniformLocation(_glprograme, "u_EnableTexture");;
    m_s_TexHandle = glGetUniformLocation(_glprograme, "s_texture");
    m_s_AlphaHanle =  glGetUniformLocation(_glprograme,"s_alpha");
    printf("\n m_a_positionHandle  =%d \nm_a_colorHandle = %d \nm_u_mvpHandle = %d, \nm_u_EnableTextureHandle = %d \nm_s_TexHandle = %d\nm_a_TexCoordHandle=%d\nu_alpha=%d",m_a_positionHandle,m_a_colorHandle,m_u_mvpHandle,m_u_EnableTextureHandle,m_s_TexHandle,m_a_TexCoordHandle,m_s_AlphaHanle);
    
    //_tex_test = CreatePngTexture("tile_floor.png");
    printf("\n_tex_test = %d",_tex_test);
    
    
    
    //config opengl
	
	//enable smooth shading
	//::glShadeModel(GL_SMOOTH);
	//set clear color value
	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//set clear depth buffer value
	::glClearDepthf(1.0f);

    
	
	::glDepthMask(GL_FALSE);
	::glDisable(GL_DEPTH_TEST);
	
#ifndef QT_CREATOR
	::glDisable(GL_ALPHA_TEST);
#endif
	::glEnable(GL_ALPHA_TEST);
	::glAlphaFunc(GL_GREATER, 0.0f);
	
	::glDisable(GL_CULL_FACE);
	
    
#ifdef USE_BLEND_ALPHA
	::glEnable(GL_BLEND);
#endif
    


}
void GLES20Lib::RenderTest()
{
    
    
}

void GLES20Lib::gles20Enable(int code)
{
    switch (code) {
        case GL_TEXTURE_2D:
            glUniform1f ( m_u_EnableTextureHandle, 1.0f );
            break;
            
        default:
            glEnable(code);
            break;
    }
}
void GLES20Lib::gles20Disable(int code)
{
    switch (code) {
        case GL_TEXTURE_2D:
            glUniform1f ( m_u_EnableTextureHandle, 0.0f );
            break;
            
        default:
            glDisable(code);
            break;
    }
}