#pragma once
#include <vector>
#include "SceneBase.h"
class RockManager;
class SpaceDome;
class PlayerShip;
class BossShip;

class BattleScene : public SceneBase
{

public:

	// 自機破壊時からのリスタート
	static constexpr float TIME_RESTART = 2.0f;

	// コンストラクタ
	BattleScene(void);

	// デストラクタ
	~BattleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// 背景岩
	RockManager* rockManager_;

	// スカイドーム
	SpaceDome* spaceDome_;

	// 自機
	PlayerShip* playerShip_;

	// ボス戦艦
	BossShip * bossShip_;

	// 自機の破壊演出時間
	float stepShipDestroy_;

	// 衝突判定
	void Collision(void);

};

