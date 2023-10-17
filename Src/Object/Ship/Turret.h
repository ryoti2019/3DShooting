#pragma once
#include <vector>
#include "../../Object/Common/Transform.h"

class Turret
{

public:

	// タレットの大きさ
	static constexpr float SCALE = 80.0f;

	// 状態
	enum class STATE
	{
		NONE,
		ATTACK,
		DESTROY
	};

	// コンストラクタ
	Turret(const Transform& transformParent,
		VECTOR localPos, VECTOR localRot);

	// デストラクタ
	~Turret(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// モデル制御の基本情報
	const Transform& transformParent_;

	// モデル制御の基本情報(砲台)
	Transform transformStand_;

	// モデル制御の基本情報(砲身)
	Transform transformBarrel_;

	// 状態
	STATE state_;

	// 親(戦艦)からの相対座標
	VECTOR localPos_;

	// 親(戦艦)からの相対角度
	VECTOR localRot_;

	// 耐久力
	int hp_;

	// 状態遷移
	void ChangeState(STATE state);

	void UpdateAttack(void);
	void UpdateDestroy(void);
	void DrawAttack(void);
	void DrawDestroy(void);

	// 親(戦艦)との回転と位置の同期
	void SyncParent(Transform& transform);

};

