#pragma once
#include "../../utils/Utils.h"

class SDK
{
public:
    // address
    uintptr_t GWorld;
    uintptr_t ReplayInterface;
    uintptr_t ViewPort;
    uintptr_t Camera;

    // pointer
    uintptr_t m_pVehicle = 0xD10;
    uintptr_t m_pInfo = 0x10A8;
    uintptr_t m_pWeaponManager = 0x10B8;

    // offset
    uintptr_t m_bMatrix = 0x60;
    uintptr_t m_pVecLocation = 0x90;
    uintptr_t m_pGodMode = 0x189;
    uintptr_t m_pHealth = 0x280;
    uintptr_t m_pHealthMax = 0x284;
    uintptr_t m_pVelocity = 0x300;
    uintptr_t m_pBoneList = 0x410;
    uintptr_t m_pArmor = 0x150C;
    uintptr_t m_pPedTask = 0x144B;

    // player info
    uintptr_t m_pName = 0xE0;
    uintptr_t m_pWantedLev = 0x8D8;

    // vehicle offset
    uintptr_t m_pVehicleVelocity = 0;
    uintptr_t m_pVehicleHealth = 0;
    uintptr_t m_pVehicleEngineHealth = 0;

    // weapon
    uintptr_t m_WepSpread = 0;
    uintptr_t m_WepDamage = 0;
    uintptr_t m_WepRecoil = 0x2F4;
    uintptr_t m_WepRange = 0x28C;

    bool InitOffset();
};

enum class ePedTask {
    TASK_NONE,
    TASK_FOOT = 1 << 4,
    TASK_UNK = 1 << 5,
    TASK_DRIVING = 1 << 6
};

enum Bone : int
{
    HEAD = 0,
    LEFTFOOT = 1,
    RIGHTFOOT = 2,
    LEFTANKLE = 3,
    RIGHTANKLE = 4,
    LEFTHAND = 5,
    RIGHTHAND = 6,
    NECK = 7,
    HIP = 8
};

extern struct CBone
{
    Vector3 gHead;      // 0
    char ped_0[0x4]{};
    Vector3 gLeftFoot;  // 1
    char ped_1[0x4]{};
    Vector3 gRightFoot; // 2
    char ped_2[0x4]{};
    Vector3 gLeftAnkle; // 3
    char ped_3[0x4]{};
    Vector3 gRightAnkle;// 4
    char ped_4[0x4]{};
    Vector3 gLeftHand;  // 5
    char ped_5[0x4]{};
    Vector3 gRightHand; // 6
    char ped_6[0x4]{};
    Vector3 gNeck;      // 7
    char ped_7[0x4]{};
    Vector3 gHip;       // 8
};

extern SDK sdk;
extern bool Vec3_Empty(const Vector3& value);
extern Vector3 Vec3_Transform(Vector3* vIn, Matrix* mIn);
extern Vector3 CalcAngle(Vector3 local_cam, Vector3 to_point);
extern float GetSpeed(Vector3 value);
extern float GetDistance(Vector3 value1, Vector3 value2);
extern bool WorldToScreen(Matrix ViewMatrix, Vector3& vWorld, Vector2& vOut);
