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

	GLuint GetVAO();
	GLuint GetPositionBuffer();
	GLuint GetNormalBuffer();
	GLuint GetUvBuffer();
	GLuint GetElementBuffer();

	void UpdateGpuStorage();
	GLulong meshId;
private:
	GLuint vertexArrayId;
	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint uvBuffer;
	GLuint elementBuffer;


};

