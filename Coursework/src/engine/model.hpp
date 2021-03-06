#pragma once

#include "mesh.hpp"

#include <tiny_obj_loader.h>
#include <string>
#include <vector>
#include <memory>

using namespace std;
using namespace tinyobj;

typedef shared_ptr<class Model> ModelPtr;

class Model
{
public:
	Model(ProgramPtr shader, const string& path, bool flipUVs = true);
	void draw(const mat4& model = {}, const mat4& view = {}, const mat4& projection = {}, const vector<ILightPtr>& lights = {});

	void shader(ProgramPtr shader)
	{
		// Set all mesh shaders
		for (auto& mesh : m_meshes)
			mesh->shader(shader);
	}

private:
	static vector<Vertex> extractVertices(const attrib_t& attrib, const vector<index_t>& attribIds, vector<size_t>* indices = nullptr);
	static vector<uvec3> transformIndices(vector<size_t> indices);
	static vector<Material> extractMaterials(const vector<material_t>& materials, const vector<int>& materialIds, const string& baseDir = "", bool flipUVs = true);

	vector<shared_ptr<Mesh>> m_meshes;
};
