#pragma once
#include "Includes.h"


class Drawings {
private:
	ImDrawList* drawList;

	size_t circlePoints = 32;
	std::vector<float>vSinTable;
	std::vector<float>vCosTable;

	void initCirclePoints();

public:
	Drawings() {
		initCirclePoints();
	}
	void SetCirclePoints(const size_t& points);
	static void render();

	inline void Circle3D(const Vector3& worldPos, const float& radius, const ImColor& color = ImColor(1.f, 1.f, 1.f), const float& thickness = 2.f) const {
		Vector3 worldSpace = worldPos;

		for (auto i = 0; i < circlePoints; i++)
		{
			worldSpace.x = worldPos.x + vCosTable[i] * radius;
			worldSpace.z = worldPos.z + vSinTable[i] * radius;
			drawList->PathLineTo(WorldToScreen(worldSpace));
		}

		drawList->PathStroke(color, ImDrawFlags_None, thickness);
	}
};