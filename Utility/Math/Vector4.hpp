#pragma once
#include <cassert>
#include <complex>

namespace Utility
{
	template <class T>
	class Vector4
	{
	public:

		T x;
		T y;
		T z;
		T w;

		//Creates a null-vector
		Vector4<T>();

		//Creates a vector (aX, aY, aZ, aW)
		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW);

		//Copy constructor (compiler generated)
		Vector4<T>(const Vector4<T>& aVector) = default;

		//Assignment operator (compiler generated)
		Vector4<T>& operator=(const Vector4<T>& aVector4) = default;

		T& operator[](const size_t anIndex);
		const T& operator[](const size_t anIndex) const;

		//Destructor (compiler generated)
		~Vector4<T>() = default;

		//Returns the squared length of the vector
		T LengthSqr() const;

		//Returns the length of the vector
		T Length() const;

		//Returns a normalized copy of this
		Vector4<T> GetNormalized() const;

		//Normalizes the vector
		void Normalize();

		//Returns the dot product of this and aVector
		T Dot(const Vector4<T>& aVector) const;
	};

	typedef Vector4<float> Vector4f;

	template <class T>
	Vector4<T>::Vector4()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	template <class T>
	Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW)
	{
		x = aX;
		y = aY;
		z = aZ;
		w = aW;
	}

	template <class T>
	T& Vector4<T>::operator[](const size_t anIndex)
	{
		assert(anIndex >= 0 && anIndex < 4 && "Tried to access a Vector4 value outside of index range, 0 gives x, 1 gives y, 2 gives z, 3 gives w");

		switch (anIndex)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
		}

		return x;
	}

	template <class T>
	const T& Vector4<T>::operator[](const size_t anIndex) const
	{
		assert(anIndex >= 0 && anIndex < 4 && "Tried to access a Vector4 value outside of index range, 0 gives x, 1 gives y, 2 gives z, 3 gives w");

		switch (anIndex)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
		}

		return x;
	}

	template <class T>
	T Vector4<T>::LengthSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <class T>
	T Vector4<T>::Length() const
	{
		return std::sqrt(LengthSqr());
	}

	template <class T>
	Vector4<T> Vector4<T>::GetNormalized() const
	{
		//assert(x != 0 || y != 0 || z != 0 && "The Vector4 was a zero-vector and a normalized version can't be returned");

		T factor = 1 / Length();
		return Vector4<T>(x * factor, y * factor, z * factor, w * factor);
	}

	template <class T>
	void Vector4<T>::Normalize()
	{
		assert(x != 0 || y != 0 || z != 0 || w != 0 && "The Vector4 was a zero-vector and can't be normalized");

		T factor = 1 / Length();

		x = x * factor;
		y = y * factor;
		z = z * factor;
		w = w * factor;
	}

	template <class T>
	T Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}

	//Returns the vector sum of aVector0 and aVector1
	template <class T> Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z, aVector0.w + aVector1.w);
	}

	//Returns the vector difference of aVector0 and aVector1
	template <class T> Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z, aVector0.w - aVector1.w);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
	{
		return Vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{
		return Vector4<T>(aScalar * aVector.x, aScalar * aVector.y, aScalar * aVector.z, aScalar * aVector.w);
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class T> Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Division by zero");
		return Vector4<T>(aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar, aVector.w / aScalar);
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T> void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T> void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T> void operator*=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T> void operator/=(Vector4<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Division by zero");
		aVector = aVector / aScalar;
	}

	//Compare operator, compares each individual value
	template <class T> bool operator==(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return aVector0.x == aVector1.x && aVector0.y == aVector1.y && aVector0.z == aVector1.z && aVector0.w == aVector1.w;
	}

	//Inverse compare operator, compares each individual value
	template <class T> bool operator!=(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return aVector0.x != aVector1.x || aVector0.y != aVector1.y || aVector0.z != aVector1.z || aVector0.w != aVector1.w;
	}
}

typedef Utility::Vector4<float> Vector4f;