#pragma once
class Mesh
{

public:
	Mesh(ufbx_mesh* loadedMesh, const GLulong& meshId);
	~Mesh();

	Vector<GLfloat> vertices;
	Vector<GLfloat> normals;
	Vector<GLfloat> uvs;
	Vector<GLfloat> tangents;
	Vector<GLfloat> colors;
	Vector<GLuint> indices;
	Vector<WorldTriangle> triangles;

	GLulong meshId;

};

