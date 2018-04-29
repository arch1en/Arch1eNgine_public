#pragma once

#include <stdafx.h>

#include <utility>

struct Color
{
	float R;
	float G;
	float B;
	float A;

	constexpr Color() : R(0.f), G(0.f), B(0.f), A(0.f) {}
	constexpr explicit Color(float Value) : R(Value), G(Value), B(Value), A(Value) {}
	constexpr explicit Color(float r, float g, float b) : R(r), G(g), B(b), A(1.f) {}
	constexpr explicit Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
	constexpr explicit Color(float rgb, float a) : R(rgb), G(rgb), B(rgb), A(a) {}

	Color& operator=(const Color& Other) { this->R = Other.R; this->G = Other.G; this->B = Other.B;  this->A = Other.A; return *this; }
	Color& operator+(const Color& Other) { this->R = this->R + Other.R; this->G = this->G + Other.G; this->B = this->B + Other.B;  this->A = this->A + Other.A; return *this; }
	Color& operator-(const Color& Other) { this->R = this->R - Other.R; this->G = this->G - Other.G; this->B = this->B - Other.B;  this->A = this->A - Other.A; return *this; }
	Color& operator*(const Color& Other) { this->R = this->R * Other.R; this->G = this->G * Other.G; this->B = this->B * Other.B;  this->A = this->A * Other.A; return *this; }
	Color& operator/(const Color& Other) { this->R = this->R / Other.R; this->G = this->G / Other.G; this->B = this->B / Other.B;  this->A = this->A / Other.A; return *this; }

	static constexpr Color White() { return Color(1.f); }
	static constexpr Color Grey() { return Color(0.5, 1.f); }
	static constexpr Color Black() { return Color(0.f); }
	static constexpr Color Red() { return Color(1.f, 0.f, 0.f, 1.f); }
	static constexpr Color Green() { return Color(0.f, 1.f, 0.f, 1.f); }
	static constexpr Color Blue() { return Color(0.f, 0.f, 1.f, 1.f); }

};