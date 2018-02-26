#pragma once

#include "program.inl"
#include "vertex.inl"
#include "texture.inl"
#include "vertex_array.inl"
#include "buffer.inl"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

using namespace std;
using namespace glm;

class Mesh
{
public:
	Mesh(Program& shader, const vector<Vertex>& vertices, const vector<uvec3>& indices = {}, const vector<shared_ptr<Texture>>& textures = {});
	void draw(const mat4& model = {}, const mat4& view = {}, const mat4& projection = {});

private:
	Program& m_shader;
	VertexArray m_vertexArray;
	Buffer m_vertexBuffer;
	Buffer m_elementBuffer;
	vector<shared_ptr<Texture>> m_textures;
	GLsizei m_drawCount;
	bool m_hasIndices;

	// Rule of five
	Mesh(Mesh&&) = delete;
	Mesh(const Mesh&) = delete;
	Mesh& operator=(Mesh&&) = delete;
	Mesh& operator=(const Mesh&) = delete;
};