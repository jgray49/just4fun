#pragma once
#include "Includes.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;};

class GameObject {
public:
	// Constructor
	GameObject() { position = Vector3(0, 0, 0); team = 100; visible = false; };

	union {
		DEFINE_MEMBER_N(short, team, 0x34);
		DEFINE_MEMBER_N(bool, visible, 0x274);
		DEFINE_MEMBER_N(Vector3, position, 0x1DC);
		DEFINE_MEMBER_N(bool, targetable, 0xD04);
	};
};