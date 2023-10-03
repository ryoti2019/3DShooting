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

private:

	// ステージ
	Transform stage_;

	// ボス戦艦
	Transform bossShip_;

};

