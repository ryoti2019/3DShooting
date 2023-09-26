#include "../Manager/ResourceManager.h"
#include "../Common/IntVector3.h"
#include "../Utility/AsoUtility.h"
#include "Common/Transform.h"
#include "Rock.h"
#include "RockManager.h"

RockManager::RockManager(const Transform& target) : target_(target)
{
	// 監視対象を設定
}

RockManager::~RockManager(void)
{
}

void RockManager::Init(void)
{
}

void RockManager::Update(void)
{
}

void RockManager::Draw(void)
{

	// マップ連想配列のループ
	// 連想配列内の要素を範囲for文で取り出す
	// 取り出した要素は、キーとバリューとで２種類ある(pair)
	for (const auto& p : mapRocks_)
	{
		// map連想配列から取り出した要素(pair)は、
		// firstでキーにアクセス
		// secondで値(バリュー)にアクセス
		// secondには、Rockクラスの生ポインタ配列が入っているので、
		// ２重ループ
		for (const auto rock : p.second)
		{
			rock->Draw();
		}
	}
	
	// デバッグ用
	DrawDebugMap();

}

void RockManager::Release(void)
{

	for (auto& p : mapRocks_)
	{
		for (auto rock : p.second)
		{
			rock->Release();
			delete rock;
		}
		p.second.clear();
	}
	mapRocks_.clear();

}

Rock* RockManager::CreateRandom(IntVector3 mapPos)
{

	// 岩のモデルをランダムに決める
	int modelId;

	int rand = GetRand(2);
	if (rand == 0)
	{
		modelId = static_cast<int>(ResourceManager::SRC::ROCK01);
	}
	if (rand == 1)
	{
		modelId = static_cast<int>(ResourceManager::SRC::ROCK02);
	}

	// 位置をランダムに
	// ワールド座標に変換
	VECTOR pos;



	// 角度をランダムに
	VECTOR angles;

	// 大きさをランダムに
	VECTOR scale;

	// 生成したRockクラスのインスタンス&初期化を行い、返り値に返す




	return nullptr;
}

void RockManager::DrawDebugMap(void)
{
	// デブリマップ
	int num = 4;
	float inumLen;
	float jnumLen;
	float lineS;
	float lineE;
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			lineS = 0.0f;
			lineE = static_cast<float>(MAP_SIZE * (num - 1));
			inumLen = static_cast<float>(i * MAP_SIZE);
			jnumLen = static_cast<float>(j * MAP_SIZE);
			DrawLine3D({ lineS, inumLen, jnumLen },
				{ lineE, inumLen, jnumLen }, 0xffffff);
			DrawLine3D({ inumLen, lineS, jnumLen },
			{ inumLen, lineE, jnumLen }, 0xffffff);
			DrawLine3D({ inumLen, jnumLen, lineS },
				{ inumLen, jnumLen, lineE }, 0xffffff);
		}
	}
	float p = static_cast<float>((MAP_SIZE * (num - 1)) / 2);
	DrawSphere3D({ p, p, p }, 100, 10, 0xff0000, 0xff0000, true);

}
