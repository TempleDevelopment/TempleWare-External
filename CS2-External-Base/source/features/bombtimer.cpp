#include "bombtimer.h"
#include "../offsets/globals.h"
#include "../offsets/offsets.h"
#include <string>
namespace features
{
	bool isPlanted = false;
	std::time_t plantTime;

	uint64_t currentTimeMillis() {
		using namespace std::chrono;
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}

	int getBombSite(bool Planted, const Memory& memory)
	{
		if (Planted)
		{
			uintptr_t cPlantedC4 = memory.Read<std::uintptr_t>(globals::client + offsets::dwPlantedC4);
			if (!cPlantedC4)
				return 0;

			int site = memory.Read<std::uintptr_t>(cPlantedC4 + offsets::m_nBombSite);
			if (!site)
				return 0;

			return site;
		}
		else
			return 0;

	}

	void BombTimer::Run(const Memory& memory) noexcept
	{
		if (!globals::BombTimerEnabled)
			return;

		auto plantedAddress = globals::client + offsets::dwPlantedC4 - 0x8;

		bool isBombPlanted = memory.Read<int>(plantedAddress);

		auto time = currentTimeMillis();

		if (isBombPlanted && !isPlanted && (plantTime == NULL || time - plantTime > 60000))
		{
			isPlanted = true;
			plantTime = time;
		}

		if (!isPlanted)
		{
			return;
		}

		static float windowWidth = 200.0f;
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
		ImGui::SetNextWindowPos({ (ImGui::GetIO().DisplaySize.x - 200.0f) / 2.0f, 80.0f }, ImGuiCond_Once);
		ImGui::SetNextWindowSize({ windowWidth, 0 }, ImGuiCond_Once);
		ImGui::Begin("Bomb Timer", nullptr, flags);

		float remaining = (40000 - (int64_t)time + plantTime) / (float)1000;

		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 180) * 0.5f);
		float barLength = remaining <= 0.0f ? 0.0f : remaining >= 40 ? 1.0f : (remaining / 40.0f);

		if (isPlanted && remaining >= 0)
		{
			std::string bombSite;
			if (!getBombSite(isBombPlanted, memory))
				bombSite = "A: ";
			else
				bombSite = "B: ";

			std::string bombText = "Bomb on " + bombSite + std::to_string(remaining).c_str() + " s";
			ImGui::Text(bombText.c_str());
		}
		else {
			ImGui::Text("C4 not planted", true);
		}

		if (isPlanted && !isBombPlanted)
		{
			isPlanted = false;
		}
		ImGui::End();
	}
}