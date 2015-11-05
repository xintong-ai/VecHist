#include "GLTextureCube.h"
#define qgl	QOpenGLContext::currentContext()->functions()

GLTexture::GLTexture()
{
	glGenTextures(1, &m_texture);
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &m_texture);
}

GLTexture2D::GLTexture2D(int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0,
		GL_LUMINANCE, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture2D::load(int width, int height, float *data)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0,
		GL_LUMINANCE, GL_FLOAT, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glEnable(GL_TEXTURE_2D);
}

void GLTexture2D::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}



GLTextureCube::GLTextureCube(int size)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

    for (int i = 0; i < 6; ++i)
		qgl->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_LUMINANCE, size, size, 0,
		GL_LUMINANCE, GL_FLOAT, 0);

	qgl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	qgl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	qgl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	qgl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	qgl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
	qgl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

//this function is to serve as initiation for floating number cube texture
//TODO:this one is not implemented yet
GLTextureCube::GLTextureCube(int size, int datatype)
{
	qgl->glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	for (int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_LUMINANCE, size, size, 0,
		GL_LUMINANCE, GL_FLOAT, 0);

	qgl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	qgl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	qgl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	qgl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	qgl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
	qgl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


void GLTextureCube::load(float* data, int size)
{
	qgl->glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	for (int i = 0; i < 6; i++) {
		// Works on x86, so probably works on all little-endian systems.
		// Does it work on big-endian systems?
		//GL_LUMINANCE will be clamped to the range [0,1], 
		//so the data has to be scaled before loading
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 4, size, size, 0,
			GL_LUMINANCE, GL_FLOAT, &data[size * size * i]);
	}

	qgl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GLTextureCube::bind()
{
	qgl->glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	qgl->glEnable(GL_TEXTURE_CUBE_MAP);
}

void GLTextureCube::unbind()
{
	qgl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	qgl->glDisable(GL_TEXTURE_CUBE_MAP);
}

GLTextureCube::~GLTextureCube()
{
}