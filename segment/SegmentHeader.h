#pragma once

#include "../../framework.h"

class SegmentHeader {

public:

    //Simple DllMain function. (Used for call OEP)
    //Docs: https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain
    typedef int (__stdcall* OEP_FUNCTION) (HMODULE hModule, DWORD callReason, LPVOID lpReserved);
    
    //Simple variables.
    enum Links {
        //DOCS: https://www.jstage.jst.go.jp/article/transinf/E98.D/4/E98.D_2014EDP7268/_pdf
        OEP = 0x4738C,
        RUNTIME = 0x3D600000,
        ALLOCATION = 0x968940,
        SIZE = 0x186A00
    };

    enum ImportType {

        /**
              What is intrnal ? Let's look at a simple function.

                 sub01:

                     int myImportSuperValue = (0x313131313) (mySuperValue);

              This function receives the value from the import (some dll) and writes it to a variable. This is the Internal.
         **/

        INTERNAL,

        //What is public? This is a global variable that points to function import. I don’t even know what to add. That's all :d
        PUBLIC
    };

    struct ImportInfo {
        //Import function name.
        const char* function;
        //Location in segment where need patch. (RVA only)
        std::multimap<ImportType, DWORD> offsetsMap;
    };
    
    SegmentHeader () {
        FillImports ();
        FillRelocations ();
    }

    std::map<const char*, std::vector<ImportInfo>> GetImports() {
        return m_imports;
    }

    std::vector<DWORD> GetRelocations() {
        return m_relocations;
    }

private:

    //Need for segment can call the necessary functions from libraries. Storage model: Module -> ImportInfo [Function, offsets ^ n].
    std::map<const char*, std::vector<ImportInfo>> m_imports;
    //Need for reconstruct own variables/methods call.
    std::vector<DWORD> m_relocations;

    //Simple (no), functions for fill vectors.

    void FillImports ();
    void FillRelocations ();

};