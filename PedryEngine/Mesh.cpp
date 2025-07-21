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
	this->meshAssetId = meshId;

	Vector<GLfloat>& storedVertices = this->vertices;
	Vector<GLfloat>& storedNormals = this->normals;
	Vector<GLfloat>& storedUVs = this->uvs;
	Vector<GLfloat>& storedTangents = this->tangents;
	Vector<GLfloat>& storedColors = this->colors;
	Vector<GLuint>& storedIndices = this->indices;
	Vector<WorldTriangle>& storedTriangles = this->triangles;

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
				storedTangents.push_back(0.01f); // handedness
			}

			// Assign unique index
			storedIndices.push_back(nextIndex);
			uniqueVertices[key] = nextIndex++;
		}
	}

	WorldTriangle triangleBuffer;
	GLuint trianglePoint = 0;
	for (GLuint trIndex = 0; trIndex < storedIndices.size(); trIndex++)
	{
		GLuint pointStride = storedIndices[trIndex] * 3;
		vec3 point = vec3(vertices[pointStride + 0], vertices[pointStride + 1], vertices[pointStride + 2]);
		switch (trianglePoint)
		{
		case 0:
			triangleBuffer.localA = point;
			break;
		case 1:
			triangleBuffer.localB = point;
			break;
		case 2:
			triangleBuffer.localC = point;
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

	}

	std::cout << vertices.size() / 3 << " || " << triangles.size() << std::endl;

	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	glGenBuffers(1, &positionBuffer);
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &uvBuffer);
	glGenBuffers(1, &elementBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_COPY);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), normals.data(), GL_STATIC_COPY);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvs.size(), uvs.data(), GL_STATIC_COPY);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_COPY);


}

void Mesh::UpdateGpuStorage()
{

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_COPY);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), normals.data(), GL_STATIC_COPY);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvs.size(), uvs.data(), GL_STATIC_COPY);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_COPY);

}

Mesh::~Mesh()
{
}

GLuint Mesh::GetVAO()
{
	return vertexArrayId;
}

GLuint Mesh::GetPositionBuffer()
{
	return positionBuffer;
}

GLuint Mesh::GetNormalBuffer()
{
	return normalBuffer;
}

GLuint Mesh::GetUvBuffer()
{
	return uvBuffer;
}

GLuint Mesh::GetElementBuffer()
{
	return elementBuffer;
}
