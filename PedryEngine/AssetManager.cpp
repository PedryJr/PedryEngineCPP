#pragma once
#include "PedryEngine.h"

Mesh* AssetManager::LoadMesh(const String& meshName)
{
	ufbx_load_opts opts = {};
	ufbx_error error;

	opts.evaluate_caches = false;
	opts.evaluate_skinning = false;
	opts.allow_unsafe = true;
	opts.allow_empty_faces = true;


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
