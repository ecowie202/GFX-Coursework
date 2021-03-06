#include "model.hpp"
#include "util.inl"

#include <unordered_map>

Model::Model(ProgramPtr shader, const string& path, bool flipUVs)
{
	// Get base directory from file path
	string baseDir = path.substr(0, path.find_last_of('/')) + '/';

	// Create variables to store result of LoadObj
	attrib_t attrib;
	vector<shape_t> shapes;
	vector<material_t> materials;
	string err;

	// Attempt to load the OBJ model
	bool obj = LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), baseDir.c_str());
	if (!obj) util::panic("Failed to load model: " + path, err);

	for (const shape_t& shape : shapes)
	{
		// Create vector to store raw OBJ indices
		vector<size_t> indexArray;

		// Extract vertices and raw OBJ indices
		vector<Vertex> vertices = extractVertices(attrib, shape.mesh.indices, &indexArray);

		// Transform raw OBJ indices to OpenGL indices
		vector<uvec3> indices = transformIndices(indexArray);

		// Extract material information
		vector<Material> mats = extractMaterials(materials, shape.mesh.material_ids, baseDir, flipUVs);

		// Add mesh
		m_meshes.push_back(make_shared<Mesh>(shader, vertices, indices, mats));
	}
}

void Model::draw(const mat4& model, const mat4& view, const mat4& projection, const vector<ILightPtr>& lights)
{
	// Draw all meshes
	for (shared_ptr<Mesh>& mesh : m_meshes)
	{
		mesh->draw(model, view, projection, lights);
	}
}

vector<Vertex> Model::extractVertices(const attrib_t& attrib, const vector<index_t>& attribIds, vector<size_t>* indices)
{
	vector<Vertex> result;

	unordered_map<Vertex, size_t> indexMap;

	for (const index_t& i : attribIds)
	{
		// Create vertex
		Vertex vertex;

		// Extract vertex position
		vertex.position = {
			attrib.vertices[3 * i.vertex_index + 0],
			attrib.vertices[3 * i.vertex_index + 1],
			attrib.vertices[3 * i.vertex_index + 2]
		};

		if (i.normal_index >= 0)
		{
			// Extract vertex normal
			vertex.normal = {
				attrib.normals[3 * i.normal_index + 0],
				attrib.normals[3 * i.normal_index + 1],
				attrib.normals[3 * i.normal_index + 2]
			};
		}

		// Set vertex color to white
		vertex.color = vec4(1);

		if (i.texcoord_index >= 0)
		{
			// Extract vertex texture coordinate
			vertex.texCoord = {
				attrib.texcoords[2 * i.texcoord_index + 0],
				attrib.texcoords[2 * i.texcoord_index + 1]
			};
		}

		// If index map does not contain this vertex...
		if (indexMap.count(vertex) == 0)
		{
			// Add vertex to index map pointing it the currently last vertex in result
			indexMap[vertex] = result.size();

			// Add vertex to result
			result.push_back(vertex);
		}

		// Add the index referred to by the index map for this vertex
		if (indices) indices->push_back(indexMap[vertex]);
	}

	return result;
}

vector<uvec3> Model::transformIndices(vector<size_t> indices)
{
	vector<uvec3> result;

	for (int i = 2; i < indices.size(); i += 3)
	{
		// Create index
		uvec3 index = {
			indices[i - 2],
			indices[i - 1],
			indices[i]
		};

		// Add index to result
		result.push_back(index);
	}

	return result;
}

vector<Material> Model::extractMaterials(const vector<material_t>& materials, const vector<int>& materialIds, const string& baseDir, bool flipUVs)
{
	map<string, shared_ptr<Texture>> textures;
	vector<Material> result;

	for (int i : materialIds)
	{
		if (i < materials.size())
		{
			Material mat;

			if (!materials[i].diffuse_texname.empty())
			{
				// Get texture path
				string path = baseDir + materials[i].diffuse_texname;

				// Check if texture has already been loaded
				if (!textures.count(path))
				{
					// Create texture and add it to result
					textures[path] = make_shared<Texture>(path, flipUVs);
				}

				mat.diffuse = textures[path];
			}

			if (!materials[i].specular_texname.empty())
			{
				// Get texture path
				string path = baseDir + materials[i].specular_texname;

				// Check if texture has already been loaded
				if (!textures.count(path))
				{
					// Create texture and add it to result
					textures[path] = make_shared<Texture>(path, flipUVs);
				}

				mat.specular = textures[path];
			}

			mat.shininess = materials[i].shininess;

			if (mat.diffuse)
			{
				result.push_back(mat);
				return result;
			}
		}
	}

	return result;
}
