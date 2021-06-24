#pragma once

#include "../../framework.h"

class SegmentFramework {

public:

   //Legacy segment function.
   typedef int (__fastcall* VirtualFunctionCaller) (int* vTable, int index);

   //A variable that indicates the original function from the hook.
   static VirtualFunctionCaller OriginalVirtualFunctionCaller;

   /**
    * Used to initialize internal values ​​in segment. (About values: SegmentUtils.cpp)
    *
    * @param nick - Player nickname in watermark (!32 character limit!)
   **/

   void CreateInfoTable ();

   /**
    * It is necessary for the internal functions to work correctly. (More about netvars can be found on unknowncheats)
    **/

   void UpdateNetVars ();

   /**
    * Intercept legacy function from the segment and replace it with your own. (About hook/lib: https://www.github.com/HoShiMin/HookLib)
    **/

   void CreateHook ();

   /**
    * It is necessary for the internal function work correctly.
    *
    * !Used to repair indexes that are broken. About vfunction: SegmentFramework.cpp#71
    **/

   static int __fastcall CustomVirtualCaller (int* vTable, int index);

protected:

    //Netvars are offsets to parent variables in valve sdk.
    struct LegacyNetVar {
        //Offset to variable. (Relocation - Old Base Address)
        int rva;
        //Set value.
        int new_value;
    };

    //Small variables.
    enum Links {
        //Unknown offset. Used for create info table.
        LIBRARY = 0x2A6900,
        //Info-table in memory size. (Value taken from segment-side function)
        TABLE = 0x18C,
        //Offsets to hook function in memory.
        HOOK = 0x3B30
    };
    
    //Used for fill info table. (More about this libs: SegmentFramework.cpp)
    std::vector<const char*> m_libraries = {
      "client_panorama.dll",
      "vguimatsurface.dll",
      "shaderapidx9.dll",
      "vstdlib.dll"
    };

    //Used for fix segment. (More about netvars: SegmentFramework.cpp)
    std::vector<LegacyNetVar> m_netvars = {
        //m_bIsScoped. (Actual value: https://github.com/frk1/hazedumper/blob/master/csgo.cs#L32)
        LegacyNetVar { 0x95A250, 0x3914 }
    };

    //Used for find offsets. (More about offsets: SegmentFramework.cpp)
    std::vector<const char*> m_signatures = {
       "55 8B EC 83 E4 F8 83 EC 70 6A 58", "56 6A 01 68 ? ? ? ? 8B F1", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D",
       "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1", "51 56 8B F1 85 F6 74 68 83", "55 8B EC 53 8B 5D 08 56 8B F1 83",
       "55 8B EC 83 E4 C0 83 EC 34 53 56 8B 75", "8B 0D ? ? ? ? 8B 46 08 68", "55 8B EC 56 8B F1 51 8D",
       "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C", "55 8B EC FF 75 18 F3 0F 10 45 ? 51",
       "55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74", "8B 35 ? ? ? ? FF 90 ? ? ? ? 50 B9 ? ? ? ? FF 56 24 5E C3",
       "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9", "0F 44 C8 33 C0 5E 39 11 0F 94 C0 C3", "B0 01 83 FE",
       "B9 ? ? ? ? FF 50 14 5F", "81 C6 ? ? ? ? 8B 4E 0C 8B 56 04 89 4D E4", "55 8B EC 51 56 8B 35 ? ? ? ? 57 83",
       "C7 00 ? ? ? ? E8 ? ? ? ? 83 EC 08 8D 4E 74", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F", "80 B9 ? ? ? ? ? 75 6C",
       "83 3D ? ? ? ? ? 57 8B F9 75 06", "A1 ? ? ? ? 74 38", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39",
       "55 8B EC 56 8B 75 0C 57 8B F9 B9 ? ? ? ? 89", "B9 ? ? ? ? A3 ? ? ? ? E8 ? ? ? ? 5D", "55 8B EC 83 EC 0C 53 56 8B 71",
       "B9 ? ? ? ? E8 ? ? ? ? 85 C0 74 0A 8B 10 8B C8 FF A2 AC", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B",
       "0F 84 ? ? ? ? 8B 88 ? ? ? ? 83 F9 FF 0F 84 ? ? ? ? 0F B7 C1 C1 E0 04 05 ? ? ? ? C1 E9 10 39 48 04 0F 85 ? ? ? ? 8B", "84 C0 74 17 8B 87",
       "84 C0 74 0A F3 0F 10 05 ? ? ? ? EB 05", "8B 97 ? ? ? ? 56 8B",  "55 8B EC 83 E4 F0 83 EC 7C 56 FF",
       "55 8B EC 8D 51 08 8B 4D 08", "55 8B EC 56 8B 35 ? ? ? ? 85 F6 0F 84 ? ? ? ? 81",
       "8B 01 FF B1 ? ? ? ? FF 90 ? ? ? ? 83", "55 8B EC 83 EC 58 56 57", "53 56 8B F1 57 8B 4E 3C",
       "55 8B EC 56 8B 75 08 57 FF 75 18", "55 8B EC 83 E4 F8 83 EC 0C 53 56 57 8B 7D 08 8B F1 F3",
       "55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3", "55 8B EC 83 E4 F8 83 EC 1C 53 56 57 8B F9 F7",
       "F3 0F 10 A6 ? ? ? ? F3 0F 11", "56 8B F1 8B 0D ? ? ? ? 57 8B 01 FF 76 70",
       "55 8B EC 83 EC 10 A1 ? ? ? ? 89 55", "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02",
       "55 8B EC 83 E4 F8 83 EC 60 56 57 8B F9 89 7C 24", "55 8B EC 83 E4 F8 83 EC 30 56 57 8B 3D",
       "55 8B EC 56 FF 75 08 8B F1 8B 06 FF 90 ? ? ? ? 8B 86", "85 C0 75 30 38 86",
       "C7 46 ? ? ? ? ? 43",  "85 C0 74 2D 83 7D", "75 04 B0 01 5F", "84 C0 75 0D F6 87",
       "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80", "84 C0 75 09 5F 5E 5B 8B E5 5D C2 14",
       "55 8B EC 83 E4 F8 83 EC 7C 53 56 57 8B D9", "F3 0F 10 4C 24 ? 84 C0 74 12", "FF D0 A1 ? ? ? ? B9", "E8 ? ? ? ? FF 76 0C 8D",
       "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81", "8B 35 ? ? ? ? FF 10 0F B7 C0 B9 ? ? ? ? 50 FF 56 08 85",
       "83 BE ? ? ? ? ? 7F 67", "E8 ? ? ? ? EB 02 33 C0 57 8B BE", "E8 ? ? ? ? 8B D0 85 D2 75 07 32 C0 5F",
       "E8 ? ? ? ? 8B 46 04 5F 5E 5B 8B E5 5D C2 08 00 6A", "75 4B 0F 57", "0F B7 05 ? ? ? ? 3D ? ? ? ? 74 3F", "56 8D 51 3C", "E8 ? ? ? ? 83 7D D8 00 7C 0F",
       "55 8B EC 83 E4 F8 81 EC ? ? ? ? A1 ? ? ? ? 53 56 8B D9", "E8 ? ? ? ? 99 2B", "E8 ? ? ? ? 8B 0D ? ? ? ? 0F 57 C9 99",
       "8D 4E 74 8B", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F", "E8 ? ? ? ? 99 2B", "E8 ? ? ? ? 8B 0D ? ? ? ? 0F 57 C9 99",
       "8B 0D ? ? ? ? F6 87 ? ? ? ? ? 8B", "55 8B EC F3 0F 10 45 ? 56 6A", "55 8B EC 83 E4 F8 8B 45 0C 83 EC 10",
       "FF 50 18 89 44 24 14 EB", "A2 ? ? ? ? 8B 45 E8", "55 8B EC 83 E4 F8 51 53 56 8B D9", "55 8B EC 83 EC 48 53 8B 5D"
    };

};