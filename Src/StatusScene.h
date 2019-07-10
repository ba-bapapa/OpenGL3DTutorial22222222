#ifndef STATUSSCENE_H_INCLUDED
#define STATUSSCENE_H_INCLUDED
#include"Scene.h"
#include"Sprite.h"
#include "Font.h"

#include<vector>

class StatusScene :public Scene {
public:
	StatusScene():Scene("StatusScene"){}
	virtual ~StatusScene() = default;

	virtual bool Initialize() override;
	virtual void ProcessInput() override;
	virtual void Update(float) override ;
	virtual void Render() override;
	virtual void Finalize() override{}

private:
	std::vector<Sprite>sprites;
	SpriteRenderer spriteRenderer;
	FontRenderer fontRenderer;

};
#endif