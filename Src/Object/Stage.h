#pragma once
#include <DxLib.h>
#include "Common/Transform.h"

class Stage
{

public:

	// コンストラクタ
	Stage(void);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// ステージの取得
	int GetModelIdStage(void);

	// ボスの取得
	Transform GetBossTransform(void);

private:

	// ステージ
	Transform stage_;

	// ボス戦艦
	Transform bossShip_;

};

