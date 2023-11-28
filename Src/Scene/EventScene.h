#pragma once
#include <vector>
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SpaceDome;
class BossShip;
class ShotEvent;

class EventScene : public SceneBase
{

public:

	// ポリゴンを斜めに動かすときのY座標の強さ
	static constexpr int POLYGON_X_POW = 3;

	// ポリゴンを斜めに動かすときのY座標の強さ
	static constexpr int POLYGON_Y_POW = 10;

	// コンストラクタ
	EventScene(void);

	// デストラクタ
	~EventScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// 割れたかどうかのフラグの設定
	void SetIsBreak(bool isBreak);

private:

	// スカイドーム用
	Transform spaceDomeTran_;

	// スカイドーム
	SpaceDome* spaceDome_;

	// ボス
	BossShip* bossShip_;

	// イベント用
	ShotEvent* shotEvent_;

	// 頂点情報の構造体
	std::vector<VERTEX2D> vertexs_;

	std::vector<VECTOR> polygonCenterPos_;

	// ポリゴンが動く方向
	VECTOR polygonDir_;

	// ポリゴンが動く速さ
	float polygonSpeed_;

	// 重力加速度
	float gravityPow_;

	// スクリーンに描画してあるテクスチャ
	int breakScreen_;

	// 壊れた時のフラグ
	bool isBreak_;

	// スクリーンを割る
	void MakebreakScreen(void);

};

