#pragma once
#include "../valve_sdk/interfaces/IGameEventmanager.hpp"
#include <vector>


#include "../singleton.hpp"
class BulletBeamsEvent : public IGameEventListener2, public Singleton<BulletBeamsEvent>
{
public:

	void FireGameEvent(IGameEvent* event);
	int  GetEventDebugID(void);

	void RegisterSelf();
	void UnregisterSelf();

	void Paint(void);

private:



};