#include "../core/framecore.h"

int bone = 0;
Vector3 GetPrediction(CPed& target, CPed& local);

bool CFramework::Init()
{
    std::thread([&]() { this->RefreshEntityData(); }).detach();

    return sdk.InitOffset();
}

bool CFramework::AimBot(CPed& target)
{

    if (g.AimKey0 == NULL || !IsKeyDown(g.AimKey0) && !IsKeyDown(g.AimKey1) || g.MenuVisible)
        return false;

    switch (g.AimKeyType)
    {
    case 0:
        if (g.AimKey1 == NULL && IsKeyDown(g.AimKey0))
            break;
        else if (!IsKeyDown(g.AimKey0) || !IsKeyDown(g.AimKey1))
            return false;
        else if (!IsKeyDown(g.AimKey0))
            return false;
        break;
    case 1:
        if (g.AimKey1 == NULL && IsKeyDown(g.AimKey0))
            break;
        else if (IsKeyDown(g.AimKey0) || IsKeyDown(g.AimKey1))
            break;

        break;
    }

    uintptr_t camera = m.Read<uintptr_t>(m.g_BaseAddress + sdk.Camera);
    Vector3 ViewAngle = m.Read<Vector3>(camera + 0x3D0);
    Vector3 CameraPos = m.Read<Vector3>(camera + 0x60);

    switch (g.Aim_Bone)
    {
    case 0: bone = 0; break;
    case 1: bone = 7; break;
    case 2: bone = 8; break;
    }

    Vector3 TargetPos = g.Aim_Prediction ? target.BoneList[bone] + GetPrediction(target, local) : target.BoneList[bone];
    Vector3 Angle = CalcAngle(CameraPos, TargetPos);
    Vector3 Delta = Angle - ViewAngle;
    Vector3 WriteAngle = ViewAngle + (Delta / g.Aim_Smooth);

    if (!Vec3_Empty(WriteAngle))
    {
        m.Write<Vector3>(camera + 0x40, WriteAngle);
        m.Write<Vector3>(camera + 0x3D0, WriteAngle);
    }

    target = CPed();
}

void CFramework::MiscAll()
{

    if (g.GodMode && !local.IsGod())
        m.Write<bool>(local.Ped + sdk.m_pGodMode, true);
    else if (!g.GodMode && local.IsGod())
        m.Write<bool>(local.Ped + sdk.m_pGodMode, false);

    if (g.AutoHeal && local.m_fHealth <= local.m_fMaxHealth)
        m.Write<float>(local.Ped + sdk.m_pHealth, local.m_fHealth + 0.2f);

    if (g.NoRecoil && m.Read<float>(local.CurrentWeapon + sdk.m_WepRecoil) > 0.f)
        m.Write<float>(local.CurrentWeapon + sdk.m_WepRecoil, 0.f);

    if (g.NoSpread && m.Read<float>(local.CurrentWeapon + sdk.m_WepSpread) > 0.f)
        m.Write<float>(local.CurrentWeapon + sdk.m_WepSpread, 0.f);
}

void CFramework::RefreshEntityData()
{
    while (g.Run)
    {
        std::vector<CPed> temp_pedlist;

        GameWorld = m.Read<uintptr_t>(m.g_BaseAddress + sdk.GWorld);
        local.Ped = m.Read<uintptr_t>(GameWorld + 0x8);
        ViewPort = m.Read<uintptr_t>(m.g_BaseAddress + sdk.ViewPort);

        uintptr_t ReplayInterface = m.Read<uintptr_t>(m.g_BaseAddress + sdk.ReplayInterface);
        uintptr_t EntityListPtr = m.Read<uintptr_t>(ReplayInterface + 0x18);
        uintptr_t entitylist = m.Read<uintptr_t>(EntityListPtr + 0x100);

        for (int i = 0; i < 256; i++)
        {
            uintptr_t ped = m.Read<uintptr_t>(entitylist + (i * 0x10));
            CPed p = CPed();

            if (ped == local.Ped)
                continue;
            else if (!p.GetPlayer(ped))
                continue;

            if (p.Update()) {
                p.Index = i;
                temp_pedlist.push_back(p);
            }
        }

        this->EntityList = temp_pedlist;
        temp_pedlist.clear();

        Sleep(500);
    }
}

Vector3 GetPrediction(CPed& target, CPed& local)
{
    Vector3 m_pVecVelocity = target.GetVelocity();

    if (Vec3_Empty(m_pVecVelocity))
        return Vector3();

    Vector3 vOut{};
    float distance = GetDistance(target.BoneList[HEAD], local.m_pVecLocation);
    float BulletSpeed = m.Read<float>(local.CurrentWeapon + 0xE0);

    if (BulletSpeed == 0.f)
        return Vector3();

    float time = distance / g.Aim_Predict;
    vOut = m_pVecVelocity * time;

    return vOut;
}
