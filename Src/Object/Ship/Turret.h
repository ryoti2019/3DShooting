#pragma once
#include <vector>
#include "../../Object/Common/Transform.h"

class ShotTurret;

class Turret
{

public:

	// タレットの大きさ
	static constexpr float SCALE = 80.0f;

	// 砲台回転動作制限角(deg)
	static constexpr float ANGLE_Y_MIN_STAND = -30.0f;
	static constexpr float ANGLE_Y_MAX_STAND = 30.0f;

	// 砲台の回転量(deg)
	static constexpr float ROT_POW_STAND = 0.2f;

	// 砲身回転動作制限角(deg)
	static constexpr float ANGLE_X_MIN_GUN = -10.0f;
	static constexpr float ANGLE_X_MAX_GUN = 20.0f;
	
	// 砲身の回転量(deg)
	static constexpr float ROT_POW_GUN = 0.2f;

	// 弾の発射間隔
	static constexpr float TIME_DELAY_SHOT = 0.3f;

	// 衝突判定：球体半径
	static constexpr float COLLISION_RADIUS = 200.0f;

	// 破壊エフェクトの表示時間
	static constexpr float TIME_DESTROY = 3.0f;

	// 状態
	enum class STATE
	{
		NONE,
		ATTACK,
		DESTROY,
		END
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

	// 弾
	std::vector<ShotTurret*>& GetShots(void);

	// 砲身のTransformの取得
	const Transform& GetTransformBarrel(void) const;

	// 状態の設定
	void SetState(Turret::STATE state);

	// HPの設定
	void SetHP(int hp);

private:

	// 弾
	std::vector<ShotTurret*> shots_;

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

	// 砲台の回転量(deg)
	float rotPowStand_;

	// 砲身の回転量(deg)
	float rotPowBarrel_;

	// 砲台の回転動作蓄積用
	VECTOR localRotAddStand_;

	VECTOR localRotAddBarrel_;

	// 砲台のフラグ
	bool isStand_;

	// 砲身のフラグ
	bool isBarrel_;

	// 弾の発射間隔
	float delayShot_;

	// 自機破壊エフェクト
	int effectDestroyResId_;
	int effectDestroyPlayId_;

	VECTOR effectDestroyPos_;

	// エフェクトの再生時間
	float effectTime_;

	// エフェクトのフラグ
	bool isEffect_;

	// 状態遷移
	void ChangeState(STATE state);

	void UpdateAttack(void);
	void UpdateDestroy(void);
	void DrawAttack(void);
	void DrawDestroy(void);

	// 親(戦艦)との回転と位置の同期
	void SyncParent(Transform& transform, VECTOR addAxis);

	// 爆破エフェクト制御
	void SyncDestroyEffect(void);

	// 操作：弾発射
	void ProcessShot(void);

	// 自機の弾を発射
	void CreateShot(void);

};

