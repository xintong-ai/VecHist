#ifndef GL_TEXTURE_CUBE_H
#define GL_TEXTURE_CUBE_H
#include <QOpenGLFunctions>
class GLTextureCube //: public GLTexture
{
	GLuint m_texture;
public:
	GLTextureCube(int size);
	~GLTextureCube();
//	explicit GLTextureCube(const QStringList& fileNames, int size = 0);
	explicit GLTextureCube(int size, int datatype);
	//void load(int size, int face, QRgb *data);
	void load(float* data, int size);
	virtual void bind();
	virtual void unbind();
};

#endif