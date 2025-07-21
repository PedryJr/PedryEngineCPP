#include "PedryEngine.h"


Texture::Texture(const String name, const GLulong textureId)
{

	glGetString(GL_EXTENSIONS);
	String texturePath = "Assets/" + name + ".png";
	int w, h, channels;
	//stbi_uc* img = stbi_load(texturePath.c_str(), &w, &h, &channels, 4);
	//if (!img) {
		//std::cerr << "Failed to load image: " << texturePath.c_str() << std::endl;
		//return;
	//}
	GLuint tex;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(tex, 1, GL_RGBA8, w, h);
	//glTextureSubImage2D(tex, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_POINT);
	glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_POINT);
	//stbi_image_free(img);

	GLuint64 handle = glGetTextureHandleARB(tex);
	//glMakeTextureHandleResidentARB(handle);

	this->textureAssetId = textureId;
	this->textureGLId = tex;
	this->textureResidentHandle = handle;

}
