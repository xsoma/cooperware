#include "config.hpp"
#include <winerror.h>
#pragma warning( disable : 4091)
#include <ShlObj.h>
#include <string>
#include <sstream>

const char* GetWeaponNames(int id)
{
	switch (id)
	{
	case 1:
		return "deagle";
	case 2:
		return "elite";
	case 3:
		return "fiveseven";
	case 4:
		return "glock";
	case 7:
		return "ak47";
	case 8:
		return "aug";
	case 9:
		return "awp";
	case 10:
		return "famas";
	case 11:
		return "g3sg1";
	case 13:
		return "galilar";
	case 14:
		return "m249";
	case 60:
		return "m4a1_silencer";
	case 16:
		return "m4a1";
	case 17:
		return "mac10";
	case 19:
		return "p90";
	case 24:
		return "ump45";
	case 25:
		return "xm1014";
	case 26:
		return "bizon";
	case 27:
		return "mag7";
	case 28:
		return "negev";
	case 29:
		return "sawedoff";
	case 30:
		return "tec9";
	case 32:
		return "hkp2000";
	case 33:
		return "mp7";
	case 34:
		return "mp9";
	case 35:
		return "nova";
	case 36:
		return "p250";
	case 38:
		return "scar20";
	case 39:
		return "sg556";
	case 40:
		return "ssg08";
	case 61:
		return "usp_silencer";
	case 63:
		return "cz75a";
	case 64:
		return "revolver";
	case 508:
		return "knife_m9_bayonet";
	case 500:
		return "bayonet";
	case 505:
		return "knife_flip";
	case 506:
		return "knife_gut";
	case 507:
		return "knife_karambit";
	case 509:
		return "knife_tactical";
	case 512:
		return "knife_falchion";
	case 514:
		return "knife_survival_bowie";
	case 515:
		return "knife_butterfly";
	case 516:
		return "knife_push";
	case 517:
		return "knife_cord";
	case 518:
		return "knife_canis";
	case 519:
		return "knife_ursus";
	case 520:
		return "knife_gypsy_jackknife";
	case 522:
		return "knife_stiletto";
	case 521:
		return "knife_outdoor";
	case 523:
		return "knife_widowmaker";
	case 525:
		return "knife_skeleton";


	default:
		return "";
	}
}

void c_config::setup()
{
	//ragebot
	SetupValue(Variables.rageaimbot.rageenable, false, ("Ragebot"), ("Enable"));
	SetupValue(Variables.rageaimbot.resolverenable, false, ("Ragebot"), ("resolver"));
	SetupValue(Variables.rageaimbot.pitchresolver, false, ("Ragebot"), ("pitchresolver"));
	SetupValue(Variables.rageaimbot.autofire, false, ("Ragebot"), ("autoshoot"));
	SetupValue(Variables.rageaimbot.doubletab, 0, ("Ragebot"), ("doubletab"));
	SetupValue(Variables.rageaimbot.doubletap_key, 0, ("Ragebot"), ("doubletap_key"));
	SetupValue(Variables.rageaimbot.autozeus, false, ("Ragebot"), ("autozeus"));
	SetupValue(Variables.rageaimbot.autoknife, false, ("Ragebot"), ("autoknife"));
	SetupValue(Variables.rageaimbot.autorevolvo, false, ("Ragebot"), ("autorevolvover"));
	SetupValue(Variables.rageaimbot.rageautoscope, false, ("Ragebot"), ("autoscope"));
	SetupValue(Variables.rageaimbot.rageautostop, false, ("Ragebot"), ("autostop"));

	SetupValue(Variables.rageaimbot.safepoint, false, ("Ragebot"), ("safepoint"));
	SetupValue(Variables.rageaimbot.baimiflethal, false, ("Ragebot"), ("baimiflethal"));
	SetupValue(Variables.rageaimbot.baimiflethal, false, ("Ragebot"), ("baimiflethal"));
	SetupValue(Variables.rageaimbot.baimkey, 0, ("Ragebot"), ("baimkey"));

	//ragebot weapon config 
	//auto
	SetupValue(Variables.rageaimbot.auto_multipoint, false, ("Ragebot"), ("auto_multipoint"));
	SetupValue(Variables.rageaimbot.auto_bodyscale, 0, ("Ragebot"), ("auto_bodyscale"));
	SetupValue(Variables.rageaimbot.auto_headscale, 0, ("Ragebot"), ("auto_headscale"));
	SetupValue(Variables.rageaimbot.auto_hitchance, 0, ("Ragebot"), ("auto_hitchance"));
	SetupValue(Variables.rageaimbot.auto_mindamage, 0, ("Ragebot"), ("auto_minimumdmage"));
	SetupValue(Variables.rageaimbot.auto_mindamagevisible, 0, ("Ragebot"), ("auto_minimumdmagevisible"));
	SetupValue(Variables.rageaimbot.auto_head, false, ("Ragebot"), ("auto_head"));
	SetupValue(Variables.rageaimbot.auto_neck, false, ("Ragebot"), ("auto_neck"));
	SetupValue(Variables.rageaimbot.auto_chest, false, ("Ragebot"), ("auto_chest"));
	SetupValue(Variables.rageaimbot.auto_arms, false, ("Ragebot"), ("auto_arms"));
	SetupValue(Variables.rageaimbot.auto_stomach, false, ("Ragebot"), ("auto_stomach"));
	SetupValue(Variables.rageaimbot.auto_pelvis, false, ("Ragebot"), ("auto_pelvis"));
	SetupValue(Variables.rageaimbot.auto_body, false, ("Ragebot"), ("auto_body"));
	SetupValue(Variables.rageaimbot.auto_legs, false, ("Ragebot"), ("auto_legs"));
	SetupValue(Variables.rageaimbot.auto_toes, false, ("Ragebot"), ("auto_toes"));

	//scout
	SetupValue(Variables.rageaimbot.scout_multipoint, false, ("Ragebot"), ("scout_multipoint"));
	SetupValue(Variables.rageaimbot.scout_bodyscale, 0, ("Ragebot"), ("scout_bodyscale"));
	SetupValue(Variables.rageaimbot.scout_headscale, 0, ("Ragebot"), ("scout_headscale"));
	SetupValue(Variables.rageaimbot.scout_hitchance, 0, ("Ragebot"), ("scout_hitchance"));
	SetupValue(Variables.rageaimbot.scout_mindamage, 0, ("Ragebot"), ("scout_minimumdmage"));
	SetupValue(Variables.rageaimbot.scout_mindamagevisible, 0, ("Ragebot"), ("scout_minimumdmagevisible"));
	SetupValue(Variables.rageaimbot.scout_head, false, ("Ragebot"), ("scout_head"));
	SetupValue(Variables.rageaimbot.scout_neck, false, ("Ragebot"), ("scout_neck"));
	SetupValue(Variables.rageaimbot.scout_chest, false, ("Ragebot"), ("scout_chest"));
	SetupValue(Variables.rageaimbot.scout_arms, false, ("Ragebot"), ("scout_arms"));
	SetupValue(Variables.rageaimbot.scout_stomach, false, ("Ragebot"), ("scout_stomach"));
	SetupValue(Variables.rageaimbot.scout_pelvis, false, ("Ragebot"), ("scout_pelvis"));
	SetupValue(Variables.rageaimbot.scout_body, false, ("Ragebot"), ("scout_body"));
	SetupValue(Variables.rageaimbot.scout_legs, false, ("Ragebot"), ("scout_legs"));
	SetupValue(Variables.rageaimbot.scout_toes, false, ("Ragebot"), ("scout_toes"));

	//awp
	SetupValue(Variables.rageaimbot.awp_multipoint, false, ("Ragebot"), ("awp_multipoint"));
	SetupValue(Variables.rageaimbot.awp_bodyscale, 0, ("Ragebot"), ("awp_bodyscale"));
	SetupValue(Variables.rageaimbot.awp_headscale, 0, ("Ragebot"), ("awp_headscale"));
	SetupValue(Variables.rageaimbot.awp_hitchance, 0, ("Ragebot"), ("awp_hitchance"));
	SetupValue(Variables.rageaimbot.awp_mindamage, 0, ("Ragebot"), ("awp_minimumdmage"));
	SetupValue(Variables.rageaimbot.awp_mindamagevisible, 0, ("Ragebot"), ("awp_minimumdmagevisible"));
	SetupValue(Variables.rageaimbot.awp_head, false, ("Ragebot"), ("awp_head"));
	SetupValue(Variables.rageaimbot.awp_neck, false, ("Ragebot"), ("awp_neck"));
	SetupValue(Variables.rageaimbot.awp_chest, false, ("Ragebot"), ("awp_chest"));
	SetupValue(Variables.rageaimbot.awp_arms, false, ("Ragebot"), ("awp_arms"));
	SetupValue(Variables.rageaimbot.awp_stomach, false, ("Ragebot"), ("awp_stomach"));
	SetupValue(Variables.rageaimbot.awp_pelvis, false, ("Ragebot"), ("awp_pelvis"));
	SetupValue(Variables.rageaimbot.awp_body, false, ("Ragebot"), ("awp_body"));
	SetupValue(Variables.rageaimbot.awp_legs, false, ("Ragebot"), ("awp_legs"));
	SetupValue(Variables.rageaimbot.awp_toes, false, ("Ragebot"), ("awp_toes"));

	//heavy_pistol
	SetupValue(Variables.rageaimbot.heavy_pistol_multipoint, false, ("Ragebot"), ("heavy_pistol_multipoint"));
	SetupValue(Variables.rageaimbot.heavy_pistol_bodyscale, 0, ("Ragebot"), ("heavy_pistol_bodyscale"));
	SetupValue(Variables.rageaimbot.heavy_pistol_headscale, 0, ("Ragebot"), ("heavy_pistol_headscale"));
	SetupValue(Variables.rageaimbot.heavy_pistol_hitchance, 0, ("Ragebot"), ("heavy_pistol_hitchance"));
	SetupValue(Variables.rageaimbot.heavy_pistol_mindamage, 0, ("Ragebot"), ("heavy_pistol_minimumdmage"));
	SetupValue(Variables.rageaimbot.heavy_pistol_mindamagevisible, 0, ("Ragebot"), ("heavy_pistol_minimumdmagevisible"));
	SetupValue(Variables.rageaimbot.heavy_pistol_head, false, ("Ragebot"), ("heavy_pistol_head"));
	SetupValue(Variables.rageaimbot.heavy_pistol_neck, false, ("Ragebot"), ("heavy_pistol_neck"));
	SetupValue(Variables.rageaimbot.heavy_pistol_chest, false, ("Ragebot"), ("heavy_pistol_chest"));
	SetupValue(Variables.rageaimbot.heavy_pistol_arms, false, ("Ragebot"), ("heavy_pistol_arms"));
	SetupValue(Variables.rageaimbot.heavy_pistol_stomach, false, ("Ragebot"), ("heavy_pistol_stomach"));
	SetupValue(Variables.rageaimbot.heavy_pistol_pelvis, false, ("Ragebot"), ("heavy_pistol_pelvis"));
	SetupValue(Variables.rageaimbot.heavy_pistol_body, false, ("Ragebot"), ("heavy_pistol_body"));
	SetupValue(Variables.rageaimbot.heavy_pistol_legs, false, ("Ragebot"), ("heavy_pistol_legs"));
	SetupValue(Variables.rageaimbot.heavy_pistol_toes, false, ("Ragebot"), ("heavy_pistol_toes"));

	//other
	SetupValue(Variables.rageaimbot.multipoint, false, ("Ragebot"), ("multipoint"));
	SetupValue(Variables.rageaimbot.bodyscale, 0, ("Ragebot"), ("bodyscale"));
	SetupValue(Variables.rageaimbot.headscale, 0, ("Ragebot"), ("headscale"));
	SetupValue(Variables.rageaimbot.hitchance, 0, ("Ragebot"), ("hitchance"));
	SetupValue(Variables.rageaimbot.minimumdmage, 0, ("Ragebot"), ("minimumdmage"));
	SetupValue(Variables.rageaimbot.minimumdmagevisible, 0, ("Ragebot"), ("minimumdmagevisible"));
	SetupValue(Variables.rageaimbot.head, false, ("Ragebot"), ("head"));
	SetupValue(Variables.rageaimbot.neck, false, ("Ragebot"), ("neck"));
	SetupValue(Variables.rageaimbot.chest, false, ("Ragebot"), ("chest"));
	SetupValue(Variables.rageaimbot.arms, false, ("Ragebot"), ("arms"));
	SetupValue(Variables.rageaimbot.stomach, false, ("Ragebot"), ("stomach"));
	SetupValue(Variables.rageaimbot.pelvis, false, ("Ragebot"), ("pelvis"));
	SetupValue(Variables.rageaimbot.body, false, ("Ragebot"), ("body"));
	SetupValue(Variables.rageaimbot.legs, false, ("Ragebot"), ("legs"));
	SetupValue(Variables.rageaimbot.toes, false, ("Ragebot"), ("toes"));



	//antiaim
	SetupValue(Variables.rageaimbot.antiaimenable, false, ("AntiAim"), ("Enable"));
	SetupValue(Variables.rageaimbot.pitch, 0, ("AntiAim"), ("pitch"));
	SetupValue(Variables.rageaimbot.autodirection, false, ("AntiAim"), ("autodirection"));
	//standing
	SetupValue(Variables.rageaimbot.standingaa, 0, ("AntiAim"), ("standingaa"));
	SetupValue(Variables.rageaimbot.jitterenablestanding, false, ("AntiAim"), ("jitterenablestanding"));
	SetupValue(Variables.rageaimbot.jittervaluestanding, 0, ("AntiAim"), ("jittervaluestanding"));
	//moving
	SetupValue(Variables.rageaimbot.movingaa, 0, ("AntiAim"), ("movingaa"));
	SetupValue(Variables.rageaimbot.jitterenablemoving, false, ("AntiAim"), ("jitterenablemoving"));
	SetupValue(Variables.rageaimbot.jittervaluemoving, 0, ("AntiAim"), ("jittervaluemoving"));
	//air
	SetupValue(Variables.rageaimbot.airaa, 0, ("AntiAim"), ("airaa"));
	SetupValue(Variables.rageaimbot.jitterenableair, false, ("AntiAim"), ("jitterenableair"));
	SetupValue(Variables.rageaimbot.jittervalueair, 0, ("AntiAim"), ("jittervalueair"));
	//desync antiaim
	SetupValue(Variables.rageaimbot.desynctype, 0, ("AntiAim"), ("desynctype"));
	//bodylean jitter and inverter key
	SetupValue(Variables.rageaimbot.staticinverter, 0, ("AntiAim"), ("staticinverter"));
	SetupValue(Variables.rageaimbot.staticrightsidevalue, 0, ("AntiAim"), ("staticrightsidevalue"));
	SetupValue(Variables.rageaimbot.staticleftsidevalue, 0, ("AntiAim"), ("staticleftsidevalue"));
	SetupValue(Variables.rageaimbot.staticjittervalue, 0, ("AntiAim"), ("staticjittervalue"));
	//manual aa
	SetupValue(Variables.rageaimbot.left, 0, ("AntiAim"), ("left"));
	SetupValue(Variables.rageaimbot.right, 0, ("AntiAim"), ("right"));
	SetupValue(Variables.rageaimbot.back, 0, ("AntiAim"), ("back"));
	SetupValue(Variables.rageaimbot.front, 0, ("AntiAim"), ("front"));
	//flag misc
	//slowwalk
	SetupValue(Variables.Misc.slowwalk, false, ("Misc"), ("slowwalk"));
	SetupValue(Variables.Misc.slowwalkspeed, 0, ("Misc"), ("slowwalkspeed"));
	//fakelag
	SetupValue(Variables.Misc.MiscFakelagChoke, 0, ("Misc"), ("fakelagchoke"));
	SetupValue(Variables.rageaimbot.fakeduckkey, 0, ("Misc"), ("fakeduckkey"));




	//legitbot
	SetupValue(Variables.enablelgit, false, ("Legit"), ("Enable"));
	SetupValue(Variables.aimbot.Backtrack, false, ("Legit"), ("Backtrack"));
	SetupValue(Variables.aimbot.BacktrackTime, 0, ("Legit"), ("bttime"));
	//weapon cfgs //snipers
	SetupValue(Variables.aimbot.Snipers.Enabled, false, ("Legit"), ("SnipersEnabled"));
	SetupValue(Variables.aimbot.Snipers.Recoil, false, ("Legit"), ("SnipersRecoil"));
	SetupValue(Variables.aimbot.Snipers.AimType, 0, ("Legit"), ("SnipersAimType"));
	SetupValue(Variables.aimbot.Snipers.Fov, 0, ("Legit"), ("SnipersFov"));
	SetupValue(Variables.aimbot.Snipers.Hitbox, 0, ("Legit"), ("SnipersHitbox"));
	//pistols
	SetupValue(Variables.aimbot.Pistols.Enabled, false, ("Legit"), ("PistolsEnabled"));
	SetupValue(Variables.aimbot.Pistols.Recoil, false, ("Legit"), ("PistolsRecoil"));
	SetupValue(Variables.aimbot.Pistols.AimType, 0, ("Legit"), ("PistolsAimType"));
	SetupValue(Variables.aimbot.Pistols.Fov, 0, ("Legit"), ("PistolsFov"));
	SetupValue(Variables.aimbot.Pistols.Hitbox, 0, ("Legit"), ("PistolsHitbox"));
	//rifle
	SetupValue(Variables.aimbot.Rifles.Enabled, false, ("Legit"), ("RiflesEnabled"));
	SetupValue(Variables.aimbot.Rifles.Recoil, false, ("Legit"), ("RiflesRecoil"));
	SetupValue(Variables.aimbot.Rifles.AimType, 0, ("Legit"), ("RiflesAimType"));
	SetupValue(Variables.aimbot.Rifles.Fov, 0, ("Legit"), ("RiflesFov"));
	SetupValue(Variables.aimbot.Rifles.Hitbox, 0, ("Legit"), ("RiflesHitbox"));



	//world
	SetupValue(Variables.Visuals.skychangar, 0, ("Visuals"), ("skychangar"));
	SetupValue(Variables.Visuals.nightmode, false, ("Visuals"), ("nightmode"));
	SetupValue(Variables.Visuals.playerhitmarker, false, ("Visuals"), ("playerhitmarker"));
	SetupValue(Variables.Visuals.hitsound, 0, ("Visuals"), ("hitsound"));
	SetupValue(Variables.Visuals.spreed_circle, false, ("Visuals"), ("spreed_circle"));
	SetupValue(Variables.Visuals.greane_prediction, false, ("Visuals"), ("greane_prediction"));
	SetupValue(Variables.Visuals.beams, false, ("Visuals"), ("beams"));
	SetupValue(Variables.Visuals.lagcomp_enable, false, ("Visuals"), ("lagcomp_enable"));
	SetupValue(Variables.Visuals.lagcomp_duration, 2, ("Visuals"), ("lagcomp_duration"));
	SetupValue(Variables.Visuals.selfcircle, false, ("Visuals"), ("selfcircle"));
	//helpers
	SetupValue(Variables.Visuals.drawc4, 0, ("Visuals"), ("drawc4"));
	SetupValue(Variables.Visuals.drawnades, false, ("Visuals"), ("drawnades"));
	SetupValue(Variables.Visuals.weapon_dropped, false, ("Visuals"), ("weapon_dropped"));
	SetupValue(Variables.Visuals.weapon_dropped_box, false, ("Visuals"), ("weapon_dropped_box"));
	SetupValue(Variables.Visuals.weapon_dropped_name, false, ("Visuals"), ("weapon_dropped_name"));
	//misc visuals
	SetupValue(Variables.Visuals.wasdindicator, false, ("Visuals"), ("wasdindicator"));
	SetupValue(Variables.Visuals.infobar, false, ("Visuals"), ("infobar"));
	SetupValue(Variables.Visuals.infobarx, 0, ("Visuals"), ("infobarx"));
	SetupValue(Variables.Visuals.infobary, 0, ("Visuals"), ("infobary"));
	SetupValue(Variables.Visuals.miscbar, false, ("Visuals"), ("miscbar"));
	SetupValue(Variables.Visuals.kill_effect, false, ("Visuals"), ("kill_effect"));
	SetupValue(Variables.Visuals.miscbarx, 0, ("Visuals"), ("miscbarx"));
	SetupValue(Variables.Visuals.miscbary, 0, ("Visuals"), ("miscbary"));
	SetupValue(Variables.Visuals.preservekillfeed, false, ("Visuals"), ("preservekillfeed"));
	SetupValue(Variables.Visuals.thirdperson_key, 0, ("Visuals"), ("thirdperson_key"));


	//removals
	SetupValue(Variables.Visuals.novisrecoil, false, ("Visuals"), ("novisrecoil"));
	SetupValue(Variables.Visuals.nopostprocsess, false, ("Visuals"), ("nopostprocsess"));
	SetupValue(Variables.Visuals.noscope, false, ("Visuals"), ("noscope"));
	SetupValue(Variables.Visuals.nosmoke, false, ("Visuals"), ("nosmoke"));
	SetupValue(Variables.Visuals.noflash, false, ("Visuals"), ("noflash"));


	//viewmodel changer
	SetupValue(Variables.Visuals.fieldofview, 0, ("Visuals"), ("fieldofview"));
	SetupValue(Variables.Visuals.viewmodelfov, 68, ("Visuals"), ("viewmodelfov"));
	SetupValue(Variables.Visuals.viewmodeloffsetx, 0, ("Visuals"), ("viewmodeloffsetx"));
	SetupValue(Variables.Visuals.viewmodeloffsety, 0, ("Visuals"), ("viewmodeloffsety"));
	SetupValue(Variables.Visuals.viewmodeloffsetz, 0, ("Visuals"), ("viewmodeloffsetz"));
	//aspect ratio changer
	SetupValue(Variables.Visuals.aspectratioenable, false, ("Visuals"), ("aspectratioenable"));
	SetupValue(Variables.Visuals.aspectratiox, 0, ("Visuals"), ("aspectratiox"));
	SetupValue(Variables.Visuals.aspectratioxy, 0, ("Visuals"), ("aspectratioxy"));



	///Player Enemy
	SetupValue(Variables.Visuals.enemy_box, false, ("VisualsEnemy"), ("enemy_box"));
	SetupValue(Variables.Visuals.enemy_skeleton, false, ("VisualsEnemy"), ("enemy_skeleton"));
	SetupValue(Variables.Visuals.enemy_skeleton_history, false, ("VisualsEnemy"), ("enemy_history_skeleton"));
	SetupValue(Variables.Visuals.enemy_health, false, ("VisualsEnemy"), ("enemy_health_bar"));
	SetupValue(Variables.Visuals.enemy_health_based, false, ("VisualsEnemy"), ("enemy_health_based"));
	SetupValue(Variables.Visuals.enemy_armor, false, ("VisualsEnemy"), ("enemy_armor_bar"));
	SetupValue(Variables.Visuals.enemy_name, false, ("VisualsEnemy"), ("enemy_name"));
	SetupValue(Variables.Visuals.enemy_weapon[0], false, ("VisualsEnemy"), ("enemy_weapon_name"));
	SetupValue(Variables.Visuals.enemy_weapon[1], false, ("VisualsEnemy"), ("enemy_weapon_icon"));
	SetupValue(Variables.Visuals.enemy_weapon[2], false, ("VisualsEnemy"), ("enemy_weapon_amount"));
	SetupValue(Variables.Visuals.enemy_flags, false, ("VisualsEnemy"), ("enemy_flags"));
	SetupValue(Variables.Visuals.enemy_gossip_arrow, false, ("VisualsEnemy"), ("enemy_gossip_arrow"));
	//Chams
	SetupValue(Variables.Visuals.VisualsChamsEnabled, false, ("VisualsEnemy"), ("enemy_chams"));
	SetupValue(Variables.Visuals.VisualsChamsIgnoreZ, false, ("VisualsEnemy"), ("enemy_chams_invisible"));
	SetupValue(Variables.Visuals.VisualsChamsMaterial, 0, ("VisualsEnemy"), ("enemy_material_chams"));
	//Chams Layer
	SetupValue(Variables.Visuals.enemy_chams_layer, false, ("VisualsEnemy"), ("enemy_chams_layer"));
	SetupValue(Variables.Visuals.enemy_material_chams_layer, 0, ("VisualsEnemy"), ("enemy_material_chams_layer"));
	//Glow
	SetupValue(Variables.Visuals.VisualsGlowEnabled, false, ("VisualsEnemy"), ("VisualsGlowEnabled"));
	SetupValue(Variables.Visuals.VisualsGlowGlowstyle, 0, ("VisualsEnemy"), ("VisualsGlowGlowstyle"));



	///Player Local
	//Chams
	SetupValue(Variables.Visuals.enablelocalchams, false, ("VisualsLocal"), ("enablelocalchams"));
	SetupValue(Variables.Visuals.VisualsLocalChamsMaterial, 0, ("VisualsLocal"), ("VisualsLocalChamsMaterial"));
	//Chams Layer
	SetupValue(Variables.Visuals.local_layer, false, ("VisualsLocal"), ("local_chams_layer"));
	SetupValue(Variables.Visuals.local_layer_material, 0, ("VisualsLocal"), ("local_material_chams_layer"));
	//Chams Desync
	SetupValue(Variables.Visuals.desyncchamsenable, false, ("VisualsLocal"), ("desyncchamsenable"));
	SetupValue(Variables.Visuals.VisualsDesyncChamsMaterial, 0, ("VisualsLocal"), ("VisualsDesyncChamsMaterial"));
	//Chams Desync Layer
	SetupValue(Variables.Visuals.desync_layer, false, ("VisualsLocal"), ("local_chams_desync_layer"));
	SetupValue(Variables.Visuals.desync_layer_material, 0, ("VisualsLocal"), ("local_material_desync_chams_layer"));
	//Chams Hand
	SetupValue(Variables.Visuals.enablehandchams, false, ("VisualsLocal"), ("handchamsenable"));
	SetupValue(Variables.Visuals.VisualsHandChamsMaterial, 0, ("VisualsLocal"), ("VisualsHandChamsMaterial"));
	//Chams Sleeve
	SetupValue(Variables.Visuals.chams_sleeve, false, ("VisualsLocal"), ("chams_sleeve"));
	SetupValue(Variables.Visuals.chams_sleeve_material, 0, ("VisualsLocal"), ("chams_sleeve_material"));
	//Chams Weapon
	SetupValue(Variables.Visuals.enableweaponchams, false, ("VisualsLocal"), ("weaponchamsenable"));
	SetupValue(Variables.Visuals.VisualsWeaponChamsMaterial, 0, ("VisualsLocal"), ("VisualsweaponChamsMaterial"));
	//Glow
	SetupValue(Variables.Visuals.VisualsLocalGlowEnabled, false, ("VisualsLocal"), ("VisualsLocalGlowEnabled"));
	SetupValue(Variables.Visuals.VisualsLocalGlowGlowstyle, 0, ("VisualsLocal"), ("VisualsLocalGlowGlowstyle"));



	///Player Team
	//Chams
	SetupValue(Variables.Visuals.Team.team_chams_enabled, false, ("VisualsTeam"), ("team_chams"));
	SetupValue(Variables.Visuals.Team.team_chams_invisible, false, ("VisualsTeam"), ("team_chams_invisible"));
	SetupValue(Variables.Visuals.Team.team_chams_material, 0, ("VisualsTeam"), ("team_chams_material"));
	//Chams Layer
	SetupValue(Variables.Visuals.Team.team_chams_layer, false, ("VisualsTeam"), ("team_chams_layer"));
	SetupValue(Variables.Visuals.Team.team_material_chams_layer, 0, ("VisualsTeam"), ("team_material_chams_layer"));



	///Colors Player Enemy
	//Box Color
	SetupValue(Variables.Visuals.enemy_box_col[0], 1.f, ("VisualsEnemyColor"), ("enemy_box_color_r"));
	SetupValue(Variables.Visuals.enemy_box_col[1], 1.f, ("VisualsEnemyColor"), ("enemy_box_color_g"));
	SetupValue(Variables.Visuals.enemy_box_col[2], 1.f, ("VisualsEnemyColor"), ("enemy_box_color_b"));
	//Skeleton Color
	SetupValue(Variables.Visuals.enemy_skeleton_col[0], 1.f, ("VisualsEnemyColor"), ("enemy_skeleton_color_r"));
	SetupValue(Variables.Visuals.enemy_skeleton_col[1], 1.f, ("VisualsEnemyColor"), ("enemy_skeleton_color_g"));
	SetupValue(Variables.Visuals.enemy_skeleton_col[2], 1.f, ("VisualsEnemyColor"), ("enemy_skeleton_color_b"));
	//History Skeleton Color
	SetupValue(Variables.Visuals.enemy_skeleton_history_col[0], 1.f, ("VisualsEnemyColor"), ("history_skeleton_color_r"));
	SetupValue(Variables.Visuals.enemy_skeleton_history_col[1], 1.f, ("VisualsEnemyColor"), ("history_skeleton_color_g"));
	SetupValue(Variables.Visuals.enemy_skeleton_history_col[2], 1.f, ("VisualsEnemyColor"), ("history_skeleton_color_b"));
	//Health Color
	SetupValue(Variables.Visuals.enemy_health_col[0], 1.f, ("VisualsEnemyColor"), ("enemy_health_bar_color_r"));
	SetupValue(Variables.Visuals.enemy_health_col[1], 1.f, ("VisualsEnemyColor"), ("enemy_health_bar_color_g"));
	SetupValue(Variables.Visuals.enemy_health_col[2], 1.f, ("VisualsEnemyColor"), ("enemy_health_bar_color_b"));
	//Armor Color
	SetupValue(Variables.Visuals.enemy_armor_col[0], 1.f, ("VisualsEnemyColor"), ("enemy_armor_bar_color_r"));
	SetupValue(Variables.Visuals.enemy_armor_col[1], 1.f, ("VisualsEnemyColor"), ("enemy_armor_bar_color_g"));
	SetupValue(Variables.Visuals.enemy_armor_col[2], 1.f, ("VisualsEnemyColor"), ("enemy_armor_bar_color_b"));
	//Name Color
	SetupValue(Variables.Visuals.enemy_name_col[0], 1.f, ("VisualsEnemyColor"), ("enemy_name_color_r"));
	SetupValue(Variables.Visuals.enemy_name_col[1], 1.f, ("VisualsEnemyColor"), ("enemy_name_color_g"));
	SetupValue(Variables.Visuals.enemy_name_col[2], 1.f, ("VisualsEnemyColor"), ("enemy_name_color_b"));
	//Weapon Color
	SetupValue(Variables.Visuals.enemy_weapon_col[0], 1.f, ("VisualsEnemyColor"), ("enemy_weapon_color_r"));
	SetupValue(Variables.Visuals.enemy_weapon_col[1], 1.f, ("VisualsEnemyColor"), ("enemy_weapon_color_g"));
	SetupValue(Variables.Visuals.enemy_weapon_col[2], 1.f, ("VisualsEnemyColor"), ("enemy_weapon_color_b"));
	//Gossip Arrow Color
	SetupValue(Variables.Visuals.enemy_gossip_arrow_col[0], 1.f, ("VisualsEnemyColor"), ("enemy_gossip_arrow_color_r"));
	SetupValue(Variables.Visuals.enemy_gossip_arrow_col[1], 1.f, ("VisualsEnemyColor"), ("enemy_gossip_arrow_color_g"));
	SetupValue(Variables.Visuals.enemy_gossip_arrow_col[2], 1.f, ("VisualsEnemyColor"), ("enemy_gossip_arrow_color_b"));
	//Chams Color
	SetupValue(Variables.Visuals.VisualsChamsColor[0], 1.f, ("VisualsEnemyColor"), ("enemy_chams_color_r"));
	SetupValue(Variables.Visuals.VisualsChamsColor[1], 1.f, ("VisualsEnemyColor"), ("enemy_chams_color_g"));
	SetupValue(Variables.Visuals.VisualsChamsColor[2], 1.f, ("VisualsEnemyColor"), ("enemy_chams_color_b"));
	SetupValue(Variables.Visuals.VisualsChamsColor[3], 1.f, ("VisualsEnemyColor"), ("enemy_chams_color_a"));
	//Chams Invisible Color
	SetupValue(Variables.Visuals.VisualsChamsColorIgnoreZ[0], 1.f, ("VisualsEnemyColor"), ("enemy_chams_invisible_color_r"));
	SetupValue(Variables.Visuals.VisualsChamsColorIgnoreZ[1], 1.f, ("VisualsEnemyColor"), ("enemy_chams_invisible_color_g"));
	SetupValue(Variables.Visuals.VisualsChamsColorIgnoreZ[2], 1.f, ("VisualsEnemyColor"), ("enemy_chams_invisible_color_b"));
	SetupValue(Variables.Visuals.VisualsChamsColorIgnoreZ[3], 1.f, ("VisualsEnemyColor"), ("enemy_chams_invisible_color_a"));
	//Chams Layer Color
	SetupValue(Variables.Visuals.enemy_chams_layer_color[0], 1.f, ("VisualsEnemyColor"), ("enemy_chams_layer_color_r"));
	SetupValue(Variables.Visuals.enemy_chams_layer_color[1], 1.f, ("VisualsEnemyColor"), ("enemy_chams_layer_color_g"));
	SetupValue(Variables.Visuals.enemy_chams_layer_color[2], 1.f, ("VisualsEnemyColor"), ("enemy_chams_layer_color_b"));
	SetupValue(Variables.Visuals.enemy_chams_layer_color[3], 1.f, ("VisualsEnemyColor"), ("enemy_chams_layer_color_a"));
	//Glow Color
	SetupValue(Variables.Visuals.VisualsGlowColor[0], 1.f, ("VisualsEnemyColor"), ("enemy_glow_color_r"));
	SetupValue(Variables.Visuals.VisualsGlowColor[1], 1.f, ("VisualsEnemyColor"), ("enemy_glow_color_g"));
	SetupValue(Variables.Visuals.VisualsGlowColor[2], 1.f, ("VisualsEnemyColor"), ("enemy_glow_color_b"));
	SetupValue(Variables.Visuals.VisualsGlowColor[3], 1.f, ("VisualsEnemyColor"), ("enemy_glow_color_a"));



	///Colors Player Local
	//Chams Color
	SetupValue(Variables.Visuals.localchamscol[0], 1.f, ("VisualsLocalColor"), ("local_chams_color_r"));
	SetupValue(Variables.Visuals.localchamscol[1], 1.f, ("VisualsLocalColor"), ("local_chams_color_g"));
	SetupValue(Variables.Visuals.localchamscol[2], 1.f, ("VisualsLocalColor"), ("local_chams_color_b"));
	SetupValue(Variables.Visuals.localchamscol[3], 1.f, ("VisualsLocalColor"), ("local_chams_color_a"));
	//Chams Layer Color
	SetupValue(Variables.Visuals.local_layer_color[0], 1.f, ("VisualsLocalColor"), ("local_chams_layer_color_r"));
	SetupValue(Variables.Visuals.local_layer_color[1], 1.f, ("VisualsLocalColor"), ("local_chams_layer_color_g"));
	SetupValue(Variables.Visuals.local_layer_color[2], 1.f, ("VisualsLocalColor"), ("local_chams_layer_color_b"));
	SetupValue(Variables.Visuals.local_layer_color[3], 1.f, ("VisualsLocalColor"), ("local_chams_layer_color_a"));
	//Chams Desync Color
	SetupValue(Variables.Visuals.localdesynccol[0], 1.f, ("VisualsLocalColor"), ("local_chams_desync_color_r"));
	SetupValue(Variables.Visuals.localdesynccol[1], 1.f, ("VisualsLocalColor"), ("local_chams_desync_color_g"));
	SetupValue(Variables.Visuals.localdesynccol[2], 1.f, ("VisualsLocalColor"), ("local_chams_desync_color_b"));
	SetupValue(Variables.Visuals.localdesynccol[3], 1.f, ("VisualsLocalColor"), ("local_chams_desync_color_a"));
	//Chams Desync Layer Color
	SetupValue(Variables.Visuals.desync_layer_color[0], 1.f, ("VisualsLocalColor"), ("local_desync_chams_layer_color_r"));
	SetupValue(Variables.Visuals.desync_layer_color[1], 1.f, ("VisualsLocalColor"), ("local_desync_chams_layer_color_g"));
	SetupValue(Variables.Visuals.desync_layer_color[2], 1.f, ("VisualsLocalColor"), ("local_desync_chams_layer_color_b"));
	SetupValue(Variables.Visuals.desync_layer_color[3], 1.f, ("VisualsLocalColor"), ("local_desync_chams_layer_color_a"));
	//Chams Hand Color
	SetupValue(Variables.Visuals.handchamscol[0], 1.f, ("VisualsLocalColor"), ("hand_chams_color_r"));
	SetupValue(Variables.Visuals.handchamscol[1], 1.f, ("VisualsLocalColor"), ("hand_chams_color_g"));
	SetupValue(Variables.Visuals.handchamscol[2], 1.f, ("VisualsLocalColor"), ("hand_chams_color_b"));
	SetupValue(Variables.Visuals.handchamscol[3], 1.f, ("VisualsLocalColor"), ("hand_chams_color_a"));
	//Chams Sleeve Color
	SetupValue(Variables.Visuals.chams_sleeve_col[0], 1.f, ("VisualsLocalColor"), ("sleeve_chams_color_r"));
	SetupValue(Variables.Visuals.chams_sleeve_col[1], 1.f, ("VisualsLocalColor"), ("sleeve_chams_color_g"));
	SetupValue(Variables.Visuals.chams_sleeve_col[2], 1.f, ("VisualsLocalColor"), ("sleeve_chams_color_b"));
	SetupValue(Variables.Visuals.chams_sleeve_col[3], 1.f, ("VisualsLocalColor"), ("sleeve_chams_color_a"));
	//Chams Weapon Color
	SetupValue(Variables.Visuals.weaponchamscol[0], 1.f, ("VisualsLocalColor"), ("weapon_chams_color_r"));
	SetupValue(Variables.Visuals.weaponchamscol[1], 1.f, ("VisualsLocalColor"), ("weapon_chams_color_g"));
	SetupValue(Variables.Visuals.weaponchamscol[2], 1.f, ("VisualsLocalColor"), ("weapon_chams_color_b"));
	SetupValue(Variables.Visuals.weaponchamscol[3], 1.f, ("VisualsLocalColor"), ("weapon_chams_color_a"));
	//Glow Color
	SetupValue(Variables.Visuals.VisualsLocalGlowColor[0], 1.f, ("VisualsLocalColor"), ("local_glow_color_r"));
	SetupValue(Variables.Visuals.VisualsLocalGlowColor[1], 1.f, ("VisualsLocalColor"), ("local_glow_color_g"));
	SetupValue(Variables.Visuals.VisualsLocalGlowColor[2], 1.f, ("VisualsLocalColor"), ("local_glow_color_b"));
	SetupValue(Variables.Visuals.VisualsLocalGlowColor[3], 1.f, ("VisualsLocalColor"), ("local_glow_color_a"));



	///Colors Player Team
	//Chams Color
	SetupValue(Variables.Visuals.Team.team_chams_col[0], 1.f, ("VisualsTeamColor"), ("team_chams_col_r"));
	SetupValue(Variables.Visuals.Team.team_chams_col[1], 1.f, ("VisualsTeamColor"), ("team_chams_col_g"));
	SetupValue(Variables.Visuals.Team.team_chams_col[2], 1.f, ("VisualsTeamColor"), ("team_chams_col_b"));
	SetupValue(Variables.Visuals.Team.team_chams_col[3], 1.f, ("VisualsTeamColor"), ("team_chams_col_a"));
	//Chams Invisible Color
	SetupValue(Variables.Visuals.Team.team_chams_invisible_col[0], 1.f, ("VisualsTeamColor"), ("team_chams_invisible_col_r"));
	SetupValue(Variables.Visuals.Team.team_chams_invisible_col[1], 1.f, ("VisualsTeamColor"), ("team_chams_invisible_col_g"));
	SetupValue(Variables.Visuals.Team.team_chams_invisible_col[2], 1.f, ("VisualsTeamColor"), ("team_chams_invisible_col_b"));
	SetupValue(Variables.Visuals.Team.team_chams_invisible_col[3], 1.f, ("VisualsTeamColor"), ("team_chams_invisible_col_a"));
	//Chams Layer Color
	SetupValue(Variables.Visuals.Team.team_chams_layer_color[0], 1.f, ("VisualsTeamColor"), ("team_chams_layer_color_r"));
	SetupValue(Variables.Visuals.Team.team_chams_layer_color[1], 1.f, ("VisualsTeamColor"), ("team_chams_layer_color_g"));
	SetupValue(Variables.Visuals.Team.team_chams_layer_color[2], 1.f, ("VisualsTeamColor"), ("team_chams_layer_color_b"));
	SetupValue(Variables.Visuals.Team.team_chams_layer_color[3], 1.f, ("VisualsTeamColor"), ("team_chams_layer_color_a"));



	///World
	//Weapon Box Color
	SetupValue(Variables.Visuals.weapon_dropped_box_col[0], 1.f, ("VisualsWorldColor"), ("weapon_dropped_box_col_r"));
	SetupValue(Variables.Visuals.weapon_dropped_box_col[1], 1.f, ("VisualsWorldColor"), ("weapon_dropped_box_col_g"));
	SetupValue(Variables.Visuals.weapon_dropped_box_col[2], 1.f, ("VisualsWorldColor"), ("weapon_dropped_box_col_b"));
	//Weapon Name Color
	SetupValue(Variables.Visuals.weapon_dropped_name_col[0], 1.f, ("VisualsWorldColor"), ("weapon_dropped_name_col_r"));
	SetupValue(Variables.Visuals.weapon_dropped_name_col[1], 1.f, ("VisualsWorldColor"), ("weapon_dropped_name_col_g"));
	SetupValue(Variables.Visuals.weapon_dropped_name_col[2], 1.f, ("VisualsWorldColor"), ("weapon_dropped_name_col_b"));
	//Spreed circle Color
	SetupValue(Variables.Visuals.spreed_circle_col[0], 1.f, ("VisualsWorldColor"), ("spreed_circle_col_r"));
	SetupValue(Variables.Visuals.spreed_circle_col[1], 1.f, ("VisualsWorldColor"), ("spreed_circle_col_g"));
	SetupValue(Variables.Visuals.spreed_circle_col[2], 1.f, ("VisualsWorldColor"), ("spreed_circle_col_b"));
	SetupValue(Variables.Visuals.spreed_circle_col[3], 1.f, ("VisualsWorldColor"), ("spreed_circle_col_a"));



	//Others
	//Marker Color
	SetupValue(Variables.Visuals.markercol[0], 1.f, ("VisualsColor"), ("markercol_r"));
	SetupValue(Variables.Visuals.markercol[1], 1.f, ("VisualsColor"), ("markercol_g"));
	SetupValue(Variables.Visuals.markercol[2], 1.f, ("VisualsColor"), ("markercol_b"));
	//Beam Color
	SetupValue(Variables.Visuals.beamcolor[0], 1.f, ("VisualsColor"), ("beamcolor_r"));
	SetupValue(Variables.Visuals.beamcolor[1], 1.f, ("VisualsColor"), ("beamcolor_g"));
	SetupValue(Variables.Visuals.beamcolor[2], 1.f, ("VisualsColor"), ("beamcolor_b"));
	//Lag Comp Color
	SetupValue(Variables.Visuals.lagcomp_col[0], 1.f, ("VisualsColor"), ("lagcomp_col_r"));
	SetupValue(Variables.Visuals.lagcomp_col[1], 1.f, ("VisualsColor"), ("lagcomp_col_g"));
	SetupValue(Variables.Visuals.lagcomp_col[2], 1.f, ("VisualsColor"), ("lagcomp_col_b"));
	//Show Grenades Color
	SetupValue(Variables.Visuals.draw_nades_col[0], 1.f, ("VisualsColor"), ("draw_nades_col_r"));
	SetupValue(Variables.Visuals.draw_nades_col[1], 1.f, ("VisualsColor"), ("draw_nades_col_g"));
	SetupValue(Variables.Visuals.draw_nades_col[2], 1.f, ("VisualsColor"), ("draw_nades_col_b"));



	//Misc
	SetupValue(Variables.Misc.anti_afk_kick, false, ("Misc"), ("anti_afk_kick"));
	SetupValue(Variables.Misc.bhop, false, ("Misc"), ("bhop"));
	SetupValue(Variables.Misc.autostrafe, false, ("Misc"), ("autostrafe"));
	SetupValue(Variables.Misc.fast_duck, false, ("Misc"), ("fast_duck"));
	SetupValue(Variables.Misc.moon_walk, false, ("Misc"), ("moon_walk"));

	SetupValue(Variables.Misc.reveal_rank, false, ("Misc"), ("reveal_rank"));
	SetupValue(Variables.Misc.reveal_money, false, ("Misc"), ("reveal_money"));
	SetupValue(Variables.Misc.serverhitboxes, false, ("Misc"), ("serverhitboxes"));

	SetupValue(Variables.Misc.eventlogs, false, ("Misc"), ("eventlogs"));



	//Misc
	//Menu Color
	SetupValue(Variables.menucolor[0], 0.06f, ("MenuColor"), ("menucolor_r"));
	SetupValue(Variables.menucolor[1], 0.65f, ("MenuColor"), ("menucolor_g"));
	SetupValue(Variables.menucolor[2], 0.86f, ("MenuColor"), ("menucolor_b"));
	//Event Logs Color
	SetupValue(Variables.Misc.event_logs_col[0], 0.06f, ("MiscColor"), ("event_logs_col_r"));
	SetupValue(Variables.Misc.event_logs_col[1], 0.65f, ("MiscColor"), ("event_logs_col_g"));
	SetupValue(Variables.Misc.event_logs_col[2], 0.86f, ("MiscColor"), ("event_logs_col_b"));
}

void c_config::SetupValue(int& value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue<int>(category, name, &value));
}

void c_config::SetupValue(float& value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue<float>(category, name, &value));
}

void c_config::SetupValue(bool& value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue<bool>(category, name, &value));
}


void c_config::Save()
{
	static char path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
	{
		char szCmd[256];
		sprintf(szCmd, "\\cooperware\\%s.cfg", Variables.Misc.config_name);

		folder = std::string(path) + "\\cooperware\\";
		file = std::string(path) + szCmd;
	}

	CreateDirectoryA(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void c_config::Load()
{
	static char path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
	{
		char szCmd[256];
		sprintf(szCmd, "\\cooperware\\%s.cfg", Variables.Misc.config_name);

		folder = std::string(path) + "\\cooperware\\";
		file = std::string(path) + szCmd;
	}

	CreateDirectoryA(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

c_config* Config = new c_config();