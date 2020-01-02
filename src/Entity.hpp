// An entity.
struct Entity {
	TexturedModel model;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Entity() {
		return;
	}

	Entity(TexturedModel model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
		this->model = model;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
};