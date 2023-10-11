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

};

