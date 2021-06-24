#pragma once

#include "../../framework.h"

class Utils {

public:

    /**
     * Get module base address from internal process.
     *
     * @param module - Module name.
     **/

    static HMODULE GetModule (const char* module);

    /**
     * Get module info from internal process.
     * 
     * @param module - Module name.
     **/

    static MODULEINFO GetModuleInfo (const char* module);

    /**
     * Get function address from module.
     *
     * @param module - Module name.
     * @param function - Function name.
     **/

    static DWORD GetFunction (const char* module, const char* function);

    /**
     * Find offsets and fill to vector.
     *
     * @param module - Module name.
     * @param signatures - Signatures vector.
     * @param vector - Fill vector.
     * @param isPanic - Call panic when offset not found.
     **/

    static void FindOffsetsToVec (const char* module, std::vector<const char*> signatures, std::vector<DWORD>& vector, bool isPanic = false);

};