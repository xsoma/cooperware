#pragma once



class IVModelInfoClient
{
public:
	inline void* GetModel(int Index)
	{
		return CallVFunction<void* (__thiscall*)(void*, int)>(this, 1)(this, Index);
	}
	int	GetModelIndex(const char* name)
	{
		typedef int(__thiscall* oGetModelName)(PVOID, const char*);
		return CallVFunction< oGetModelName >(this, 2)(this, name);
	}
	inline const char* GetModelName(const void* Model) {
		return CallVFunction<const char* (__thiscall*)(void*, const void*)>(this, 3)(this, Model);
	}

	void GetModelMaterials(const model_t* model, int count, IMaterial** ppMaterial)
	{
		typedef void* (__thiscall* oGetModelMaterials)(PVOID, const model_t*, int, IMaterial**);
		CallVFunction<oGetModelMaterials>(this, 19)(this, model, count, ppMaterial);
	}

	studiohdr_t* GetStudiomodel(const model_t* mod)
	{
		typedef studiohdr_t* (__stdcall* oGetStudiomodel)(const model_t*);
		return CallVFunction<oGetStudiomodel>(this, 32)(mod);
	}
};

