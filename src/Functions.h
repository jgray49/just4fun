#pragma once
#include "Includes.h"

namespace Functions {
	[[nodiscard]] static ImVec2 WorldToScreen(const Vector3& pos) {
		Vector4 clipCoords;
		clipCoords.x = pos.x * Globals::multipliedMatrix[0] + pos.y * Globals::multipliedMatrix[4] + pos.z * Globals::multipliedMatrix[8] + Globals::multipliedMatrix[12];
		clipCoords.y = pos.x * Globals::multipliedMatrix[1] + pos.y * Globals::multipliedMatrix[5] + pos.z * Globals::multipliedMatrix[9] + Globals::multipliedMatrix[13];
		clipCoords.z = pos.x * Globals::multipliedMatrix[2] + pos.y * Globals::multipliedMatrix[6] + pos.z * Globals::multipliedMatrix[10] + Globals::multipliedMatrix[14];
		clipCoords.w = pos.x * Globals::multipliedMatrix[3] + pos.y * Globals::multipliedMatrix[7] + pos.z * Globals::multipliedMatrix[11] + Globals::multipliedMatrix[15];

		//clipCoords.X = pos.X * matrix[0] + pos.Y * matrix[1] + pos.Z * matrix[2] + matrix[3];
		//clipCoords.Y = pos.X * matrix[4] + pos.Y * matrix[5] + pos.Z * matrix[6] + matrix[7];
		//clipCoords.Z = pos.X * matrix[8] + pos.Y * matrix[9] + pos.Z * matrix[10] + matrix[11];
		//clipCoords.W = pos.X * matrix[12] + pos.Y * matrix[13] + pos.Z * matrix[14] + matrix[15];

		Vector2 returnVec = { 0.f, 0.f };
		if (clipCoords.w < 0.1f) return returnVec;

		Vector3 temp;
		temp.x = clipCoords.x / clipCoords.w;
		temp.y = clipCoords.y / clipCoords.w;
		temp.z = clipCoords.z / clipCoords.w;

		// todo move this
		static DWORD dwRenderer = *reinterpret_cast<DWORD*>(Globals::dwBaseAddress + oRenderer);
		static ImVec2 windowSize = ImVec2(*reinterpret_cast<int*>(dwRenderer + oRendererWidth), *reinterpret_cast<int*>(dwRenderer + oRendererHeight));

		returnVec.x = (windowSize.x / 2.0f * temp.x) + (temp.x + windowSize.x / 2.0f);
		returnVec.y = -(windowSize.y / 2.0f * temp.y) + (temp.y + windowSize.y / 2.0f);

		return returnVec;
	}
}