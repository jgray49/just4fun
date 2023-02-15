#include "Menu.h"

void Menu::render() {
	ImGui::SetNextWindowPos(ImVec2(234, 234), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	ImGui::Begin("##menu", &Settings::bShowMenu, ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("menu test");
	if (ImGui::CollapsingHeader("Orbwalker")) {
		ImGui::Checkbox("Enable", &Settings::bOrbwalkEnabled);
	}
	//if (ImGui::CollapsingHeader("Drawings")) {
	//	//ImGui::Checkbox("Enable", nullptr);
	//}
	//if (ImGui::CollapsingHeader("Evade")) {
	//	//ImGui::Checkbox("Enable", nullptr);
	//}
	//if (ImGui::CollapsingHeader("Scripts")) {
	//	//ImGui::Checkbox("Enable", nullptr);
	//}
	ImGui::End();
}
