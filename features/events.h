#pragma once
#include "../valve_sdk/interfaces/IGameEventmanager.hpp"
#include "../Singleton.hpp"
#include "../valve_sdk/math/Vector.hpp"
#include "../valve_sdk/misc/Color.hpp"
#include <string>
#include <vector>
#include <cmath>

struct HitMarkerInfo
{
	float m_flExpTime;
	int m_iDmg;
};



class cGameEvent : public IGameEventListener2
{


public:

	void FireGameEvent(IGameEvent* event);
	int GetEventDebugID();
	void RegisterSelf();
	void Register();
private:

};

extern cGameEvent g_Event;







float GetCurtime325325();

class Color2 {
public:




	float r, g, b, a;

	Color2() {
		r = 0.f, g = 0.f, b = 0.f, a = 255.f;
	}

	Color2(float red, float green, float blue, float alpha) {
		r = red, g = green, b = blue, a = alpha;
	}

	Color2(float red, float green, float blue) {
		r = red, g = green, b = blue, a = 255.f;
	}

	constexpr Color2& FromHSV(float h, float s, float v)
	{
		float colOut[3]{ };
		if (s == 0.0f)
		{
			r = g = b = static_cast<int>(v * 255);
			return *this;
		}

		h = std::fmodf(h, 1.0f) / (60.0f / 360.0f);
		int   i = static_cast<int>(h);
		float f = h - static_cast<float>(i);
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i)
		{
		case 0:
			colOut[0] = v;
			colOut[1] = t;
			colOut[2] = p;
			break;
		case 1:
			colOut[0] = q;
			colOut[1] = v;
			colOut[2] = p;
			break;
		case 2:
			colOut[0] = p;
			colOut[1] = v;
			colOut[2] = t;
			break;
		case 3:
			colOut[0] = p;
			colOut[1] = q;
			colOut[2] = v;
			break;
		case 4:
			colOut[0] = t;
			colOut[1] = p;
			colOut[2] = v;
			break;
		case 5: default:
			colOut[0] = v;
			colOut[1] = p;
			colOut[2] = q;
			break;
		}

		r = static_cast<int>(colOut[0] * 255);
		g = static_cast<int>(colOut[1] * 255);
		b = static_cast<int>(colOut[2] * 255);
		return *this;
	}

	constexpr auto ToHSV(float& h, float& s, float& v)
	{
		float col[3] = { r / 255.f, g / 255.f, b / 255.f };

		float K = 0.f;
		if (col[1] < col[2])
		{
			swap(col[1], col[2]);
			K = -1.f;
		}
		if (col[0] < col[1])
		{
			swap(col[0], col[1]);
			K = -2.f / 6.f - K;
		}

		const float chroma = col[0] - (col[1] < col[2] ? col[1] : col[2]);
		h = colfabs(K + (col[1] - col[2]) / (6.f * chroma + 1e-20f));
		s = chroma / (col[0] + 1e-20f);
		v = col[1];
	}
	static Color2 FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color2(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return Color2(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return Color2(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return Color2(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return Color2(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return Color2(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}
private:
	constexpr void  swap(float& a, float& b) { float tmp = a; a = b; b = tmp; }
	constexpr float colfabs(const float& x) { return x < 0 ? x * -1 : x; }

};


class ColorLine
{
public:
	ColorLine() {}

	ColorLine(std::string text, Color2 color = Color2(255, 255, 255, 255))
	{
		texts.push_back(text);
		colors.push_back(color);
	}

	void PushBack(std::string text, Color2 color = Color2(255, 255, 255, 255))
	{
		texts.push_back(text);
		colors.push_back(color);
	}

	void PushFront(std::string text, Color2 color = Color2(255, 255, 255, 255))
	{
		texts.insert(texts.begin(), text);
		colors.insert(colors.begin(), color);
	}

	void ChangeAlpha(int alpha)
	{
		for (auto& color : colors)
			color.a = alpha;
	}

	void Draw(int x, int y, unsigned int font);

	std::string Text()
	{
		std::string text;
		for (const auto string : texts)
			text += string;

		return text;
	}

private:
	std::vector<std::string> texts;
	std::vector<Color2> colors;
};

class InGameLogger
{
public:
	struct Log
	{
		Log()
		{
			time = GetCurtime325325();
		}

		Log(std::string text, Color2 color = Color2(255, 255, 255, 255))
		{
			color_line = ColorLine(text, color);
			time = GetCurtime325325();
		}

		ColorLine color_line;
		float time;
	};

public:
	void Do();

	void AddLog(Log log)
	{
		log_queue.insert(log_queue.begin(), log);
	}

private:
	std::vector<Log> log_queue;

private:
	const float text_time = 5.f;
	const float text_fade_in_time = 0;
	const float text_fade_out_time = 0.0f;

	const int max_lines_at_once = 10;

	const int ideal_height = 20;
	const int ideal_width = 20;

	const int slide_in_distance = 20;
	const int slide_out_distance = 20;
	const float slide_out_speed = 0.0f;
};

extern InGameLogger in_game_logger;
