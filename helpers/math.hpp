#pragma once

#include "../valve_sdk/sdk.hpp"

#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define M_PI 3.14159265358979323846
#define PI_F	((float)(M_PI)) 
#define TICK_INTERVAL			(g_pGlobalVars->interval_per_tick)
#define TICKS_TO_TIME(t) ( g_pGlobalVars->interval_per_tick * (t) )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#include <vector>
namespace Math
{
	template < typename t >
	static t lerp(const t& t1, const t& t2, float progress)
	{
		return t1 + (t2 - t1) * progress;
	}
	inline float FASTSQRT(float x)
	{
		unsigned int i = *(unsigned int*)&x;

		i += 127 << 23;
		// approximation of square root
		i >>= 1;
		return *(float*)&i;
	}


	
	float VectorDistance(const Vector& v1, const Vector& v2);
	Vector CalcAngle(const Vector& src, const Vector& dst);
	float GetFOV(const Vector& viewAngle, const Vector& aimAngle);
	bool IsOnScreen(Vector origin, Vector& screen);
	void angle_matrix(const Vector& angles, const Vector& position, matrix3x4_t& matrix);
	extern   matrix3x4_t angle_matrix(const Vector angle, const Vector pos);
	template<class T>
	void Normalize3(T& angles)
	{
		for (auto i = 0; i < 3; i++) {
			while (angles[i] < -180.0f) angles[i] += 360.0f;
			while (angles[i] > 180.0f) angles[i] -= 360.0f;
		}
	}
	template<class T, class U>
	T clamp(T& angles)
	{
		T a = angles;
		normalize_angles(a);
		clamp_ang(a);

		if (isnan(a.x) || isinf(a.x) ||
			isnan(a.y) || isinf(a.y) ||
			isnan(a.z) || isinf(a.z)) {
			return false;
		}
		else {
			angles = a;
			return true;
		}
	}
	void ClampAngles(Vector& angles);
	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	void AngleVectors(const Vector& angles, Vector& forward);
	void AngleVectors(const Vector& angles, Vector& forward, Vector& right, Vector& up);
	void VectorAngles(const Vector& forward, Vector& angles);
	bool WorldToScreen(const Vector& in, Vector& out);
	bool SanitizeAngles(Vector& angles);
	void SetViewAngles(Vector angles);
	void CalcAngle(Vector src, Vector dst, Vector& angles);
	float RandomFloat(float min, float max);
	float Distance(Vector2D point1, Vector2D point2);
	float DistanceToRay(const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along = nullptr, Vector* pointOnRay = nullptr);
	float GetFoV(Vector qAngles, Vector vecSource, Vector vecDestination, bool bDistanceBased);
	float NormalizeYaw(float value);
	float GRD_TO_BOG(float GRD);
	float DistancePointToLine(Vector Point, Vector LineOrigin, Vector Dir);
	
}