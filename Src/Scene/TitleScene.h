#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include <vector>
class SpaceDome;

class TitleScene : public SceneBase
{

public:

	// テキストの構造体
	struct TEXT 
	{
		std::vector<int> handle;
		VECTOR pos;
	};

	// テキストの数
	static constexpr int NUM_TEXT = 60;

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

	// スペースドーム
	Transform spaceDomeTrans_;

	// スカイドーム
	SpaceDome* spaceDome_;

	// 構造型の変数
	TEXT text_;

	// ロゴ描画
	void DrawLogo(void);

};