#pragma once
#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../singleton.hpp"
#include "../imgui/imgui.h"


class Render: public Singleton<Render>
{
	friend class Singleton<Render>;
private:

public:

	vgui::HFont visualsFont;
	vgui::HFont weaponIconFont;
	vgui::HFont flagfont;


	vgui::HFont eventesp;
	vgui::HFont esp_font;
	vgui::HFont bombtimerfont;
	vgui::HFont ui_font;
	vgui::HFont wasdindicator2;
	vgui::HFont infobaryas;
	vgui::HFont infobaryas2;
	vgui::HFont indicators2;
	void CreateFonts();
	void Text(int X, int Y, const char* Text, vgui::HFont Font, Color DrawColor, bool Center);

	void TextSize(int& Width, int& Height, const char* Text, vgui::HFont Font);
	Vector2D GetTextSize(unsigned int Font, std::string Input, ...);
	void FilledRectange(int X1, int Y1, int X2, int Y2, Color DrawColor);
	void OutlinedRectange(int X1, int Y1, int X2, int Y2, Color DrawColor);
	void OutlinedCircle(int X1, int Y1, int Radius, int Segments, Color DrawColor);
	void Line(int X1, int Y1, int X2, int Y2, Color DrawColor);
	void DrawString(unsigned long font, int x, int y, Color color, unsigned long alignment, const char* msg, ...);
	void TexturedPolygon(int n, std::vector<Vertex_t> vertice, Color color);
};

class Chams: public Singleton<Chams>
{
	friend class Singleton<Chams>;
	Chams();
	~Chams();

public:
	void OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state,
		const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
private:

	IMaterial* materialRegular = nullptr;
	IMaterial* materialRegularIgnoreZ = nullptr;
	IMaterial* materialFlat = nullptr;
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialMetallic = nullptr;
	IMaterial* materialMetallicIgnoreZ = nullptr;
	IMaterial* materialDogtag = nullptr;
	IMaterial* materialGlowArmsrace = nullptr;
	IMaterial* materialEsoGlow = nullptr;
};

class Glow: public Singleton<Glow>
{
	friend class Singleton<Glow>;

	Glow();
	~Glow();

public:
	void Run();
	void Shutdown();
};

class Visuals : public Singleton<Visuals>
{
	friend class Singleton<Visuals>;
private:
	RECT GetBBox(C_BasePlayer* Player, Vector TransformedPoints[]);

	struct
	{
		C_BasePlayer* Player;
		RECT Box;
		Vector HeadPos;
		Vector Origin;
	} Context;

public:
	std::unordered_map< int, char > m_weapon_icons =
	{
		{ WEAPON_DEAGLE, 'A' },
		{ WEAPON_ELITE, 'B' },
		{ WEAPON_FIVESEVEN, 'C' },
		{ WEAPON_GLOCK, 'D' },
		{ WEAPON_AK47, 'W' },
		{ WEAPON_AUG, 'U' },
		{ WEAPON_AWP, 'Z' },
		{ WEAPON_FAMAS, 'R' },
		{ WEAPON_G3SG1, 'X' },
		{ WEAPON_GALILAR, 'Q' },
		{ WEAPON_M249, 'g' },
		{ WEAPON_M4A1, 'S' },
		{ WEAPON_MAC10, 'K' },
		{ WEAPON_P90, 'P' },
		{ WEAPON_UMP45, 'L' },
		{ WEAPON_XM1014, 'b' },
		{ WEAPON_BIZON, 'M' },
		{ WEAPON_MAG7, 'd' },
		{ WEAPON_NEGEV, 'f' },
		{ WEAPON_SAWEDOFF, 'c' },
		{ WEAPON_TEC9, 'H' },
		{ WEAPON_TASER, 'h' },
		{ WEAPON_HKP2000, 'E' },
		{ WEAPON_MP7, 'N' },
		{ WEAPON_MP9, 'O' },
		{ WEAPON_NOVA, 'e' },
		{ WEAPON_P250, 'F' },
		{ WEAPON_SCAR20, 'Y' },
		{ WEAPON_SG553, 'V' },
		{ WEAPON_SSG08, 'a' },
		{ WEAPON_KNIFE, ']' },
		{ WEAPON_FLASHBANG, 'i' },
		{ WEAPON_HEGRENADE, 'j' },
		{ WEAPON_SMOKEGRENADE, 'k' },
		{ WEAPON_MOLOTOV, 'l' },
		{ WEAPON_DECOY, 'm' },
		{ WEAPON_INCGRENADE, 'n' },
		{ WEAPON_C4, 'o' },
		{ WEAPON_KNIFE_T, '[' },
		{ WEAPON_M4A1_SILENCER, 'T' },
		{ WEAPON_USP_SILENCER, 'G' },
		{ WEAPON_CZ75A, 'I' },
		{ WEAPON_REVOLVER, 'J' },
		{ WEAPON_KNIFE_BAYONET, '1' },
		{ WEAPON_KNIFE_FLIP, '2' },
		{ WEAPON_KNIFE_GUT, '3' },
		{ WEAPON_KNIFE_KARAMBIT, '4' },
		{ WEAPON_KNIFE_M9_BAYONET, '5' },
		{ WEAPON_KNIFE_TACTICAL, '6' },
		{ WEAPON_KNIFE_FALCHION, '0' },
		{ WEAPON_KNIFE_SURVIVAL_BOWIE, '7' },
		{ WEAPON_KNIFE_BUTTERFLY, '8' },
		{ WEAPON_KNIFE_PUSH, '9' },
	};

	void Draw3dCube(float scalar, Vector angles, Vector middle_origin, Color outline);
	bool Begin(C_BasePlayer* Player);
	void Box();
	void Name();
	void Weapon();
	void Health();
	void RenderArmour();

	void RenderSkelet();
	void RenderSkeletBacktrack(C_BasePlayer* player);

	struct inferno_t : public C_BasePlayer
	{
		float get_spawn_time()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + 0x20);
		}

		static float get_expiry_time()
		{
			return 7.f;
		}
	};

	void drawbombplanetd(C_BasePlayer* entity);
	void RenderWeapon(C_BasePlayer* entity);
	void RenderNadeEsp(C_BasePlayer* nade);

	void DrawFovArrows(C_BasePlayer* entity);

	void debugshotsmissed(C_BasePlayer* entity);

	void aaindicator();

	void DrawFlags();

	void drawcirclearoundlocal();

	void run_viewmodel();
	void DrawPoV();
	void DrawSpreedCircle();
};