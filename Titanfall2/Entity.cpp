#include "pch.h"
#include "Entity.h"
#include "Addresses.h"

bool Entity::IsLocalPlayer()
{
    return (uintptr_t) data == Addresses::Get("localplayer");
}

bool Entity::IsPlayer()
{
    return !strcmp(data->Name, "player") && strstr(data->Name, "titan") == NULL;
}

bool Entity::IsAlive()
{
    return data->Health > 0;
}

bool Entity::IsFriendly()
{
    EntityData* localPlayerData = (EntityData*) Addresses::Get("localplayer");
    return data->Team == localPlayerData->Team;
}

bool Entity::IsTitan()
{
    return strstr(data->BottomLeftIcon, "titan_loadout") != NULL;
}

bool Entity::IsAI()
{
    return !strcmp(data->Name, "npc_soldier") != NULL;
}

bool Entity::IsSafeToAccess()
{
    // TODO: Make this less terrible
    return (data != nullptr && ((data->Id > 0 && data->Id < 0xFFF) || IsLocalPlayer()) && (uintptr_t*)(data->Name) != 0x0 && (uintptr_t*)(data->Name) != (uintptr_t*)0xFFFFFFFFFFFFFFFF);
}

void Entity::SetLastPos(Vector3 &lastPos)
{
    velocity.x = lastPos.x - this->lastPos.x;
    velocity.y = lastPos.y - this->lastPos.y;
    velocity.z = lastPos.z - this->lastPos.z;

    this->lastPos = lastPos;
}

Vector3 Entity::GetBone(const char* name)
{
    Vector3 pos = { 0, 0, 0 };
    std::map<const char*, unsigned int> bones;
    if (!IsSafeToAccess() || data->Bones == nullptr)
        return pos;
    else {
        if (IsAI() || IsPlayer())
            bones = humanBones;
        else
            return pos;

        unsigned int index = bones.find(name)->second;

        float* x = (float*)((uintptr_t)data->Bones + 0xC + 0x30 * index);
        float* y = (float*)((uintptr_t)data->Bones + 0x1C + 0x30 * index);
        float* z = (float*)((uintptr_t)data->Bones + 0x2C + 0x30 * index);

        if (x == nullptr || y == nullptr || z == nullptr)
            pos = { 0, 0, 0 };
        else
            pos = { *x, *y, *z };

        return pos;
    }
}

Vector3 Entity::GetAimPoint(const char* boneName, float multiplier)
{
    Vector3 nextPoint = GetBone(boneName);
    nextPoint.x += velocity.x * multiplier;
    nextPoint.y += velocity.y * multiplier;
    nextPoint.z += velocity.z * multiplier;

    return nextPoint;
}

Vector3 Entity::GetVelocity()
{
    return velocity;
}

float Entity::GetDistanceFrom(Entity* ent)
{
    return sqrt(pow(data->Position1.x - ent->data->Position1.x, 2) + pow(data->Position1.y - ent->data->Position1.y, 2) + pow(data->Position1.z - ent->data->Position1.z, 2));
}

