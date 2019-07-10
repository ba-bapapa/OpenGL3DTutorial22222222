#include"MainGameScene.h"
#include"StatusScene.h"
#include"GameOverScene.h"
#include"GLFWEW.h"
#include <glm/gtc/matrix_transform.hpp>


/**
*シーンを初期化する
*/
bool MainGameScene::Initialize() {
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/tree.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);
	meshBuffer.Init(1'000'000 * sizeof(Mesh::Vertex), 3'000'000 * sizeof(GLushort));
	meshBuffer.LoadMesh("Res/red_pine_tree.gltf");

	// ハイトマップを作成する.
	if (!heightMap.LoadFromFile("Res/Terrain.tga", 20.0f, 0.5f)) {
		return false;

	}
	if (!heightMap.CreateMesh(meshBuffer, "Terrain")) {
		return false;
		
	}

	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");


	return true;
}

void MainGameScene::ProcessInput() {
	{
		GLFWEW::Window&window = GLFWEW::Window::Instance();
		
		// カメラ操作.
		const GamePad gamepad = window.GetGamePad();
		glm::vec3 velocity(0);
		if (gamepad.buttons & GamePad::DPAD_LEFT) {
			velocity.x = -1;
			
		}else if (gamepad.buttons & GamePad::DPAD_RIGHT) {
			velocity.x = 1;
			
		}
		if (gamepad.buttons & GamePad::DPAD_DOWN) {
			velocity.z = 1;
			
		}else if (gamepad.buttons & GamePad::DPAD_UP) {
			velocity.z = -1;
			
		}
		if (velocity.x || velocity.z) {
			velocity = normalize(velocity) * 20.0f;
			
		}
		camera.velocity = velocity;


		if (!flag) {
			flag = true;
			if (window.GetGamePad().buttonDown&GamePad::START) {
				flag = true;
				SceneStack::Instance().push(std::make_shared<StatusScene>());
			}
		}
		else {
			GLFWEW::Window&window = GLFWEW::Window::Instance();
			if (window.GetGamePad().buttonDown&GamePad::START) {
				SceneStack::Instance().Replace(std::make_shared<GameOverScene>());
			}
		}
	}
}
/**
*シーンを更新する
*/
void MainGameScene::Update(float deltaTime)
{
	
	spriteRenderer.BeginUpdate();
	for (const Sprite&e : sprites) {
		spriteRenderer.AddVertices(e);
	}
	spriteRenderer.EndUpdate();

	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float w = window.Width();
	const float h = window.Height();
	const float lineHeight = fontRenderer.LineHeight();

	// カメラの状態を更新.
	if (dot(camera.velocity, camera.velocity)) {
		camera.target += camera.velocity * deltaTime;
		camera.target.y = heightMap.Height(camera.target);
		camera.position = camera.target + glm::vec3(0, 50, 50);

	}


	fontRenderer.BeginUpdate();
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), glm::vec4(1, 1, 0, 1), L"メイン画面");
	fontRenderer.AddString(glm::vec2(-128, 0),glm::vec4(1,1,0,1 ),L"メインゲーム");
	fontRenderer.EndUpdate();

}
/**
*シーンを描画する
*/

void MainGameScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);

	const glm::mat4 matView = glm::lookAt(camera.position, camera.target, camera.up);

	const float aspectRatio =
		static_cast<float>(window.Width()) / static_cast<float>(window.Height());
	const glm::mat4 matProj =
		glm::perspective(glm::radians(30.0f), aspectRatio, 1.0f, 1000.0f);
	glm::vec3 cubePos(100, 0, 100);
	cubePos.y = heightMap.Height(cubePos);
	const glm::mat4 matModel = glm::translate(glm::mat4(1), cubePos);
	Mesh::Draw(meshBuffer.GetFile("Cube"), matProj * matView, matModel);
	Mesh::Draw(meshBuffer.GetFile("Terrain"), matProj * matView, glm::mat4(1));

	glm::vec3 treePos(110, 0, 110);
	treePos.y = heightMap.Height(treePos);
	const glm::mat4 matTreeModel = glm::translate(glm::mat4(1), treePos) * glm::scale(glm::mat4(1), glm::vec3(3));
	Mesh::Draw(meshBuffer.GetFile("Res/red_pine_tree.gltf"), matProj * matView, matTreeModel);

}