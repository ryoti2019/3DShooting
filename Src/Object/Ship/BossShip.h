#pragma once
#include "../Common/Transform.h"
class GameScene;

class BossShip
{

public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 4.0f;

	// 旋回スピード
	static constexpr float ROT_POW_DEG = 3.0f;

	// 状態
	enum class STATE
	{
		NONE,
		EVENT,
		BATTLE,
		DESTROY,
		END,
	};

	// コンストラクタ
	BossShip(void);

	// デストラクタ
	~BossShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	Transform GetTransform(void);

	// 生存判定
	bool IsAlive(void) const;

private:

	// モデル制御の基本情報
	Transform transform_;

	// 状態
	STATE state_;
	
	// 状態遷移
	void ChangeState(STATE state);

	// 前方移動
	void MoveForward(void);

	// 旋回
	void Rotation(void);

};


