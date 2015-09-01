#include "../core/texture.cpp"
#include "../core/shader.cpp"
#include "../core/mesh.cpp"
#include "../core/render_target.cpp"

namespace renderer
{
	struct State
	{
		ThreadContext* thread;
		Rect view;
		Mesh* mesh;
		Target* render_target;
	};
}
