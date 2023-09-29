#include <DxLib.h>
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

	// 監視対象の座標をマップ座標に変換
	VECTOR monitorMapPos = target_.pos;
	monitorMapPos.x /= MAP_SIZE;
	monitorMapPos.y /= MAP_SIZE;
	monitorMapPos.z /= MAP_SIZE;

	// 変換されたマップ座標を中心に27マップ検索
	IntVector3 startMapPos = IntVector3(monitorMapPos);

	// 岩が生成されているか確認する用(マップ座標)
	IntVector3 checkMapPos;
	startMapPos.Sub(1);


	// 27マップ分のループ
	const int loop = 3;
	for (int x = 0; x < loop; x++)
	{
		for (int y = 0; y < loop; y++)
		{
			for (int z = 0; z < loop; z++)
			{

				// チェックするマップ座標を作る
				checkMapPos = { x + startMapPos.x,
								y + startMapPos.y,
								z + startMapPos.z };

				// 既に岩を生成しているマップ座標かチェック
				if (mapRocks_.count(checkMapPos) == 0)
				{

					std::vector<Rock*>tmpRocks;


					// 岩を指定個数分、ランダムに生成する
					for (int i = 0; i < NUM_CREATE; i++)
					{
						// 岩を1つ生成する
						tmpRocks.emplace_back(CreateRandom(checkMapPos));
					}

					// マップ連想配列にキーとバリューをセットに格納
					// マップ管理配列に、マップ座標と生成した複数の岩を格納
					mapRocks_.emplace(checkMapPos, tmpRocks);

				}

			}
		}
	}

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

	// 岩を1個作る

	// 岩のモデルをランダムに決める
	int modelId = 0;

	int Rand = GetRand(2);
	if (Rand == 0)
	{
		modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ROCK01);
	}
	if (Rand == 1)
	{
		modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ROCK02);
	}

	// 位置をランダムに
	// ワールド座標に変換
	int randPosX = rand();
	int randPosY = rand();
	int randPosZ = rand();
	VECTOR pos;
	pos = { mapPos.x,mapPos.y,mapPos.z };
	//pos = { 0.0f,0.0f,0.0f };
	// 角度をランダムに
	int randAngleX = rand();
	int randAngleY = rand();
	int randAngleZ = rand();
	VECTOR angles;
	angles = { 1.0f * randAngleX,1.0f * randAngleY,1.0f * randAngleZ };
	//angles = { 1.0f,1.0f,1.0f };
	// 大きさをランダムに
	int randScaleX = rand();
	int randScaleY = rand();
	int randScaleZ = rand();
	VECTOR scale;
	scale = { 10.0f ,10.0f ,10.0f  };
	//scale = { 10.0f,10.0f,10.0f };
	// 生成したRockクラスのインスタンス&初期化を行い、返り値に返す
	Rock* ret = new Rock();
	ret->Init(modelId, pos, angles, scale);

	return ret;

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
