#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class SpaceDome;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// タイトルロゴ
	int imgTitleLogo_;

	Transform spaceDomeTrans_;

	// スカイドーム
	SpaceDome* spaceDome_;

	// ロゴ描画
	void DrawLogo(void);

};
