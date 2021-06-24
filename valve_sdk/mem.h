#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <wintrust.h>
#include <softpub.h>

#include <Psapi.h>
#include <cstdint>
#include <cstring>
#include <intrin.h>

#include <array>

#include "../hash.h"

#pragma warning( disable : 4172 )  

// generate 'pseudo-random' xor key based on file, date and line.
#define GET_XOR_KEYUI8  ( ( CONST_HASH( __FILE__ __TIMESTAMP__ ) + __LINE__ ) % UINT8_MAX )
#define GET_XOR_KEYUI16 ( ( CONST_HASH( __FILE__ __TIMESTAMP__ ) + __LINE__ ) % UINT16_MAX )
#define GET_XOR_KEYUI32 ( ( CONST_HASH( __FILE__ __TIMESTAMP__ ) + __LINE__ ) % UINT32_MAX )

namespace xor {
	// actual xor implementation.
	template< class t, const size_t len, const t key >
	class Gen {
	private:
		std::array< t, len > m_buffer;

	private:
		// encrypt single character.
		constexpr t enc(const t c) const noexcept {
			return c ^ key;
		}

		// decrypt single character.
		__forceinline t dec(const t c) const noexcept {
			return c ^ key;
		}

	public:
		// iterate each byte and decrypt.
		__forceinline auto data() noexcept {
			for (size_t i{ 0u }; i < len; ++i)
				m_buffer[i] = dec(m_buffer[i]);

			return m_buffer.data();
		}

		// ctor.
		template< size_t... seq >
		constexpr __forceinline Gen(const t(&s)[len], std::index_sequence< seq... >) noexcept : m_buffer{ enc(s[seq])... } {}
	};
}

// todo - dex;  the issue with this code is that the xor key will be static since it's in the actual function call
//              the macro needs to pass a key instead using __COUNTER__ or __LINE__ as the first template arg, the rest will be handled by the compiler (like size).
//              sadly, this increasess compile time by a fuck ton.

template< class t, const size_t len >
constexpr __forceinline auto XorStr(const t(&s)[len]) {
	return xor ::Gen< t, len, GET_XOR_KEYUI8 >(s, std::make_index_sequence< len >()).data();
}

#ifdef _DEBUG
#define XOR( s ) ( s )
#else
#define XOR( s ) ( XorStr( s ) )
#endif


#define stringify(x) #x
#define concat_impl(x, y) x##y
#define concat(x, y) concat_impl(x, y)

template<typename t, t v>
struct constant_holder
{
	enum class val_holder : t
	{
		val = v
	};
};

#define constant(value) ((decltype(value))constant_holder<decltype(value), value>::val_holder::val)

namespace random {
	constexpr auto time = __TIME__;
	constexpr auto seed = static_cast<int>(time[7]) + static_cast<int>(time[6]) * 10 + static_cast<int>(time[4]) * 60 + static_cast<int>(time[3]) * 600 + static_cast<int>(time[1]) * 3600 + static_cast<int>(time[0]) * 36000;

	template <int n>
	struct gen {
	private:
		static constexpr unsigned a = 16807;
		static constexpr unsigned m = 2147483647;

		static constexpr unsigned s = gen<n - 1>::value;
		static constexpr unsigned lo = a * (s & 0xFFFF);
		static constexpr unsigned hi = a * (s >> 16);
		static constexpr unsigned lo2 = lo + ((hi & 0x7FFF) << 16);
		static constexpr unsigned hi2 = hi >> 15;
		static constexpr unsigned lo3 = lo2 + hi;

	public:
		static constexpr unsigned max = m;
		static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
	};

	template <>
	struct gen<0> {
		static constexpr unsigned value = seed;
	};

	template <int n, int m>
	struct _int {
		static constexpr auto value = gen<n + 1>::value % m;
	};

	template <int n>
	struct _char {
		static const char value = static_cast<char>(1 + _int<n, 0x7F - 1>::value);
	};
}

#pragma region oldXorStr
/*template <size_t n, char k>
struct xorstr
{
private:
	static constexpr char enc(const char c)
	{
		return c ^ k;
	}

public:
	template <size_t... s>
	constexpr __forceinline xorstr(const char* str, std::index_sequence<s...>) : encrypted{ enc(str[s])... } { }

	__forceinline std::string dec()
	{
		std::string dec;
		dec.resize(n);

		for (auto i = 0; i < n; i++)
			dec[i] = encrypted[i] ^ k;

		return dec;
	}

	__forceinline std::string ot(bool decrypt = true)
	{
		std::string dec;
		dec.resize(n);

		for (auto i = 0; i < n; i++)
		{
			dec[i] = decrypt ? (encrypted[i] ^ k) : encrypted[i];
			encrypted[i] = '\0';
		}

		return dec;
	}

	std::array<char, n> encrypted{};
};

#define _(s) xorstr<sizeof(s), random::_char<__COUNTER__>::value>(s, std::make_index_sequence<sizeof(s)>()).dec().c_str()
#define _ot(s) xorstr<sizeof(s), random::_char<__COUNTER__>::value>(s, std::make_index_sequence<sizeof(s)>()).ot().c_str()
#define __(s) []() -> std::pair<std::string, char> { \
	constexpr auto key = random::_char<__COUNTER__>::value; \
	return std::make_pair(xorstr<sizeof(s), key>(s, std::make_index_sequence<sizeof(s)>()).ot(false), key); \
}()
#define _rt(n, s) auto (n) = reinterpret_cast<char*>(alloca(((s).first.size() + 1) * sizeof(char))); \
	for (size_t i = 0; i < (s).first.size(); i++) \
        (n)[i] = (s).first[i] ^ (s).second; \
    (n)[(s).first.size()] = '\0'*/
#pragma endregion


constexpr uint32_t seed = 0x45C3370D;
constexpr uint32_t prime = 0x1000193;
__declspec(dllexport) extern uint32_t runtime_basis;

inline uint32_t fnv1a_rt(const char* key) {
	const char* data = const_cast<char*>(key);
	auto hash = runtime_basis;

	//hash ^= __readfsdword(0x20);

	for (size_t i = 0; i < strlen(key); ++i) {
		const uint8_t value = data[i];
		hash = hash ^ value;
		hash *= prime;
	}

	return hash;
}

inline uint32_t fnv1a_fl_rt(const char* key, const uint32_t length) {
	const char* data = const_cast<char*>(key);
	auto hash = runtime_basis;

	//hash ^= __readfsdword(0x20);

	for (size_t i = 0; i < length; ++i) {
		const auto value = static_cast<uint8_t>(data[i]);
		hash = hash ^ value;
		hash *= prime;
	}

	return hash;
}

constexpr uint32_t fnv1a_ct(const char* str, const uint32_t value = seed) noexcept {
	return !*str ? value : fnv1a_ct(str + 1, static_cast<unsigned>(1ull *
		(value ^ static_cast<uint8_t>(*str))* prime));
}

#define fnv1a(s) constant(fnv1a_ct(s))

namespace memory
{
	__forceinline uintptr_t get_module_handle(const uint32_t module, const uint32_t process = 0)
	{
		MODULEENTRY32 entry;
		entry.dwSize = sizeof(MODULEENTRY32);

		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process);

		if (Module32First(snapshot, &entry))
			while (Module32Next(snapshot, &entry))
			{
				std::string name = entry.szModule;
				std::transform(name.begin(), name.end(), name.begin(), ::tolower);

				if (fnv1a_rt(name.c_str()) == module)
				{
					CloseHandle(snapshot);
					return reinterpret_cast<uintptr_t>(entry.hModule);
				}
			}

		CloseHandle(snapshot);
		return 0;
	}

	__forceinline int32_t rva_2_offset(const uint32_t rva, PIMAGE_NT_HEADERS nt_headers, const bool in_memory = false) {
		if (rva == 0 || !in_memory)
			return rva;

		auto sec = IMAGE_FIRST_SECTION(nt_headers);
		for (size_t i = 0; i < nt_headers->FileHeader.NumberOfSections; i++) {
			if (rva >= sec->VirtualAddress && rva < sec->VirtualAddress + sec->Misc.VirtualSize)
				break;
			sec++;
		}

		return rva - sec->VirtualAddress + sec->PointerToRawData;
	}

	__forceinline uintptr_t get_proc_address(const uintptr_t module, const uint32_t function, const bool in_memory = false)
	{
		const auto dos_headers = reinterpret_cast<IMAGE_DOS_HEADER*>(module);
		if (dos_headers->e_magic != IMAGE_DOS_SIGNATURE)
			return 0;

		const auto nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(dos_headers->e_lfanew + module);
		if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
			return 0;

		const auto exports = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(
			rva_2_offset(nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,
				nt_headers, in_memory) + module);

		const auto names = reinterpret_cast<uint32_t*>(rva_2_offset(exports->AddressOfNames, nt_headers, in_memory) + module);

		auto ordinal_index = static_cast<uint32_t>(-1);
		for (uint32_t i = 0; i < exports->NumberOfFunctions; i++)
		{
			const auto function_name = reinterpret_cast<const char*>(rva_2_offset(names[i], nt_headers, in_memory) + module);

			if (fnv1a_rt(function_name) == function)
			{
				ordinal_index = i;
				break;
			}
		}

		if (ordinal_index > exports->NumberOfFunctions)
			return 0;

		const auto ordinals = reinterpret_cast<uint16_t*>(rva_2_offset(exports->AddressOfNameOrdinals, nt_headers, in_memory) + module);
		const auto addresses = reinterpret_cast<uint32_t*>(rva_2_offset(exports->AddressOfFunctions, nt_headers, in_memory) + module);
		return rva_2_offset(addresses[ordinals[ordinal_index]], nt_headers, in_memory) + module;
	}

	__forceinline uint8_t* find_sig(const uint32_t offset, const char* signature, const uint32_t range = 0u)
	{
		static auto pattern_to_bytes = [](const char* pattern) -> std::vector<int> {
			auto bytes = std::vector<int32_t>{ };
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					current++;

					if (*current == '?')
						current++;

					bytes.push_back(-1);
				}
				else
					bytes.push_back(static_cast<int32_t>(strtoul(current, &current, 0x10)));
			}

			return bytes;
		};

		const auto scan_bytes = reinterpret_cast<std::uint8_t*>(offset);
		auto pattern_bytes = pattern_to_bytes(signature);
		const auto s = pattern_bytes.size();
		const auto d = pattern_bytes.data();

		for (auto i = 0ul; i < range - s; ++i) {
			auto found = true;

			for (auto j = 0ul; j < s; ++j)
				if (scan_bytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}

			if (found)
				return &scan_bytes[i];
		}

		return nullptr;
	}

	__forceinline uint8_t* find_module_sig(const uint32_t name, const char* signature)
	{
		const auto module = get_module_handle(name);

		if (module) {
			const auto dos_header = PIMAGE_DOS_HEADER(module);
			const auto nt_headers = PIMAGE_NT_HEADERS(reinterpret_cast<std::uint8_t*>(module) + dos_header->e_lfanew);

			return find_sig(module, signature, nt_headers->OptionalHeader.SizeOfImage);
		}

		return nullptr;
	}

	template <uint32_t module, uint32_t function>
	__forceinline static void* import()
	{
		static void* fn;

		if (!fn)
			fn = reinterpret_cast<void*>(get_proc_address(get_module_handle(module), function));

		return fn;
	}

}
