#include <SDL.h>
#include "engine/graphics/Renderer.h"
#include "engine/graphics/RenderObject.h"

#pragma comment(lib, "nclgl.lib")

int main(int argc, char* argv[]) {

	//SDL_Window *window;
	//SDL_Init(SDL_INIT_EVERYTHING);
	//window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	//if (window == nullptr) {
	//	std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	//	return;
	//}
	//
	//SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//if (renderer == nullptr) {
	//	std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
	//	return;
	//}

	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);
	//SDL_Quit();

	/*Window w = Window();
	Renderer r(w);

	r.SetProjectionMatrix(Matrix4::Perspective(1,10,1.33f,45.0f));
	r.SetViewMatrix(Matrix4::Translation(Vector3(0,0,-5)));

	Mesh*m		= Mesh::GenerateTriangle();
	Shader* s	= new Shader("testvert.glsl", "testFrag.glsl");

	RenderObject o(m,s);
	o.SetModelMatrix(Matrix4::Scale(Vector3(1,1,1)));
	r.AddRenderObject(o);
	
	while(w.UpdateWindow()) {
		float msec = w.GetTimer()->GetTimedMS();

		r.UpdateScene(msec);
		r.ClearBuffers();
		r.RenderScene();
		r.SwapBuffers();
	}*/

	return 0;
}