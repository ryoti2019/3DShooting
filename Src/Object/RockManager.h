#pragma once
#include <vector>
#include <map>
#include <DxLib.h>
#include "../Common/IntVector3.h"

class Rock;
class ResourceManager;
class Transform;

class RockManager
{

public:

	// 1マップあたりのサイズ
	static constexpr int MAP_SIZE = 2000;

	// 1マップあたりに生成する岩の数
	static constexpr int NUM_CREATE = 15;

	// コンストラクタ
	RockManager(const Transform& target);

	// デストラクタ
	~RockManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// 監視対象
	const Transform& target_;
	
	// 岩のポインタ
	Rock* rock_;

	// マップ管理
	// キー		：マップ座標
	// バリュー	：岩の配列(15個分)
	std::map<IntVector3, std::vector<Rock*>> mapRocks_;

	// {0,0,0} =>岩A、岩B、岩C
	// {1,0,0} =>岩X、岩Y、岩Z

	// ランダム生成
	Rock* CreateRandom(IntVector3 mapPos);

	// デバッグ用マップ表示
	void DrawDebugMap(void);

};

