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

// -=- VECTOR2 -=-

template<class T = float>
struct Vector2
{
public:

	struct
	{
		T x;
		T y;
	};

	Vector2(T X, T Y)
	{
		this->x = X;
		this->y = Y;
	}

	template<class T>
	Vector2(const Vector2<T>& Object)
	{
		this->x = Object.x;
		this->y = Object.y;
	}

	Vector2<T> operator+(const Vector2<T>& RHS)
	{
		Vector2<T> NewVector;

		NewVector.x = this->x + RHS.x;
		NewVector.y = this->y + RHS.y;

		return NewVector;
	}

	Vector2<T> operator-(const Vector2<T>& RHS)
	{
		Vector2<T> NewVector;

		NewVector.x = this->x - RHS.x;
		NewVector.y = this->y - RHS.y;

		return NewVector;
	}

	Vector2<T> operator*(const Vector2<T>& RHS)
	{
		Vector2<T> NewVector;

		NewVector.x = this->x * RHS.x;
		NewVector.y = this->y * RHS.y;

		return NewVector;
	}

	Vector2<T> operator/(const Vector2<T>& RHS)
	{
		Vector2<T> NewVector;

		NewVector.x = this->x / RHS.x;
		NewVector.y = this->y / RHS.y;

		return NewVector;
	}

};

// ~=~ VECTOR2 ~=~

// -=- VECTOR3 -=-

template<class T = float>
struct Vector3
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
		x = x + RHS.x;
		y = y + RHS.y;
		z = z + RHS.z;

		return *this;
	}

	Vector3<T> Vector3<T>::operator-(const Vector3<T>& RHS)
	{
		Vector3<T> NewVector;

		NewVector.x = this->x - RHS.x;
		NewVector.y = this->y - RHS.y;
		NewVector.z = this->z - RHS.z;

		return NewVector;
	}

	Vector3<T> Vector3<T>::operator*(const Vector3<T>& RHS)
	{
		Vector3<T> NewVector;

		NewVector.x = this->x * RHS.x;
		NewVector.y = this->y * RHS.y;
		NewVector.z = this->z * RHS.z;

		return NewVector;
	}

	Vector3<T> Vector3<T>::operator/(const Vector3<T>& RHS)
	{
		Vector3<T> NewVector;

		NewVector.x = this->x / RHS.x;
		NewVector.y = this->y / RHS.y;
		NewVector.z = this->z / RHS.z;

		return NewVector;
	}

};

// ~=~ VECTOR3 ~=~

// -=- VECTOR4 -=-

template<class T = float>
struct Vector4
{
public:

	T x;
	T y;
	T z;
	T w;

	Vector4(T X, T Y, T Z, T W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	Vector4(const Vector4<T>& InObject)
	{
		x = InObject.x;
		y = InObject.y;
		z = InObject.z;
		w = InObject.w;
	}

	Vector4 operator=(const Vector4<T>& RHS)
	{
		x = RHS.X;
		y = RHS.Y;
		z = RHS.Z;
		w = RHS.W;
	}

	Vector4<T> operator+(const Vector4<T>& RHS)
	{
		x = x + RHS.x;
		y = y + RHS.y;
		z = z + RHS.z;
		w = w + RHS.w;

		return *this;
	}

	Vector4<T> operator-(const Vector4<T>& RHS)
	{
		Vector4<T> NewVector;

		NewVector.x = this->x - RHS.x;
		NewVector.y = this->y - RHS.y;
		NewVector.z = this->z - RHS.z;
		NewVector.w = this->w - RHS.w;

		return NewVector;
	}

	Vector4<T> operator*(const Vector4<T>& RHS)
	{
		Vector4<T> NewVector;

		NewVector.x = this->x * RHS.x;
		NewVector.y = this->y * RHS.y;
		NewVector.z = this->z * RHS.z;
		NewVector.w = this->w * RHS.w;

		return NewVector;
	}

	Vector4<T> operator/(const Vector4<T>& RHS)
	{
		Vector4<T> NewVector;

		NewVector.x = this->x / RHS.x;
		NewVector.y = this->y / RHS.y;
		NewVector.z = this->z / RHS.z;
		NewVector.w = this->w / RHS.w;

		return NewVector;
	}

};

// ~=~ VECTOR4 ~=~

template<unsigned int X, unsigned int Y, class T = float>
struct Matrix
{
	explicit Matrix()
	{
		MatrixBody = new T[X * Y];
	}

	// Rule of Five

	// copy constructor
	Matrix(const Matrix& aMatrix)
	{
		this = aMatrix;
	}

	// move constructor
	explicit Matrix(Matrix&& aMatrix)
	{
		this = aMatrix;
	}

	Matrix& operator=(const Matrix& RHS)
	{
		MatrixBody = RHS.MatrixBody;
		return *this;
	}

	Matrix& operator=(Matrix&& RHS)
	{
		MatrixBody = RHS.MatrixBody;
		return *this;
	}

	// destructor
	~Matrix()
	{
		delete[] MatrixBody;
	}

	explicit Matrix(Vector3<> aVector)
	{

	}

	T& operator[](int Index) 
	{

		return MatrixBody[Index];
	}

private:

	T* MatrixBody;

	//decltype(X) MatrixBody[X][Y];
};

template<class T>
bool IsPowerOfTwo(T InParam)
{
	return (InParam & (InParam - 1) == 0);
}
