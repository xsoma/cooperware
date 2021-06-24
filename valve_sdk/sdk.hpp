#pragma once

#define NOMINMAX
#include <Windows.h>

#include "Misc/Enums.hpp"
#include "Misc/vfunc.hpp"

#include "Math/VMatrix.hpp"
#include "Math/QAngle.hpp"
#include "Math/Vector.hpp"
#include "Misc/Studio.hpp"

#include "Interfaces/IAppSystem.hpp"
#include "Interfaces/IBaseClientDll.hpp"
#include "Interfaces/IClientEntity.hpp"
#include "Interfaces/IClientEntityList.hpp"
#include "Interfaces/IClientMode.hpp"
#include "Interfaces/IConVar.hpp"
#include "Interfaces/ICvar.hpp"
#include "Interfaces/IEngineTrace.hpp"
#include "Interfaces/IVEngineClient.hpp"
#include "Interfaces/IVDebugOverlay.hpp"
#include "Interfaces/ISurface.hpp"
#include "Interfaces/CInput.hpp"
#include "Interfaces/IVModelInfoClient.hpp"
#include "Interfaces/IVModelRender.hpp"
#include "Interfaces/IRenderView.hpp"
#include "Interfaces/IGameEventmanager.hpp"
#include "Interfaces/IMaterialSystem.hpp"
#include "Interfaces/IMoveHelper.hpp"
#include "Interfaces/IMDLCache.hpp"
#include "Interfaces/IPrediction.hpp"
#include "Interfaces/IPanel.hpp"
#include "Interfaces/IEngineSound.hpp"
#include "Interfaces/IViewRender.hpp"
#include "Interfaces/CClientState.hpp"
#include "Interfaces/IPhysics.hpp"
#include "Interfaces/IInputSystem.hpp"
#include "interfaces/IRefCounted.hpp"
#include "interfaces/IViewRenderBeams.hpp"
#include "Misc/Convar.hpp"
#include "Misc/CUserCmd.hpp"
#include "Misc/glow_outline_effect.hpp"
#include "Misc/datamap.hpp"

#include "netvars.hpp"
#include "interfaces/memalloc.h"


#include <algorithm>

struct IDirect3DDevice9;

namespace Interfaces
{
    void Initialize();
    void Dump();
}

inline IVEngineClient*       g_EngineClient;
inline IBaseClientDLL*       g_CHLClient;
inline IClientEntityList*    g_EntityList;
inline CGlobalVarsBase* g_pGlobalVars;
inline IEngineTrace*         g_EngineTrace;
inline ICvar*                g_CVar;
inline IPanel*               g_VGuiPanel;
inline IClientMode*          g_ClientMode;
inline IVDebugOverlay*       g_DebugOverlay;
inline ISurface*             g_VGuiSurface;
inline CInput*               g_Input;
inline IVModelInfoClient*    g_MdlInfo;
inline IVModelRender*        g_MdlRender;
inline IVRenderView*         g_RenderView;
inline IMaterialSystem*      g_MatSystem;
inline IGameEventManager2*   g_GameEvents;
inline IMoveHelper*          g_MoveHelper;
inline IMDLCache*            g_MdlCache;
inline IPrediction*          g_Prediction;
inline CGameMovement*        g_GameMovement;
inline IEngineSound*         g_EngineSound;
inline CGlowObjectManager*   g_GlowObjManager;
inline IViewRender*          g_ViewRender;
inline IDirect3DDevice9*     g_D3DDevice9;
inline CClientState*         g_ClientState;
inline IPhysicsSurfaceProps* g_PhysSurface;
inline IInputSystem*         g_InputSystem;
inline IViewRenderBeams* g_RenderBeams;
inline IMemAlloc* g_pMemAlloc;
template<typename... Args>
void ConMsg(const char* pMsg, Args... args)
{
    static auto import = (void(*)(const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConMsg@@YAXPBDZZ");
    return import(pMsg, args...);
}
template<typename... Args>
void ConColorMsg(const Color& clr, const char* pMsg, Args... args)
{
    static auto import = (void(*)(const Color&, const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ");
    return import(clr, pMsg, args...);
}

#include "Misc/EHandle.hpp"

class C_LocalPlayer
{
    friend bool operator==(const C_LocalPlayer& lhs, void* rhs);
public:
    C_LocalPlayer() : m_local(nullptr) {}

    operator bool() const { return *m_local != nullptr; }
    operator C_BasePlayer*() const { return *m_local; }

    C_BasePlayer* operator->() { return *m_local; }

private:
    C_BasePlayer** m_local;
};

inline C_LocalPlayer g_LocalPlayer;

namespace Proxies
{

    void nSequence(const CRecvProxyData* pData, void* pStruct, void* pOut);
}

extern RecvVarProxyFn o_nSequence;










using hash32_t = unsigned int;

namespace util {
	namespace hash {
		// FNV-1a constants.
		enum : hash32_t {
			FNV1A_PRIME = 0x1000193,
			FNV1A_BASIS = 0x811C9DC5
		};

		// compile-time strlen.
		constexpr size_t ct_strlen(const char* str) {
			size_t out = 1;

			for (; str[out] != '\0'; ++out);

			return out;
		}

		// hash data.
		constexpr hash32_t fnv1a_32(const uint8_t* data, const size_t len) {
			hash32_t out = FNV1A_BASIS;

			for (size_t i = 0; i < len; ++i)
				out = (out ^ data[i]) * FNV1A_PRIME;

			return out;
		}

		// hash c-style string.
		constexpr hash32_t fnv1a_32(const char* str) {
			hash32_t out = FNV1A_BASIS;
			size_t len = ct_strlen(str);

			for (size_t i = 0; i < len; ++i)
				out = (out ^ str[i]) * FNV1A_PRIME;

			return out;
		}

		// hash C++-style string (runtime only).
		__forceinline hash32_t fnv1a_32(const std::string& str) {
			return fnv1a_32((uint8_t*)str.c_str(), str.size());
		}
	}
}
namespace pe {

	namespace types {
		enum EXCEPTION_DISPOSITION : size_t {
			ExceptionContinueExecution = 0,
			ExceptionContinueSearch,
			ExceptionNestedException,
			ExceptionCollidedUnwind
		};

		struct EXCEPTION_REGISTRATION_RECORD {
			EXCEPTION_REGISTRATION_RECORD* Next;
			EXCEPTION_DISPOSITION Handler;
		};

		struct NT_TIB {
			EXCEPTION_REGISTRATION_RECORD* ExceptionList;
			uintptr_t StackBase;
			uintptr_t StackLimit;
			uintptr_t SubSystemTib;

			union {
				uintptr_t FiberData;
				uint32_t Version;
			};

			uintptr_t ArbitraryUserPointer;
			NT_TIB* Self;
		};

		struct CLIENT_ID {
			uintptr_t UniqueProcess;
			uintptr_t UniqueThread;
		};

		struct LIST_ENTRY {
			LIST_ENTRY* Flink;
			LIST_ENTRY* Blink;
		};

		struct PEB_LDR_DATA {
			uint32_t Length;
			uint8_t Initialized;
			uintptr_t SsHandle;
			LIST_ENTRY InLoadOrderModuleList;
			LIST_ENTRY InMemoryOrderModuleList;
		};

		struct UNICODE_STRING {
			uint16_t Length;
			uint16_t MaximumLength;
			wchar_t* Buffer;
		};

		struct ACTIVATION_CONTEXT {
			// todo - dex; no symbols for this?
		};

		struct LDR_DATA_TABLE_ENTRY {
			LIST_ENTRY InLoadOrderLinks;
			LIST_ENTRY InMemoryOrderLinks;
			LIST_ENTRY InInitializationOrderLinks;
			uintptr_t DllBase;
			uintptr_t EntryPoint;
			uint32_t SizeOfImage;
			UNICODE_STRING FullDllName;
			UNICODE_STRING BaseDllName;
		};

		// Process Environment Block.
		struct PEB {
			uint8_t InheritedAddressSpace;
			uint8_t ReadImageFileExecOptions;
			uint8_t BeingDebugged;

			union {
				uint8_t BitField;

				struct {
					uint8_t ImageUsesLargePages : 1;
					uint8_t IsProtectedProcess : 1;
					uint8_t IsLegacyProcess : 1;
					uint8_t IsImageDynamicallyRelocated : 1;
					uint8_t SkipPatchingUser32Forwarders : 1;
					uint8_t SpareBits : 1 /* 3 */;
				};
			};

			uintptr_t Mutant;
			uintptr_t ImageBaseAddress;
			PEB_LDR_DATA* Ldr;
		};

		// Thread Environment Block ( aka Thread Information Block ).
		struct TEB {
			NT_TIB NtTib;
			uintptr_t EnvironmentPointer;
			CLIENT_ID ClientId;
			uintptr_t ActiveRpcHandle;
			uintptr_t ThreadLocalStoragePointer;
			PEB* ProcessEnvironmentBlock;
			uint32_t LastErrorValue;
			uint32_t CountOfOwnedCriticalSections;
			uintptr_t CsrClientThread;
			uintptr_t Win32ThreadInfo;
			uint32_t User32Reserved[26];
			uint32_t UserReserved[5];
			uintptr_t WOW32Reserved; // note - dex; some syscalls use this, but it depends on architecture.
		};
	}

	static bool get_file_headers(uintptr_t base, PIMAGE_DOS_HEADER& out_dos, PIMAGE_NT_HEADERS& out_nt);

	template< typename t >
	class DataEntry_t {
	public:
		uintptr_t m_va; // virtual offset.
		uintptr_t m_size; // size of data entry.
		t* m_va_ptr; // virtual offset ptr.
		// uintptr_t m_end;  // end ptr of data entry.

		DataEntry_t() : m_va{ 0 }, m_size{ 0 }, m_va_ptr{ nullptr } { }

		// valid checks.
		explicit operator bool() const {
			return m_va;
		}

		bool operator !() const {
			return !m_va;
		}
	};

	class Module {
	private:
		types::LDR_DATA_TABLE_ENTRY* m_ldr_entry;
		IMAGE_DOS_HEADER* m_dos;
		IMAGE_NT_HEADERS* m_nt;
		uintptr_t m_base;
		std::wstring m_pathW;

	public:
		Module();

		bool init(types::LDR_DATA_TABLE_ENTRY* ldr_entry);

		uintptr_t get_base() const;
		IMAGE_DOS_HEADER* get_dos_header() const;
		IMAGE_NT_HEADERS* get_nt_headers() const;
		size_t get_code_size() const;
		size_t get_image_size() const;
		uintptr_t get_code_base() const;
		std::wstring get_pathW() const;
		std::string get_pathA() const;
		std::wstring get_module_nameW() const;
		std::string get_module_nameA() const;

		explicit operator uintptr_t() const {
			return m_base;
		}

		// valid checks.
		explicit operator bool() const {
			return get_base();
		}

		bool operator !() const {
			return !get_base();
		}

		template< typename t = uintptr_t >
		t RVA(size_t offset) const {
			if (!m_base || !offset)
				return t{ };

			return (t)(m_base + offset);
		}

		// returns info about a data directory entry from OptionalHeader->DataDirectory array.
		template< typename t >
		DataEntry_t< t > get_data_dir(size_t entry) const {
			DataEntry_t< t > out;

			if (!m_nt || entry > IMAGE_NUMBEROF_DIRECTORY_ENTRIES)
				return { };

			auto data_dir = &m_nt->OptionalHeader.DataDirectory[entry];
			if (!data_dir)
				return { };

			out.m_va = data_dir->VirtualAddress;
			out.m_size = data_dir->Size;
			out.m_va_ptr = RVA< t* >(data_dir->VirtualAddress);;

			return out;
		}
	};
}

namespace pe {
	// container type for "get_all_modules" function.
	using modules_t = std::vector< Module >;

	// get Thread Environment Block.
	__forceinline types::TEB* get_TEB() {
		return reinterpret_cast<pe::types::TEB*>(__readfsdword(0x18));
	}

	// get Thread Environment Block.
	__forceinline types::PEB* get_PEB() {
		auto teb{ get_TEB() };
		if (!teb)
			return nullptr;

		return teb->ProcessEnvironmentBlock;
	}

	// returns a vector containing all modules in current process.
	static bool get_all_modules(modules_t& out) {
		Module mod;

		types::PEB* peb = get_PEB();
		if (!peb)
			return false;

		// valid list?
		if (!peb->Ldr->InMemoryOrderModuleList.Flink)
			return false;

		types::LIST_ENTRY* list = &peb->Ldr->InMemoryOrderModuleList;

		// iterate doubly linked list.
		for (auto i = list->Flink; i != list; i = i->Flink) {
			// get current entry.
			types::LDR_DATA_TABLE_ENTRY* ldr_entry = CONTAINING_RECORD(i, types::LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			if (!ldr_entry)
				continue;

			// attempt to initialize current module.
			if (!mod.init(ldr_entry))
				continue;

			out.push_back(mod);
		}

		return !out.empty();
	}

	// get module by name hash.
	static pe::Module get_module(hash32_t hash) {
		// PE::Types::PEB  *peb;
		modules_t modules;

		if (!get_all_modules(modules))
			return { };

		for (const auto& m : modules) {
			if (util::hash::fnv1a_32(m.get_module_nameA()) == hash)
				return m;
		}

		return { };
	}

	// get module by name.
	static pe::Module get_module(std::string name) {
		if (name.empty())
			return get_module(0);

		std::transform(name.begin(), name.end(), name.begin(), tolower);

		return get_module(util::hash::fnv1a_32(name));
	}

	// get export by hash.
	template< typename t = uintptr_t >
	static t get_export(const pe::Module& mod, hash32_t hash) {
		if (!mod)
			return t{ };

		// get export data directory info.
		auto export_dir = mod.get_data_dir< IMAGE_EXPORT_DIRECTORY >(IMAGE_DIRECTORY_ENTRY_EXPORT);
		if (!export_dir)
			return t{ };

		// get needed arrays.
		auto names = mod.RVA< uint32_t* >(export_dir.m_va_ptr->AddressOfNames);
		auto funcs = mod.RVA< uint32_t* >(export_dir.m_va_ptr->AddressOfFunctions);
		auto ords = mod.RVA< uint16_t* >(export_dir.m_va_ptr->AddressOfNameOrdinals);
		if (!names || !funcs || !ords)
			return t{ };

		for (size_t i = 0; i < export_dir.m_va_ptr->NumberOfNames; ++i) {
			std::string export_name = mod.RVA< const char* >(names[i]);
			if (export_name.empty())
				continue;

			if (util::hash::fnv1a_32(export_name) == hash) {
				uintptr_t export_ptr = mod.RVA(funcs[ords[i]]);
				if (!export_ptr)
					continue;

				// if the export ptr is inside the dir,  then it's a fowarder export and we must resolve it.
				if (export_ptr >= reinterpret_cast<uintptr_t>(export_dir.m_va_ptr) && export_ptr < (reinterpret_cast<uintptr_t>(export_dir.m_va_ptr) + export_dir.
					m_size)) {
					// get forwarder string.
					std::string fwd_str = reinterpret_cast<const char*>(export_ptr);

					size_t delim = fwd_str.find_last_of('.');
					if (delim == std::string::npos)
						return t{ };

					// get forwarder module name.
					std::string fwd_module_name = fwd_str.substr(0, delim + 1);
					fwd_module_name += "dll";

					// get forwarder export name.
					std::string fwd_export_name = fwd_str.substr(delim);

					// get real export ptr (recursively).
					export_ptr = get_export(get_module(fwd_module_name), fwd_export_name);
					if (!export_ptr)
						return { };

					return (t)(export_ptr);
				}

				return (t)(export_ptr);
			}
		}

		return t{ };
	}

	// get export by name.
	template< typename t = uintptr_t >
	static t get_export(const pe::Module& mod, const std::string& name) {
		return get_export< t >(mod, util::hash::fnv1a_32(name));
	}
}

namespace pattern {
	// wraps a uint8_t and has a bool if the byte should be ignored.
	class PatternByte_t {
	private:
		uint8_t m_byte;
		bool m_is_wildcard;

	public:
		PatternByte_t() : m_byte{ 0 }, m_is_wildcard{ true } { }
		PatternByte_t(uint8_t byte, bool is_wildcard) : m_byte{ byte }, m_is_wildcard{ is_wildcard } { }

		uint8_t get_byte() const {
			return m_byte;
		}

		bool is_wildcard() const {
			return m_is_wildcard;
		}

		bool match(const uint8_t other) const {
			return m_byte == other || m_is_wildcard;
		}
	};

	// converts a string into a pattern.
	class Pattern {
	private:
		using pattern_t = std::vector< PatternByte_t >;
		using pattern_citer_t = pattern_t::const_iterator;

		pattern_t m_pattern;

	public:
		Pattern() = default;
		explicit Pattern(const std::string& str);

		const PatternByte_t& operator [](size_t index) const {
			if (index >= m_pattern.size())
				return { };

			return m_pattern[index];
		}

		bool empty() const {
			return m_pattern.empty();
		}

		pattern_citer_t begin() const {
			return m_pattern.cbegin();
		}

		pattern_citer_t end() const {
			return m_pattern.cend();
		}
	};

	uintptr_t find(uintptr_t start, size_t len, const Pattern& pattern);
	uintptr_t find(uintptr_t start, size_t len, const std::string& pattern);
	uintptr_t find(const pe::Module& module, const std::string& pattern);
	uintptr_t find(hash32_t hash, const std::string& pattern);
	uintptr_t find(const std::string& str, const std::string& pattern);

	template < typename t = uintptr_t >
	static t find(const std::string& str, const std::string& pattern) {
		return (t)(find(str, pattern));
	}

	template < typename t = uintptr_t >
	static t find(const pe::Module& module, const std::string& pattern, int offset = 0) {
		return (t)(find(module, pattern) + offset);
	}
}