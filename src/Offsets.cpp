#include "offsets.h"

Offsets::Offsets() {};
DWORD Offsets::BaseAddress = NULL;

DWORD Offsets::HeroList = 0x18D9B6C;
DWORD Offsets::oLocalPlayer = 0x31767BC;

DWORD Offsets::oChatClient = 0x31768A8;
DWORD Offsets::oChatOpen = 0x820;

DWORD Offsets::fGetAttackDelay = 0x2A0860;
DWORD Offsets::fGetAttackCastDelay = 0x2A0760;
DWORD Offsets::fPrintChat = 0x607640;
DWORD Offsets::fNewIssueOrder = 0x68F900;
DWORD Offsets::fIsAlive = 0x17AF50;

DWORD Offsets::oAtkRange = 0x13A4;

DWORD Offsets::oHudInstance = 0x18D9AD4;

DWORD Offsets::oRenderer = 0x31A28B8;
DWORD Offsets::oRendererWidth = 0x8;
DWORD Offsets::oRendererHeight = 0xC;
DWORD Offsets::oViewProjMatrices = 0x319CF18;
