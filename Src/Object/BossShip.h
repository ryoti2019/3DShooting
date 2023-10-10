#pragma once
#include "../Object/Common/Transform.h"
class GameScene;

class BossShip
{

public:

	// 移動スピード
		static constexpr float SPEED_MOVE = 4.0f;

	// コンストラクタ
	BossShip(void);

	// デストラクタ
	~BossShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// モデル制御の基本情報
	Transform transform_;

};


