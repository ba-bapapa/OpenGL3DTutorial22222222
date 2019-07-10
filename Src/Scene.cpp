/**
*@file Scene.cpp
*/
#include "Scene.h"
#include <iostream>
/**
*コンストラクタ
*/
Scene::Scene(const char*name) :name(name) {
	std::cout << "Scene コンストラクタ:" << name << "\n";

}

/**
*デストラクタ
*/
Scene::~Scene() {
	Finalize();
	std::cout << "Scene デストラクタ:" << name << "\n";
}

/**
*シーンを活動状態にする
*/
void Scene::Play() {
	isActive = true;
	std::cout << "Scene Play:" << name << "\n";
}

/**
*シーンを停止状態にする
*/
void Scene::Stop() {
	isActive = false;
	std::cout << "Scene Stop:" << name << "\n";
}

/**
*シーンを表示する
*/
void Scene::Show() {
	isVisible = true;
	std::cout << "Scene Show:" << name << "\n";
}

/**
*シーンを非表示にする
*/
void Scene::Hide() {
	isVisible = false;
	std::cout << "Scene Hide:" << name << "\n";
}

/**
*シーンを取得する
*/
const std::string&Scene::Name()const {
	return name;
}

/**
*シーンの活動状態を調べる
*/
bool Scene::IsActive() const {
	return isActive;
}

/**
*シーンの表示状態を調べる
*/
bool Scene::IsVisible()const {
	return isVisible;
}

/**
*シーンスタックを取得する
*
*@return シーンスタック
*/

SceneStack&SceneStack::Instance() {
	static SceneStack instance;
	return instance;
}

/**
*コンストラクタ
*/
SceneStack::SceneStack() {
	stack.reserve(16);
}

/**
*シーンをプッシュする
*
*@param p　新しいシーン
*/
void SceneStack::push(ScenePtr p) {
	if (!stack.empty()) {
		Current().Stop();
	}
	stack.push_back(p);
	std::cout << "[シーン プッシュ]" << p->Name() << "\n";
	Current().Initialize();
	Current().Play();
}

/**
*シーンをポップする
*/
void SceneStack::pop() {
	if (stack.empty()) {
		std::cout << "[シーンポップ][警告]シーンスタックが空です.\n";
		return;
	}
	Current().Stop();
	Current().Finalize();
	const std::string sceneName = Current().Name();
	stack.pop_back();
	std::cout << "[シーンポップ]" << sceneName << "\n";
	if (!stack.empty()) {
		Current().Play();
	}
}

/**
*シーンを置き換える
*@param p 新しいシーン
*/
void SceneStack::Replace(ScenePtr p) {
	std::string sceneName = "(Empty)";
	if (stack.empty()) {
		std::cout << "[シーンリプレース][警告]シーンスタックが空です.\n";
	}
	else {
		sceneName = Current().Name();
		Current().Stop();
		Current().Finalize();
		stack.pop_back();
	}
	stack.push_back(p);
	std::cout << "[シーンリプレース]" << sceneName << "->" << p->Name() << "\n";
	Current().Initialize();
	Current().Play();
}
/**
*現在のシーンを取得する
*/

Scene&SceneStack::Current() {
	return*stack.back();

}
/**
*現在のシーンを取得する
*/
const Scene&SceneStack::Current() const {
	return*stack.back();
}

/**
*シーンの数を取得する
*/

size_t SceneStack::Size() const {
	return stack.size();

}

/**
*スタックが空かどうか調べる
*/
bool SceneStack::Empty() const {
	return stack.empty();
}

/**
*シーンを更新する
*/
void SceneStack::Update(float deltaTime) {
	//for (ScenePtr&e : stack) {
		//e->ProcessInput();
	//}
	if (!Empty())
	{
		Current().ProcessInput();
	}
	for (ScenePtr&e : stack) {
		e->Update(deltaTime);
	}
}

/**
*シーンを描画する
*/
void SceneStack::Render() {
	for (ScenePtr&e:stack) {
		if (e->IsVisible()) {
			e->Render();
		}
	}
}