#pragma once

#include "../../framework.h"
#include "../segment/SegmentHeader.h"
#include "../frameworks/SegmentFramework.h"
#include "../utils/PanicUtils.h"

//Just a global variable that points to a segment.
extern const unsigned char g_SegmentData[];

class Segment {

public:

	//#SafeRuntime.
	static DWORD UnsafeAllocatedPointer;
	static DWORD UnsafeLibraryPointer;

	/**
	 * Get allocation pointer. (Throws "exception" if pointer is null)
	 **/

	static DWORD GetSafeAllocationPointer() {
		PanicUtils::RequireNonNull (PanicUtils::Layers::ROUTINE, UnsafeAllocatedPointer, "Allocation pointer is null.");
		return UnsafeAllocatedPointer;
	}

	/**
	 * Get current module pointer. (Throws "exception" if pointer is null)
	 **/

	static HMODULE GetSafeLibraryPointer() {
		PanicUtils::RequireNonNull(PanicUtils::Layers::ROUTINE, UnsafeLibraryPointer, "Library pointer is null.");
		return reinterpret_cast<HMODULE> (UnsafeLibraryPointer);
	}

	//Getters.

	SegmentHeader GetHeader() {
		return m_Header;
	}

	SegmentFramework GetFramework() {
		return m_Framework;
    }

private:

	SegmentHeader m_Header;
	SegmentFramework m_Framework;

};