#pragma once
class AssetManager
{
public:

	Vector<Mesh*> loadedMeshes;

	static Mesh* LoadMesh(const String& meshName);



	static FORCE_INLINE GLulong NameToId(const String& name) {
		return static_cast<uint64_t>(std::hash<std::string>{}(name));
	}

};

