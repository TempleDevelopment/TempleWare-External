#include "CPed.h"

bool CPed::GetPlayer(uintptr_t& address)
{
	Ped = address;
	return Ped == NULL ? false : true;
}

bool CPed::Update()
{
    Vehicle = m.Read<uintptr_t>(Ped + sdk.m_pVehicle);
    PlayerInfo = m.Read<uintptr_t>(Ped + sdk.m_pInfo);
    uintptr_t weapon = m.Read<uintptr_t>(Ped + sdk.m_pWeaponManager);
    CurrentWeapon = m.Read<uintptr_t>(weapon + 0x20);

    // PedData
    m_fHealth = m.Read<float>(Ped + sdk.m_pHealth);
    m_pVecLocation = m.Read<Vector3>(Ped + sdk.m_pVecLocation);

    if (IsDead())
        return false;

    m_fArmor = m.Read<float>(Ped + sdk.m_pArmor);
    m_fMaxHealth = m.Read<float>(Ped + sdk.m_pHealthMax);
    m_bMatrix = m.Read<Matrix>(Ped + sdk.m_bMatrix);
    GetBoneList();

    return true;
}

bool CPed::IsDead()
{
    return m_fHealth <= 0.f || Vec3_Empty(m_pVecLocation);
}

bool CPed::IsPlayer()
{
    return PlayerInfo != NULL;
}

bool CPed::InVehicle()
{
    return m.Read<uint8_t>(Ped + sdk.m_pPedTask) & (uint8_t)ePedTask::TASK_DRIVING;
}

bool CPed::IsGod()
{
    return m.Read<bool>(Ped + sdk.m_pGodMode) == true;
}

void CPed::GetBoneList()
{
    CBone bone = m.Read<CBone>(Ped + sdk.m_pBoneList);
    BoneList[HEAD] = Vec3_Transform(&bone.gHead, &m_bMatrix);
    BoneList[LEFTFOOT] = Vec3_Transform(&bone.gLeftFoot, &m_bMatrix);
    BoneList[RIGHTFOOT] = Vec3_Transform(&bone.gRightFoot, &m_bMatrix);
    BoneList[LEFTANKLE] = Vec3_Transform(&bone.gLeftAnkle, &m_bMatrix);
    BoneList[RIGHTANKLE] = Vec3_Transform(&bone.gRightAnkle, &m_bMatrix);
    BoneList[LEFTHAND] = Vec3_Transform(&bone.gLeftHand, &m_bMatrix);
    BoneList[RIGHTHAND] = Vec3_Transform(&bone.gRightHand, &m_bMatrix);
    BoneList[NECK] = Vec3_Transform(&bone.gNeck, &m_bMatrix);
    BoneList[HIP] = Vec3_Transform(&bone.gHip, &m_bMatrix);
}

Vector3 CPed::GetVelocity()
{
    return InVehicle() ? m.Read<Vector3>(Vehicle + sdk.m_pVehicleVelocity) : m.Read<Vector3>(Ped + sdk.m_pVelocity);
}