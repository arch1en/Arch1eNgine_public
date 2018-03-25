#pragma once

#include <stdafx.h>

struct Color
{
	float r;
	float g;
	float b;
	float a;

	Color()
	{
		Color(0.f);
	}

	Color(float Value)
	{
		r = g = b = a = Value;
	}

	Color(float R, float G, float B)
	{
		r = R;
		g = G;
		b = B;
	}

	Color(float R, float G, float B, float A)
	{
		Color(R, G, B);
		a = A;
	}

	Color operator+(const Color& Other)
	{
		return Color(this->r + Other.r, this->g + Other.g, this->b + Other.b, this->a + Other.a);
	}

	Color operator-(const Color& Other)
	{
		return Color(this->r - Other.r, this->g - Other.g, this->b - Other.b, this->a - Other.a);
	}

	Color operator*(const Color& Other)
	{
		return Color(this->r * Other.r, this->g * Other.g, this->b * Other.b, this->a * Other.a);
	}

	Color operator/(const Color& Other)
	{
		return Color(this->r / Other.r, this->g / Other.g, this->b / Other.b, this->a / Other.a);
	}

	static const Color White;
	static const Color Grey;
	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
};

const Color Color::White(1.f);
const Color Color::Grey(0.5f, 0.5f, 0.5f, 1.0f);
const Color Color::Black(0.f, 0.f, 0.f, 1.f);
const Color Color::Red(1.f, 0.f, 0.f, 1.f);
const Color Color::Green(0.f, 1.f, 0.f, 1.f);
const Color Color::Blue(0.f, 0.f, 1.f, 1.f);
