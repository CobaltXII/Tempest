// A terrain heightmap.
struct TerrainHeightmap {
	float* heightmap;
	float heightmapSize;
	int heightmapResolution;
	GLuint textureID;
	Heightmap sampler;

	TerrainHeightmap() {
		return;
	}

	TerrainHeightmap(float size, int resolution) {
		this->heightmapSize = size;
		this->heightmapResolution = resolution;
		sampler.noise.SetSeed(time(NULL));
		heightmap = new float[heightmapResolution * heightmapResolution];
		for (int i = 0; i < heightmapResolution; i++) {
			for (int j = 0; j < heightmapResolution; j++) {
				float u = float(i) / float(heightmapResolution - 1) * 2.0f - 1.0f;
				float v = float(j) / float(heightmapResolution - 1) * 2.0f - 1.0f;
				heightmap[j * heightmapResolution + i] = sampler.sample(u * heightmapSize, v * heightmapSize);
			}
		}
		textureID = Manager::createTexture();
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, heightmapResolution, heightmapResolution, 0, GL_RED, GL_FLOAT, heightmap);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Regenerate.
	void regenerate() {
		// No.
		static int bruh = 0;
		sampler.noise.SetSeed(time(NULL) + ++--++--++bruh);
		for (int i = 0; i < heightmapResolution; i++) {
			for (int j = 0; j < heightmapResolution; j++) {
				float u = float(i) / float(heightmapResolution - 1) * 2.0f - 1.0f;
				float v = float(j) / float(heightmapResolution - 1) * 2.0f - 1.0f;
				heightmap[j * heightmapResolution + i] = sampler.sample(u * heightmapSize, v * heightmapSize);
			}
		}
		update();
	}

	// Update everything.
	void update() {
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, heightmapResolution, heightmapResolution, 0, GL_RED, GL_FLOAT, heightmap);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

// A terrain object.
struct TerrainObject {
	Model model;
	TerrainHeightmap heightmap;
	Texture texture1;
	Texture texture2;
	Texture texture3;

	TerrainObject() {
		return;
	}

	TerrainObject(Model model, TerrainHeightmap heightmap, Texture texture1, Texture texture2, Texture texture3) {
		this->model = model;
		this->heightmap = heightmap;
		this->texture1 = texture1;
		this->texture2 = texture2;
		this->texture3 = texture3;
	}

	// Get the heightmap value at a certain point.
	float sample(float u, float v) {
		int x = int(((u / heightmap.heightmapSize) + 1.0f) / 2.0f * heightmap.heightmapResolution);
		int y = int(((v / heightmap.heightmapSize) + 1.0f) / 2.0f * heightmap.heightmapResolution);
		if (x < 0 || x >= heightmap.heightmapResolution ||
			y < 0 || y >= heightmap.heightmapResolution) {
			return 0.0f;
		}
		return heightmap.heightmap[y * heightmap.heightmapResolution + x];
	}

	// Set the heightmap value at a certain point.
	void set(float u, float v, float height) {
		int x = int(((u / heightmap.heightmapSize) + 1.0f) / 2.0f * heightmap.heightmapResolution);
		int y = int(((v / heightmap.heightmapSize) + 1.0f) / 2.0f * heightmap.heightmapResolution);
		if (x < 0 || x >= heightmap.heightmapResolution ||
			y < 0 || y >= heightmap.heightmapResolution) {
			return;
		}
		heightmap.heightmap[y * heightmap.heightmapResolution + x] = height;
	}

	// Raycasting.
	float raycast(glm::vec3 ro, glm::vec3 rd) {
		float dt = 0.1f;
		float mint = 0.001f;
		float maxt = 2000.0f;
		for (float t = mint; t < maxt; t += dt) {
			glm::vec3 p = ro + rd * t;
			if (p.y < sample(p.x, p.z)) {
				return t - 0.5f * dt;
			}
		}
		return -1.0f;
	}

	// Update everything.
	void update() {
		heightmap.update();
	}

	// Add height.
	void addHeight(float u, float v, float radius, float height) {
		radius /= 2.0f;
		int x = int(((u / heightmap.heightmapSize) + 1.0f) / 2.0f * heightmap.heightmapResolution);
		int y = int(((v / heightmap.heightmapSize) + 1.0f) / 2.0f * heightmap.heightmapResolution);
		int r = radius;
		for (int q = y - r; q <= y + r; q++) {
			for (int p = x - r; p < x + r; p++) {
				if (p < 0 || p >= heightmap.heightmapResolution ||
					q < 0 || q >= heightmap.heightmapResolution) {
					continue;
				}
				int dx = (p - x) * (p - x);
				int dy = (q - y) * (q - y);
				if (dx + dy < r * r) {
					float effect = std::pow((radius - std::sqrt(dx + dy)) / radius, 1.0f / 3.0f);
					heightmap.heightmap[q * heightmap.heightmapResolution + p] += height * effect;
				}
			}
		}
	}

	// Average height.
	void averageHeight(float u, float v, float radius, float power) {
		int x = int(((u / heightmap.heightmapSize) + 1.0f) / 2.0f * heightmap.heightmapResolution);
		int y = int(((v / heightmap.heightmapSize) + 1.0f) / 2.0f * heightmap.heightmapResolution);
		int r = radius;
		float sum = 0.0f;
		int count = 0;
		for (int q = y - r; q <= y + r; q++) {
			for (int p = x - r; p < x + r; p++) {
				if (p < 0 || p >= heightmap.heightmapResolution ||
					q < 0 || q >= heightmap.heightmapResolution) {
					continue;
				}
				int dx = (p - x) * (p - x);
				int dy = (q - y) * (q - y);
				if (dx + dy < r * r) {
					sum += heightmap.heightmap[q * heightmap.heightmapResolution + p];
					count++;
				}
			}
		}
		if (count == 0) {
			return;
		}
		float average = sum / float(count);
		for (int q = y - r; q <= y + r; q++) {
			for (int p = x - r; p < x + r; p++) {
				if (p < 0 || p >= heightmap.heightmapResolution ||
					q < 0 || q >= heightmap.heightmapResolution) {
					continue;
				}
				int dx = (p - x) * (p - x);
				int dy = (q - y) * (q - y);
				if (dx + dy < r * r) {
					float height = heightmap.heightmap[q * heightmap.heightmapResolution + p];
					float add = average - height;
					float effect = std::pow((radius - std::sqrt(dx + dy)) / radius, 1.0f / 3.0f);
					heightmap.heightmap[q * heightmap.heightmapResolution + p] += add * effect * power;
				}
			}
		}
	}
};

// Terrain library.
namespace Terrain {
	// Generate terrain.
	TerrainObject generateTerrain(float size, int resolution, Texture texture1, Texture texture2, Texture texture3) {
		std::vector<float> vertices;
		std::vector<float> textures;
		for (int i = 0; i < resolution; i++) {
			for (int j = 0; j < resolution; j++) {
				float u = float(i) / float(resolution - 1);
				float v = float(j) / float(resolution - 1);
				float x = u * size * 2.0f - size;
				float z = v * size * 2.0f - size;
				vertices.push_back(x);
				vertices.push_back(0.0f);
				vertices.push_back(z);
				textures.push_back(u);
				textures.push_back(v);
			}
		}

		std::vector<unsigned int> indices;
		for (int gz = 0; gz < resolution - 1; gz++) {
			for (int gx = 0; gx < resolution - 1; gx++) {
				int topLeft = gz * resolution + gx;
				int topRight = topLeft + 1;
				int bottomLeft = (gz + 1) * resolution + gx;
				int bottomRight = bottomLeft + 1;
				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}

		std::vector<float> verticesOut;
		std::vector<float> texturesOut;
		for (int i = 0; i < indices.size(); i++) {
			unsigned int j = indices[indices.size() - i - 1];
			verticesOut.push_back(vertices[j * 3 + 0]);
			verticesOut.push_back(vertices[j * 3 + 1]);
			verticesOut.push_back(vertices[j * 3 + 2]);
			texturesOut.push_back(textures[j * 2 + 0]);
			texturesOut.push_back(textures[j * 2 + 1]);
		}

		Model out = Manager::createModelWithoutNormals(verticesOut.data(), texturesOut.data(), verticesOut.size() / 3);
		return TerrainObject(out, TerrainHeightmap(size, resolution), texture1, texture2, texture3);
	}
};