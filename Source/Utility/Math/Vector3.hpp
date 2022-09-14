#pragma once
#include "../UtilityFunctions.hpp"

#include <cassert>
#include <complex>

namespace Utility
{
	template <class T>
	class Vector3
	{
	public:

		T x;
		T y;
		T z;

		//Creates a null-vector
		Vector3<T>();

		//Creates a vector where all components are the same
		Vector3<T>(const T& aValue);

		//Creates a vector (aX, aY, aZ)
		Vector3<T>(const T& aX, const T& aY, const T& aZ);

		//Copy constructor (compiler generated)
		Vector3<T>(const Vector3<T>& aVector) = default;

		//Assignment operator (compiler generated)
		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;

		T& operator[](const int& anIndex);
		const T& operator[](const int& anIndex) const;

		//Destructor (compiler generated)
		~Vector3<T>() = default;

		//Returns the squared length of the vector
		T LengthSqr() const;

		//Returns the length of the vector
		T Length() const;

		static Vector3<T> Lerp(const Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector, float aFactor);
		static Vector3<T> NormalizedLerp(const Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector, float aFactor);

		//Returns a normalized copy of this
		Vector3<T> GetNormalized() const;

		//Normalizes the vector
		void Normalize();

		//Returns the dot product of this and aVector
		T Dot(const Vector3<T>& aVector) const;

		//Returns the cross product of this and aVector
		Vector3<T> Cross(const Vector3<T>& aVector) const;

		static Vector3<T> Zero() { return Vector3<T>(0, 0, 0); }
		static Vector3<T> One() { return Vector3<T>(1, 1, 1); }
		static Vector3<T> Up() { return Vector3<T>(0, 1, 0); }
		static Vector3<T> Down() { return Vector3<T>(0, -1, 0); }
		static Vector3<T> Right() { return Vector3<T>(1, 0, 0); }
		static Vector3<T> Left() { return Vector3<T>(-1, 0, 0); }
		static Vector3<T> Forward() { return Vector3<T>(0, 0, 1); }
		static Vector3<T> Backward() { return Vector3<T>(0, 0, -1); }
	};

	typedef Vector3<float> Vector3f;

	template <class T>
	Vector3<T>::Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	template <class T>
	Vector3<T>::Vector3(const T& aValue)
	{
		x = aValue;
		y = aValue;
		z = aValue;
	}

	template <class T>
	Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ)
	{
		x = aX;
		y = aY;
		z = aZ;
	}

	template <class T>
	T& Vector3<T>::operator[](const int& anIndex)
	{
		assert(anIndex < 3 && "Tried to access a Vector3 value outside of index range, 0 gives x, 1 gives y, 2 gives z");

		switch (anIndex)
		{
			case 0: 
				return x;
			case 1: 
				return y;
			case 2: 
				return z;
		}
	}

	template <class T>
	const T& Vector3<T>::operator[](const int& anIndex) const
	{
		assert(anIndex < 3 && "Tried to access a Vector3 value outside of index range, 0 gives x, 1 gives y, 2 gives z");

		switch (anIndex)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
		}
	}

	template <class T>
	T Vector3<T>::LengthSqr() const
	{
		return x * x + y * y + z * z;
	}

	template <class T>
	T Vector3<T>::Length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	template<class T>
	Vector3<T> Vector3<T>::Lerp(const Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector, float aFactor)
	{
		return Vector3<T>(
			Utility::Lerp(aFirstVector.x, aSecondVector.x, aFactor), 
			Utility::Lerp(aFirstVector.y, aSecondVector.y, aFactor), 
			Utility::Lerp(aFirstVector.z, aSecondVector.z, aFactor));
	}

	template<class T>
	Vector3<T> Vector3<T>::NormalizedLerp(const Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector, float aFactor)
	{
		Vector3<T> lerped = Lerp(aFirstVector, aSecondVector, aFactor);

		if (lerped.LengthSqr() > 0)
		{
			lerped.Normalize();
		}

		return lerped;
	}

	template <class T>
	Vector3<T> Vector3<T>::GetNormalized() const
	{
		assert(x != 0 || y != 0 || z != 0 && "The Vector3 was a zero-vector and a normalized version can't be returned");

		T factor = 1 / Length();
		return Vector3<T>(x * factor, y * factor, z * factor);
	}

	template <class T>
	void Vector3<T>::Normalize()
	{
		assert(x != 0 || y != 0 || z != 0 && "The Vector3 was a zero-vector and can't be normalized");

		T factor = 1 / Length();

		x = x * factor;
		y = y * factor;
		z = z * factor;
	}

	template <class T>
	T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z;
	}

	template <class T>
	Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
		return Vector3<T>(y * aVector.z - z * aVector.y, z * aVector.x - x * aVector.z, x * aVector.y - y * aVector.x);
	}

	//Returns the vector sum of aVector0 and aVector1
	template <class T>
	Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
	}

	//Returns the vector difference of aVector0 and aVector1
	template <class T>
	Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		return Vector3<T>(aScalar * aVector.x, aScalar * aVector.y, aScalar * aVector.z);
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class T>
	Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Division by zero");
		return Vector3<T>(aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar);
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T>
	void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T>
	void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T>
	void operator*=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T>
	void operator/=(Vector3<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Division by zero");
		aVector = aVector / aScalar;
	}

	//Compare operator, compares each individual value
	template <class T>
	bool operator==(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return aVector0.x == aVector1.x && aVector0.y == aVector1.y && aVector0.z == aVector1.z;
	}

	//Inverse compare operator, compares each individual value
	template <class T>
	bool operator!=(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return aVector0.x != aVector1.x || aVector0.y != aVector1.y || aVector0.z != aVector1.z;
	}
}
