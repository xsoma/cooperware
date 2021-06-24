#include "../valve_sdk/csgostructs.hpp"
#include "Skinchanger.hpp"

#include "../helpers/json.hpp"

#include <fstream>
#include "../menu/menu.hpp"
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
std::unordered_map<char*, char*> killIcons = {};
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
c_base_handle worldmodel_handle;
C_BaseCombatWeapon* worldmodel;
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))

void xdSkinchanger()
{
	auto pLocal = g_LocalPlayer;
	if (!pLocal)
		return;

	auto weapons = pLocal->m_hMyWeapons();
	if (!weapons)
		return;


	for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		C_BaseEntity* pEntity = (C_BaseEntity*)g_EntityList->GetClientEntityFromHandle(weapons[i]);
		if (!pEntity)
			return;
		if (pEntity)
		{
			C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)pEntity;
			if (!pWeapon)
				return;

			if (Variables.Skinchanger.Enabled)
			{
				int Model = Variables.Skinchanger.Knife;
				int weapon = pWeapon->m_iItemDefinitionIndex();


				switch (weapon)
				{
					//Pistols
				case 61: // USP
				{
					int usp = Variables.Skinchanger.customidusp;

					pWeapon->m_nFallbackPaintKit() = usp;


				}
				break;
				case 32: // P2000
				{

					int p2000 = Variables.Skinchanger.customidp2000;

					pWeapon->m_nFallbackPaintKit() = p2000;


				}
				break;
				case 4: // Glock-18
				{
					int glock18 = Variables.Skinchanger.customidglock18;

					pWeapon->m_nFallbackPaintKit() = glock18;


				}
				break;
				case 2: // dual
				{
					int dual = Variables.Skinchanger.customiddual;

					pWeapon->m_nFallbackPaintKit() = dual;


				}
				break;
				case 36: // P250
				{
					int p250 = Variables.Skinchanger.customidp250;

					pWeapon->m_nFallbackPaintKit() = p250;


				}
				break;
				case 63: // CZ-75
				{
					int cz75 = Variables.Skinchanger.customidcz75;

					pWeapon->m_nFallbackPaintKit() = cz75;


				}
				break;
				case 3: // Five-7
				{
					int five7 = Variables.Skinchanger.customidfive7;

					pWeapon->m_nFallbackPaintKit() = five7;

				}
				break;
				case 30: // tec9
				{
					int tec9 = Variables.Skinchanger.customidtec9;

					pWeapon->m_nFallbackPaintKit() = tec9;

				}
				break;
				case 1: // Deagle
				{
					int deagle = Variables.Skinchanger.customiddeagle;

					pWeapon->m_nFallbackPaintKit() = deagle;


				}
				break;
				case 64: // Revolver
				{
					int revolver = Variables.Skinchanger.customidrevolver;

					pWeapon->m_nFallbackPaintKit() = revolver;

				}
				break;
				//heavies
				case 35: // NOVA
				{
					int nova = Variables.Skinchanger.customidnova;

					pWeapon->m_nFallbackPaintKit() = nova;


				}
				break;
				case 25: // XM1014
				{
					int xm1014 = Variables.Skinchanger.customidxm1014;

					pWeapon->m_nFallbackPaintKit() = xm1014;


				}
				break;
				case 27: // MAG-7
				{
					int mag7 = Variables.Skinchanger.customidmag7;

					pWeapon->m_nFallbackPaintKit() = mag7;


				}
				break;
				case 29: // Sawed-Off
				{
					int sawedoff = Variables.Skinchanger.customidsawedoff;

					pWeapon->m_nFallbackPaintKit() = sawedoff;


				}
				break;
				case 28: // Negev
				{
					int negev = Variables.Skinchanger.customidnegev;

					pWeapon->m_nFallbackPaintKit() = negev;


				}
				break;
				case 14: // M249
				{
					int m249 = Variables.Skinchanger.customidm249;

					pWeapon->m_nFallbackPaintKit() = m249;


				}
				break;
				//mp's
				case 34: // MP9
				{

					int mp9 = Variables.Skinchanger.customidmp9;

					pWeapon->m_nFallbackPaintKit() = mp9;


				}
				break;
				case 17: // MAC-10
				{
					int mac10 = Variables.Skinchanger.customidmac10;

					pWeapon->m_nFallbackPaintKit() = mac10;


				}
				break;
				case 33: // MP7
				{
					int mp7 = Variables.Skinchanger.customidmp7;

					pWeapon->m_nFallbackPaintKit() = mp7;


				}
				break;
				case 24: // UMP-45
				{
					int ump45 = Variables.Skinchanger.customidump45;

					pWeapon->m_nFallbackPaintKit() = ump45;

				}
				break;
				case 19: // P90
				{
					int p90 = Variables.Skinchanger.customidp90;

					pWeapon->m_nFallbackPaintKit() = p90;


				}
				break;
				case 26: // PP-Bizon
				{
					int ppbizon = Variables.Skinchanger.customidppbizon;

					pWeapon->m_nFallbackPaintKit() = ppbizon;


				}
				break;
				//Rifles
				case 13: // Galil
				{
					int Galil = Variables.Skinchanger.customidGalil;

					pWeapon->m_nFallbackPaintKit() = Galil;

				}
				break;
				case 10: // Famas
				{
					int famas = Variables.Skinchanger.customidfamas;

					pWeapon->m_nFallbackPaintKit() = famas;

				}
				break;
				case 16: // M4A4
				{
					int m4a4 = Variables.Skinchanger.customidm4a4;

					pWeapon->m_nFallbackPaintKit() = m4a4;

				}
				break;
				case 60: // M4A1-S
				{
					int m4a1 = Variables.Skinchanger.customidm4a1;

					pWeapon->m_nFallbackPaintKit() = m4a1;

				}
				break;
				case 7: // AK47
				{

					int ak47 = Variables.Skinchanger.customidak47;

					pWeapon->m_nFallbackPaintKit() = ak47;


				}
				break;
				case 8: // AUG
				{
					int aug = Variables.Skinchanger.customidaug;

					pWeapon->m_nFallbackPaintKit() = aug;

				}
				break;
				case 39: // SG 553
				{
					int sg553 = Variables.Skinchanger.customidsg553;

					pWeapon->m_nFallbackPaintKit() = sg553;

				}
				break;

				//snipers

				case 40: // SSG 08
				{
					int ssg08 = Variables.Skinchanger.customidssg08;

					pWeapon->m_nFallbackPaintKit() = ssg08;

				}
				break;
				case 9: // AWP
				{
					int awp = Variables.Skinchanger.customidawp;

					pWeapon->m_nFallbackPaintKit() = awp;

				}
				break;
				case 38: // SCAR-20
				{

					int scar20 = Variables.Skinchanger.customidscar20;

					pWeapon->m_nFallbackPaintKit() = scar20;

				}
				break;
				case 11: // G3SG1
				{
					int g3sg1 = Variables.Skinchanger.customidg3sg1;

					pWeapon->m_nFallbackPaintKit() = g3sg1;





				}
				break;
				case 23: // mp5
				{
					int mp5 = Variables.Skinchanger.customidmp5;

					pWeapon->m_nFallbackPaintKit() = mp5;





				}
				break;
				default:
					break;
				}


				if (pEntity->GetClientClass()->m_ClassID == ClassId::ClassId_CKnife)
				{

					worldmodel_handle = pWeapon->m_hWeaponWorldModel();
					if (worldmodel_handle) worldmodel = (C_BaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(worldmodel_handle);
					if (Model == 0) // Bayonet
					{
						int iBayonet = g_MdlInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
						pWeapon->m_nModelIndex() = iBayonet; // m_nModelIndex
						pWeapon->m_iViewModelIndex() = iBayonet;
						if (worldmodel) worldmodel->m_nModelIndex() = iBayonet + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 500;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "bayonet";
						killIcons["knife_t"] = "bayonet";

						int customknifeid = Variables.Skinchanger.customidknife;



						pWeapon->m_nFallbackPaintKit() = customknifeid;

					}
					else if (Model == 9) // Bowie Knife
					{
						int iBowie = g_MdlInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
						pWeapon->m_nModelIndex() = iBowie; // m_nModelIndex
						pWeapon->m_iViewModelIndex() = iBowie;
						if (worldmodel) worldmodel->m_nModelIndex() = iBowie + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 514;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_survival_bowie";
						killIcons["knife_t"] = "knife_survival_bowie";
						int customknifeid = Variables.Skinchanger.customidknife;
						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 6) // Butterfly Knife
					{
						int iButterfly = g_MdlInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
						pWeapon->m_nModelIndex() = iButterfly; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = iButterfly;
						if (worldmodel) worldmodel->m_nModelIndex() = iButterfly + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 515;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_butterfly";
						killIcons["knife_t"] = "knife_butterfly";
						int customknifeid = Variables.Skinchanger.customidknife;


						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 7) // Falchion Knife
					{
						int iFalchion = g_MdlInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
						pWeapon->m_nModelIndex() = iFalchion; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = iFalchion;
						if (worldmodel) worldmodel->m_nModelIndex() = iFalchion + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 512;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_falchion";
						killIcons["knife_t"] = "knife_falchion";
						int customknifeid = Variables.Skinchanger.customidknife;


						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 1) // Flip Knife
					{
						int iFlip = g_MdlInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
						pWeapon->m_nModelIndex() = iFlip; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = iFlip;
						if (worldmodel) worldmodel->m_nModelIndex() = iFlip + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 505;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_flip";
						killIcons["knife_t"] = "knife_flip";
						int customknifeid = Variables.Skinchanger.customidknife;


						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 2) // Gut Knife
					{
						int iGut = g_MdlInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
						pWeapon->m_nModelIndex() = iGut; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = iGut;
						if (worldmodel) worldmodel->m_nModelIndex() = iGut + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 506;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_gut";
						killIcons["knife_t"] = "knife_gut";
						int customknifeid = Variables.Skinchanger.customidknife;


						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 5) // Huntsman Knife
					{
						int iHuntsman = g_MdlInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
						pWeapon->m_nModelIndex() = iHuntsman; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = iHuntsman;
						if (worldmodel) worldmodel->m_nModelIndex() = iHuntsman + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 509;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_tactical";
						killIcons["knife_t"] = "knife_tactical";
						int customknifeid = Variables.Skinchanger.customidknife;


						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 3) // Karambit
					{
						int iKarambit = g_MdlInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
						pWeapon->m_nModelIndex() = iKarambit; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = iKarambit;
						if (worldmodel) worldmodel->m_nModelIndex() = iKarambit + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 507;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_karambit";
						killIcons["knife_t"] = "knife_karambit";
						int customknifeid = Variables.Skinchanger.customidknife;

						pWeapon->m_nFallbackPaintKit() = customknifeid;

					}
					else if (Model == 4) // M9 Bayonet
					{
						int iM9Bayonet = g_MdlInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
						pWeapon->m_nModelIndex() = iM9Bayonet; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = iM9Bayonet;
						if (worldmodel) worldmodel->m_nModelIndex() = iM9Bayonet + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 508;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_m9_bayonet";
						killIcons["knife_t"] = "knife_m9_bayonet";
						int customknifeid = Variables.Skinchanger.customidknife;


						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 8)
					{
						int iDagger = g_MdlInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
						pWeapon->m_nModelIndex() = iDagger; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = iDagger;
						if (worldmodel) worldmodel->m_nModelIndex() = iDagger + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 516;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_push";
						killIcons["knife_t"] = "knife_push";
						int customknifeid = Variables.Skinchanger.customidknife;


						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 10)
					{
						int Navaja = g_MdlInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
						pWeapon->m_nModelIndex() = Navaja; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = Navaja;
						if (worldmodel) worldmodel->m_nModelIndex() = Navaja + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 520;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_push";
						killIcons["knife_t"] = "knife_push";
						int customknifeid = Variables.Skinchanger.customidknife;


						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 11) // Shadow Daggers
					{
						int Stiletto = g_MdlInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
						pWeapon->m_nModelIndex() = Stiletto; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = Stiletto;
						if (worldmodel) worldmodel->m_nModelIndex() = Stiletto + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 522;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_push";
						killIcons["knife_t"] = "knife_push";
						int customknifeid = Variables.Skinchanger.customidknife;


						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 12)
					{
						int Ursus = g_MdlInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
						pWeapon->m_nModelIndex() = Ursus; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = Ursus;
						if (worldmodel) worldmodel->m_nModelIndex() = Ursus + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 519;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_push";
						killIcons["knife_t"] = "knife_push";
						int customknifeid = Variables.Skinchanger.customidknife;


						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 13)
					{
						int Talon = g_MdlInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
						pWeapon->m_nModelIndex() = Talon; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = Talon;
						if (worldmodel) worldmodel->m_nModelIndex() = Talon + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 523;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_push";
						killIcons["knife_t"] = "knife_push";
						int customknifeid = Variables.Skinchanger.customidknife;

						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}
					else if (Model == 14)
					{
						int v_knife_css = g_MdlInfo->GetModelIndex("models/weapons/v_knife_css.mdl");
						pWeapon->m_nModelIndex() = v_knife_css; // m_nModelIndex
							pWeapon->m_iViewModelIndex() = v_knife_css;
						if (worldmodel) worldmodel->m_nModelIndex() = v_knife_css + 1;
						*pWeapon->m_iItemDefinitionIndex2() = 503;
						pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_push";
						killIcons["knife_t"] = "knife_push";
						int customknifeid = Variables.Skinchanger.customidknife;

						pWeapon->m_nFallbackPaintKit() = customknifeid;
					}

				}
				pWeapon->m_OriginalOwnerXuid() = 0;
				pWeapon->m_OriginalOwnerXuidHigh() = 0;
				pWeapon->m_flFallbackWear() = 0.001f;
				*pWeapon->fixItemIDHigh() = 1;
			}
		}
	}
}








#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1

#define SEQUENCE_CSS1 13
#define SEQUENCE_CSS2 14


using namespace std;
// Helper function to generate a random sequence number.
inline int RandomSequence(int low, int high) {
	return (rand() % (high - low + 1) + low);
}

#define	LIFE_ALIVE 0

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);



RecvVarProxyFn fnSequenceProxyFn = nullptr;

RecvVarProxyFn oRecvnModelIndex;
void Hooked_RecvProxy_Viewmodel(CRecvProxyData* pData, void* pStruct, void* pOut)
{
	// Get the knife view model id's
	int default_t = g_MdlInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = g_MdlInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int iBayonet = g_MdlInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = g_MdlInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = g_MdlInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGut = g_MdlInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = g_MdlInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = g_MdlInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = g_MdlInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = g_MdlInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = g_MdlInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iBowie = g_MdlInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	int iGunGame = g_MdlInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	int Navaja = g_MdlInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
	int Stiletto = g_MdlInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
	int Ursus = g_MdlInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
	int Talon = g_MdlInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
	int css = g_MdlInfo->GetModelIndex("models/weapons/v_knife_css.mdl");

	// Get local player (just to stop replacing spectators knifes)
	auto pLocal = g_LocalPlayer;

	if (Variables.Skinchanger.Enabled && pLocal && pLocal->IsAlive())
	{
		// If we are alive and holding a default knife(if we already have a knife don't worry about changing)
		if (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iBowie ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == Navaja ||
			pData->m_Value.m_Int == Stiletto ||
			pData->m_Value.m_Int == Talon ||
			pData->m_Value.m_Int == css)
		{
			// Set whatever knife we want
			if (Variables.Skinchanger.Knife == 0)
				pData->m_Value.m_Int = iBayonet;
			else if (Variables.Skinchanger.Knife == 9)
				pData->m_Value.m_Int = iBowie;
			else if (Variables.Skinchanger.Knife == 6)
				pData->m_Value.m_Int = iButterfly;
			else if (Variables.Skinchanger.Knife == 7)
				pData->m_Value.m_Int = iFalchion;
			else if (Variables.Skinchanger.Knife == 1)
				pData->m_Value.m_Int = iFlip;
			else if (Variables.Skinchanger.Knife == 2)
				pData->m_Value.m_Int = iGut;
			else if (Variables.Skinchanger.Knife == 5)
				pData->m_Value.m_Int = iHuntsman;
			else if (Variables.Skinchanger.Knife == 3)
				pData->m_Value.m_Int = iKarambit;
			else if (Variables.Skinchanger.Knife == 4)
				pData->m_Value.m_Int = iM9Bayonet;
			else if (Variables.Skinchanger.Knife == 8)
				pData->m_Value.m_Int = iDagger;
			else if (Variables.Skinchanger.Knife == 10)
				pData->m_Value.m_Int = Navaja;
			else if (Variables.Skinchanger.Knife == 11)
				pData->m_Value.m_Int = Stiletto;
			else if (Variables.Skinchanger.Knife == 12)
				pData->m_Value.m_Int = Ursus;
			else if (Variables.Skinchanger.Knife == 14)
				pData->m_Value.m_Int = css;
		}
	}
	// Carry on the to original proxy
	oRecvnModelIndex(pData, pStruct, pOut);
}





void SetViewModelSequence2(const CRecvProxyData* pDataConst, void* pStruct, void* pOut)
{

	// Make the incoming data editable.
	CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);
	if (!pData)
		return;
	// Confirm that we are replacing our view model and not someone elses.
	C_BaseViewModel* pViewModel = static_cast<C_BaseViewModel*>(pStruct);
	if (!pViewModel)
		return;
	if (pViewModel)
	{
		C_BasePlayer* pOwner = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(pViewModel->GetOwner() & 0xFFF));
		if (!pOwner)
			return;
		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->EntIndex() == g_EngineClient->GetLocalPlayer())
		{
			// Get the filename of the current view model.
			std::string szModel = g_MdlInfo->GetModelName(g_MdlInfo->GetModel(pViewModel->m_nModelIndex()));

			// Store the current sequence.
			int m_nSequence = pData->m_Value.m_Int;
			if (szModel == "models/weapons/v_knife_butterfly.mdl")
			{
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
				}
			}
			else if (szModel == "models/weapons/v_knife_falchion_advanced.mdl")
			{
				// Fix animations for the Falchion Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (szModel == "models/weapons/v_knife_push.mdl")
			{
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (szModel == "models/weapons/v_knife_survival_bowie.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1;
					break;
				default:
					m_nSequence--;
				}
			}
			else if (szModel == "models/weapons/v_knife_ursus.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (szModel == "models/weapons/v_knife_stiletto.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (szModel == "models/weapons/v_knife_widowmaker.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			pData->m_Value.m_Int = m_nSequence;
		}
	}
	fnSequenceProxyFn(pData, pStruct, pOut);
}

void AnimationFixHook()
{
	for (ClientClass* pClass = g_CHLClient->GetAllClasses(); pClass; pClass = pClass->m_pNext)
	{
		if (!pClass)
			return;
		if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel")) {
			// Search for the 'm_nModelIndex' property.
			RecvTable* pClassTable = pClass->m_pRecvTable;

			for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
				RecvProp* pProp = &pClassTable->m_pProps[nIndex];

				if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
					continue;

				// Store the original proxy function.
				fnSequenceProxyFn = static_cast<RecvVarProxyFn>(pProp->m_ProxyFn);

				// Replace the proxy function with our sequence changer.
				pProp->m_ProxyFn = static_cast<RecvVarProxyFn>(SetViewModelSequence2);

				break;
			}

			break;
		}
	}
}


void NetvarHook()
{


	AnimationFixHook();
	ClientClass* pClass = g_CHLClient->GetAllClasses();
	if (!pClass)
		return;
	while (pClass)
	{
		const char* pszName = pClass->m_pRecvTable->m_pNetTableName;

		for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
		{
			RecvProp* pProp = &(pClass->m_pRecvTable->m_pProps[i]);
			const char* name = pProp->m_pVarName;

			if (!strcmp(pszName, "DT_BaseViewModel"))
			{
				// Knives
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = (RecvVarProxyFn)Hooked_RecvProxy_Viewmodel;
				}
			}

		}

		pClass = pClass->m_pNext;
	}
}




























































