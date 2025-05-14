#pragma once

#include "PedryEngine.h"


struct TangentBuilder {
	vec3 pos[3];
	vec2 uv[3];
	vec3 norm[3];
};

struct VertexKey {
	GLuint posIndex;
	GLuint normIndex;
	GLuint uvIndex;
	GLuint tangentIndex;
	GLuint colorIndex;

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
		GLuint operator()(const VertexKey& k) const {
			GLuint h = std::hash<GLuint>()(k.posIndex);
			h ^= std::hash<GLuint>()(k.normIndex) << 1;
			h ^= std::hash<GLuint>()(k.uvIndex) << 2;
			h ^= std::hash<GLuint>()(k.tangentIndex) << 3;
			h ^= std::hash<GLuint>()(k.colorIndex) << 4;
			return h;
		}
	};
}


Mesh::Mesh(ufbx_mesh* loadedMesh, const GLulong& meshId)
{
	this->meshId = meshId;

	Vector<GLfloat>& storedVertices = this->vertices;
	Vector<GLfloat>& storedNormals = this->normals;
	Vector<GLfloat>& storedUVs = this->uvs;
	Vector<GLfloat>& storedTangents = this->tangents;
	Vector<GLfloat>& storedColors = this->colors;
	Vector<GLuint>& storedIndices = this->indices;
	Vector<Triangle>& storedTriangles = this->triangles;

	std::unordered_map<VertexKey, GLuint> uniqueVertices;
	GLuint nextIndex = 0;

	for (GLuint i = 0; i < loadedMesh->vertex_indices.count; ++i)
	{
		GLuint posIndex = loadedMesh->vertex_position.indices.data[i];
		GLuint normIndex = loadedMesh->vertex_normal.indices.data[i];
		GLuint uvIndex = loadedMesh->vertex_uv.indices.count > 0 ? loadedMesh->vertex_uv.indices.data[i] : 0;
		GLuint tangentIndex = loadedMesh->vertex_tangent.indices.count > 0 ? loadedMesh->vertex_tangent.indices.data[i] : 0;
		GLuint colorIndex = loadedMesh->vertex_color.indices.count > 0 ? loadedMesh->vertex_color.indices.data[i] : 0;

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
				storedTangents.push_back(0.0F); // handedness
			}

			// Assign unique index
			storedIndices.push_back(nextIndex);
			uniqueVertices[key] = nextIndex++;
		}
	}

	Triangle triangleBuffer = {vec3(0), vec3(0), vec3(0)};
	GLuint trianglePoint = 0;
	for (GLuint trIndex = 0; trIndex < storedIndices.size(); trIndex++)
	{
		GLuint pointStride = storedIndices[trIndex] * 3;
		vec3 point = vec3(vertices[pointStride + 0], vertices[pointStride + 1], vertices[pointStride + 2]);
		switch (trianglePoint)
		{
		case 0:
			triangleBuffer.a = point;
			break;
		case 1:
			triangleBuffer.b = point;
			break;
		case 2:
			triangleBuffer.c = point;
			break;

		default:
			break;

		}

		trianglePoint++;

		if(trianglePoint == 3)
		{
			storedTriangles.push_back(triangleBuffer);
			trianglePoint = 0;
		}

		/*
		triangleBuffer.a.x = vertices[indices[trIndex + 0] * 3 + 0];
		triangleBuffer.a.y = vertices[indices[trIndex + 0] * 3 + 1];
		triangleBuffer.a.z = vertices[indices[trIndex + 0] * 3 + 2];

		triangleBuffer.b.x = vertices[indices[trIndex + 1] * 3 + 0];
		triangleBuffer.b.y = vertices[indices[trIndex + 1] * 3 + 1];
		triangleBuffer.b.z = vertices[indices[trIndex + 1] * 3 + 2];

		triangleBuffer.c.x = vertices[indices[trIndex + 2] * 3 + 0];
		triangleBuffer.c.y = vertices[indices[trIndex + 2] * 3 + 1];
		triangleBuffer.c.z = vertices[indices[trIndex + 2] * 3 + 2];*/

	}

	std::cout << vertices.size() / 3 << " || " << triangles.size() << std::endl;

}

Mesh::~Mesh()
{
}
