// Load a Wavefront (.obj) object file.
void loadWavefront(std::string path, std::vector<float>& vertices, std::vector<float>& textures, std::vector<float>& normals) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

	if (!warn.empty()) {
		std::cerr << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		fatalError("loadWavefront(\"%s\")", path.c_str());
	}

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			if (fv == 3) {
				// Triangles only.
				for (size_t v = 0; v < fv; v++) {
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					textures.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
					textures.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					normals.push_back(attrib.normals[3 * idx.normal_index + 0]);
					normals.push_back(attrib.normals[3 * idx.normal_index + 1]);
					normals.push_back(attrib.normals[3 * idx.normal_index + 2]);
				}
			}

			index_offset += fv;
		}
	}
}