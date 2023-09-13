#pragma once
#include "Common/Transform.h"
class PlayerShip
{

public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 10.0f;

	// コンストラクタ
	PlayerShip(void);

	// デストラクタ
	~PlayerShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// モデル制御の基本情報
	Transform transform_;

	// テスト
};

