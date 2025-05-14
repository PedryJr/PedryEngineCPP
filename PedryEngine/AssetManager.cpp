#pragma once
#include "PedryEngine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Mesh* AssetManager::LoadMesh(const String& meshName)
{
	ufbx_load_opts opts = {};
	ufbx_error error;

	

	ufbx_scene* scene = ufbx_load_file(("Assets/" + meshName + ".fbx").c_str(), &opts, &error);
	
	//ufbx_vec3_list vertices = scene->meshes[0]->vertices;
	//ufbx_vec3_list normals = scene->meshes[0]->vertex_normal;
	//ufbx_uint32_list indices = scene->meshes[0]->vertex_indices;

	GLulong meshId = NameToId(meshName);

	Mesh* mesh = new Mesh(scene->meshes[0], meshId);

	String logOutput = meshName;
	logOutput += " model loaded with ";
	logOutput += ToString(mesh->vertices.size() / 3);
	logOutput += " vertices and ";
	logOutput += ToString(mesh->indices.size());
	logOutput += " indices with the meshId of ";
	logOutput += ToString(mesh->meshId);
	logOutput += ".";
	Log(logOutput);

	return mesh;
}

GLuint AssetManager::LoadTexture(const String& textureName)
{
	String texturePath = "Assets/" + textureName + ".png";
	int w, h, channels;
	stbi_uc* img = stbi_load(texturePath.c_str(), &w, &h, &channels, 4);
	if (!img) {
		std::cerr << "Failed to load image: " << texturePath.c_str() << std::endl;
		return 0;
	}
	GLuint tex;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(tex, 1, GL_RGBA8, w, h);
	glTextureSubImage2D(tex, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(img);
	return tex;
}