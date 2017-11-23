#if !_GLES20LIB_H_
#define _GLES20LIB_H_ 1
#include <OpenGLES/ES2/gl.h>
#include "PMatrix.h"

extern "C" int CreatePngTexture(char* filename);
typedef struct {
    float Position[3];
    float Color[4];
    float TexCoord[2]; // New
} Vertex;

class GLES20Lib 
{
public:
    int _screen_width;
    int _screen_height;
    GLuint _glprograme;
    GLuint _tex_test;

    static const GLbyte _vShaderStr[];
    
    static const GLbyte _fShaderStr[];
    //void SetScreenSize(int width, int height);
    //void Flush2D();
    void InitGLES20();
    GLuint LoadShader(const char *shaderSrc, GLenum type);
    static void glClearColor(float a, float r, float g, float b);
    GLuint GetCurrentPrograme();
    void UsedCurrentPrograme();

    void RenderTest();
    int m_a_positionHandle;
    int m_a_colorHandle;
    int m_u_mvpHandle;
    int m_u_EnableTextureHandle;
    int m_a_TexCoordHandle;
    int m_s_TexHandle;
    int m_s_AlphaHanle;
    void gles20Enable(int code);
    void gles20Disable(int code);

};

#endif
