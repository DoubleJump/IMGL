namespace renderer
{
	struct State
	{
		memory::Block* storage;
		Rect view;
		Shader* shader;
		Mesh* screen_mesh;
		//Mesh* sphere;
		Texture* texture;
		Target* target;
	};
}
