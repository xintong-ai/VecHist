#ifndef GL_TEXTURE_CUBE_H
#define GL_TEXTURE_CUBE_H
#include <QOpenGLFunctions>


class GLTexture
{
public:
	GLTexture();
	virtual ~GLTexture();
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual bool failed() const { return m_failed; }
protected:
	GLuint m_texture;
	bool m_failed;
};

class GLTextureCube: public GLTexture
{
public:
	GLTextureCube(int size);
	~GLTextureCube();
	explicit GLTextureCube(int size, int datatype);
	void load(float* data, int size);
	virtual void bind();
	virtual void unbind();
};

class GLTexture2D : public GLTexture
{
public:
	GLTexture2D(int width, int height);
	void load(int width, int height, float *data);
	virtual void bind();
	virtual void unbind();
};

#endif