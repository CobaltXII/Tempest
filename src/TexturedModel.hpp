// A textured model.
struct TexturedModel {
	Model model;
	Texture texture;

	TexturedModel() {
		return;
	}

	TexturedModel(Model model, Texture texture) {
		this->model = model;
		this->texture = texture;
	}
};