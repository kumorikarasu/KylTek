#pragma once
#ifndef SPRITE_H_
#define SPRITE_H_

/*
	This Struct Contains EVERY Sprite/Animation used in the game

	Usage - Create 1 of these in main event;
*/
#define CLR_NULL		D3DCOLOR_ARGB(0,0,0,0);
#define CLR_WHITE		D3DCOLOR_ARGB(255, 255, 255, 255)
#define CLR_GREY		D3DCOLOR_ARGB(255, 128, 128, 128)
#define CLR_BLACK		D3DCOLOR_ARGB(255, 0, 0, 0)
#define CLR_RED			D3DCOLOR_ARGB(255, 255, 0, 0)
#define CLR_GREEN		D3DCOLOR_ARGB(255, 0, 255, 0)
#define CLR_BLUE		D3DCOLOR_ARGB(255, 0, 0, 255)
#define CLR_LIGHTBLUE	D3DCOLOR_ARGB(255, 0, 255, 255)
#define CLR_MAJENTA		D3DCOLOR_ARGB(255, 255, 0, 255)
#define CLR_YELLOW		D3DCOLOR_ARGB(255, 255, 255, 0)

struct Character{
	CSprite* Eyes;
	CSprite* Mouth;
	CSprite* Torso;
	CSprite* JetPack;
	CSprite* Feet;
	CSprite* FullHead;
};
struct menu{
	CSprite* menuBgk;
};
struct Characters{
	Character* ME;
	Character* Yellow;
};
struct Weapons{
	CSprite* AssaultRifle;
	CSprite* Shotgun;
	CSprite* RocketLauncher;
	CSprite* Rocket;
};
struct Bgs{
	CSprite* City1;
};
struct HUDSprites{
	CSprite* Crosshair;
};
struct Vehicles{
	CSprite* Helicopter;
};
struct LevelDesign{
	CSprite* Brick1;
	CSprite* Sidewalk1;
	CSprite* TSBuilding1;
};
struct Miscs{
	CSprite* Console;
	CSprite* Rect;
	CSprite* CursorDefault;
};
struct VSBspr{
	CSprite* up;
	CSprite* up_over;
	CSprite* up_down;
	CSprite* down;
	CSprite* down_over;
	CSprite* down_down;
	CSprite* bar;
	CSprite* bar_over;
	CSprite* bar_down;
};
struct HSBspr{
	CSprite* left;
	CSprite* left_over;
	CSprite* left_down;
	CSprite* right;
	CSprite* right_over;
	CSprite* right_down;
	CSprite* bar;
	CSprite* bar_over;
	CSprite* bar_down;
};
struct GUIspr{
	VSBspr* VSB;
	HSBspr* HSB;
};
struct Sprites{
	Characters* Char;
	Weapons* Weapon;
	Bgs* Bg;
	LevelDesign* Level; 
	Vehicles* Vehicle;
	HUDSprites* HUD;
	Miscs* Misc;
	GUIspr* GUI;
	menu* Menu;
};

#endif