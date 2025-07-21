#pragma once
class Texture
{
public:
	Texture(const String name, const GLulong shaderId);

	GLulong textureAssetId;
	GLuint textureGLId;
	GLuint64 textureResidentHandle;

};

