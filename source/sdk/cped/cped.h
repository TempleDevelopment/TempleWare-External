#pragma once
#include "../GameSDK/GameSDK.h"

class CPed
{
public:
	// Pointer
    uintptr_t Ped;
	uintptr_t Vehicle;
	uintptr_t PlayerInfo;
	uintptr_t CurrentWeapon;

	int Index;
	float m_fArmor;
	float m_fHealth;
	float m_fMaxHealth;
	Vector3 m_pVecLocation;
	Vector3 m_pVecVelocity;
	Vector3 BoneList[9];
	Matrix m_bMatrix;

	bool GetPlayer(uintptr_t& address);
	bool Update();
	bool IsDead();
	bool IsPlayer();
	bool InVehicle();
	bool IsGod();

	void GetBoneList();
	Vector3 GetVelocity();
};