#pragma once
#include "PedryEngine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unordered_map<GLulong, Mesh*> AssetManager::meshCache;
std::unordered_map<GLulong, Shader*> AssetManager::shaderCache;
std::unordered_map<GLulong, GLuint64> AssetManager::textureCache;

Mesh* AssetManager::LoadMesh(const String& meshName)
{

	GLulong meshId = NameToId(meshName);

	Mesh* mesh = GetMeshFromCache(meshId);
	
	if (mesh) return mesh;

	ufbx_load_opts opts = {};
	ufbx_error error;

	

	ufbx_scene* scene = ufbx_load_file(("Assets/" + meshName + ".fbx").c_str(), &opts, &error);
	
	//ufbx_vec3_list vertices = scene->meshes[0]->vertices;
	//ufbx_vec3_list normals = scene->meshes[0]->vertex_normal;
	//ufbx_uint32_list indices = scene->meshes[0]->vertex_indices;


	mesh = new Mesh(scene->meshes[0], meshId);

	String logOutput = meshName;
	logOutput += " model loaded with ";
	logOutput += ToString(mesh->vertices.size() / 3);
	logOutput += " vertices and ";
	logOutput += ToString(mesh->indices.size());
	logOutput += " indices with the meshId of ";
	logOutput += ToString(mesh->meshId);
	logOutput += ".";
	Log(logOutput);

	AddMeshToCache(meshId, mesh);
	ufbx_free_scene(scene);

	return mesh;
}

GLuint64 AssetManager::LoadTexture(const String& textureName)
{

	GLulong textureId = NameToId(textureName);

	//TODO: CHANGE TO TEXTURE CLASS FOR CLEANUP
	GLuint64 texture = GetTextureFromCache(textureId);

	if (texture != -1) return texture;

	String texturePath = "Assets/" + textureName + ".png";
	int w, h, channels;
	stbi_uc* img = stbi_load(texturePath.c_str(), &w, &h, &channels, 4);
	if (!img) {
		std::cerr << "Failed to load image: " << texturePath.c_str() << std::endl;
		return 0;
	}
	GLuint tex;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(tex, 1, GL_RGBA32F, w, h);
	glTextureSubImage2D(tex, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_POINT);
	glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_POINT);
	stbi_image_free(img);

	GLuint64 handle = glGetTextureHandleARB(tex);
	glMakeTextureHandleResidentARB(handle);

	AddTextureToCache(textureId, handle);

	return handle;
}

Shader* AssetManager::LoadShader(const String& shaderName)
{

	Shader* shadowShader = LoadShadowShader();

	GLulong shaderId = NameToId(shaderName);
	Shader* shader = GetShaderFromCache(shaderId);
	if (shader) return shader;
	
	shader = new Shader(shaderName, shaderId);
	shader->shadowShader = shadowShader;
	shadowShader->shadowShader = shadowShader;
	
	String logOutput = shaderName;
	logOutput += " shader loaded with the shaderId of ";
	logOutput += ToString(shader->shaderId);
	logOutput += ".";
	Log(logOutput);
	AddShaderToCache(shaderId, shader);
	return shader;
}

Shader* AssetManager::GetShaderFromCache(const GLulong& id)
{
	auto it = shaderCache.find(id);
	if (it != shaderCache.end()) return it->second;
	return nullptr;
}


void AssetManager::AddShaderToCache(const GLulong& id, Shader* shader)
{
	shaderCache[id] = shader;
}


void AssetManager::RemoveShaderFromCache(const GLulong& id)
{
	auto it = shaderCache.find(id);
	if (it != shaderCache.end())
	{
		delete it->second;
		shaderCache.erase(it);
	}
}

GLuint64 AssetManager::GetTextureFromCache(const GLulong& id)
{
	auto it = textureCache.find(id);
	if (it != textureCache.end()) return it->second;
	return -1;
}
void AssetManager::AddTextureToCache(const GLulong& id, GLuint64 texture)
{
	textureCache[id] = texture;
}
void AssetManager::RemoveTextureFromCache(const GLulong& id)
{
	auto it = textureCache.find(id);
	if (it != textureCache.end())
	{
		//TODO: TEXTURE CLASS FOR CLEANUP
		//delete it->second;
		textureCache.erase(it);
	}
}


Mesh* AssetManager::GetMeshFromCache(const GLulong& id)
{
	auto it = meshCache.find(id);
	if (it != meshCache.end()) return it->second;
	return nullptr;
}

void AssetManager::AddMeshToCache(const GLulong& id, Mesh* mesh)
{
	meshCache[id] = mesh;
}

void AssetManager::RemoveMeshFromCache(const GLulong& id)
{
	auto it = meshCache.find(id);
	if (it != meshCache.end())
	{
		delete it->second;
		meshCache.erase(it);
	}
}

Shader* AssetManager::LoadShadowShader()
{

	String shadowName = "Shadow";

	GLulong shaderId = NameToId(shadowName);

	Shader* shadowShader = GetShaderFromCache(shaderId);
	if (shadowShader) return shadowShader;
	shadowShader = new Shader(shadowName, shaderId);
	String logOutput = "Shadow shader loaded with the shaderId of ";
	logOutput += ToString(shadowShader->shaderId);
	logOutput += ".";
	Log(logOutput);
	AddShaderToCache(shaderId, shadowShader);
	return shadowShader;

}