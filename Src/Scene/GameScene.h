#pragma once
#include "SceneBase.h"

class Grid;
class SpaceDome;
class PlayerShip;
class NPC;
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

private:

	// スカイドーム
	SpaceDome* spaceDome_;

	// グリッド線
	Grid* grid_;

	// プレイヤー
	PlayerShip* playerShip_;

	// NPC機
	NPC* npc_;

	// 背景のスペースデブリ
	RockManager* rockManager_;

	// ステージ
	Stage* stage_;

	// 自機破壊演出時間
	float stepShipDestroy_;

	// 赤の強度（初期値は最大）
	int redIntensity_;

	// 減らす数
	int delta_;

	// 衝突判定
	void Collision(void);

};
