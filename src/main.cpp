#include <AEngine.h>

#include <GLM/glm/vec3.hpp> // glm::vec3
#include <GLM/glm/vec4.hpp> // glm::vec4
#include <GLM/glm/mat4x4.hpp> // glm::mat4
#include <GLM/glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace glm;

mat4 camera(float Translate, glm::vec2 const& Rotate) {
	mat4 Projection = perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	mat4 View       = translate(mat4(1.0f), vec3(0.0f, 0.0f, -Translate));
	View            = rotate(View, Rotate.y, vec3(-1.0f, 0.0f, 0.0f));
	View            = rotate(View, Rotate.x, vec3(0.0f, 1.0f, 0.0f));
	mat4 Model      = scale(mat4(1.0f), vec3(0.5f));
	return Projection * View * Model;
}

int main(int argc, char** argv) {
	AEngine::Log::Init();
	AE_CRITICAL("Initialized Logger");
	auto engine = new AEngine::CoreEngine();
	engine->addOverlay(new AEngine::ImGUILayer());
	engine->start();
	delete engine;
	return 0;
}
