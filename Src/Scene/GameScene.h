#pragma once
#include "SceneBase.h"

class Grid;
class SpaceDome;
class PlayerShip;
class RockManager;
class Stage;

class GameScene : public SceneBase
{

public:

	// 自機破壊時からのリスタート
	static constexpr float TIME_RESTART = 2.0f;

	// 戦艦を中心としたイベントシーン突入エリアの距離
	static constexpr float DIS_EVENT_AREA = 8000.0f;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// 自機破壊時間演出時間の取得
	float GetStepShipDestroy(void);

private:

	// スカイドーム
	SpaceDome* spaceDome_;

	// グリッド線
	Grid* grid_;

	// プレイヤー
	PlayerShip* playerShip_;

	// 背景のスペースデブリ
	RockManager* rockManager_;

	// ステージ
	Stage* stage_;

	// 自機破壊演出時間
	float stepShipDestroy_;

	// 衝突判定
	void Collision(void);

};
