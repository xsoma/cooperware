#include "Math.hpp"
#include "utils.hpp"
#include "../options.hpp"


#include <cmath>
#include <xmmintrin.h>
#include <pmmintrin.h>

static const float invtwopi = 0.1591549f;
static const float twopi = 6.283185f;
static const float threehalfpi = 4.7123889f;
static const float pi = 3.141593f;
static const float halfpi = 1.570796f;
static const __m128 signmask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));

static const __declspec(align(16)) float null[4] = { 0.f, 0.f, 0.f, 0.f };
static const __declspec(align(16)) float _pi2[4] = { 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f };
static const __declspec(align(16)) float _pi[4] = { 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f };

typedef __declspec(align(16)) union
{
	float f[4];
	__m128 v;
} m128;

__forceinline __m128 sqrt_ps(const __m128 squared)
{
	return _mm_sqrt_ps(squared);
}

__forceinline __m128 cos_52s_ps(const __m128 x)
{
	const auto c1 = _mm_set1_ps(0.9999932946f);
	const auto c2 = _mm_set1_ps(-0.4999124376f);
	const auto c3 = _mm_set1_ps(0.0414877472f);
	const auto c4 = _mm_set1_ps(-0.0012712095f);
	const auto x2 = _mm_mul_ps(x, x);
	return _mm_add_ps(c1, _mm_mul_ps(x2, _mm_add_ps(c2, _mm_mul_ps(x2, _mm_add_ps(c3, _mm_mul_ps(c4, x2))))));
}

__forceinline __m128 cos_ps(__m128 angle)
{
	angle = _mm_andnot_ps(signmask, angle);
	angle = _mm_sub_ps(angle, _mm_mul_ps(_mm_cvtepi32_ps(_mm_cvttps_epi32(_mm_mul_ps(angle, _mm_set1_ps(invtwopi)))), _mm_set1_ps(twopi)));

	auto cosangle = angle;
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(pi), angle))));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(pi)), signmask));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(threehalfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(twopi), angle))));

	auto result = cos_52s_ps(cosangle);
	result = _mm_xor_ps(result, _mm_and_ps(_mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_cmplt_ps(angle, _mm_set1_ps(threehalfpi))), signmask));
	return result;
}

__forceinline __m128 sin_ps(const __m128 angle)
{
	return cos_ps(_mm_sub_ps(_mm_set1_ps(halfpi), angle));
}

__forceinline void sincos_ps(__m128 angle, __m128* sin, __m128* cos) {
	const auto anglesign = _mm_or_ps(_mm_set1_ps(1.f), _mm_and_ps(signmask, angle));
	angle = _mm_andnot_ps(signmask, angle);
	angle = _mm_sub_ps(angle, _mm_mul_ps(_mm_cvtepi32_ps(_mm_cvttps_epi32(_mm_mul_ps(angle, _mm_set1_ps(invtwopi)))), _mm_set1_ps(twopi)));

	auto cosangle = angle;
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(pi), angle))));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(pi)), signmask));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(threehalfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(twopi), angle))));

	auto result = cos_52s_ps(cosangle);
	result = _mm_xor_ps(result, _mm_and_ps(_mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_cmplt_ps(angle, _mm_set1_ps(threehalfpi))), signmask));
	*cos = result;

	const auto sinmultiplier = _mm_mul_ps(anglesign, _mm_or_ps(_mm_set1_ps(1.f), _mm_and_ps(_mm_cmpgt_ps(angle, _mm_set1_ps(pi)), signmask)));
	*sin = _mm_mul_ps(sinmultiplier, sqrt_ps(_mm_sub_ps(_mm_set1_ps(1.f), _mm_mul_ps(result, result))));
}

namespace Math
{

#define QUICK_MATH
	float fast_sin(float x) 
	{
		x *= float(0.159155);
		x -= floor(x);
		const auto xx = x * x;
		auto y = -6.87897;
		y = y * xx + 33.7755;
		y = y * xx - 72.5257;
		y = y * xx + 80.5874;
		y = y * xx - 41.2408;
		y = y * xx + 6.28077;
		return float(x * y);
	}

	inline float fast_cos(const float x) 
	{
		return fast_sin(x + 1.5708f);
	}
	inline void sin_cos(const float radians, float* sine, float* cosine) 
	{
#ifdef QUICK_MATH
		* sine = fast_sin(radians);
		*cosine = fast_cos(radians);
#else
		* sine = sin(radians);
		*cosine = cos(radians);
#endif
	}

	void angle_matrix(const Vector& angles, matrix3x4_t& matrix)
	{
		float sr, sp, sy, cr, cp, cy;

		sin_cos(DEG2RAD(angles.x), &sp, &cp);
		sin_cos(DEG2RAD(angles.y), &sy, &cy);
		sin_cos(DEG2RAD(angles.z), &sr, &cr);

		// matrix = (YAW * PITCH) * ROLL
		matrix[0][0] = cp * cy;
		matrix[1][0] = cp * sy;
		matrix[2][0] = -sp;

		const auto crcy = cr * cy;
		const auto crsy = cr * sy;
		const auto srcy = sr * cy;
		const auto srsy = sr * sy;
		matrix[0][1] = sp * srcy - crsy;
		matrix[1][1] = sp * srsy + crcy;
		matrix[2][1] = sr * cp;

		matrix[0][2] = (sp * crcy + srsy);
		matrix[1][2] = (sp * crsy - srcy);
		matrix[2][2] = cr * cp;

		matrix[0][3] = 0.0f;
		matrix[1][3] = 0.0f;
		matrix[2][3] = 0.0f;
	}
	void matrix_set_column(const Vector& in, int column, matrix3x4_t& out)
	{
		out[0][column] = in.x;
		out[1][column] = in.y;
		out[2][column] = in.z;
	}

	void angle_matrix(const Vector& angles, const Vector& position, matrix3x4_t& matrix) 
	{
		angle_matrix(angles, matrix);
		matrix_set_column(position, 3, matrix);
	}
	__forceinline  matrix3x4_t angle_matrix(const Vector angles)
	{
		matrix3x4_t result{};

		m128 angle, sin, cos;
		angle.f[0] = DEG2RAD(angles.x);
		angle.f[1] = DEG2RAD(angles.y);
		angle.f[2] = DEG2RAD(angles.z);
		sincos_ps(angle.v, &sin.v, &cos.v);

		result[0][0] = cos.f[0] * cos.f[1];
		result[1][0] = cos.f[0] * sin.f[1];
		result[2][0] = -sin.f[0];

		const auto crcy = cos.f[2] * cos.f[1];
		const auto crsy = cos.f[2] * sin.f[1];
		const auto srcy = sin.f[2] * cos.f[1];
		const auto srsy = sin.f[2] * sin.f[1];

		result[0][1] = sin.f[0] * srcy - crsy;
		result[1][1] = sin.f[0] * srsy + crcy;
		result[2][1] = sin.f[2] * cos.f[0];

		result[0][2] = sin.f[0] * crcy + srsy;
		result[1][2] = sin.f[0] * crsy - srcy;
		result[2][2] = cos.f[2] * cos.f[0];

		return result;
	}

	  matrix3x4_t angle_matrix(const Vector angle, const Vector pos)
	{
		auto result = angle_matrix(angle);
		result[0][3] = pos.x;
		result[1][3] = pos.y;
		result[2][3] = pos.z;
		return result;
	}



	//--------------------------------------------------------------------------------
	float VectorDistance(const Vector& v1, const Vector& v2)
	{
		return FASTSQRT(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
	}
	//--------------------------------------------------------------------------------
	Vector CalcAngle(const Vector& src, const Vector& dst)
	{
		Vector vAngle;
		Vector delta((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));
		double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

		vAngle.x = float(atanf(float(delta.z / hyp)) * 57.295779513082f);
		vAngle.y = float(atanf(float(delta.y / delta.x)) * 57.295779513082f);
		vAngle.z = 0.0f;

		if (delta.x >= 0.0)
			vAngle.y += 180.0f;

		return vAngle;
	}
	//--------------------------------------------------------------------------------
	float GetFOV(const Vector& viewAngle, const Vector& aimAngle)
	{
		Vector ang, aim;

		AngleVectors(viewAngle, aim);
		AngleVectors(aimAngle, ang);

		return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
	}
	//--------------------------------------------------------------------------------
	bool IsOnScreen(Vector origin, Vector& screen)
	{
		if (!WorldToScreen(origin, screen)) return false;
		int iScreenWidth, iScreenHeight;
		g_EngineClient->GetScreenSize(iScreenWidth, iScreenHeight);
		bool xOk = iScreenWidth > screen.x&& screen.x > 0, yOk = iScreenHeight > screen.y&& screen.y > 0;
		return xOk && yOk;
	}
	//--------------------------------------------------------------------------------
	void ClampAngles(Vector& angles)
	{
		if (angles.y > 180.0f)
			angles.y = 180.0f;
		else if (angles.y < -180.0f)
			angles.y = -180.0f;

		if (angles.x > 89.0f)
			angles.x = 89.0f;
		else if (angles.x < -89.0f)
			angles.x = -89.0f;

		angles.z = 0;
	}
	//--------------------------------------------------------------------------------
	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		out[0] = in1.Dot(in2[0]) + in2[0][3];
		out[1] = in1.Dot(in2[1]) + in2[1][3];
		out[2] = in1.Dot(in2[2]) + in2[2][3];
	}
	//--------------------------------------------------------------------------------
	void AngleVectors(const Vector& angles, Vector& forward)
	{
		float	sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
		DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}
	//--------------------------------------------------------------------------------
	void AngleVectors(const Vector& angles, Vector& forward, Vector& right, Vector& up)
	{
		float sr, sp, sy, cr, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
		DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));
		DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angles[2]));

		forward.x = (cp * cy);
		forward.y = (cp * sy);
		forward.z = (-sp);
		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr * cy);
		right.z = (-1 * sr * cp);
		up.x = (cr * sp * cy + -sr * -sy);
		up.y = (cr * sp * sy + -sr * cy);
		up.z = (cr * cp);
	}
	//--------------------------------------------------------------------------------
	void VectorAngles(const Vector& forward, Vector& angles)
	{
		float	tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0) {
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else {
			yaw = (atan2(forward[1], forward[0]) * 180 / DirectX::XM_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / DirectX::XM_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}
	//--------------------------------------------------------------------------------
	static bool screen_transform(const Vector& in, Vector& out)
	{
		static DWORD dwViewMatrix = (DWORD)Utils::PatternScan(GetModuleHandleW(L"client.dll"), "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9");
		const VMatrix& w2sMatrix = *(VMatrix*)(*(DWORD*)(dwViewMatrix + 3) + 0xB0);

		out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
		out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
		out.z = 0.0f;

		float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

		if (w < 0.001f) {
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		out.x /= w;
		out.y /= w;

		return true;
	}
	//--------------------------------------------------------------------------------
	bool WorldToScreen(const Vector& in, Vector& out)
	{
		if (screen_transform(in, out)) {
			int w, h;
			g_EngineClient->GetScreenSize(w, h);

			out.x = (w / 2.0f) + (out.x * w) / 2.0f;
			out.y = (h / 2.0f) - (out.y * h) / 2.0f;

			return true;
		}
		return false;
	}
	//--------------------------------------------------------------------------------
	bool SanitizeAngles(Vector& angles)
	{
		angles.x = fmaxf(fminf(angles.x, 89.f), -89.f);
		while (angles.y > 180.f) angles.y -= 360.f;
		while (angles.y < -180.f) angles.y += 360.f;
		angles.z = 0.f;

		if (!isfinite(angles.x) || !isfinite(angles.y) || !isfinite(angles.z))
		{
			//printf("NAN OR INF ANGLES!\n");
			return false;
		}

		if (angles.x > 89.f || angles.x < -89.f || angles.y > 180.f || angles.y < -180.f || angles.z != 0.f)
		{
			//printf("ALL CHECKS FAILED SITUATION FUBAR: %f | %f | %f\n", angles.x, angles.y, angles.z);
			return false;
		}

		return true;
	}
	//--------------------------------------------------------------------------------
	void SetViewAngles(Vector angles)
	{
		if (SanitizeAngles(angles))
			g_EngineClient->SetViewAngles(angles);
	}
	//--------------------------------------------------------------------------------
	void CalcAngle(Vector src, Vector dst, Vector& angles)
	{
		Vector delta = src - dst;
		double hyp = delta.Length2D();
		angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
		angles.x = (atan(delta.z / hyp) * 57.295779513082f);
		angles[2] = 0.00;

		if (delta.x >= 0.0)
			angles.y += 180.0f;
	}
	//--------------------------------------------------------------------------------
	float RandomFloat(float min, float max)
	{
		static auto ranFloat = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
		if (ranFloat)
			return ranFloat(min, max);
		else
			return 0.f;
	}
	float DistancePointToLine(Vector Point, Vector LineOrigin, Vector Dir)
	{
		auto PointDir = Point - LineOrigin;

		auto TempOffset = PointDir.Dot(Dir) / (Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);
		if (TempOffset < 0.000001f)
			return FLT_MAX;

		auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

		return (Point - PerpendicularPoint).Length();
	}
	float Distance(Vector2D point1, Vector2D point2)
	{
		float diffY = point1.y - point2.y;
		float diffX = point1.x - point2.x;
		return sqrt((diffY * diffY) + (diffX * diffX));
	}
	float NormalizeYaw(float value)
	{
		while (value > 180)
			value -= 360.f;

		while (value < -180)
			value += 360.f;
		return value;
	}
	float GetFoV(Vector qAngles, Vector vecSource, Vector vecDestination, bool bDistanceBased)
	{
		auto MakeVector = [](Vector qAngles)
		{
			auto ret = Vector();
			auto pitch = float(qAngles[0] * M_PI / 180.f);
			auto yaw = float(qAngles[1] * M_PI / 180.f);
			auto tmp = float(cos(pitch));
			ret.x = float(-tmp * -cos(yaw));
			ret.y = float(sin(yaw) * tmp);
			ret.z = float(-sin(pitch));
			return ret;
		};

		Vector ang, aim;
		double fov;

		ang = CalcAngle(vecSource, vecDestination);
		aim = MakeVector(qAngles);
		ang = MakeVector(ang);

		auto mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
		auto mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
		auto u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];

		fov = acos(u_dot_v / (mag_s * mag_d)) * (180.f / M_PI);

		if (bDistanceBased) {
			fov *= 1.4;
			float xDist = abs(vecSource[0] - vecDestination[0]);
			float yDist = abs(vecSource[1] - vecDestination[1]);
			float Distance = sqrt((xDist * xDist) + (yDist * yDist));

			Distance /= 650.f;

			if (Distance < 0.7f)
				Distance = 0.7f;

			if (Distance > 6.5)
				Distance = 6.5;

			fov *= Distance;
		}

		return (float)fov;
	}
	
}



















































