#pragma once
#include "SceneBase.h"

class Grid;
class SpaceDome;
class PlayerShip;
class RockManager;

class GameScene : public SceneBase
{

public:

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

	// 背景のスペースデブリ
	RockManager* rockManager_;

};
