#include "GLTextureCube.h"
#define qgl	QOpenGLContext::currentContext()->functions()

GLTextureCube::GLTextureCube(int size)
{
	glGenTextures(1, &m_texture);
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
////by Tong
//GLTextureCube::GLTextureCube(float *data, int size)
//{
//
//}

//GLTextureCube::GLTextureCube(const QStringList& fileNames, int size)
//{
//    // TODO: Add error handling.
//
//    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
//
//    int index = 0;
//    foreach (QString file, fileNames) {
//        QImage image(file);
//        if (image.isNull()) {
//            m_failed = true;
//            break;
//        }
//
//        image = image.convertToFormat(QImage::Format_ARGB32);
//
//        //qDebug() << "Image size:" << image.width() << "x" << image.height();
//        if (size <= 0)
//            size = image.width();
//        if (size != image.width() || size != image.height())
//            image = image.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//
//        // Works on x86, so probably works on all little-endian systems.
//        // Does it work on big-endian systems?
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, 4, image.width(), image.height(), 0,
//            GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
//
//        if (++index == 6)
//            break;
//    }
//
//    // Clear remaining faces.
//    while (index < 6) {
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, 4, size, size, 0,
//            GL_BGRA, GL_UNSIGNED_BYTE, 0);
//        ++index;
//    }
//
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//}

//void GLTextureCube::load(int size, int face, QRgb *data)
//{
//    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, 4, size, size, 0,
//            GL_BGRA, GL_UNSIGNED_BYTE, data);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//}

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
	glDeleteTextures(1, &m_texture);
}