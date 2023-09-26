#pragma once
#include <DxLib.h>
#include "Common/Transform.h"

class Rock
{

public:

	// コンストラクタ
	Rock(void);

	// デストラクタ
	~Rock(void);

	// 初期処理
	void Init(int modelId, VECTOR pos, VECTOR angle, VECTOR scale);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// モデル制御の基本情報
	Transform transform_;

};
