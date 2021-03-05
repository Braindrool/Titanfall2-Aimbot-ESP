#pragma once
#include "pch.h"

// TODO: Cleanup
class EntityData
{
public:
	char pad_0000[24]; //0x0000
	int32_t Id; //0x0018
	char pad_001C[20]; //0x001C
	int32_t IdOld; //0x0030
	char pad_0034[92]; //0x0034
	Vector3 Position; //0x0090
	char pad_009C[664]; //0x009C
	int32_t PossibleTeam; //0x0334
	char pad_0338[20]; //0x0338
	int32_t PossibleTeam2; //0x034C
	char pad_0350[4]; //0x0350
	Vector3 Position1; //0x0354
	char pad_0360[12]; //0x0360
	Vector3 Position2; //0x036C
	char pad_0378[20]; //0x0378
	int32_t Team; //0x038C
	char pad_0390[20]; //0x0390
	int32_t PossibleTeam3; //0x03A4
	char pad_03A8[232]; //0x03A8
	int32_t MaxHealth; //0x0490
	char pad_0494[4]; //0x0494
	char* Name; //0x0498
	char pad_04A0[772]; //0x04A0
	char* ModelName; //0x07A4
	char pad_07AC[268]; //0x07AC
	class C_Player* Titan; //0x08B8
	int32_t HealthIncludingTitan; //0x08C0
	char pad_08C4[1084]; //0x08C4
	int32_t PossibleTeam4; //0x0D00
	char pad_0D04[740]; //0x0D04
	void* Bones; //0x0FF0
	char pad_0FF8[924]; //0x0FF8
	int32_t Type; //0x1394
	char pad_1398[640]; //0x1398
	int32_t Health; //0x1618
	char BottomLeftIcon[8]; //0x161C
	char pad_1624[872]; //0x1624
	Vector3 Position3; //0x198C
	char pad_1998[1272]; //0x1998
	Vector3 ViewAnglesWritable; //0x1E90
	char pad_1E9C[4516]; //0x1E9C
}; //Size: 0x3040

