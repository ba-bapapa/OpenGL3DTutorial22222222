#include "TitleScene.h"
#include"MainGameScene.h"
#include"GLFWEW.h"


/**
*シーンを初期化する
*/
bool TitleScene::Initialize() {
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/TitleBg.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);

	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");


	return true;
}

void TitleScene::ProcessInput() {
	GLFWEW::Window&window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown&GamePad::START) {
		SceneStack::Instance().Replace(std::make_shared<MainGameScene>());
	}
}

/**
*シーンを更新する
*/
void TitleScene::Update(float deltaTime) {
	spriteRenderer.BeginUpdate();
	for (const Sprite&e : sprites) {
		spriteRenderer.AddVertices(e);

		const GLFWEW::Window& window = GLFWEW::Window::Instance();
		const float w = window.Width();
		const float h = window.Height();
		const float lineHeight = fontRenderer.LineHeight();
		fontRenderer.BeginUpdate();
		fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), glm::vec4(1, 1, 0, 1), L"タイトル画面");
		fontRenderer.AddString(glm::vec2(-128, 0), glm::vec4(1, 1, 0, 1), L"アクションゲーム");
		fontRenderer.EndUpdate();

	}
	spriteRenderer.EndUpdate();
}
/**
*シーンを描画する
*/

void TitleScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);

	

	}