#pragma once
#include "ShotBase.h"
class Transform;

class ShotEvent : public ShotBase
{

public:

	// コンストラクタ
	ShotEvent(void);
	// デストラクタ
	~ShotEvent(void);

	ShotBase::STATE GetState(void);

protected:

	// パラメータ設定
	void SetParam(void) override;

	// 生存チェック
	void CheckAlive(void) override;

};
