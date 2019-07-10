/**
* @file Main.cpp
*/
#include "TitleScene.h"
#include"GLFWEW.h"
#include <Windows.h>
#include <iostream>
int main() {
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	window.Init(1280, 720, u8"アクションゲーム");
	SceneStack& sceneStack = SceneStack::Instance();
	sceneStack.push(std::make_shared<TitleScene>());

	while (!window.ShouoldClose()) {
		const float deltaTime = window.DeltaTime();
		window.UpdateTimer();
		sceneStack.Update(deltaTime);

		// ESCキーが押されたら終了ウィンドウを表示.
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
			if (MessageBox(nullptr, "ゲームを終了しますか？", "終了", MB_OKCANCEL) == IDOK) {
				break;
			}
		}


		// バックバッファを消去する.
		glClearColor(0.8f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	    // GLコンテキストのパラメータを設定.
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);


		sceneStack.Render();
		window.SwapBuffers();
	}
}