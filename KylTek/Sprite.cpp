#include "Main.h" 
#include "CGraphicsHandler.h"

void CGraphicsHandler::SetupSprites(){
	/*---------------------------------------------------------------------------------------------------------------------------------
	-Initialize
	---------------------------------------------------------------------------------------------------------------------------------*/
	//D3DXCreateSprite(m_pD3DDevice,&g_pg->DrawSprite);
	//D3DXCreateLine(m_pD3DDevice, &g_pDrawLine);
	SprList = new Sprites();
	SprList->Char = new Characters();
	SprList->Char->ME = new Character();
	SprList->Char->Yellow = new Character();

	SprList->Weapon = new Weapons();
	SprList->Bg = new Bgs();
	SprList->HUD = new HUDSprites();
	SprList->Vehicle = new Vehicles();
	SprList->Level = new LevelDesign();
	SprList->Misc = new Miscs();
	SprList->GUI = new GUIspr();
	SprList->GUI->HSB = new HSBspr();
	SprList->GUI->VSB = new VSBspr();
	SprList->Menu = new menu();

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Characters
	---------------------------------------------------------------------------------------------------------------------------------*/

	//ME
	//SprList->Char->ME->Eyes = new CSprite(m_pD3DDevice, "Resources/Sprites/Characters/ME/me_head_64.PNG",2,0, ColorARGB(255, 111,49,152));
	//SprList->Char->ME->Mouth = new CSprite(m_pD3DDevice, "Resources/Sprites/Characters/ME/me_head_64.PNG", ColorARGB(255, 111,49,152),2,0);
//	SprList->Char->ME->Torso = new CSprite(m_pD3DDevice, "../Resources/Sprites/Characters/ME/me.PNG", 1,0, ColorARGB(255, 111,49,152));
	//SprList->Char->ME->Feet = new CSprite(m_pD3DDevice, "Resources/Sprites/Characters/ME/me_feet_32.PNG", ColorARGB(255, 111,49,152),1,0);
//	SprList->Char->ME->JetPack = new CSprite(m_pD3DDevice, "../Resources/Sprites/BKG.PNG",1,0);
	//SprList->Char->ME->FullHead = new CSprite(m_pD3DDevice, "../Resources/Sprites/Characters/ME/me_head_64.PNG",2,0, ColorARGB(255, 111,49,152));
	//Yellow
	//SprList->Char->Yellow->Eyes = new CSprite(m_pD3DDevice, "Resources/Sprites/Characters/Yellow/me_head_64.PNG", ColorARGB(255, 111,49,152),2,0);
	//SprList->Char->Yellow->Mouth = new CSprite(m_pD3DDevice, "Resources/Sprites/Characters/Yellow/me_head_64.PNG", ColorARGB(255, 111,49,152),2,0);
//	SprList->Char->Yellow->Feet = new CSprite(m_pD3DDevice, "Resources/Sprites/Characters/Yellow/me_head_64.PNG", ColorARGB(255, 111,49,152),2,0);
	//SprList->Char->Yellow->Torso = new CSprite(m_pD3DDevice, "Resources/Sprites/Characters/ME/me_head_64.PNG", ColorARGB(255, 111,49,152),2,0);
	//SprList->Char->Yellow->JetPack = new CSprite(m_pD3DDevice, "Resources/Sprites/Characters/Yellow/me_head_64.PNG", ColorARGB(255, 111,49,152),2,0);
	//SprList->Char->Yellow->FullHead = new CSprite(m_pD3DDevice, "Resources/Sprites/Characters/Yellow/me_head_64.PNG", ColorARGB(255, 111,49,152),2,0);

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Weapons
	---------------------------------------------------------------------------------------------------------------------------------*/

	//Assult Rifle
//	SprList->Weapon->AssaultRifle = new CSprite(m_pD3DDevice, "../Resources/Sprites/Weapons/Assault Rifle/gun_assult.PNG", 1,0, ColorARGB(255, 111,49,152));
	//Shotgun
	//SprList->Weapon->Shotgun = new CSprite(m_pD3DDevice, "Resources/Sprites/Weapons/Shogun/shotgun.PNG", ColorARGB(255, 111,49,152),0,0);
	//RocketLauncher
	//SprList->Weapon->RocketLauncher = new CSprite(m_pD3DDevice, "Resources/Sprites/Weapons/Rocket Launcher/launcher.PNG", ColorARGB(255, 111,49,152),0,0);
	//SprList->Weapon->Rocket = new CSprite(m_pD3DDevice, "Resources/Sprites/Weapons/Rocket Launcher/rocket.PNG", ColorARGB(255, 111,49,152),0,0);

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Backgrounds
	---------------------------------------------------------------------------------------------------------------------------------*/
	
	SprList->Menu->menuBgk = new CSprite(m_pD3DDevice, L"../Resources/Sprites/menu/title.PNG", 0,0, ColorARGB(255, 111,49,152));
//	SprList->Bg->City1 = new CSprite(m_pD3DDevice, "../Resources/Sprites/Level Design/City/YI_castle.PNG", 0,0, ColorARGB(255, 111,49,152));

	/*---------------------------------------------------------------------------------------------------------------------------------
	-HUD
	---------------------------------------------------------------------------------------------------------------------------------*/

	//Crosshair
//	SprList->HUD->Crosshair = new CSprite(m_pD3DDevice, "../Resources/Sprites/HUD/crosshair.png", 0,0, ColorARGB(255, 111,49,152));

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Vehicles
	---------------------------------------------------------------------------------------------------------------------------------*/

	//SprList->Vehicle->Helicopter = new CSprite(m_pD3DDevice, "Resources/Sprites/Vehicles/Helicopter/helicopter.PNG", ColorARGB(255, 111,49,152),0,0);

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Level Design
	---------------------------------------------------------------------------------------------------------------------------------*/

//	SprList->Level->Sidewalk1 = new CSprite(m_pD3DDevice, "../Resources/Sprites/Level Design/City/sidewalk1.PNG", 0,0, ColorARGB(255, 111,49,152));
//	SprList->Level->TSBuilding1 = new CSprite(m_pD3DDevice, "../Resources/Sprites/Level Design/City/ts_building1.PNG", 4,4,0, ColorARGB(255, 111,49,152));
	//SprList->Level->Brick1 = new CSprite(m_pD3DDevice, "Resources/Sprites/Level Design/City/brick1.PNG", ColorARGB(255, 111,49,152),0,0);

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Misc
	---------------------------------------------------------------------------------------------------------------------------------*/

	SprList->Misc->Console = new CSprite(m_pD3DDevice, L"../Resources/Sprites/Misc/console.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->Misc->CursorDefault = new CSprite(m_pD3DDevice, L"../Resources/Sprites/Misc/cursor_default.PNG", ColorARGB(255, 111,49,152),0,0);

	/*---------------------------------------------------------------------------------------------------------------------------------
	-GUI
	---------------------------------------------------------------------------------------------------------------------------------*/
	SprList->GUI->HSB->bar = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/horizbar.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->HSB->bar_over = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/horizbar_over.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->HSB->bar_down = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/horizbar_down.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->HSB->left = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/left.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->HSB->left_over = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/left_over.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->HSB->left_down = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/left_down.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->HSB->right = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/right.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->HSB->right_over = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/right_over.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->HSB->right_down = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/right_down.PNG",0,0, ColorARGB(255, 111,49,152));

	SprList->GUI->VSB->bar = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/vertbar.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->VSB->bar_over = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/vertbar_over.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->VSB->bar_down = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/vertbar_down.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->VSB->up = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/up.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->VSB->up_over = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/up_over.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->VSB->up_down = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/up_down.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->VSB->down = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/down.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->VSB->down_over = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/down_over.PNG",0,0, ColorARGB(255, 111,49,152));
	SprList->GUI->VSB->down_down = new CSprite(m_pD3DDevice, L"../Resources/Sprites/GUI/Scroll Bar/down_down.PNG",0,0, ColorARGB(255, 111,49,152));
}

void CGraphicsHandler::CleanupSprites(){
	/*---------------------------------------------------------------------------------------------------------------------------------
	-Characters
	---------------------------------------------------------------------------------------------------------------------------------*/

	//ME
	SAFE_DELETE( SprList->Char->ME->Eyes );
	SAFE_DELETE( SprList->Char->ME->Mouth );
	SAFE_DELETE( SprList->Char->ME->Feet );
	SAFE_DELETE( SprList->Char->ME->Torso );
	SAFE_DELETE( SprList->Char->ME->JetPack );
	SAFE_DELETE( SprList->Char->ME->FullHead );
	//Yellow
	SAFE_DELETE( SprList->Char->Yellow->Eyes );
	SAFE_DELETE( SprList->Char->Yellow->Mouth );
	SAFE_DELETE( SprList->Char->Yellow->Feet );
	SAFE_DELETE( SprList->Char->Yellow->Torso );
	SAFE_DELETE( SprList->Char->Yellow->JetPack );
	SAFE_DELETE( SprList->Char->Yellow->FullHead );

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Weapons
	---------------------------------------------------------------------------------------------------------------------------------*/

	//Assult Rifle
	SAFE_DELETE( SprList->Weapon->AssaultRifle );
	//Shotgun
	//delete SprList->Weapon->Shotgun;
	//RocketLauncher
	//delete SprList->Weapon->RocketLauncher;
	//delete SprList->Weapon->Rocket;

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Backgrounds
	---------------------------------------------------------------------------------------------------------------------------------*/

	SAFE_DELETE( SprList->Menu->menuBgk );
	SAFE_DELETE( SprList->Bg->City1 );

	/*---------------------------------------------------------------------------------------------------------------------------------
	-HUD
	---------------------------------------------------------------------------------------------------------------------------------*/

	//Crosshair
	SAFE_DELETE( SprList->HUD->Crosshair );

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Vehicles
	---------------------------------------------------------------------------------------------------------------------------------*/

	//delete SprList->Vehicle->Helicopter;

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Level Design
	---------------------------------------------------------------------------------------------------------------------------------*/

	SAFE_DELETE( SprList->Level->Sidewalk1 );
	SAFE_DELETE( SprList->Level->TSBuilding1 );
	//delete SprList->Level->Brick1;

	/*---------------------------------------------------------------------------------------------------------------------------------
	-Misc
	---------------------------------------------------------------------------------------------------------------------------------*/

	SAFE_DELETE( SprList->Misc->Console );
	SAFE_DELETE( SprList->Misc->CursorDefault );

	/*---------------------------------------------------------------------------------------------------------------------------------
	-GUI
	---------------------------------------------------------------------------------------------------------------------------------*/
	SAFE_DELETE( SprList->GUI->HSB->bar );
	SAFE_DELETE( SprList->GUI->HSB->bar_over );
	SAFE_DELETE( SprList->GUI->HSB->bar_down );
	SAFE_DELETE( SprList->GUI->HSB->left );
	SAFE_DELETE( SprList->GUI->HSB->left_over );
	SAFE_DELETE( SprList->GUI->HSB->left_down );
	SAFE_DELETE( SprList->GUI->HSB->right );
	SAFE_DELETE( SprList->GUI->HSB->right_over );
	SAFE_DELETE( SprList->GUI->HSB->right_down );

	SAFE_DELETE( SprList->GUI->VSB->bar );
	SAFE_DELETE( SprList->GUI->VSB->bar_over );
	SAFE_DELETE( SprList->GUI->VSB->bar_down );
	SAFE_DELETE( SprList->GUI->VSB->up );
	SAFE_DELETE( SprList->GUI->VSB->up_over );
	SAFE_DELETE( SprList->GUI->VSB->up_down );
	SAFE_DELETE( SprList->GUI->VSB->down );
	SAFE_DELETE( SprList->GUI->VSB->down_over );
	SAFE_DELETE( SprList->GUI->VSB->down_down );
}