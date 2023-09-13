#pragma once
#include "../Object/Common/Transform.h"
class SpaceDome
{

public:

	// コンストラクタ
	SpaceDome(void);

	// デストラクタ
	~SpaceDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	
	// モデル制御の基本情報
	Transform transform_;

};

