#pragma once
#include <Windows.h>
/*
localplayer 3163080
currenthealth e7c
maxhealth e8c
currentmanna 29c
maxmana 2ac
atkrange 

*/
class Offsets {
public:
	Offsets();
	static DWORD BaseAddress;
	static DWORD HeroList;
	static DWORD oLocalPlayer;

	static DWORD oChatClient;
	static DWORD oChatOpen;

	static DWORD fGetAttackDelay;
	static DWORD fGetAttackCastDelay;
	static DWORD fPrintChat;
	static DWORD fNewIssueOrder;
	static DWORD fIsAlive;

	static DWORD oAtkRange;

	static DWORD oHudInstance;

	static DWORD oRenderer;
	static DWORD oRendererWidth;
	static DWORD oRendererHeight;
	static DWORD oViewProjMatrices;
};

