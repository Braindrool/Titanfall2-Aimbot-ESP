#include "pch.h"
#include "Skeleton.h"

Window* Skeleton::win;

void Skeleton::DrawSkeleton(Entity* ent) {
	bool lowLod = true;
	if (ent->data->Bones != nullptr) {
		if (lowLod) {
			// Body
			DrawBone(ent, "neck", "hips");

			// Left leg
			DrawBone(ent, "hips", "leftknee");
			DrawBone(ent, "leftknee", "leftankle");

			// Right leg
			DrawBone(ent, "hips", "rightknee");
			DrawBone(ent, "rightknee", "rightankle");

			// Left Arm
			DrawBone(ent, "neck", "leftelbow");
			DrawBone(ent, "leftelbow", "leftwrist");

			// Right Arm
			DrawBone(ent, "neck", "rightelbow");
			DrawBone(ent, "rightelbow", "rightwrist");
		}
		else {
			// Body
			DrawBone(ent, "head", "neck");
			DrawBone(ent, "neck", "chest");
			DrawBone(ent, "chest", "abdomen");
			DrawBone(ent, "abdomen", "hips");

			// Left leg
			DrawBone(ent, "hips", "lefthip");
			DrawBone(ent, "lefthip", "leftknee");
			DrawBone(ent, "leftknee", "leftankle");
			DrawBone(ent, "leftankle", "leftfoot");

			// Right leg
			DrawBone(ent, "hips", "righthip");
			DrawBone(ent, "righthip", "rightknee");
			DrawBone(ent, "rightknee", "rightankle");
			DrawBone(ent, "rightankle", "rightfoot");

			// Left Arm
			DrawBone(ent, "neck", "leftshoulder");
			DrawBone(ent, "leftshoulder", "leftupperarm");
			DrawBone(ent, "leftupperarm", "leftelbow");
			DrawBone(ent, "leftelbow", "leftforearm");
			DrawBone(ent, "leftforearm", "leftwrist");

			// Right Arm
			DrawBone(ent, "neck", "rightshoulder");
			DrawBone(ent, "rightshoulder", "rightupperarm");
			DrawBone(ent, "rightupperarm", "rightelbow");
			DrawBone(ent, "rightelbow", "rightforearm");
			DrawBone(ent, "rightforearm", "rightwrist");
		}
	}
}

void Skeleton::DrawBone(Entity* ent, const char* bone1, const char* bone2)
{
	Vector2 screen1, screen2;
	if (WorldToScreen::D3D(ent->GetBone(bone1), screen1, (float*)Addresses::Get("viewmatrix"), win->GetWindowPos().right - win->GetWindowPos().left, win->GetWindowPos().bottom - win->GetWindowPos().top)
		&& WorldToScreen::D3D(ent->GetBone(bone2), screen2, (float*)Addresses::Get("viewmatrix"), win->GetWindowPos().right - win->GetWindowPos().left, win->GetWindowPos().bottom - win->GetWindowPos().top)) {
		win->d2d->DrawLine(screen1.x, screen1.y, screen2.x, screen2.y);
	}
}

void Skeleton::DrawBoneNumbers(Entity* ent)
{
	for (int i = 0; i < 100; i++) {
		float* x1 = (float*)((uintptr_t)ent->data->Bones + 0xC + 0x30 * i);
		float* y1 = (float*)((uintptr_t)ent->data->Bones + 0x1C + 0x30 * i);
		float* z1 = (float*)((uintptr_t)ent->data->Bones + 0x2C + 0x30 * i);

		if (x1 == nullptr || y1 == nullptr || z1 == nullptr)
			continue;

		float* x2 = (float*)((uintptr_t)ent->data->Bones + 0xC + 0x30 * (i + 1));
		float* y2 = (float*)((uintptr_t)ent->data->Bones + 0x1C + 0x30 * (i + 1));
		float* z2 = (float*)((uintptr_t)ent->data->Bones + 0x2C + 0x30 * (i + 1));

		if (x2 == nullptr || y2 == nullptr || z2 == nullptr)
			continue;

		Vector3 bone = { *x1, *y1, *z1 };

		Vector2 screen;

		WorldToScreen::D3D(bone, screen, (float*)Addresses::Get("viewmatrix"), win->GetWindowPos().right - win->GetWindowPos().left, win->GetWindowPos().bottom - win->GetWindowPos().top);

		win->d2d->DrawText(screen.x - 50, screen.y - 50, screen.x + 50, screen.y + 50, std::to_wstring(i), { 0, 255, 0 });
	}
}

void Skeleton::SetWindow(Window* win)
{
	Skeleton::win = win;
}
