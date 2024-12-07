/*
 * TempleWare Visual Enhancement System
 * Features:
 * - Performance Monitoring (FPS Counter)
 * - ESP System with Distance-Based Rendering
 * - Advanced Entity Tracking & Visualization
 * - Customizable Visual Elements (Box ESP, Skeleton)
 * - Integrated Aimbot Target Selection
 */

#include "../core/framecore.h"

// ==========================================
// Performance Monitoring System
// Handles FPS display and crosshair rendering
// ==========================================
void CFramework::DrawPerformanceMetrics()
{
    std::string text = std::to_string((int)ImGui::GetIO().Framerate) + "FPS";
    StringEx(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), text.c_str());

    ImGui::GetStyle().AntiAliasedLines = false;

    if (g.AimbotEnabled && g.Aim_DrawFov)
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(g.GameRect.right / 2.f, g.GameRect.bottom / 2.f), g.Aim_Fov, FOV_User);

    if (g.Crosshair)
    {
        switch (g.CrosshairType)
        {
        case 0: {
            ImVec2 Center = ImVec2(g.GameRect.right / 2, g.GameRect.bottom / 2);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x - g.CrosshairSize, Center.y), ImVec2((Center.x + g.CrosshairSize) + 1, Center.y), CrosshairColor, 1);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x, Center.y - g.CrosshairSize), ImVec2(Center.x, (Center.y + g.CrosshairSize) + 1), CrosshairColor, 1);
        }   break;
        case 1:
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.CrosshairSize + 1, ImColor(0.f, 0.f, 0.f, 1.f), NULL);
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.CrosshairSize, CrosshairColor, NULL);
            break;
        }
    }

    ImGui::GetStyle().AntiAliasedLines = true;
}

// ==========================================
// ESP & Entity Tracking System
// Manages entity visualization and targeting
// - Distance-based entity filtering
// - Box ESP with multiple styles
// - Skeleton visualization
// - Health/Armor indicators
// - Integrated aimbot targeting
// ==========================================
void CFramework::DrawEntityOverlays()
{
    CPed* pLocal = &local;
    static CPed target = CPed();

    float MinFov = 9999.f;
    float MinDistance = 9999.f;
    Vector2 Center = Vector2(g.GameRect.right / 2.f, g.GameRect.bottom / 2.f);
    Matrix ViewMatrix = m.Read<Matrix>(ViewPort + 0x24C);

    if (!pLocal->Update())
        return;

    for (auto& entity : EntityList)
    {
        CPed* pEntity = &entity;

        if (!pEntity->Update())
            continue;

        float pDistance = GetDistance(pEntity->m_pVecLocation, pLocal->m_pVecLocation);

        if (pDistance > g.ESP_MaxDistance)
            continue;
        else if (!g.ESP_NPC && !pEntity->IsPlayer())
            continue;

        Vector2 pBase{}, pHead{}, pNeck{}, pLeftFoot{}, pRightFoot{};
        if (!WorldToScreen(ViewMatrix, pEntity->m_pVecLocation, pBase) || !WorldToScreen(ViewMatrix, pEntity->BoneList[HEAD], pHead) || !WorldToScreen(ViewMatrix, pEntity->BoneList[NECK], pNeck) || !WorldToScreen(ViewMatrix, pEntity->BoneList[LEFTFOOT], pLeftFoot) || !WorldToScreen(ViewMatrix, pEntity->BoneList[RIGHTFOOT], pRightFoot))
            continue;

        float HeadToNeck = pNeck.y - pHead.y;
        float pTop = pHead.y - (HeadToNeck * 2.5f);
        float pBottom = pLeftFoot.y > pRightFoot.y ? pLeftFoot.y : pRightFoot.y;
        float pHeight = pBottom - pTop;
        float pWidth = pHeight / 3.5f;
        float bScale = pWidth / 1.5f;
        ImColor color = pEntity->IsPlayer() ? ESP_PLAYER : ESP_NPC;

        if (pEntity->IsGod())
            color = ESP_GOD;

        if (g.ESP_Line)
            DrawLine(ImVec2(g.GameRect.right / 2.f, g.GameRect.bottom), ImVec2(pBase.x, pBottom), color, 1.f);

        if (g.ESP_Box)
        {
            if (g.ESP_BoxFilled)
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(pBase.x - pWidth, pTop), ImVec2(pBase.x + pWidth, pBottom), ESP_Filled);

            switch (g.ESP_BoxType)
            {
            case 0: 
                DrawLine(ImVec2(pBase.x - pWidth, pTop), ImVec2(pBase.x + pWidth, pTop), color, 1.f);
                DrawLine(ImVec2(pBase.x - pWidth, pTop), ImVec2(pBase.x - pWidth, pBottom), color, 1.f);
                DrawLine(ImVec2(pBase.x + pWidth, pTop), ImVec2(pBase.x + pWidth, pBottom), color, 1.f);
                DrawLine(ImVec2(pBase.x - pWidth, pBottom), ImVec2(pBase.x + pWidth, pBottom), color, 1.f);
                break;
            case 1: 
                DrawLine(ImVec2((pBase.x - pWidth), pTop), ImVec2((pBase.x - pWidth) + bScale, pTop), color, 1.f); 
                DrawLine(ImVec2((pBase.x + pWidth), pTop), ImVec2((pBase.x + pWidth) - bScale, pTop), color, 1.f);
                DrawLine(ImVec2(pBase.x - pWidth, pTop), ImVec2(pBase.x - pWidth, pTop + bScale), color, 1.f); 
                DrawLine(ImVec2(pBase.x - pWidth, pBottom), ImVec2(pBase.x - pWidth, pBottom - bScale), color, 1.f);
                DrawLine(ImVec2(pBase.x + pWidth, pTop), ImVec2(pBase.x + pWidth, pTop + bScale), color, 1.f); 
                DrawLine(ImVec2(pBase.x + pWidth, pBottom), ImVec2(pBase.x + pWidth, pBottom - bScale), color, 1.f);
                DrawLine(ImVec2((pBase.x - pWidth), pBottom), ImVec2((pBase.x - pWidth) + bScale, pBottom), color, 1.f); 
                DrawLine(ImVec2((pBase.x + pWidth), pBottom), ImVec2((pBase.x + pWidth) - bScale, pBottom), color, 1.f);
                break;
            default:
                break;
            }
        }

        if (g.ESP_Skeleton)
        {

            Circle(ImVec2(pHead.x, pHead.y), HeadToNeck, ESP_Skeleton);

            Vector3 bList[][2] = { { pEntity->BoneList[NECK], pEntity->BoneList[HIP] }, { pEntity->BoneList[NECK], pEntity->BoneList[LEFTHAND] }, { pEntity->BoneList[NECK], pEntity->BoneList[RIGHTHAND] }, { pEntity->BoneList[HIP], pEntity->BoneList[LEFTANKLE] }, { pEntity->BoneList[HIP], pEntity->BoneList[RIGHTANKLE] } };

            for (int j = 0; j < 5; j++)
            {
                Vector2 ScreenB1{}, ScreenB2{};
                if (Vec3_Empty(bList[j][0]) || Vec3_Empty(bList[j][1]))
                    break;

                if (!WorldToScreen(ViewMatrix, bList[j][0], ScreenB1) || !WorldToScreen(ViewMatrix, bList[j][1], ScreenB2))
                    break;

                DrawLine(ImVec2(ScreenB1.x, ScreenB1.y), ImVec2(ScreenB2.x, ScreenB2.y), ESP_Skeleton, 1.f);
            }
        }

        if (g.ESP_HealthBar)
        {
            HealthBar((pBase.x - pWidth) - 5.f, pBottom, 2.f, -pHeight, pEntity->m_fHealth, pEntity->m_fMaxHealth);

            if (pEntity->m_fArmor > 0.f)
                ArmorBar((pBase.x + pWidth) + 3.f, pBottom, 2.f, -pHeight, pEntity->m_fArmor, 100);
        }

        if (g.ESP_Distance)
        {
            std::string DistStr = std::to_string((int)pDistance) + "m";
            StringEx(ImVec2(pBase.x - ImGui::CalcTextSize(DistStr.c_str()).x / 2.f, pBottom), ImColor(1.f, 1.f, 1.f, 1.f), ImGui::GetFontSize(), DistStr.c_str());
        }

        if (g.ESP_Name)
        {

        }

        if (g.AimbotEnabled)
        {
            if (IsKeyDown(g.AimKey0) || IsKeyDown(g.AimKey1))
            {
                if (g.Aim_MaxDistance < pDistance)
                    continue;
                else if (!g.Aim_NPC && !pEntity->IsPlayer())
                    continue;

                for (int j = 0; j < 9; j++)
                {
                    Vector2 fov_check{};
                    if (!WorldToScreen(ViewMatrix, pEntity->BoneList[j], fov_check))
                        continue;

                    float FOV = abs((Center - fov_check).Length());

                    if (FOV < g.Aim_Fov)
                    {
                        switch (g.Aim_Type)
                        {
                        case 0:
                            if (FOV < MinFov)
                            {
                                target = entity;
                                MinFov = FOV;

                                break;
                            }
                            break;
                        case 1:
                            if (pDistance < MinDistance)
                            {
                                target = entity;
                                MinDistance = pDistance;

                                break;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }

    if (g.AimbotEnabled)
    {
        if (target.Ped != NULL)
            AimBot(target);
    }
}
