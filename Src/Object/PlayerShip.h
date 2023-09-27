#pragma once
#include "Common/Transform.h"

class PlayerShip
{

public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 10.0f;

	// 自機の回転量
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	// コンストラクタ
	PlayerShip(void);

	// デストラクタ
	~PlayerShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// プレイヤーのTransformの取得
	const Transform& GetTransform(void) const;

private:

	// モデル制御の基本情報
	Transform transform_;

	// 操作
	void ProcessTurn(void);

	void Turn(float deg, VECTOR axis);

};

