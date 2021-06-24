#include <Windows.h>
#include <chrono>
#include <d3dx9.h>
#include <intrin.h>
#include <ShlObj_core.h>
#include "menu.hpp"
#include "../config.hpp"
#include "../config.hpp"
#include "../imgui/imgui.h"
#include "../menuarrays.hpp"
#include "../hooks/hooks.hpp"
#include "../features/misc.hpp"

#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS

ImFont* bigmenu_font;
ImFont* menu_font;
ImFont* smallmenu_font;

static int selectedcfg = 0;
static std::string cfgname = " default";

std::vector<std::string> configs;

void getconfig( )
{
	//get all files on folder

	configs.clear( );

	static char path [ MAX_PATH ];
	std::string szPath1;

	if ( !SUCCEEDED( SHGetFolderPathA( NULL, CSIDL_MYDOCUMENTS, NULL, 0, path ) ) )
		return;

	szPath1 = std::string( path ) + "\\cooperware\\*";


	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	configs.push_back( " choose config" );

	hf = FindFirstFile( szPath1.c_str( ), &FindFileData );
	if ( hf != INVALID_HANDLE_VALUE ) {
		do {
			std::string filename = FindFileData.cFileName;

			if ( filename == "." )
				continue;

			if ( filename == ".." )
				continue;

			if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				if ( filename.find( ".cfg" ) != std::string::npos )
				{
					configs.push_back( std::string( filename ) );
				}
			}
		} while ( FindNextFile( hf, &FindFileData ) != 0 );
		FindClose( hf );
	}
}

void Menu::Initialize( )
{
	Visible = false;

	ImGui::CreateContext( );

	ImGui_ImplDX9_Init( InputSys::Get( ).GetMainWindow( ), g_D3DDevice9 );

	CreateStyle( );
}

void Menu::Shutdown( )
{
	ImGui_ImplDX9_Shutdown( );
}

void Menu::OnDeviceLost( )
{
	ImGui_ImplDX9_InvalidateDeviceObjects( );
}

void Menu::OnDeviceReset( )
{
	ImGui_ImplDX9_CreateDeviceObjects( );
}

IDirect3DTexture9* menuLogo = nullptr;
IDirect3DTexture9* menuLogo2 = nullptr;
IDirect3DTexture9* menuLogo3 = nullptr;
namespace ImGui
{

	static auto vector_getterxd = [ ] ( void* vec, int idx, const char** out_text )
	{
		auto& vector = *static_cast< std::vector<std::string>* >(vec);
		if ( idx < 0 || idx >= static_cast< int >(vector.size( )) ) { return false; }
		*out_text = vector.at( idx ).c_str( );
		return true;
	};

	IMGUI_API  bool TabLabels( int numTabs, const char** tabLabels, int& selectedIndex, const char** tabLabelTooltips, bool autoLayout, int* pOptionalHoveredIndex ) {
		ImGuiStyle& style = ImGui::GetStyle( );

		const ImVec4 color = style.Colors [ ImGuiCol_Button ];
		const ImVec4 Text = style.Colors [ ImGuiCol_Text ];
		const ImVec4 colorActive = style.Colors [ ImGuiCol_ButtonActive ];
		const ImVec4 colorHover = style.Colors [ ImGuiCol_ButtonHovered ];
		const ImVec2 itemSpacing = style.ItemSpacing;
		style.ItemSpacing.x = 2.5;
		style.ItemSpacing.y = 1;
		if ( numTabs > 0 && (selectedIndex < 0 || selectedIndex >= numTabs) ) selectedIndex = 0;
		if ( pOptionalHoveredIndex ) *pOptionalHoveredIndex = -1;

		const float btnOffset = 2.f * style.FramePadding.x;
		const float sameLineOffset = 2.f * style.ItemSpacing.x;
		const float uniqueLineOffset = 2.f * style.WindowPadding.x;

		float windowWidth = 0.f, sumX = 0.f;
		if ( autoLayout ) windowWidth = ImGui::GetWindowWidth( ) - uniqueLineOffset;

		bool selection_changed = false;
		for ( int i = 0; i < numTabs; i++ )
		{
			if ( i == selectedIndex )
			{
				style.Colors [ ImGuiCol_Text ] = ImVec4( 1.f, 1.f, 1.f, 1.f );
				style.Colors [ ImGuiCol_Button ] = colorActive;
				style.Colors [ ImGuiCol_ButtonActive ] = colorActive;
				style.Colors [ ImGuiCol_ButtonHovered ] = colorActive;
			}
			else
			{
				style.Colors [ ImGuiCol_Text ] = Text;
				style.Colors [ ImGuiCol_Button ] = color;
				style.Colors [ ImGuiCol_ButtonActive ] = colorActive;
				style.Colors [ ImGuiCol_ButtonHovered ] = colorHover;
			}

			ImGui::PushID( i );

			if ( !autoLayout ) { if ( i > 0 ) ImGui::SameLine( ); }
			else if ( sumX > 0.f ) {
				sumX += sameLineOffset;
				sumX += ImGui::CalcTextSize( tabLabels [ i ] ).x + btnOffset;
				if ( sumX > windowWidth ) sumX = 0.f;
				else ImGui::SameLine( );
			}
			if ( ImGui::Button( tabLabels [ i ], ImVec2( (windowWidth / numTabs) - 2, 20 ) ) ) { selection_changed = (selectedIndex != i); selectedIndex = i; }
			if ( autoLayout && sumX == 0.f ) {
				sumX = ImGui::GetItemRectSize( ).x;
			}
			if ( pOptionalHoveredIndex ) {
				if ( ImGui::IsItemHovered( ) ) {
					*pOptionalHoveredIndex = i;
					if ( tabLabelTooltips && tabLabelTooltips [ i ] && strlen( tabLabelTooltips [ i ] ) > 0 )  ImGui::SetTooltip( "%s", tabLabelTooltips [ i ] );
				}
			}
			else if ( tabLabelTooltips && tabLabelTooltips [ i ] && ImGui::IsItemHovered( ) && strlen( tabLabelTooltips [ i ] ) > 0 ) ImGui::SetTooltip( "%s", tabLabelTooltips [ i ] );
			ImGui::PopID( );
		}

		style.Colors [ ImGuiCol_Text ] = Text;
		style.Colors [ ImGuiCol_Button ] = color;
		style.Colors [ ImGuiCol_ButtonActive ] = colorActive;
		style.Colors [ ImGuiCol_ButtonHovered ] = colorHover;
		style.ItemSpacing = itemSpacing;
		return selection_changed;
	}


	IMGUI_API bool ComboBoxArrayxd( const char* label, int* currIndex, std::vector<std::string>& values )
	{
		if ( values.empty( ) ) { return false; }
		return Combo( label, currIndex, vector_getterxd,
			static_cast< void* >(&values), values.size( ) );
	}


}

void Menu::RenderMenu()
{
	if (!Visible)
		return;

	ImGui_ImplDX9_NewFrame();

	ImGui::GetIO().MouseDrawCursor = Visible;

	auto& style = ImGui::GetStyle();

	ImGui::Begin("CooperWare cheat free", &Visible, ImVec2(697, 475), 0.95f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	{
		static int renderPageMenu = 0;

		if (ImGui::Button("Legit", ImVec2(130, 22)))
			renderPageMenu = 0;

		ImGui::SameLine();

		if (ImGui::Button("Rage", ImVec2(130, 22)))
			renderPageMenu = 1;

		ImGui::SameLine();

		if (ImGui::Button("Antiaim", ImVec2(129, 22)))
			renderPageMenu = 2;

		ImGui::SameLine();

		if (ImGui::Button("Visuals", ImVec2(130, 22)))
			renderPageMenu = 3;

		ImGui::SameLine();

		if (ImGui::Button("Misc", ImVec2(130, 22)))
			renderPageMenu = 4;

		if (renderPageMenu == 0) {
			ImGui::Columns(2, NULL, false);

			ImGui::BeginChild("##legitMain", ImVec2(334, 401), false);
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
				ImGui::Spacing(); ImGui::Text("Aimbot"); ImGui::Separator();
				ImGui::PopStyleColor();

				ImGui::Checkbox("enable", &Variables.enablelgit);
				ImGui::Checkbox("backtrack", &Variables.aimbot.Backtrack);
				if (Variables.aimbot.Backtrack)
				{
					ImGui::SliderInt("ms##backtrack_time", &Variables.aimbot.BacktrackTime, 0, 200);
				}

				static int legitTabWeapon = 0;

				ImGui::Spacing();

				if (ImGui::Button("pistols", ImVec2(100, 21)))
					legitTabWeapon = 0;

				ImGui::SameLine();

				if (ImGui::Button("rifles", ImVec2(100, 21)))
					legitTabWeapon = 1;

				ImGui::SameLine();

				if (ImGui::Button("snipers", ImVec2(100, 21)))
					legitTabWeapon = 2;

				ImGui::Spacing();

				if (legitTabWeapon == 0) {
					ImGui::Checkbox("enable pistols##pistol_config_enable", &Variables.aimbot.Pistols.Enabled);
					ImGui::Combo("type##pistol_config_aim_type", &Variables.aimbot.Pistols.AimType, aimlegitrtype, ARRAYSIZE(aimlegitrtype));
					ImGui::SliderFloat("fov##pistol_config_fov", &Variables.aimbot.Pistols.Fov, 0, 10);
					ImGui::Checkbox("auto rcs##pistol_config_rcs", &Variables.aimbot.Pistols.Recoil);
					ImGui::Combo("hitbox##pistol_config_hitbox", &Variables.aimbot.Pistols.Hitbox, hitbxlegit, ARRAYSIZE(hitbxlegit));
				}
				else if (legitTabWeapon == 1) {
					ImGui::Checkbox("enable rifles##rifles_config_enable", &Variables.aimbot.Rifles.Enabled);
					ImGui::Combo("type##rifles_config_aim_type", &Variables.aimbot.Rifles.AimType, aimlegitrtype, ARRAYSIZE(aimlegitrtype));
					ImGui::SliderFloat("fov##rifles_config_fov", &Variables.aimbot.Rifles.Fov, 0, 10);
					ImGui::Checkbox("auto rcs##rifles_config_rcs", &Variables.aimbot.Rifles.Recoil);
					ImGui::Combo("hitbox##rifles_config_hitbox", &Variables.aimbot.Rifles.Hitbox, hitbxlegit, ARRAYSIZE(hitbxlegit));
				}
				else if (legitTabWeapon == 2) {
					ImGui::Checkbox("enable snipers##sniper_config_enable", &Variables.aimbot.Snipers.Enabled);
					ImGui::Combo("type##sniper_config_aim_type", &Variables.aimbot.Snipers.AimType, aimlegitrtype, ARRAYSIZE(aimlegitrtype));
					ImGui::SliderFloat("fov##sniper_config_fov", &Variables.aimbot.Snipers.Fov, 0, 10);
					ImGui::Checkbox("auto rcs##sniper_config_rcs", &Variables.aimbot.Snipers.Recoil);
					ImGui::Combo("hitbox##sniper_config_hitbox", &Variables.aimbot.Snipers.Hitbox, hitbxlegit, ARRAYSIZE(hitbxlegit));
				}

			}ImGui::EndChild();
		}
		else if (renderPageMenu == 1) {
			ImGui::Columns(2, NULL, false);

			ImGui::BeginChild("ragebotAimbot", ImVec2(334, 302), false);
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
				ImGui::Spacing(); ImGui::Text("Aimbot"); ImGui::Separator();
				ImGui::PopStyleColor();

				ImGui::Checkbox("enable", &Variables.rageaimbot.rageenable);

				ImGui::Checkbox("auto fire", &Variables.rageaimbot.autofire);
				ImGui::Checkbox("resolver", &Variables.rageaimbot.resolverenable);
				ImGui::Checkbox("pitch resolver", &Variables.rageaimbot.pitchresolver);
				ImGui::SliderFloat("body_max_rotation", &Variables.rageaimbot.body_max_rotation, 0, 360);

				ImGui::Combo("double tap", &Variables.rageaimbot.doubletab, dttt, ARRAYSIZE(dttt));
				if (Variables.rageaimbot.doubletab == 1)
				{
					ImGui::Hotkey("##doubletapkey", &Variables.rageaimbot.doubletap_key, ImVec2(40, 20));
				}

				ImGui::Checkbox("safe point", &Variables.rageaimbot.safepoint);

				ImGui::Checkbox("auto stop", &Variables.rageaimbot.rageautostop);
				ImGui::Checkbox("auto scope", &Variables.rageaimbot.rageautoscope);
				ImGui::Checkbox("auto zeus", &Variables.rageaimbot.autozeus);
				ImGui::Checkbox("auto knife", &Variables.rageaimbot.autoknife);
				ImGui::Checkbox("auto revolver", &Variables.rageaimbot.autorevolvo);
			} ImGui::EndChild();

			ImGui::BeginChild("ragebotBodyOptions", ImVec2(334, 95), false);
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
				ImGui::Spacing(); ImGui::Text("Body options"); ImGui::Separator();
				ImGui::PopStyleColor();

				ImGui::Checkbox("baim if lethal", &Variables.rageaimbot.baimiflethal);
				ImGui::Checkbox("baim after 3 misses", &Variables.rageaimbot.baimafter3misses);
				ImGui::Hotkey("baim key", &Variables.rageaimbot.baimkey);
			} ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild("ragebotWeaponConfig", ImVec2(334, 350), false);
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
				ImGui::Spacing(); ImGui::Text("Weapon config"); ImGui::Separator();
				ImGui::PopStyleColor();

				static int weapons = 0;

				ImGui::Combo("weapons", &weapons, wps, ARRAYSIZE(wps));
				if (weapons == 0)
				{
					ImGui::SliderInt("hitchance", &Variables.rageaimbot.auto_hitchance, 0, 100);
					ImGui::SliderInt("minimum damage", &Variables.rageaimbot.auto_mindamage, 0, 100);
					ImGui::SliderInt("visible damage", &Variables.rageaimbot.auto_mindamagevisible, 0, 100);
					ImGui::Checkbox("multipoint##multipoint", &Variables.rageaimbot.auto_multipoint);
					if (Variables.rageaimbot.auto_multipoint)
					{
						ImGui::SliderInt("head scale", &Variables.rageaimbot.auto_headscale, 0.0, 1.0);
						ImGui::SliderInt("body scale", &Variables.rageaimbot.auto_bodyscale, 0.0, 1.0);
					}
					ImGui::ListBoxHeader(""); {
						ImGui::Selectable("head", &Variables.rageaimbot.auto_head);
						ImGui::Selectable("neck", &Variables.rageaimbot.auto_neck);
						ImGui::Selectable("chest", &Variables.rageaimbot.auto_chest);
						ImGui::Selectable("body", &Variables.rageaimbot.auto_body);
						ImGui::Selectable("stomach", &Variables.rageaimbot.auto_stomach);
						ImGui::Selectable("pelvis", &Variables.rageaimbot.auto_pelvis);
						ImGui::Selectable("arms", &Variables.rageaimbot.auto_arms);
						ImGui::Selectable("legs", &Variables.rageaimbot.auto_legs);
						ImGui::Selectable("feet", &Variables.rageaimbot.auto_toes);
					}
					ImGui::ListBoxFooter();
				}
				if (weapons == 1)
				{
					ImGui::SliderInt("hitchance", &Variables.rageaimbot.scout_hitchance, 0, 100);
					ImGui::SliderInt("minimum damage", &Variables.rageaimbot.scout_mindamage, 0, 100);
					ImGui::SliderInt("visible damage", &Variables.rageaimbot.scout_mindamagevisible, 0, 100);
					ImGui::Checkbox("multipoint##multipoint", &Variables.rageaimbot.scout_multipoint);
					if (Variables.rageaimbot.scout_multipoint)
					{
						ImGui::SliderInt("head scale", &Variables.rageaimbot.scout_headscale, 0.0, 1.0);
						ImGui::SliderInt("body scale", &Variables.rageaimbot.scout_bodyscale, 0.0, 1.0);
					}
					ImGui::ListBoxHeader(""); {
						ImGui::Selectable("head", &Variables.rageaimbot.scout_head);
						ImGui::Selectable("neck", &Variables.rageaimbot.scout_neck);
						ImGui::Selectable("chest", &Variables.rageaimbot.scout_chest);
						ImGui::Selectable("body", &Variables.rageaimbot.scout_body);
						ImGui::Selectable("stomach", &Variables.rageaimbot.scout_stomach);
						ImGui::Selectable("pelvis", &Variables.rageaimbot.scout_pelvis);
						ImGui::Selectable("arms", &Variables.rageaimbot.scout_arms);
						ImGui::Selectable("legs", &Variables.rageaimbot.scout_legs);
						ImGui::Selectable("feet", &Variables.rageaimbot.scout_toes);
					}
					ImGui::ListBoxFooter();
				}
				if (weapons == 2)
				{
					ImGui::SliderInt("hitchance", &Variables.rageaimbot.awp_hitchance, 0, 100);
					ImGui::SliderInt("minimum damage", &Variables.rageaimbot.awp_mindamage, 0, 100);
					ImGui::SliderInt("visible damage", &Variables.rageaimbot.awp_mindamagevisible, 0, 100);
					ImGui::Checkbox("multipoint##multipoint", &Variables.rageaimbot.awp_multipoint);
					if (Variables.rageaimbot.awp_multipoint)
					{
						ImGui::SliderInt("head scale", &Variables.rageaimbot.awp_headscale, 0.0, 1.0);
						ImGui::SliderInt("body scale", &Variables.rageaimbot.awp_bodyscale, 0.0, 1.0);
					}
					ImGui::ListBoxHeader(""); {
						ImGui::Selectable("head", &Variables.rageaimbot.awp_head);
						ImGui::Selectable("neck", &Variables.rageaimbot.awp_neck);
						ImGui::Selectable("chest", &Variables.rageaimbot.awp_chest);
						ImGui::Selectable("body", &Variables.rageaimbot.awp_body);
						ImGui::Selectable("stomach", &Variables.rageaimbot.awp_stomach);
						ImGui::Selectable("pelvis", &Variables.rageaimbot.awp_pelvis);
						ImGui::Selectable("arms", &Variables.rageaimbot.awp_arms);
						ImGui::Selectable("legs", &Variables.rageaimbot.awp_legs);
						ImGui::Selectable("feet", &Variables.rageaimbot.awp_toes);
					}
					ImGui::ListBoxFooter();
				}
				if (weapons == 3)
				{
					ImGui::SliderInt("hitchance", &Variables.rageaimbot.heavy_pistol_hitchance, 0, 100);
					ImGui::SliderInt("minimum damage", &Variables.rageaimbot.heavy_pistol_mindamage, 0, 100);
					ImGui::SliderInt("visible damage", &Variables.rageaimbot.heavy_pistol_mindamagevisible, 0, 100);
					ImGui::Checkbox("multipoint##multipoint", &Variables.rageaimbot.heavy_pistol_multipoint);
					if (Variables.rageaimbot.heavy_pistol_multipoint)
					{
						ImGui::SliderInt("head scale", &Variables.rageaimbot.heavy_pistol_headscale, 0.0, 1.0);
						ImGui::SliderInt("body scale", &Variables.rageaimbot.heavy_pistol_bodyscale, 0.0, 1.0);
					}
					ImGui::ListBoxHeader(""); {
						ImGui::Selectable("head", &Variables.rageaimbot.heavy_pistol_head);
						ImGui::Selectable("neck", &Variables.rageaimbot.heavy_pistol_neck);
						ImGui::Selectable("chest", &Variables.rageaimbot.heavy_pistol_chest);
						ImGui::Selectable("body", &Variables.rageaimbot.heavy_pistol_body);
						ImGui::Selectable("stomach", &Variables.rageaimbot.heavy_pistol_stomach);
						ImGui::Selectable("pelvis", &Variables.rageaimbot.heavy_pistol_pelvis);
						ImGui::Selectable("arms", &Variables.rageaimbot.heavy_pistol_arms);
						ImGui::Selectable("legs", &Variables.rageaimbot.heavy_pistol_legs);
						ImGui::Selectable("feet", &Variables.rageaimbot.heavy_pistol_toes);
					}
					ImGui::ListBoxFooter();
				}
				if (weapons == 4)
				{
					ImGui::SliderInt("hitchance", &Variables.rageaimbot.hitchance, 0, 100);
					ImGui::SliderInt("minimum damage", &Variables.rageaimbot.minimumdmage, 0, 100);
					ImGui::SliderInt("visible damage", &Variables.rageaimbot.minimumdmagevisible, 0, 100);
					ImGui::Checkbox("multipoint##multipoint", &Variables.rageaimbot.multipoint);
					if (Variables.rageaimbot.multipoint)
					{
						ImGui::SliderInt("head scale", &Variables.rageaimbot.headscale, 0.0, 1.0);
						ImGui::SliderInt("body scale", &Variables.rageaimbot.bodyscale, 0.0, 1.0);
					}
					ImGui::ListBoxHeader(""); {
						ImGui::Selectable("head", &Variables.rageaimbot.head);
						ImGui::Selectable("neck", &Variables.rageaimbot.neck);
						ImGui::Selectable("chest", &Variables.rageaimbot.chest);
						ImGui::Selectable("body", &Variables.rageaimbot.body);
						ImGui::Selectable("stomach", &Variables.rageaimbot.stomach);
						ImGui::Selectable("pelvis", &Variables.rageaimbot.pelvis);
						ImGui::Selectable("arms", &Variables.rageaimbot.arms);
						ImGui::Selectable("legs", &Variables.rageaimbot.legs);
						ImGui::Selectable("feet", &Variables.rageaimbot.toes);
					}
					ImGui::ListBoxFooter();
				}
			
			} ImGui::EndChild();
		}
		else if (renderPageMenu == 2) {
			ImGui::BeginChild("antiaimTab", ImVec2(334, 267), false);
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
				ImGui::Spacing(); ImGui::Text("Antiaim"); ImGui::Separator();
				ImGui::PopStyleColor();
			
				ImGui::Checkbox("enable##aaenable", &Variables.rageaimbot.antiaimenable);
				ImGui::Combo("pitch##pitchu", &Variables.rageaimbot.pitch, pitchzzz, ARRAYSIZE(pitchzzz));
				ImGui::Checkbox("auto direction", &Variables.rageaimbot.autodirection);

				ImGui::Combo("desync type", &Variables.rageaimbot.desynctype, desynbc, ARRAYSIZE(desynbc));
				if (Variables.rageaimbot.desynctype == 1)
				{
					ImGui::Hotkey("desync inverter", &Variables.rageaimbot.staticinverter, ImVec2(40, 20));
					ImGui::SliderInt("bodylean", &Variables.rageaimbot.staticrightsidevalue, 0, 100);
					ImGui::SliderInt("inverted bodylean", &Variables.rageaimbot.staticleftsidevalue, 0, 100);
				}

				ImGui::Checkbox("slow walk", &Variables.Misc.slowwalk);
				if (Variables.Misc.slowwalk)
				{
					ImGui::SliderInt("speed", &Variables.Misc.slowwalkspeed, 0, 100);
					ImGui::Hotkey("##tastafgm1", &Variables.rageaimbot.slowwalkkey, ImVec2(40, 20));
				}

				ImGui::Text("fake lag");
				ImGui::SliderInt("amount", &Variables.Misc.MiscFakelagChoke, 0, 14);
				ImGui::Spacing();

				ImGui::Checkbox("fake duck##1", &Variables.rageaimbot.fakeduckenable);
				if (Variables.Misc.slowwalk)
					ImGui::Hotkey("##tastafgm", &Variables.rageaimbot.fakeduckkey, ImVec2(40, 20));
				ImGui::Checkbox("view fakelag", &Variables.Misc.serverhitboxes);
			} ImGui::EndChild();

			ImGui::BeginChild("antiaimStates", ImVec2(334, 130), false);
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
				ImGui::Spacing(); ImGui::Text("State"); ImGui::Separator();
				ImGui::PopStyleColor();

				static int aaState = 0;

				if (ImGui::Button("stand", ImVec2(89, 16)))
					aaState = 0;

				ImGui::SameLine();

				if (ImGui::Button("move", ImVec2(89, 16)))
					aaState = 1;

				ImGui::SameLine();

				if (ImGui::Button("air", ImVec2(89, 16)))
					aaState = 2;

				if (aaState == 0)
				{
					ImGui::Combo("stand yaw##standaa", &Variables.rageaimbot.standingaa, pitchzzz2, ARRAYSIZE(pitchzzz));
					ImGui::Checkbox("stand jitter##standhitterenable", &Variables.rageaimbot.jitterenablestanding);
					if (Variables.rageaimbot.standingaa == 2)
					{
						ImGui::Hotkey("left", &Variables.rageaimbot.left);
						ImGui::Hotkey("right", &Variables.rageaimbot.right);
						ImGui::Hotkey("back", &Variables.rageaimbot.back);
						ImGui::Hotkey("front", &Variables.rageaimbot.front);
					}
					if (Variables.rageaimbot.jitterenablestanding)
						ImGui::SliderInt("value##standjittervaluezz", &Variables.rageaimbot.jittervaluestanding, -100, 100);
				}
				else if (aaState == 1)
				{
					ImGui::Combo("move yaw##niveaa", &Variables.rageaimbot.movingaa, pitchzzz2, ARRAYSIZE(pitchzzz));
					ImGui::Checkbox("move jitter##jittermovingenable", &Variables.rageaimbot.jitterenablemoving);
					if (Variables.rageaimbot.movingaa == 2)
					{
						ImGui::Hotkey("left", &Variables.rageaimbot.left);
						ImGui::Hotkey("right", &Variables.rageaimbot.right);
						ImGui::Hotkey("back", &Variables.rageaimbot.back);
						ImGui::Hotkey("front", &Variables.rageaimbot.front);
					}
					if (Variables.rageaimbot.jitterenablemoving)
						ImGui::SliderInt("value##jittervaluemoving", &Variables.rageaimbot.jittervaluemoving, -100, 100);
				}
				else if (aaState == 2)
				{
					ImGui::Combo("air yaw##airaaa", &Variables.rageaimbot.airaa, pitchzzz2, ARRAYSIZE(pitchzzz));
					ImGui::Checkbox("air jitter##airjhitterenable", &Variables.rageaimbot.jitterenableair);
					if (Variables.rageaimbot.airaa == 2)
					{
						ImGui::Hotkey("left", &Variables.rageaimbot.left);
						ImGui::Hotkey("right", &Variables.rageaimbot.right);
						ImGui::Hotkey("back", &Variables.rageaimbot.back);
						ImGui::Hotkey("front", &Variables.rageaimbot.front);
					}
					if (Variables.rageaimbot.jitterenableair)
						ImGui::SliderInt("value##jittervalueeaiurrre", &Variables.rageaimbot.jittervalueair, -100, 100);

				}
			} ImGui::EndChild();
		}
		else if (renderPageMenu == 3) {
			//Do not exceed ImVec2(680, 400)

			static int visualsSubTab = 0;

			if (ImGui::Button("local", ImVec2(130, 22)))
				visualsSubTab = 0;

			ImGui::SameLine();

			if (ImGui::Button("enemy", ImVec2(130, 22)))
				visualsSubTab = 1;

			ImGui::SameLine();

			if (ImGui::Button("team", ImVec2(129, 22)))
				visualsSubTab = 2;

			ImGui::SameLine();

			if (ImGui::Button("world", ImVec2(130, 22)))
				visualsSubTab = 3;

			ImGui::SameLine();

			if (ImGui::Button("others", ImVec2(130, 22)))
				visualsSubTab = 4;

			if (visualsSubTab == 0) // Local tab
			{
				ImGui::Columns(2, NULL, false);

				ImGui::BeginChild("##localVisuals", ImVec2(334, 375), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Visuals"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Text("In constrution");

				} ImGui::EndChild();

				ImGui::NextColumn();

				ImGui::BeginChild("##localChams", ImVec2(334, 292), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Chams"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Checkbox("enable chams##local_chams", &Variables.Visuals.enablelocalchams); ImGui::SameLine();
					ImGui::ColorEdit4("##local_chams_color", Variables.Visuals.localchamscol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material##local_material_chams", &Variables.Visuals.VisualsLocalChamsMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));

					ImGui::Checkbox("chams layer##local_chams_layer", &Variables.Visuals.local_layer); ImGui::SameLine();
					ImGui::ColorEdit4("##local_chams_layer_color", Variables.Visuals.local_layer_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material layer##local_material_chams_layer", &Variables.Visuals.local_layer_material, ChamsMaterials, ARRAYSIZE(ChamsMaterials));


					ImGui::Checkbox("desync chams##local_chams_desync", &Variables.Visuals.desyncchamsenable); ImGui::SameLine();
					ImGui::ColorEdit4("##local_chams_desync_color", Variables.Visuals.localdesynccol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material##local_material_chams_desync", &Variables.Visuals.VisualsDesyncChamsMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));

					ImGui::Checkbox("desync chams layer##local_chams_desync_layer", &Variables.Visuals.desync_layer); ImGui::SameLine();
					ImGui::ColorEdit4("##local_desync_chams_layer_color", Variables.Visuals.desync_layer_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material layer##local_material_desync_chams_layer", &Variables.Visuals.desync_layer_material, ChamsMaterials, ARRAYSIZE(ChamsMaterials));

					ImGui::Checkbox("hand chams", &Variables.Visuals.enablehandchams); ImGui::SameLine();
					ImGui::ColorEdit4("##hand_chams_color", Variables.Visuals.handchamscol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material##material_hand_chams", &Variables.Visuals.VisualsHandChamsMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));

					ImGui::Checkbox("sleeve chams", &Variables.Visuals.chams_sleeve); ImGui::SameLine();
					ImGui::ColorEdit4("##sleeve_chams_color", Variables.Visuals.chams_sleeve_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material##material_sleeve_chams", &Variables.Visuals.chams_sleeve_material, ChamsMaterials, ARRAYSIZE(ChamsMaterials));

					ImGui::Checkbox("weapon chams", &Variables.Visuals.enableweaponchams); ImGui::SameLine();
					ImGui::ColorEdit4("##weapon_chams_color", Variables.Visuals.weaponchamscol, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material##material_weapon_chams", &Variables.Visuals.VisualsWeaponChamsMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));
				
				} ImGui::EndChild();

				ImGui::BeginChild("##localGlow", ImVec2(334, 80), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Glow"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Checkbox("glow##local_glow", &Variables.Visuals.VisualsLocalGlowEnabled); ImGui::SameLine();
					ImGui::ColorEdit4("##local_glow_color", Variables.Visuals.VisualsLocalGlowColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);

					ImGui::Combo("style##local_glow_style", &Variables.Visuals.VisualsLocalGlowGlowstyle, GlowStyles, ARRAYSIZE(GlowStyles));
				
				} ImGui::EndChild();
			}
			else if (visualsSubTab == 1) // Enemy tab
			{
				ImGui::Columns(2, NULL, false);

				ImGui::BeginChild("##enemyVisuals", ImVec2(334, 375), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Visuals"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Checkbox("draw box##enemy_box", &Variables.Visuals.enemy_box); ImGui::SameLine();
					ImGui::ColorEdit3("##enemy_box_color", Variables.Visuals.enemy_box_col, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("draw skeleton##enemy_skeleton", &Variables.Visuals.enemy_skeleton); ImGui::SameLine();
					ImGui::ColorEdit3("##enemy_skeleton_color", Variables.Visuals.enemy_skeleton_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

					//ImGui::Checkbox("draw skeleton history##enemy_history_skeleton", &Variables.Visuals.enemy_skeleton_history); ImGui::SameLine();
					//ImGui::ColorEdit3("##history_skeleton_color", Variables.Visuals.enemy_skeleton_history_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

					ImGui::Checkbox("health bar##enemy_health_bar", &Variables.Visuals.enemy_health); ImGui::SameLine();
					ImGui::ColorEdit3("##enemy_health_bar_color", Variables.Visuals.enemy_health_col, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("health based color##enemy_health_base", &Variables.Visuals.enemy_health_based); ImGui::Spacing();

					ImGui::Checkbox("armor bar##enemy_armor_bar", &Variables.Visuals.enemy_armor); ImGui::SameLine();
					ImGui::ColorEdit3("##enemy_armor_bar_color", Variables.Visuals.enemy_armor_col, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("name##enemy_name", &Variables.Visuals.enemy_name); ImGui::SameLine();
					ImGui::ColorEdit3("##enemy_name_color", Variables.Visuals.enemy_name_col, ImGuiColorEditFlags_NoInputs);

					static std::string previewEnemyWeapon = "";

					if (ImGui::BeginCombo("weapon type", previewEnemyWeapon.c_str()))
					{
						previewEnemyWeapon = "";
						std::vector<std::string> vec;

						for (int i = 0; i < IM_ARRAYSIZE(WeaponType); i++)
						{
							ImGui::Selectable(WeaponType[i], &Variables.Visuals.enemy_weapon[i] , ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);

							if (Variables.Visuals.enemy_weapon[i]) {
								vec.push_back(WeaponType[i]);
							}
						}
						for (size_t i = 0; i < vec.size(); i++)
						{
							if (vec.size() == 1)
								previewEnemyWeapon += vec.at(i);
							else if (!(i == vec.size() - 1))
								previewEnemyWeapon += vec.at(i) + ", ";
							else
								previewEnemyWeapon += vec.at(i);
						}
						ImGui::EndCombo();
					} ImGui::SameLine();
					ImGui::ColorEdit3("##enemy_weapon_color", Variables.Visuals.enemy_weapon_col, ImGuiColorEditFlags_NoInputs); ImGui::Spacing();

					//ImGui::Checkbox("weapon##enemy_weapon", &Variables.Visuals.enemy_weapon); ImGui::SameLine();

					ImGui::Checkbox("flags##enemy_flags", &Variables.Visuals.enemy_flags); ImGui::Spacing();

					ImGui::Checkbox("gossip arrow##enemy_gossip_arrow", &Variables.Visuals.enemy_gossip_arrow); ImGui::SameLine();
					ImGui::ColorEdit3("##enemy_gossip_arrow_color", Variables.Visuals.enemy_gossip_arrow_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);

				} ImGui::EndChild();

				ImGui::NextColumn();

				ImGui::BeginChild("##enemyChams", ImVec2(334, 165), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Chams"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Checkbox("enable chams##enemy_chams", &Variables.Visuals.VisualsChamsEnabled); ImGui::SameLine();
					ImGui::ColorEdit4("##enemy_chams_color", Variables.Visuals.VisualsChamsColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material##enemy_material_chams", &Variables.Visuals.VisualsChamsMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));

					ImGui::Checkbox("invisible chams##enemy_chams_invisible", &Variables.Visuals.VisualsChamsIgnoreZ); ImGui::SameLine();
					ImGui::ColorEdit4("##enemy_chams_invisible_color", Variables.Visuals.VisualsChamsColorIgnoreZ, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);

					ImGui::Checkbox("chams layer##enemy_chams_layer", &Variables.Visuals.enemy_chams_layer); ImGui::SameLine();
					ImGui::ColorEdit4("##enemy_chams_layer_color", Variables.Visuals.enemy_chams_layer_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material layer##enemy_material_chams_layer", &Variables.Visuals.enemy_material_chams_layer, ChamsMaterials, ARRAYSIZE(ChamsMaterials));
				
				} ImGui::EndChild();

				ImGui::BeginChild("##enemyGlow", ImVec2(334, 77), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Glow"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Checkbox("glow##enemy_glow", &Variables.Visuals.VisualsGlowEnabled); ImGui::SameLine();
					ImGui::ColorEdit4("##enemy_glow_color", Variables.Visuals.VisualsGlowColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("style##enemy_glow_style", &Variables.Visuals.VisualsGlowGlowstyle, GlowStyles, ARRAYSIZE(GlowStyles));

				} ImGui::EndChild();
			}
			else if (visualsSubTab == 2) // Team tab
			{
			ImGui::Columns(2, NULL, false);

				ImGui::BeginChild("##teamVisuals", ImVec2(334, 375), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Visuals"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Text("In constrution");

				} ImGui::EndChild();

				ImGui::NextColumn();

				ImGui::BeginChild("##TeamChams", ImVec2(334, 165), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Chams"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Checkbox("enable chams##team_chams", &Variables.Visuals.Team.team_chams_enabled); ImGui::SameLine();
					ImGui::ColorEdit4("##team_chams_color", Variables.Visuals.Team.team_chams_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material##team_material_chams", &Variables.Visuals.Team.team_chams_material, ChamsMaterials, ARRAYSIZE(ChamsMaterials));

					ImGui::Checkbox("invisible chams##team_chams_invisible", &Variables.Visuals.Team.team_chams_invisible); ImGui::SameLine();
					ImGui::ColorEdit4("##team_chams_invisible_color", Variables.Visuals.Team.team_chams_invisible_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);

					ImGui::Checkbox("chams layer##team_chams_layer", &Variables.Visuals.Team.team_chams_layer); ImGui::SameLine();
					ImGui::ColorEdit4("##team_chams_layer_color", Variables.Visuals.Team.team_chams_layer_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
					ImGui::Combo("material layer##team_material_chams_layer", &Variables.Visuals.Team.team_material_chams_layer, ChamsMaterials, ARRAYSIZE(ChamsMaterials));

				} ImGui::EndChild();

				ImGui::BeginChild("##TeamGlow", ImVec2(334, 80), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Glow"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Text("In constrution");

				} ImGui::EndChild();

			}
			else if (visualsSubTab == 3) // World tab
			{
				ImGui::Spacing();

				ImGui::BeginChild("##WorldVisuals", ImVec2(334, 86), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Weapon"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Checkbox("enable render", &Variables.Visuals.weapon_dropped); ImGui::Spacing();

					ImGui::Checkbox("draw box", &Variables.Visuals.weapon_dropped_box); ImGui::SameLine();
					ImGui::ColorEdit3("##weapon_box_color", Variables.Visuals.weapon_dropped_box_col, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("name", &Variables.Visuals.weapon_dropped_name); ImGui::SameLine();
					ImGui::ColorEdit3("##weapon_name_color", Variables.Visuals.weapon_dropped_name_col, ImGuiColorEditFlags_NoInputs);

				}ImGui::EndChild();

				ImGui::BeginChild("##WorldRemovals", ImVec2(334, 116), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Removals"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Checkbox("scope", &Variables.Visuals.noscope);
					ImGui::Checkbox("smoke", &Variables.Visuals.nosmoke);
					ImGui::Checkbox("flash", &Variables.Visuals.noflash);
					ImGui::Checkbox("visual recoil", &Variables.Visuals.novisrecoil);
					ImGui::Checkbox("post processing", &Variables.Visuals.nopostprocsess);
				}ImGui::EndChild();
			}
			else if (visualsSubTab == 4) // Others tab
			{
				static const char* Skyboxmode[] =
				{
					"off",
					"ymaja",
					"himalaya",
					"mijtm",
					"jungle",
					"new jok",
					"light",
					"night",
					"storm light",
				};

				ImGui::Columns(2, NULL, false);

				ImGui::BeginChild("##othersEnvironment", ImVec2(334, 375), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Environment"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Hotkey("thirdperson key", &Variables.Visuals.thirdperson_key, ImVec2(40, 20));

					ImGui::Combo("sky changer", &Variables.Visuals.skychangar, Skyboxmode, IM_ARRAYSIZE(Skyboxmode));
					ImGui::Checkbox("night mode", &Variables.Visuals.nightmode);
					ImGui::Checkbox("show beams", &Variables.Visuals.beams); ImGui::SameLine();
					ImGui::ColorEdit3("##beamcolor", Variables.Visuals.beamcolor, ImGuiColorEditFlags_NoInputs);

					ImGui::SliderFloat("field of view", &Variables.Visuals.fieldofview, 0, 150, ("%.1f"));
					ImGui::SliderFloat("viewmodel fov", &Variables.Visuals.viewmodelfov, 0, 170, ("%.1f"));
					ImGui::SliderFloat("view offset x", &Variables.Visuals.viewmodeloffsetx, -30, 30, ("%.1f"));
					ImGui::SliderFloat("view offset y", &Variables.Visuals.viewmodeloffsety, -30, 30, ("%.1f"));
					ImGui::SliderFloat("view offset z", &Variables.Visuals.viewmodeloffsetz, -30, 30, ("%.1f"));
					
				} ImGui::EndChild();

				ImGui::NextColumn();

				ImGui::BeginChild("##othersHealpers", ImVec2(334, 375), false);
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
					ImGui::Spacing(); ImGui::Text("Healpers"); ImGui::Separator();
					ImGui::PopStyleColor();

					ImGui::Checkbox("indicator", &Variables.Visuals.indicator);
					ImGui::Checkbox("bomber time", &Variables.Visuals.drawc4);
					ImGui::Checkbox("grenade prediction", &Variables.Visuals.greane_prediction);
					ImGui::Checkbox("show grenade", &Variables.Visuals.drawnades); ImGui::SameLine();
					
					ImGui::ColorEdit3("##show_grenade_col", Variables.Visuals.draw_nades_col, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("spreed circle", &Variables.Visuals.spreed_circle); ImGui::SameLine();
					ImGui::ColorEdit3("##spreed_circle_col", Variables.Visuals.spreed_circle_col, ImGuiColorEditFlags_NoInputs);

					ImGui::Checkbox("lagcomp hitbox", &Variables.Visuals.lagcomp_enable); ImGui::SameLine();
					ImGui::ColorEdit3("##lagcomp_col", Variables.Visuals.lagcomp_col, ImGuiColorEditFlags_NoInputs);
					if (Variables.Visuals.lagcomp_enable) {
						ImGui::SliderFloat("duration", &Variables.Visuals.lagcomp_duration, 0, 5, ("%.2f"));
					}

					ImGui::Checkbox("hitmarker", &Variables.Visuals.playerhitmarker); ImGui::SameLine();
					ImGui::ColorEdit3("##markercol", Variables.Visuals.markercol, ImGuiColorEditFlags_NoInputs);
					ImGui::Combo("hitsound", &Variables.Visuals.hitsound, hitsounds, ARRAYSIZE(hitsounds));

					ImGui::Checkbox("aspect ratio", &Variables.Visuals.aspectratioenable);
					ImGui::SliderInt("aspect x", &Variables.Visuals.aspectratiox, 0, 1000, ("%.1f"));
					ImGui::SliderInt("aspect y", &Variables.Visuals.aspectratioxy, 0, 1000, ("%.1f"));
					ImGui::Checkbox("preserve killfeed", &Variables.Visuals.preservekillfeed);

					//ImGui::Checkbox("keystrokes", &Variables.Visuals.wasdindicator);
					//ImGui::Checkbox("kill effect", &Variables.Visuals.kill_effect);
					ImGui::Checkbox("self circle", &Variables.Visuals.selfcircle);
					
				} ImGui::EndChild();

			}

		}
		else if (renderPageMenu == 4) {
			ImGui::Columns(2, NULL, false);

			ImGui::BeginChild("##MiscMoviment", ImVec2(334, 105), false);
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
				ImGui::Spacing(); ImGui::Text("Moviment"); ImGui::Separator();
				ImGui::PopStyleColor();

				ImGui::Checkbox("anti afk", &Variables.Misc.anti_afk_kick);
				ImGui::Checkbox("bunny hop", &Variables.Misc.bhop);
				ImGui::Checkbox("auto strafe", &Variables.Misc.autostrafe);
				ImGui::Checkbox("fast duck", &Variables.Misc.fast_duck);
				ImGui::Checkbox("moon walk", &Variables.Misc.moon_walk);

			} ImGui::EndChild();

			ImGui::BeginChild("##MiscInGame", ImVec2(334, 83), false);
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
				ImGui::Spacing(); ImGui::Text("In game"); ImGui::Separator();
				ImGui::PopStyleColor();

				ImGui::Checkbox("reveal rank", &Variables.Misc.reveal_rank);
				ImGui::Checkbox("reveal money", &Variables.Misc.reveal_money);

			} ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild("##MiscInterface", ImVec2(334, 66), false);
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
				ImGui::Spacing(); ImGui::Text("Interface"); ImGui::Separator();
				ImGui::PopStyleColor();

				ImGui::Text("menu color"); ImGui::SameLine();
				ImGui::ColorEdit3("##menucol", Variables.menucolor, ImGuiColorEditFlags_NoInputs);

				ImGui::Checkbox("event logs", &Variables.Misc.eventlogs); ImGui::SameLine();
				ImGui::ColorEdit3("##event_logs_col", Variables.Misc.event_logs_col, ImGuiColorEditFlags_NoInputs);

			} ImGui::EndChild();

			ImGui::BeginChild("##MiscConfigLoading", ImVec2(334, 136), false);
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Variables.menucolor[0], Variables.menucolor[1], Variables.menucolor[2], 1.00f));
				ImGui::Spacing(); ImGui::Text("Configuration"); ImGui::Separator();
				ImGui::PopStyleColor();

				using namespace ImGui;

				getconfig();
				ImGui::Spacing();
				if (ImGui::Combo("your config", &selectedcfg, [](void* data, int idx, const char** out_text)
					{
						*out_text = configs[idx].c_str();
						return true;
					}, nullptr, configs.size(), 10))
				{
					cfgname = configs[selectedcfg];
					cfgname.erase(cfgname.length() - 4, 4);
					strcpy(Variables.Misc.config_name, cfgname.c_str());
				}

				ImGui::Spacing();

				if (ImGui::Button("save", ImVec2(104, 21)))
					Config->Save();

				ImGui::SameLine();

				if (ImGui::Button("load", ImVec2(104, 21)))
					Config->Load();

				ImGui::SameLine();

				if (ImGui::Button("reset settings", ImVec2(106, 21)))
					Config->setup();

				ImGui::Spacing(); ImGui::Spacing();

				ImGui::Text("config name:"); ImGui::SameLine();
				ImGui::InputText("##name_config", Variables.Misc.config_name, 30);

				ImGui::Spacing();

				static bool create;

				if (ImGui::Button("create", ImVec2(-1, 21)))
				{
					Config->Save();
					create = !create;
				}

			} ImGui::EndChild();
		}
	
		ImGui::End();
	}


	ImGui::Render();
	//ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}


void Menu::Show()
{
	Visible = true;
}

void Menu::Hide()
{
	Visible = false;
}

void Menu::Toggle()
{
	Visible = !Visible;
}


void Menu::CreateStyle()
{
	ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( Droid_compressed_data, Droid_compressed_size, 14.f, NULL, ImGui::GetIO( ).Fonts->GetGlyphRangesCyrillic( ) );
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.18f, 0.31f, 0.43f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.08f, 0.08f, 0.1f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.1f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.12f, 0.15f, 0.2f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.31f, 0.43f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.32f, 0.46f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.22f, 0.22f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.1f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.05f, 0.05f, 0.08f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.31f, 0.43f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.16f, 0.32f, 0.46f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.32f, 0.46f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.16f, 0.32f, 0.46f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.18f, 0.31f, 0.43f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.16f, 0.32f, 0.46f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.16f, 0.32f, 0.46f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);


	style.Alpha = 1.0f;									// Global alpha applies to everything in ImGui
	style.WindowPadding = ImVec2( 8, 15 );				// Padding within a window
	style.WindowRounding = 5.f;							// Radius of window corners rounding. Set to 0.0f to have rectangular windows
	style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );		// Alignment for title bar text
	style.ChildWindowRounding = 5.f;					// Radius of child window corners rounding. Set to 0.0f to have rectangular windows
	style.FramePadding = ImVec2( 4, 1 );				// Padding within a framed rectangle (used by most widgets)
	style.FrameRounding = 0.0f;							// Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
	style.ItemSpacing = ImVec2( 8, 4 );					// Horizontal and vertical spacing between widgets/lines
	style.ItemInnerSpacing = ImVec2( 4, 4 );			// Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
	style.TouchExtraPadding = ImVec2( 0, 0 );			// Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
	style.IndentSpacing = 21.0f;						// Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
	style.ColumnsMinSpacing = 15.0f;					// Minimum horizontal spacing between two columns
	style.ScrollbarSize = 10.0f;						// Width of the vertical scrollbar, Height of the horizontal scrollbar
	style.ScrollbarRounding = 20.0f;					// Radius of grab corners rounding for scrollbar
	style.GrabMinSize = 8.f;							// Minimum width/height of a grab box for slider/scrollbar
	style.GrabRounding = 0.0f;							// Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
	style.ButtonTextAlign = ImVec2( 0.5f, 0.5f );		// Alignment of button text when button is larger than text.
	style.DisplayWindowPadding = ImVec2( 22, 22 );		// Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
	style.DisplaySafeAreaPadding = ImVec2( 4, 4 );      // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
	style.AntiAliasedLines = true;						// Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
	style.CurveTessellationTol = 1.25f;					// Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.

	bigmenu_font = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedBase85TTF( rawData_compressed_data_base85, 70 );
	menu_font = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedBase85TTF( rawData_compressed_data_base85, 18 );
	smallmenu_font = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedBase85TTF( smalll_compressed_data_base85, 13 );
}

















































