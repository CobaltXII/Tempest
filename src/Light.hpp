// A light.
struct Light {
	glm::vec3 direction;
	glm::vec3 color;

	Light() {
		return;
	}

	Light(glm::vec3 direction, glm::vec3 color) {
		this->direction = direction;
		this->color = color;
	}
};