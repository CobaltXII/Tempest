// A heightmap.
struct Heightmap {
	FastNoise noise;

	float scale = 0.2f;
	float amplitude = 80.0f;

	Heightmap() {
		noise.SetNoiseType(FastNoise::SimplexFractal);
		noise.SetFractalOctaves(8);
	}

	// Get the heightmap value at a certain point.
	float sample(float u, float v) {
		return noise.GetNoise(u * scale, v * scale) * amplitude;
	}

	// Get the normal at a certain point.
	glm::vec3 normal(float u, float v) {
		const float epsilon = 1.0f;
		float heightL = sample(u - epsilon, v);
		float heightR = sample(u + epsilon, v);
		float heightD = sample(u, v - epsilon);
		float heightU = sample(u, v + epsilon);
		return glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));
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
};