#include "sdk.hpp"
#include "interfaces/memalloc.h"
#include "../Helpers/Utils.hpp"
#include "../features/predictionsystem.h"
#include <windows.h>
#include <winuser.h>
#include <intrin.h>
#include <cstdint>
#include <string>
#include <stdio.h>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <random>
#include <thread>
#include <chrono>

namespace Interfaces
{
    CreateInterfaceFn get_module_factory(HMODULE module)
    {
        return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
    }

    template<typename T>
    T* get_interface(CreateInterfaceFn f, const char* szInterfaceVersion)
    {
        auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

        if(!result) {
            throw std::runtime_error(std::string("[get_interface] Failed to GetOffset interface: ") + szInterfaceVersion);
        }

        return result;
    }

    void Initialize()
    {
        auto engineFactory    = get_module_factory(GetModuleHandleW(L"engine.dll"));
        auto clientFactory    = get_module_factory(GetModuleHandleW(L"client.dll"));
        auto valveStdFactory  = get_module_factory(GetModuleHandleW(L"vstdlib.dll"));
        auto vguiFactory      = get_module_factory(GetModuleHandleW(L"vguimatsurface.dll"));
        auto vgui2Factory     = get_module_factory(GetModuleHandleW(L"vgui2.dll"));
        auto matSysFactory    = get_module_factory(GetModuleHandleW(L"materialsystem.dll"));
        auto dataCacheFactory = get_module_factory(GetModuleHandleW(L"datacache.dll"));
        auto vphysicsFactory  = get_module_factory(GetModuleHandleW(L"vphysics.dll"));
        auto inputSysFactory  = get_module_factory(GetModuleHandleW(L"inputsystem.dll"));
        
        g_CHLClient           = get_interface<IBaseClientDLL>      (clientFactory   , "VClient018");
        g_EntityList          = get_interface<IClientEntityList>   (clientFactory   , "VClientEntityList003");
        g_Prediction          = get_interface<IPrediction>         (clientFactory   , "VClientPrediction001");
        g_GameMovement        = get_interface<CGameMovement>       (clientFactory   , "GameMovement001");
        g_MdlCache            = get_interface<IMDLCache>           (dataCacheFactory, "MDLCache004");
        g_EngineClient        = get_interface<IVEngineClient>      (engineFactory   , "VEngineClient014");
        g_MdlInfo             = get_interface<IVModelInfoClient>   (engineFactory   , "VModelInfoClient004");
        g_MdlRender           = get_interface<IVModelRender>       (engineFactory   , "VEngineModel016");
        g_RenderView          = get_interface<IVRenderView>        (engineFactory   , "VEngineRenderView014");
        g_EngineTrace         = get_interface<IEngineTrace>        (engineFactory   , "EngineTraceClient004");
        g_DebugOverlay        = get_interface<IVDebugOverlay>      (engineFactory   , "VDebugOverlay004");
        g_GameEvents          = get_interface<IGameEventManager2>  (engineFactory   , "GAMEEVENTSMANAGER002");
        g_EngineSound         = get_interface<IEngineSound>        (engineFactory   , "IEngineSoundClient003");
        g_MatSystem           = get_interface<IMaterialSystem>     (matSysFactory   , "VMaterialSystem080");
        g_CVar                = get_interface<ICvar>               (valveStdFactory , "VEngineCvar007");
        g_VGuiPanel           = get_interface<IPanel>              (vgui2Factory    , "VGUI_Panel009");
        g_VGuiSurface         = get_interface<ISurface>            (vguiFactory     , "VGUI_Surface031");
        g_PhysSurface         = get_interface<IPhysicsSurfaceProps>(vphysicsFactory , "VPhysicsSurfaceProps001");
        g_InputSystem         = get_interface<IInputSystem>        (inputSysFactory , "InputSystemVersion001");

        auto client = GetModuleHandleW(L"client.dll");
        auto engine = GetModuleHandleW(L"engine.dll");
        auto dx9api = GetModuleHandleW(L"shaderapidx9.dll");

        g_pGlobalVars = **(CGlobalVarsBase * **)((*(DWORD * *)g_CHLClient)[0] + 0x1F);
        g_ClientMode = **(IClientMode * **)((*(DWORD * *)g_CHLClient)[10] + 0x5);
		    g_Input = *(CInput * *)(Utils::PatternScan(client, "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
		    g_MoveHelper      =      **(IMoveHelper***)(Utils::PatternScan(client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
		    g_GlowObjManager = *(CGlowObjectManager * *)(Utils::PatternScan(client, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
		    g_ViewRender      =        *(IViewRender**)(Utils::PatternScan(client, "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
		    g_D3DDevice9 = **(IDirect3DDevice9 * **)(Utils::PatternScan(dx9api, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
		    g_ClientState     =     **(CClientState***)(Utils::PatternScan(engine, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
		    g_LocalPlayer = *(C_LocalPlayer*)(Utils::PatternScan(client, "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);
			g_RenderBeams = *(IViewRenderBeams**)(Utils::PatternScan(client, "A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 0x1);
			g_pMemAlloc = *reinterpret_cast<IMemAlloc**> (GetProcAddress(GetModuleHandleA("tier0.dll"), "g_pMemAlloc"));

            Prediction::Init();
    }

    void Dump()
    {
        // Ugly macros ugh
        #define STRINGIFY_IMPL(s) #s
        #define STRINGIFY(s)      STRINGIFY_IMPL(s)
        #define PRINT_INTERFACE(name) Utils::ConsolePrint("%-20s: %p\n", STRINGIFY(name), name)

        PRINT_INTERFACE(g_CHLClient   );
        PRINT_INTERFACE(g_EntityList  );
        PRINT_INTERFACE(g_Prediction  );
        PRINT_INTERFACE(g_GameMovement);
        PRINT_INTERFACE(g_MdlCache    );
        PRINT_INTERFACE(g_EngineClient);
        PRINT_INTERFACE(g_MdlInfo     );
        PRINT_INTERFACE(g_MdlRender   );
        PRINT_INTERFACE(g_RenderView  );
        PRINT_INTERFACE(g_EngineTrace );
        PRINT_INTERFACE(g_DebugOverlay);
        PRINT_INTERFACE(g_GameEvents  );
        PRINT_INTERFACE(g_EngineSound );
        PRINT_INTERFACE(g_MatSystem   );
        PRINT_INTERFACE(g_CVar        );
        PRINT_INTERFACE(g_VGuiPanel   );
        PRINT_INTERFACE(g_VGuiSurface );
        PRINT_INTERFACE(g_PhysSurface );
        PRINT_INTERFACE(g_InputSystem );
    }
}


std::string wide_to_multibyte(const std::wstring& str) {
    if (str.empty())
        return {};

    int str_len = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0, nullptr, nullptr);

    std::string out;
    out.resize(str_len);

    WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), &out[0], str_len, nullptr, nullptr);

    return out;
}



// get DOS / NT headers.
bool pe::get_file_headers(uintptr_t base, PIMAGE_DOS_HEADER& out_dos, PIMAGE_NT_HEADERS& out_nt) {
    // get DOS header and check for invalid DOS / DOS signature.
    auto dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (!dos || dos->e_magic != IMAGE_DOS_SIGNATURE /* "MZ" */)
        return false;

    // get NT headers and check for invalid NT / NT signature.
    auto nt = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);
    if (!nt || nt->Signature != IMAGE_NT_SIGNATURE /* "PE\0\0" */)
        return false;

    out_dos = dos;
    out_nt = nt;

    return true;
}

pe::Module::Module() : m_ldr_entry{ nullptr }, m_dos{ nullptr }, m_nt{ nullptr }, m_base{ 0 } { }

bool pe::Module::init(types::LDR_DATA_TABLE_ENTRY* ldr_entry) {
    IMAGE_DOS_HEADER* dos;
    IMAGE_NT_HEADERS* nt;

    if (!ldr_entry)
        return false;

    uintptr_t base = ldr_entry->DllBase;

    if (!get_file_headers(base, dos, nt))
        return false;

    m_ldr_entry = ldr_entry;
    m_dos = dos;
    m_nt = nt;
    m_base = base;
    m_pathW = ldr_entry->FullDllName.Buffer;

    std::transform(m_pathW.begin(), m_pathW.end(), m_pathW.begin(), tolower);

    return true;
}

uintptr_t pe::Module::get_base() const {
    return m_base;
}

IMAGE_DOS_HEADER* pe::Module::get_dos_header() const {
    return (m_dos) ? m_dos : nullptr;
}

IMAGE_NT_HEADERS* pe::Module::get_nt_headers() const {
    return (m_nt) ? m_nt : nullptr;
}

size_t pe::Module::get_code_size() const {
    return (m_nt) ? m_nt->OptionalHeader.SizeOfCode : 0;
}

size_t pe::Module::get_image_size() const {
    return (m_nt) ? m_nt->OptionalHeader.SizeOfImage : 0;
}

uintptr_t pe::Module::get_code_base() const {
    return (m_nt) ? RVA(m_nt->OptionalHeader.BaseOfCode) : 0;
}

std::wstring pe::Module::get_pathW() const {
    return m_pathW;
}

std::string pe::Module::get_pathA() const {
    return wide_to_multibyte(get_pathW());
}

std::wstring pe::Module::get_module_nameW() const {
    if (m_pathW.empty())
        return { };

    std::wstring out = get_pathW();

    size_t delim = out.find_last_of('\\');
    if (delim == std::wstring::npos)
        return { };

    // keep only name + extension.
    return out.substr(delim + 1);
}

std::string pe::Module::get_module_nameA() const {
    return wide_to_multibyte(get_module_nameW());
}






pattern::Pattern::Pattern(const std::string& str) {
    std::string part;
    if (str.empty())
        return;

    auto ss = std::stringstream(str);

    // iterate string stream, splitting by space delim.
    do {
        // too long...
        if (part.empty() || part.size() > 2)
            continue;

        // ensure it's a valid wildcard or byte.
        if (part[0] == '?')
            m_pattern.push_back({});

        else if (std::isxdigit(static_cast<uint8_t>(part[0])) && std::isxdigit(static_cast<uint8_t>(part[1])))
            m_pattern.push_back({ static_cast<uint8_t>(std::strtoul(part.c_str(), nullptr, 16)), false });
    } while (ss >> part);
}

uintptr_t pattern::find(uintptr_t start, size_t len, const Pattern& pattern) {
    static auto search_pred = [](const uint8_t a, const PatternByte_t& b) {
        return b.match(a);
    };

    if (!start || !len || pattern.empty())
        return 0;

    uint8_t* scan_start = reinterpret_cast<uint8_t*>(start);
    uint8_t* scan_end = scan_start + len;

    auto it = std::search(
        scan_start,
        scan_end,
        pattern.begin(),
        pattern.end(),
        search_pred
    );

    // nothing found...
    if (it == scan_end)
        return 0;

    return reinterpret_cast<uintptr_t>(it);
}

// scan for pattern in range.
uintptr_t pattern::find(uintptr_t start, size_t len, const std::string& pattern) {
    return find(start, len, Pattern(pattern));
}

// scan for pattern in entire module.
uintptr_t pattern::find(const pe::Module& module, const std::string& pattern) {
    return find(module.get_code_base(), module.get_code_size(), pattern);
}

// scan for pattern in entire module by module name hash.
uintptr_t pattern::find(hash32_t hash, const std::string& pattern) {
    if (!hash || pattern.empty())
        return 0;

    pe::Module module = pe::get_module(hash);
    if (!module)
        return 0;

    return find(module.get_code_base(), module.get_code_size(), pattern);
}

// scan for pattern in entire module by module name string.
uintptr_t pattern::find(const std::string& str, const std::string& pattern) {
    if (str.empty() || pattern.empty())
        return 0;

    pe::Module module = pe::get_module(str);
    if (!module)
        return 0;

    return find(module.get_code_base(), module.get_code_size(), pattern);
}































































