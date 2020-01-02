// A model.
struct Model {
	GLuint vaoID;
	int vertexCount;

	Model() {
		return;
	}

	Model(GLuint vaoID, int vertexCount) {
		this->vaoID = vaoID;
		this->vertexCount = vertexCount;
	}
};