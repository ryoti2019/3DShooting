#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Common/Quaternion.h"
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"
#include "Turret.h"

Turret::Turret(const Transform& transformParent,
	VECTOR localPos, VECTOR localRot) : transformParent_(transformParent)
{

	// e(íŠÍ)‚©‚ç‚Ì‘Š‘ÎÀ•W
	localPos_ = localPos;

	// e(íŠÍ)‚©‚ç‚Ì‘Š‘ÎŠp“x
	localRot_ = localRot;

	// ‚R‚cƒ‚ƒfƒ‹‚Ì“Ç
	ResourceManager& ins = ResourceManager::GetInstance();
	transformStand_.SetModel(
		ins.LoadModelDuplicate(ResourceManager::SRC::TURRET_STAND));
	transformBarrel_.SetModel(
		ins.LoadModelDuplicate(ResourceManager::SRC::TURRET_GUN));

	// UŒ‚‚ğ‰Šúó‘Ô‚É‚·‚é
	ChangeState(STATE::ATTACK);

	// –C‘äƒ[ƒJƒ‹‰ñ“]
	localRotAddStand_ = { 0.0f, 0.0f, 0.0f };

	// –Cgƒ[ƒJƒ‹‰ñ“]
	localRotAddBarrel_ = { 0.0f, 0.0f, 0.0f };

	// UŒ‚‚ğ‰Šúó‘Ô‚É‚·‚é
	ChangeState(STATE::ATTACK);

	isStand_ = false;

	isBarrel_ = false;

}

Turret::~Turret(void)
{
	
}

void Turret::Init(void)
{
	// ‘Ï‹v—Í
	hp_ = 2;

#pragma region –C‘ä‚Ìİ’è

	// ƒ‚ƒfƒ‹§Œä‚ÌŠî–{î•ñ
	transformStand_.scl = { SCALE, SCALE, SCALE };

	// ‰ñ“]‚È‚µ
	transformStand_.quaRot = Quaternion();

	// –C‘ä‚Ì‰ñ“]—Ê(deg)
	rotPowStand_ = ROT_POW_STAND;

	// ƒ[ƒJƒ‹‰ñ“](³–Ê‚ğZ‚Ì³•ûŒü‚É‡‚í‚¹‚é)
	transformStand_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// eƒ‚ƒfƒ‹‚Æ“¯Šú
	SyncParent(transformStand_,{ 0.0f, 0.0f, 0.0f });

#pragma endregion

#pragma region –Cg‚Ìİ’è

	// ƒ‚ƒfƒ‹§Œä‚ÌŠî–{î•ñ
	transformBarrel_.scl = { SCALE, SCALE, SCALE };

	// ‰ñ“]‚È‚µ
	transformBarrel_.quaRot = Quaternion();

	// –Cg‚Ì‰ñ“]—Ê(deg)
	rotPowBarrel_ = ROT_POW_GUN;

	// ƒ[ƒJƒ‹‰ñ“](³–Ê‚ğZ‚Ì³•ûŒü‚É‡‚í‚¹‚é)
	transformBarrel_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// eƒ‚ƒfƒ‹‚Æ“¯Šú
	SyncParent(transformBarrel_,{ 0.0f, 0.0f, 0.0f });

#pragma endregion

}

void Turret::Update(void)
{

	switch (state_)
	{
	case Turret::STATE::NONE:
		break;
	case Turret::STATE::ATTACK:
		UpdateAttack();
		break;
	case Turret::STATE::DESTROY:
		UpdateDestroy();
		break;
	}

}

void Turret::UpdateAttack(void)
{

	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	float standDeg;

#pragma region –C‘ä‚Ì‰ñ“]“®ì

	if (isStand_)
	{
		// –C‘ä‚Ì‰Ò“­‰ñ“]
		localRotAddStand_.y += (rotPowStand_ * deltaTime) ;
	}
	else
	{
		// –C‘ä‚Ì‰Ò“­‰ñ“]
		localRotAddStand_.y += (rotPowStand_ * deltaTime)* -1.0f;
	}

	standDeg = AsoUtility::Rad2DegF(localRotAddStand_.y);
	//¦©•ª‚Ål‚¦‚ÄÀ‘•‚µ‚Ä‚İ‚Ü‚µ‚å‚¤
	//‰ñ“]‚ğ”½“] = —Í‚ğ”½“]‚·‚é‚É‚ÍA - 1.0f‚ÅŠ|‚¯Z‚ğ‚·‚é‚Æ—Ç‚¢‚Å‚µ‚å‚¤

	if (ANGLE_Y_MAX_STAND < standDeg)
	{
		isStand_ = false;
	}
	else if (ANGLE_Y_MIN_STAND > standDeg)
	{
		isStand_ = true;
	}

	// –C‘ä‚Ì‰ñ“]‚ÆˆÊ’u‚ğ’²®(‰Ò“­•ª‚Ì‰ñ“]‚ğ‰Á‚¦‚é)
	SyncParent(transformStand_, localRotAddStand_);

#pragma endregion

	float barrelDeg;

#pragma region –Cg‚Ì‰ñ“]“®ì

	if (isBarrel_)
	{
		// –C‘ä‚Ì‰Ò“­‰ñ“]
		localRotAddBarrel_.x += (rotPowBarrel_ * deltaTime);
	}
	else
	{
		// –C‘ä‚Ì‰Ò“­‰ñ“]
		localRotAddBarrel_.x += (rotPowBarrel_ * deltaTime) * -1.0f;
	}

	// –Cg‚Ì‰Ò“­‰ñ“]
	barrelDeg = AsoUtility::Rad2DegF(localRotAddBarrel_.x);
	//¦©•ª‚Ål‚¦‚ÄÀ‘•‚µ‚Ä‚İ‚Ü‚µ‚å‚¤
	//‰ñ“]‚ğ”½“] = —Í‚ğ”½“]‚·‚é‚É‚ÍA - 1.0f‚ÅŠ|‚¯Z‚ğ‚·‚é‚Æ—Ç‚¢‚Å‚µ‚å‚¤

	if (ANGLE_X_MAX_GUN < barrelDeg)
	{
		isBarrel_ = false;
	}
	else if (ANGLE_X_MIN_GUN > barrelDeg)
	{
		isBarrel_ = true;
	}


	// –Cg‚Ì‰ñ“]‚ÆˆÊ’u‚ğ’²®(‰Ò“­•ª‚Ì‰ñ“]‚ğ‰Á‚¦‚é)
	//SyncParent(transformBarrel_, localRotAddBarrel_);

#pragma endregion

	// –C‘ä‚ª‰ñ“]‚µ‚½•ª–Cg‚à‰ñ“]‚·‚é•K—v‚ª‚ ‚é‚Ì‚Å‚»‚Ì‰ñ“]‚ğì‚é
	Quaternion stand = Quaternion::Euler(localRotAddStand_);
	Quaternion gun = Quaternion::Euler(localRotAddBarrel_);
	Quaternion mix = stand.Mult(gun);

	// –C‘ä‚Ì‰ñ“]‚ÆˆÊ’u‚ğ’²®(‰Ò“­•ª‚Ì‰ñ“]‚ğ‰Á‚¦‚é)
	SyncParent(transformBarrel_, mix.ToEuler());

}

void Turret::UpdateDestroy(void)
{
}

void Turret::Draw(void)
{

	switch (state_)
	{
	case Turret::STATE::NONE:
		break;
	case Turret::STATE::ATTACK:
		DrawAttack();
		break;
	case Turret::STATE::DESTROY:
		DrawDestroy();
		break;
	}

}

void Turret::DrawAttack(void)
{

	// ‚R‚cƒ‚ƒfƒ‹‚Ì•`‰æ
	MV1DrawModel(transformStand_.modelId);
	MV1DrawModel(transformBarrel_.modelId);

}

void Turret::DrawDestroy(void)
{
}

void Turret::SyncParent(Transform& transform, VECTOR addAxis)
{

	// e(íŠÍ)‚Ì‰ñ“]î•ñ‚ğæ“¾
	Quaternion parentRot = transformParent_.quaRot;

#pragma region •â‘«
	// –C‘ä‚Ìƒ[ƒJƒ‹‰ñ“]
	// Unity Z¨X¨Y = Y * X * Z
	//axis = Quaternion::AngleAxis(localRotAdd_.y, AsoUtility::AXIS_Y);
	//localRot = localRot.Mult(axis);

	//axis = Quaternion::AngleAxis(localRotAdd_.x, AsoUtility::AXIS_X);
	//localRot = localRot.Mult(axis);

	//axis = Quaternion::AngleAxis(localRotAdd_.z, AsoUtility::AXIS_Z);
	//localRot = localRot.Mult(axis);

#pragma endregion

	// e(íŠÍ)‰ñ“]‚©‚ç‚Ì‘Š‘Î‰ñ“]
	Quaternion rot = Quaternion::Identity();
	rot = rot.Mult(Quaternion::Euler(localRot_));

	// ‰Ò“­•ª‚Ì‰ñ“]‚ğ‰Á‚¦‚é
	rot = rot.Mult(Quaternion::Euler(addAxis));

	// e(íŠÍ)‚Ì‰ñ“]‚Æ‘Š‘Î‰ñ“]‚ğ‡¬
	transform.quaRot = parentRot.Mult(rot);

	// e(íŠÍ)‚Æ‚Ì‘Š‘ÎÀ•W‚ğAe(íŠÍ)‚Ì‰ñ“]î•ñ‚É‡‚í‚¹‚Ä‰ñ“]
	VECTOR localRotPos = Quaternion::PosAxis(parentRot, localPos_);

	// ‘Š‘ÎÀ•W‚ğƒ[ƒ‹ƒhÀ•W‚É’¼‚·
	transform.pos = VAdd(transformParent_.pos, VScale(localRotPos, SCALE));

	// ƒ‚ƒfƒ‹§Œä‚ÌŠî–{î•ñXV
	transform.Update();

}

void Turret::Release(void)
{
}

void Turret::ChangeState(STATE state)
{

	// ó‘Ô‚Ì•ÏX
	state_ = state;
	// ó‘Ô‘JˆÚ‚Ì‰Šú‰»ˆ—
	switch (state_)
	{
	case Turret::STATE::NONE:
		break;
	case Turret::STATE::ATTACK:
		break;
	case Turret::STATE::DESTROY:
		break;
	}

}
