////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Math
//  @author     : Artur Ostrowski
//  @usage      : Library with math functions. Currently not usable.
//  @version    : 1.0.0
//
////////////////////////////////////////
#pragma once

//#include "stdafx.h"

template<class T = float>
struct MODULE_API Vector2
{
public:

	struct
	{
		T x;
		T y;
	};

	Vector2::Vector2(T X, T Y)
	{
		x = X;
		y = Y;
	}

	template<class T>
	Vector2::Vector2(const Vector2<T>& Object)
	{
		this->x = Object.x;
		this->y = Object.y;
	}

	Vector2<T> Vector2::operator+(const Vector2<T>& RHS)
	{
		Vector2<T> NewVector;

		NewVector.x = this->x + RHS.x;
		NewVector.y = this->y + RHS.y;

		return NewVector;
	}

	Vector2<T> Vector2::operator-(const Vector2<T>& RHS)
	{
		Vector2<T> NewVector;

		NewVector.x = this->x - RHS.x;
		NewVector.y = this->y - RHS.y;

		return NewVector;
	}

	Vector2<T> Vector2::operator*(const Vector2<T>& RHS)
	{
		Vector2<T> NewVector;

		NewVector.x = this->x * RHS.x;
		NewVector.y = this->y * RHS.y;

		return NewVector;
	}

	Vector2<T> Vector2::operator/(const Vector2<T>& RHS)
	{
		Vector2<T> NewVector;

		NewVector.x = this->x / RHS.x;
		NewVector.y = this->y / RHS.y;

		return NewVector;
	}

};

template<class T>
struct MODULE_API Vector3
{
public:
	struct
	{
		T x;
		T y;
		T z;
	};

	Vector3::Vector3(T X, T Y, T Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	template<class T>
	Vector3::Vector3(const Vector3<T>& InObject)
	{
		this->x = InObject.x;
		this->y = InObject.y;
		this->z = InObject.z;
	}

	Vector3<T> Vector3<T>::operator+(const Vector3<T>& RHS)
	{
		Vector3<T> NewVector;

		NewVector.x = this.x + RHS.x;
		NewVector.y = this.y + RHS.y;
		NewVector.z = this.z + RHS.z;

		return NewVector;
	}

	Vector3<T> Vector3<T>::operator-(const Vector3<T>& RHS)
	{
		Vector3<T> NewVector;

		NewVector.x = this.x - RHS.x;
		NewVector.y = this.y - RHS.y;
		NewVector.z = this.z - RHS.z;

		return NewVector;
	}

	Vector3<T> Vector3<T>::operator*(const Vector3<T>& RHS)
	{
		Vector3<T> NewVector;

		NewVector.x = this.x * RHS.x;
		NewVector.y = this.y * RHS.y;
		NewVector.z = this.z * RHS.z;

		return NewVector;
	}

	Vector3<T> Vector3<T>::operator/(const Vector3<T>& RHS)
	{
		Vector3<T> NewVector;

		NewVector.x = this.x / RHS.x;
		NewVector.y = this.y / RHS.y;
		NewVector.z = this.z / RHS.z;

		return NewVector;
	}

};

template<class T>
bool MODULE_API IsPowerOfTwo(T InParam)
{
	return (InParam & (InParam - 1) == 0);
}

