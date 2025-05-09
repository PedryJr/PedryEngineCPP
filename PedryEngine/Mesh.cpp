#pragma once
#include "PedryEngine.h"
#include <unordered_map>

struct TangentBuilder {
	vec3 pos[3];
	vec2 uv[3];
	vec3 norm[3];
};

struct VertexKey {
	GLint posIndex;
	GLint normIndex;
	GLint uvIndex;
	GLint tangentIndex;
	GLint colorIndex;

	bool operator==(const VertexKey& other) const {
		return posIndex == other.posIndex &&
			normIndex == other.normIndex &&
			uvIndex == other.uvIndex &&
			tangentIndex == other.tangentIndex &&
			colorIndex == other.colorIndex;
	}
};

namespace std {
	template<> struct hash<VertexKey> {
		size_t operator()(const VertexKey& k) const {
			size_t h = std::hash<GLint>()(k.posIndex);
			h ^= std::hash<GLint>()(k.normIndex) << 1;
			h ^= std::hash<GLint>()(k.uvIndex) << 2;
			h ^= std::hash<GLint>()(k.tangentIndex) << 3;
			h ^= std::hash<GLint>()(k.colorIndex) << 4;
			return h;
		}
	};
}

#define to_vec2(ufbx_vec2) vec2(ufbx_vec2.x, ufbx_vec2.y)
#define to_vec3(ufbx_vec3) vec3(ufbx_vec3.x, ufbx_vec3.y, ufbx_vec3.z)

Mesh::Mesh(ufbx_mesh* loadedMesh, const GLulong& meshId)
{
	this->meshId = meshId;

	Vector<GLfloat>& storedVertices = this->vertices;
	Vector<GLfloat>& storedNormals = this->normals;
	Vector<GLfloat>& storedUVs = this->uvs;
	Vector<GLfloat>& storedTangents = this->tangents;
	Vector<GLfloat>& storedColors = this->colors;
	Vector<GLuint>& storedIndices = this->indices;

	std::unordered_map<VertexKey, GLuint> uniqueVertices;
	GLuint nextIndex = 0;

	for (GLint i = 0; i < loadedMesh->vertex_indices.count; ++i)
	{
		GLint posIndex = loadedMesh->vertex_position.indices.data[i];
		GLint normIndex = loadedMesh->vertex_normal.indices.data[i];
		GLint uvIndex = loadedMesh->vertex_uv.indices.count > 0 ? loadedMesh->vertex_uv.indices.data[i] : 0;
		GLint tangentIndex = loadedMesh->vertex_tangent.indices.count > 0 ? loadedMesh->vertex_tangent.indices.data[i] : 0;
		GLint colorIndex = loadedMesh->vertex_color.indices.count > 0 ? loadedMesh->vertex_color.indices.data[i] : 0;

		VertexKey key{ posIndex, normIndex, uvIndex, tangentIndex, colorIndex };

		auto it = uniqueVertices.find(key);
		if (it != uniqueVertices.end()) {
			storedIndices.push_back(it->second);
		}
		else {
			// Add position
			ufbx_vec3 pos = loadedMesh->vertex_position.values.data[posIndex];
			storedVertices.push_back(pos.x);
			storedVertices.push_back(pos.y);
			storedVertices.push_back(pos.z);

			// Add normal
			ufbx_vec3 norm = loadedMesh->vertex_normal.values.data[normIndex];
			storedNormals.push_back(norm.x);
			storedNormals.push_back(norm.y);
			storedNormals.push_back(norm.z);

			// Add UV (optional)
			if (loadedMesh->vertex_uv.values.count > 0) {
				ufbx_vec2 uv = loadedMesh->vertex_uv.values.data[uvIndex];
				storedUVs.push_back(uv.x);
				storedUVs.push_back(uv.y);
			}

			// Add tangent (optional)
			if (loadedMesh->vertex_tangent.values.count > 0) {
				ufbx_vec3 tangent = loadedMesh->vertex_tangent.values.data[tangentIndex]; // might be vec4
				storedTangents.push_back(tangent.x);
				storedTangents.push_back(tangent.y);
				storedTangents.push_back(tangent.z);
				storedTangents.push_back(0); // handedness
			}

			// Assign unique index
			storedIndices.push_back(nextIndex);
			uniqueVertices[key] = nextIndex++;
		}
	}

}

Mesh::~Mesh()
{
}
