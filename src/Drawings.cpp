#include "Drawings.h"

void Drawings::initCirclePoints() {
	vCosTable.resize(circlePoints);
	vSinTable.resize(circlePoints);

	for (auto i = 0; i < circlePoints; i++) {
		vCosTable[i] = std::cosf(static_cast<float>(i) * (std::numbers::pi_v<float> *2.f) / static_cast<float>(circlePoints - 1));
		vSinTable[i] = std::sinf(static_cast<float>(i) * (std::numbers::pi_v<float> *2.f) / static_cast<float>(circlePoints - 1));
	}
}

void Drawings::SetCirclePoints(const size_t& points) {
	if (circlePoints != points) {
		circlePoints = points;
		initCirclePoints();
	}
}

void Drawings::render() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize));
	ImGui::Begin("##overlay", nullptr, ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Text("drawings.cpp");
	ImGui::Text("line2");
	ImGui::Button("herro");

	ImGui::GetForegroundDrawList()->PushClipRectFullScreen();

	ImGui::End();
}
