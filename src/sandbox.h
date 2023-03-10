#pragma once
#include <vector>
#include <Windows.h>
#include "Offsets.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GameObject.h"





// Just approximate your ping, I haven't bothered implementing the GetPing() function
double Ping = 45;

// Timestamps to keep track of what you're doing
double lastAttack = 0;
double lastMove = 0;
double lastLoop = 0;

// WorldToScreen variables
int width = 0;
int height = 0;

float viewMatrix[16];
float projMatrix[16];
float viewProjMatrix[16];

void MultiplyMatrices(float* out, float* a, int row1, int col1, float* b, int row2, int col2) {
	int size = row1 * col2;
	for (int i = 0; i < row1; i++) {
		for (int j = 0; j < col2; j++) {
			float sum = 0.f;
			for (int k = 0; k < col1; k++)
				sum = sum + a[i * col1 + k] * b[k * col2 + j];
			out[i * col2 + j] = sum;
		}
	}
}

static void LoadW2S() {
	char buff[128];

	DWORD_PTR renderBase = (*(DWORD*)((DWORD)GetModuleHandle(NULL) + Offsets::oRenderer));

	memcpy(buff, (void*)(renderBase), 128);
	width = *reinterpret_cast<int*>(buff + Offsets::oRendererWidth);
	height = *reinterpret_cast<int*>(buff + Offsets::oRendererHeight);

	memcpy(buff, (void*)((DWORD)GetModuleHandle(NULL) + Offsets::oViewProjMatrices), 128);
	memcpy(viewMatrix, buff, 16 * sizeof(float));
	memcpy(projMatrix, &buff[16 * sizeof(float)], 16 * sizeof(float));

	MultiplyMatrices(viewProjMatrix, viewMatrix, 4, 4, projMatrix, 4, 4);
}


[[nodiscard]] static ImVec2 WorldToScreen(const Vector3& pos) {

	// Initialize the output vector with zeroes
	ImVec2 out = { 0.f, 0.f };
	// Get the dimensions of the screen
	Vector2 screen = { (float)width, (float)height };

	// Compute the clip coordinates of the given position
	Vector4 clipCoords;
	clipCoords.x = pos.x * viewProjMatrix[0] + pos.y * viewProjMatrix[4] + pos.z * viewProjMatrix[8] + viewProjMatrix[12];
	clipCoords.y = pos.x * viewProjMatrix[1] + pos.y * viewProjMatrix[5] + pos.z * viewProjMatrix[9] + viewProjMatrix[13];
	clipCoords.z = pos.x * viewProjMatrix[2] + pos.y * viewProjMatrix[6] + pos.z * viewProjMatrix[10] + viewProjMatrix[14];
	clipCoords.w = pos.x * viewProjMatrix[3] + pos.y * viewProjMatrix[7] + pos.z * viewProjMatrix[11] + viewProjMatrix[15];

	// If the clip coordinates have a negative w-value, set it to 1
	if (clipCoords.w < 1.0f)
		clipCoords.w = 1.f;

	// Compute the normalized device coordinates of the given position
	Vector3 M;
	M.x = clipCoords.x / clipCoords.w;
	M.y = clipCoords.y / clipCoords.w;
	M.z = clipCoords.z / clipCoords.w;


	// Compute the screen coordinates of the given position
	out.x = (screen.x / 2.f * M.x) + (M.x + screen.x / 2.f);
	out.y = -(screen.y / 2.f * M.y) + (M.y + screen.y / 2.f);

	// Return the computed screen coordinates
	return out;
}

Vector3 GetMousePos()
{
	DWORD MousePtr = *(DWORD*)(*(DWORD*)((DWORD)GetModuleHandle(NULL) + Offsets::oHudInstance) + 0x14) + 0x1C;

	Vector3 mousePos;

	mousePos.x = *(float*)(MousePtr + 0x0);
	mousePos.y = *(float*)(MousePtr + 0x4);
	mousePos.z = *(float*)(MousePtr + 0x8);

	return mousePos;
}

ImDrawList* drawList;
std::vector<float>vSinTable;
std::vector<float>vCosTable;

void drawCircle3D(Vector3 worldPos, float radius, ImColor color, float thickness) {
	Vector3 worldSpace = worldPos;

	for (auto i = 0; i < 32; i++)
	{
		worldSpace.x = worldPos.x + vCosTable[i] * radius;
		worldSpace.z = worldPos.z + vSinTable[i] * radius;

		drawList->PathLineTo(WorldToScreen(worldSpace));
	}

	ImGui::GetForegroundDrawList()->PathStroke(color, ImDrawFlags_None, thickness);
}

void __stdcall OrbWalker() {

	// Base Address + LocalPlayer + Champions vector
	// Standard setup
	DWORD BaseAddress = (DWORD)GetModuleHandle(NULL);
	GameObject* LocalPlayer = (GameObject*)(*(DWORD*)(BaseAddress + Offsets::oLocalPlayer));
	std::vector<GameObject*> Champions;

	// Defining the functions we care about
	typedef bool(__thiscall* fnIsAlive)(GameObject* obj);
	typedef float(__cdecl* fnGetAttackDelay)(GameObject* obj);
	typedef float(__cdecl* fnGetAttackCastDelay)(GameObject* obj);
	typedef void(__thiscall* fnPrintChat)(DWORD, const char*, int);
	typedef int(__thiscall* fnNewIssueOrder)(DWORD HudThisPtr, int state, int IsAttack, int IsAttackCommand, int x, int y, int AttackAndMove);
	

	// Function objects
	fnGetAttackDelay GetAttackDelay = (fnGetAttackDelay)(BaseAddress + Offsets::fGetAttackDelay);
	fnGetAttackCastDelay GetAttackCastDelay = (fnGetAttackCastDelay)(BaseAddress + Offsets::fGetAttackCastDelay);
	fnPrintChat PrintChat = (fnPrintChat)(BaseAddress + Offsets::fPrintChat);
	fnNewIssueOrder NewIssueOrder = (fnNewIssueOrder)(BaseAddress + Offsets::fNewIssueOrder);
	fnIsAlive IsAlive = (fnIsAlive)(BaseAddress + Offsets::fIsAlive);

	// Testing that we are properly injected
	PrintChat(BaseAddress + Offsets::oChatClient, "just4fun", 0xFFFFFF);

	// Orbwalking
	while (true) {

		// Little delay so your computer doesn't die
		if (GetAsyncKeyState(0x56) && GetTickCount64() - lastLoop > 3) {

			// Checking if chat is open! Don't want to try spacegliding while typing...
			bool isChatOpen = *(int*)(*(DWORD*)(BaseAddress + Offsets::oChatClient) + Offsets::oChatOpen);

			if (!isChatOpen) {
				// Make sure Champions list is empty
				Champions.clear();

				// This will be our target!
				GameObject* target = new GameObject();

				// Load Champions' positions
				int vectorSize = (DWORD)(*(DWORD*)(*(DWORD*)(BaseAddress + Offsets::HeroList) + 0x8));

				for (int i = 0; i < vectorSize; i++) {
					// Pointer to champion from the HeroClient
					GameObject* heroObject = (GameObject*)(*(DWORD*)(*(DWORD*)(*(DWORD*)(BaseAddress + Offsets::HeroList) + 0x4) + 0x4 * i));

					// Check if it's the closest target that's been found (within reason)
					//if (IsAlive(heroObject) && heroObject->targetable && heroObject->visible && heroObject->team != LocalPlayer->team && heroObject->position.DistanceTo(LocalPlayer->position) < 700 && heroObject->position.DistanceTo(LocalPlayer->position) < target->position.DistanceTo(LocalPlayer->position))
						// If so, that's the one!
						//target = heroObject;

					// Add the heroObject to the list of champions anyway, you can use it later for other things
					Champions.push_back(heroObject);
				}

				// First check is to make sure that a target actually exists
				// Second check is to make sure that you aren't attacking during your attack's "cooldown"
				if (target->position.x != 0 && GetTickCount64() >= lastAttack + GetAttackDelay(LocalPlayer) * 1000.f) {

					// Load WorldToScreen
					LoadW2S();

					// Attack closest champion
					Vector2 AttackPosition = { 1,1 };//WorldToScreen(target->position);

					// Send input to attack
					DWORD HUDInput = *(DWORD*)(*(DWORD*)(BaseAddress + Offsets::oHudInstance) + 0x24);
					NewIssueOrder(HUDInput, 0, 0, true, AttackPosition.x, AttackPosition.y, 0);
					NewIssueOrder(HUDInput, 1, 0, true, AttackPosition.x, AttackPosition.y, 0);

					// Update timer so that you don't try attacking on cooldown
					lastAttack = GetTickCount64();
				}

				// First check is to make sure you aren't moving too often
				// Second check is to make sure you aren't moving in your windup
				else if (GetTickCount64() > lastMove + 30 && GetTickCount64() >= lastAttack + GetAttackCastDelay(LocalPlayer) * 1000.f + Ping + 10) {

					// Load WorldToScreen
					LoadW2S();

					// Click with your mouse's current position
					Vector2 MovePosition = { 1,1 };// WorldToScreen(GetMousePos());

					// Send an input to right click at mouse position
					DWORD HUDInput = *(DWORD*)(*(DWORD*)(BaseAddress + Offsets::oHudInstance) + 0x24);
					NewIssueOrder(HUDInput, 0, 0, true, MovePosition.x, MovePosition.y, 0);
					NewIssueOrder(HUDInput, 1, 0, true, MovePosition.x, MovePosition.y, 0);

					// Update timer so that you don't spam inputs and DC
					lastMove = GetTickCount64();
				}
			}
			// Update lastLoop so you don't overwork the thread
			lastLoop = GetTickCount64();
		}
	}
}

