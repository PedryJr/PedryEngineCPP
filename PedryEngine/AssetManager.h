#pragma once
class AssetManager
{
public:

	static Mesh* LoadMesh(const String& meshName);
	static GLuint64 LoadTexture(const String& textureName);
	static Texture* NewLoadTexture(const String& textureName);
	static Shader* LoadShader(const String& shaderName);


	static FORCE_INLINE GLulong NameToId(const String& name) {
		return static_cast<uint64_t>(std::hash<std::string>{}(name));
	}

private:
	static Shader* LoadShadowShader();

	// Mesh cache
	static std::unordered_map<GLulong, Mesh*> meshCache;
	static std::unordered_map<GLulong, Shader*> shaderCache;
	static std::unordered_map<GLulong, GLuint64> textureCache;
	static std::unordered_map<GLulong, Texture*> newTextureCache;

	static Mesh* GetMeshFromCache(const GLulong& id);
	static Shader* GetShaderFromCache(const GLulong& id);
	static GLuint64 GetTextureFromCache(const GLulong& id, GLuint64& texture);
	static Texture* GetNewTextureFromCache(const GLulong& id);

	static void AddMeshToCache(const GLulong& id, Mesh* mesh);
	static void AddShaderToCache(const GLulong& id, Shader* shader);
	static void AddTextureToCache(const GLulong& id, GLuint64 texture);
	static void AddNewTextureToCache(const GLulong& id, Texture* texture);

	static void RemoveMeshFromCache(const GLulong& id);
	static void RemoveShaderFromCache(const GLulong& id);
	static void RemoveTextureFromCache(const GLulong& id);
	static void RemoveNewTextureFromCache(const GLulong& id);

};

