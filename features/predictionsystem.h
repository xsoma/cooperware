#pragma once
#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../singleton.hpp"
#include "../helpers/input.hpp"

#include <deque>





namespace Prediction
{
	 void Init();
	void StartPrediction(CUserCmd* pCmd, C_BasePlayer* entity);
	void Predict();
	void EndPrediction(C_BasePlayer* entity);
}
