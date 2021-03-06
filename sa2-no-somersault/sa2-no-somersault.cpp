#include "stdafx.h"


#pragma pack(push, 8)
struct SonicCharObj2_
{
	CharObj2Base base;
	uint8_t gap1BC[417];
	char SomersaultNextAction;
	uint8_t gap35E[8];
	__int16 SomersaultTime;
	__int16 SpindashCounter;
	char field_36A[42];
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CharObj2Base_
{
	char PlayerNum;
	char CharID;
	char Costume;
	char CharID2;
	char ActionWindowItems[8];
	char ActionWindowItemCount;
	char field_D[3];
	__int16 Powerups;
	int field_12;
	__int16 UnderwaterTime;
	__int16 IdleTime;
	uint8_t gap1A[10];
	int Upgrades;
	float field_28;
	char field_2C[28];
	float MechHP;
	int field_4C[6];
	NJS_VECTOR Speed;
	uint8_t gap70[32];
	float *field_90;
	ObjectMaster *HeldObject;
	uint8_t gap98[4];
	ObjectMaster *HoldTarget;
	uint8_t gapA0[28];
	NJS_MOTION **Animation;
	PhysicsData PhysData;
	int field_144[12];
	CharAnimInfo AnimInfo;
	float idk;
	float idk2;
	int CollisionFlags;
	float idk4;
	float idk5;
	float idk6;
	float idk7;
};
#pragma pack(pop)

void __cdecl Sonic_ChecksForDamage_r(EntityData1 *a1, EntityData2 *a2, CharObj2Base_ *a3, SonicCharObj2_ *a5);
static Trampoline Sonic_ChecksForDamage_t(0x00719920, 0x00719928, Sonic_ChecksForDamage_r);
void __cdecl Sonic_ChecksForDamage_r(EntityData1 *a1, EntityData2 *a2, CharObj2Base_ *a3, SonicCharObj2_ *a5)
{
	if (a1->Action == Action_SpinRelease)
	{
		a5->SpindashCounter = 2;
	}
	else if (a5->SomersaultNextAction == Action_SpinRelease)
	{
		a5->SpindashCounter = 0;
	}

	a5->SomersaultNextAction = a1->Action;
	auto original = static_cast<decltype(Sonic_ChecksForDamage_r)*>(Sonic_ChecksForDamage_t.Target());
	original(a1, a2, a3, a5);
}

extern "C"
{
	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer, nullptr, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0 };

	void __declspec(dllexport) Init()
	{
		// instant spindash
		WriteData<2>(reinterpret_cast<uint8_t*>(0x00723E19), 0x90i8);

		WriteData<1>(reinterpret_cast<uint8_t*>(0x00717A54), Action_SpinRelease); // enable physics parameter changes for spin
		WriteData<14>(reinterpret_cast<uint8_t*>(0x00717AD1), 0x90i8); // disable yoffset modification
		WriteData<14>(reinterpret_cast<uint8_t*>(0x00717AC3), 0x90i8); // disable floor grip modification

		WriteData<5>(reinterpret_cast<uint8_t*>(0x00723F0B), 0x90i8); // disable DoObstacleSomersault
		WriteData<5>(reinterpret_cast<uint8_t*>(0x00723F5C), 0x90i8); // disable (in our case, constant) rumble
		WriteData<1>(reinterpret_cast<uint8_t*>(0x00723ED0), 0xebi8); // disable secondary somersault type

		WriteData<1>(reinterpret_cast<uint8_t*>(0x0046ECF5), Action_SpinRelease); // allows spindash to break boxes
		WriteData<1>(reinterpret_cast<uint8_t*>(0x0046ECF9), Action_SpinRelease); // ditto

		// 00723E51 -> 80 3A 04 90 90 90 90 90
		static const uint8_t check_patch[] = {
			0x80,
			0x3A,
			0x04,
			0x90,
			0x90,
			0x90,
			0x90,
			0x90,
		};

		WriteData(reinterpret_cast<void*>(0x00723E51), check_patch);

		// 00723E59 -> 74
		WriteData<1>(reinterpret_cast<uint8_t*>(0x00723E59), 0x74);
	}
}
