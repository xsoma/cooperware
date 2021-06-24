#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "valve_sdk/Misc/Color.hpp"

class Options
{
public:

	bool enablelgit;
	struct
	{
		struct
		{
			bool Enabled;
			int AimType;
			float Fov;
			float Smooth;
			bool Recoil;
			int Hitbox;
		} Rifles;
		struct
		{
			bool Enabled;
			int AimType;
			float Fov;
			float Smooth;
			bool Recoil;
			int Hitbox;
		} Pistols;
		struct
		{
			bool Enabled;
			int AimType;
			float Fov;
			float Smooth;
			bool Recoil;
			int Hitbox;
		} Snipers;
		bool Backtrack;
		int BacktrackTime = 200.f;
	} aimbot;


	struct
	{
		bool autofire;

		bool rageenable;
		bool pitchresolver;
		bool resolverenable;
		float body_max_rotation = 60.f;
		bool autodirection;
		bool antiaimenable;
		int pitch;
		int standingaa;
		int movingaa;
		int airaa;

		bool jitterenablestanding;
		bool jitterenablemoving;
		bool jitterenableair;
		int jittervaluestanding;
		int jittervaluemoving;
		int jittervalueair;

		bool desyncenable;
		int desynctype;

		int staticleftsidevalue;
		int staticrightsidevalue;
		int staticinverter;
		int staticjittervalue;

		int left;
		int right;
		int back;
		int front;

		int move_left;
		int move_right;
		int move_back;
		int move_front;

		int air_left;
		int air_right;
		int air_back;
		int air_front;

		int doubletab;
		int doubletap_key;


		bool rageautoscope;
		bool rageautostop;
		bool autorevolvo;
		bool autozeus;
		bool autoknife;


		int fakeduckkey;
		bool antifreestanding;
		bool baimafter3misses;
		int baimkey;
		bool baimiflethal;
		bool safepoint;

		//auto
		int auto_mindamagevisible;
		int auto_mindamage;
		int auto_hitchance;
		bool auto_multipoint;
		int auto_bodyscale;
		int auto_headscale;
		bool auto_head;
		bool auto_neck;
		bool auto_chest;
		bool auto_body;
		bool auto_stomach;
		bool auto_arms;
		bool auto_pelvis;
		bool auto_legs;
		bool auto_toes;

		//scout
		int scout_mindamagevisible;
		int scout_mindamage;
		int scout_hitchance;
		bool scout_multipoint;
		int scout_bodyscale;
		int scout_headscale;
		bool scout_head;
		bool scout_neck;
		bool scout_chest;
		bool scout_body;
		bool scout_stomach;
		bool scout_arms;
		bool scout_pelvis;
		bool scout_legs;
		bool scout_toes;

		//awp
		int awp_mindamagevisible;
		int awp_mindamage;
		int awp_hitchance;
		bool awp_multipoint;
		int awp_bodyscale;
		int awp_headscale;
		bool awp_head;
		bool awp_neck;
		bool awp_chest;
		bool awp_body;
		bool awp_stomach;
		bool awp_arms;
		bool awp_pelvis;
		bool awp_legs;
		bool awp_toes;

		//heavy_pistol
		int heavy_pistol_mindamagevisible;
		int heavy_pistol_mindamage;
		int heavy_pistol_hitchance;
		bool heavy_pistol_multipoint;
		int heavy_pistol_bodyscale;
		int heavy_pistol_headscale;
		bool heavy_pistol_head;
		bool heavy_pistol_neck;
		bool heavy_pistol_chest;
		bool heavy_pistol_body;
		bool heavy_pistol_stomach;
		bool heavy_pistol_arms;
		bool heavy_pistol_pelvis;
		bool heavy_pistol_legs;
		bool heavy_pistol_toes;

		//other
		int hitchance = 0;
		int minimumdmage = 0;
		int minimumdmagevisible = 0;
		bool multipoint;
		int headscale;
		int bodyscale;
		bool head;
		bool neck;
		bool chest;
		bool body;
		bool stomach;
		bool arms;
		bool pelvis;
		bool legs;
		bool toes;

		int slowwalkkey;
		bool fakeduckenable;
	} rageaimbot;


	struct
	{
		//visuals
		bool enemy_flags;

		bool enemy_box = false;
		bool enemy_name = false;
		bool enemy_weapon[3] = { false, false, false };
		bool enemy_health = false;
		bool enemy_health_based = false;
		bool enemy_armor = false;
		bool enemy_skeleton = false;
		bool enemy_skeleton_history = false;

		float enemy_box_col [ 3 ] = { 1.f, 1.f, 1.f };
		float enemy_name_col [ 3 ] = { 1.f, 1.f, 1.f };
		float enemy_weapon_col [ 3 ] = { 1.f, 1.f, 1.f };
		float enemy_health_col [ 3 ] = { 1.f, 1.f, 1.f };
		float enemy_armor_col [ 3 ] = { 1.f, 1.f, 1.f };

		float enemy_skeleton_col [ 3 ] = { 1.f, 1.f, 1.f };
		float enemy_skeleton_history_col [ 3 ] = { 1.f, 1.f, 1.f };

		bool weapon_dropped = false;
		bool weapon_dropped_box = false;
		bool weapon_dropped_name = false;

		float weapon_dropped_box_col[3] = { 1.f, 1.f, 1.f };
		float weapon_dropped_name_col[3] = { 1.f, 1.f, 1.f };

		bool drawc4 = false;
		bool enemy_gossip_arrow = false;
		float enemy_gossip_arrow_col [ 3 ] = { 1.f, 1.f, 1.f };
		bool pwatermark = false;
		bool drawnades = false;
		float draw_nades_col[3] = { 1.f, 1.f, 1.f };

		bool VisualsChamsEnabled = false;
		float VisualsChamsColor [ 4 ] = { 1.f, 1.f, 1.f, 1.f };
		int VisualsChamsMaterial = 2;
		bool VisualsChamsIgnoreZ = false;
		float VisualsChamsColorIgnoreZ [ 4 ] = { 1.f, 1.f, 1.f , 1.f };
		bool enemy_chams_layer = false;
		int enemy_material_chams_layer;
		float enemy_chams_layer_color[4] = { 1.f, 1.f, 1.f, 1.f };

		int VisualsLocalChamsMaterial;
		int VisualsDesyncChamsMaterial;
		int VisualsHandChamsMaterial;
		int chams_sleeve_material;
		int VisualsWeaponChamsMaterial;

		struct
		{
			bool team_chams_enabled = false;
			float team_chams_col[4] = { 1.f, 1.f, 1.f, 1.f };
			int team_chams_material;
			bool team_chams_invisible = false;
			float team_chams_invisible_col[4] = { 1.f, 1.f, 1.f , 1.f };

			bool team_chams_layer = false;
			int team_material_chams_layer;
			float team_chams_layer_color[4] = { 1.f, 1.f, 1.f, 1.f };
		} Team;

		bool desync_layer;
		int desync_layer_material;
		float desync_layer_color[ 4 ] = { 1.f, 1.f, 1.f, 1.f };
		bool indicator;
		bool local_layer;
		int local_layer_material;
		float local_layer_color[ 4 ] = { 1.f, 1.f, 1.f,1.f };

		bool hand_layer;
		int hand_layer_material;
		float hand_layer_color[ 4 ] = { 1.f, 1.f, 1.f,1.f };

		bool desyncchamsenable;
		float localdesynccol [ 4 ] = { 1.f, 1.f, 1.f, 1.f };

		bool enablelocalchams;
		float localchamscol [ 4 ] = { 1.f, 1.f, 1.f, 1.f };

		bool enablehandchams;
		float handchamscol [ 4 ] = { 1.f, 1.f, 1.f, 1.f };

		bool chams_sleeve;
		float chams_sleeve_col[4] = { 1.f, 1.f, 1.f, 1.f };

		bool enableweaponchams;
		float weaponchamscol [ 4 ] = { 1.f, 1.f, 1.f, 1.f };

		bool VisualsGlowEnabled = false;

		int VisualsGlowGlowstyle = 0;
		float VisualsGlowColor [ 4 ] = { 1.f, 1.f, 1.f, 1.f };
		//int VisualsGlowAlpha = 255;

		bool VisualsLocalGlowEnabled = false;

		int VisualsLocalGlowGlowstyle = 0;
		float VisualsLocalGlowColor [ 4 ] = { 1.f, 1.f, 1.f, 1.f };
		//int VisualsLocalGlowAlphaz = 255;

		float fieldofview = 0;
		float viewmodelfov = 68;
		float viewmodeloffsetx = 0;
		float viewmodeloffsety = 0;
		float viewmodeloffsetz = 0;
		bool aspectratioenable;
		int aspectratiox;
		int aspectratioxy;

		bool wasdindicator = false;
		bool playerhitmarker = false;
		int skychangar;
		bool nightmode = false;
		bool lagcomp_enable = false;
		float lagcomp_col[3] = { 1.f, 1.f, 1.f };
		float lagcomp_duration = 2;
		int hitsound;
		bool spreed_circle = false;
		float spreed_circle_col[4] = { 1.f, 1.f, 1.f };
		bool greane_prediction = false;
		bool beams = false;

		bool selfcircle = false;

		float markercol [ 3 ] = { 1.f, 1.f, 1.f };
		float beamcolor [ 3 ] = { 1.f, 1.f, 1.f };

		int thirdperson_key;

		bool infobar = false;
		int infobarx;
		int infobary;

		bool miscbar = false;
		int miscbarx;
		int miscbary;
		bool preservekillfeed = false;
		bool kill_effect = false;
		bool noscope = false;
		bool nopostprocsess = false;
		bool novisrecoil = false;
		bool nosmoke = false;
		bool noflash = false;
	} Visuals;

	struct
	{
		int MiscFakelagChoke = 0;

		//Moviment
		bool anti_afk_kick = false;
		bool bhop = false;
		bool autostrafe = false;
		bool fast_duck = false;
		bool moon_walk = false;

		//InGame
		bool reveal_rank = false;
		bool reveal_money = false;
		bool serverhitboxes = false;

		//Menu
		bool eventlogs = false;
		float event_logs_col[3] = { 0.06f, 0.65f, 0.86f };

		bool slowwalk = false;
		int slowwalkspeed;
		char config_name [ 52 ];
		int config_selection;
		char configname [ 128 ];
	} Misc;

	struct
	{
		bool Enabled;
		//knife
		int weapoinzz;
		int Knife;

		int customidknife;

		int customidusp;
		int customidp2000;
		int customidglock18;
		int customiddeagle;
		int customidrevolver;
		int customidawp;
		int customidssg08;
		int customiddual;
		int customidp250;
		int customidcz75;
		int customidfive7;
		int customidtec9;
		int customidnova;
		int customidxm1014;
		int customidmag7;
		int customidsawedoff;
		int customidnegev;
		int customidm249;
		int customidmp9;
		int customidmac10;
		int customidmp7;
		int customidump45;
		int customidp90;
		int customidppbizon;
		int customidGalil;
		int customidfamas;
		int customidm4a4;
		int customidm4a1;
		int customidak47;
		int customidaug;
		int customidsg553;
		int customidscar20;
		int customidg3sg1;
		int customidmp5;

	} Skinchanger;

	float menucolor [ 3 ] = { 0.06f, 0.65f, 0.86f };
};
inline Options Variables;
