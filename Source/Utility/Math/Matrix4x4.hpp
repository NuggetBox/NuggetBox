#pragma once
#include <array>

#include "MathDefines.h"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace Utility
{
	template <class T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		Matrix4x4<T>();
		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);

		// () operator for read/writing element (row, column)
		T& operator()(size_t aRow, size_t aColumn);
		// () operator for reading element (row, column)
		const T& operator()(size_t aRow, size_t aColumn) const;

		Vector4<T> GetRowVector(size_t aRow) const;
		Vector4<T> GetColumnVector(size_t aColumn) const;

		void operator+=(const Matrix4x4<T>& aMatrix);
		void operator-=(const Matrix4x4<T>& aMatrix);
		void operator*=(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>& operator=(const Matrix4x4<T>& aMatrix);

		// Create a rotation matrix that rotates a Vector3 around the X axis
		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		// Create a rotation matrix that rotates a Vector3 around the Y axis
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		// Create a rotation matrix that rotates a Vector3 around the Z axis
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);

		static Matrix4x4<T> CreateRotationMatrix(Vector3<T> aRotationVector);

		static Matrix4x4<T> CreateProjectionMatrixPerspective(float aResolutionX, float aResolutionY, float aNearPlane, float aFarPlane, float someFovDegrees);
		static Matrix4x4<T> CreateProjectionMatrixOrthographic(float aResolutionX, float aResolutionY, float aNearPlane, float aFarPlane);

		static void Decompose(const Matrix4x4<T>& aMatrix, Vector3<T>& aOutPosition, Vector3<T>& aOutRotation, Vector3<T>& aOutScale);

		// Static function for creating a transpose of a matrix.
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);

		static Matrix4x4<T> Lerp(const Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T>& aSecondMatrix, float aFactor);

		// Gets the transposed version of this matrix
		Matrix4x4<T> GetTranspose() const;

		// Assumes aTransform is made up of nothing but rotations and translations.
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);

	private:
		std::array<Vector4<T>, 4> myMatrix;
	};

	typedef Matrix4x4<float> Matrix4f;

	template <class T>
	Matrix4x4<T>::Matrix4x4()
	{
		myMatrix[0] = Vector4<T>(1, 0, 0, 0);
		myMatrix[1] = Vector4<T>(0, 1, 0, 0);
		myMatrix[2] = Vector4<T>(0, 0, 1, 0);
		myMatrix[3] = Vector4<T>(0, 0, 0, 1);
	}

	template <class T>
	Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		*this = aMatrix;
	}

	template <class T>
	T& Matrix4x4<T>::operator()(const size_t aRow, const size_t aColumn)
	{
		assert(aRow >= 1 && aRow <= 4 && "Row index out of range, allowed indices for Matrix4x4 row: 1, 2, 3, 4");
		assert(aColumn >= 1 && aColumn <= 4 && "Column index out of range, allowed indices for Matrix4x4 column: 1, 2, 3, 4");

		return myMatrix[static_cast<int>(aRow) - 1][static_cast<int>(aColumn) - 1];
	}

	template <class T>
	const T& Matrix4x4<T>::operator()(const size_t aRow, const size_t aColumn) const
	{
		assert(aRow >= 1 && aRow <= 4 && "Row index out of range, allowed indices for Matrix4x4 row: 1, 2, 3, 4");
		assert(aColumn >= 1 && aColumn <= 4 && "Column index out of range, allowed indices for Matrix4x4 column: 1, 2, 3, 4");

		return myMatrix[static_cast<int>(aRow) - 1][static_cast<int>(aColumn) - 1];
	}

	template <class T>
	Vector4<T> Matrix4x4<T>::GetRowVector(const size_t aRow) const
	{
		assert(aRow >= 1 && aRow <= 4 && "Row index out of range, allowed indices for Matrix4x4 row: 1, 2, 3, 4");

		return myMatrix[static_cast<int>(aRow) - 1];
	}

	template <class T>
	Vector4<T> Matrix4x4<T>::GetColumnVector(const size_t aColumn) const
	{
		assert(aColumn >= 1 && aColumn <= 4 && "Column index out of range, allowed indices for Matrix4x4 column: 1, 2, 3, 4");

		switch (aColumn)
		{
			case 1:
				return Vector4<T>(myMatrix[0].x, myMatrix[1].x, myMatrix[2].x, myMatrix[3].x);
			case 2:
				return Vector4<T>(myMatrix[0].y, myMatrix[1].y, myMatrix[2].y, myMatrix[3].y);
			case 3:
				return Vector4<T>(myMatrix[0].z, myMatrix[1].z, myMatrix[2].z, myMatrix[3].z);
			case 4:
				return Vector4<T>(myMatrix[0].w, myMatrix[1].w, myMatrix[2].w, myMatrix[3].w);
			default:
				return Vector4<T>(-1, -1, -1, -1);
		}
	}

	template <class T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T>& aSecondMatrix)
	{
		Matrix4x4<T> temp;

		for (int i = 1; i <= 4; ++i)
		{
			for (int j = 1; j <= 4; ++j)
			{
				temp(i, j) = aFirstMatrix(i, j) + aSecondMatrix(i, j);
			}
		}

		return temp;
	}

	template <class T>
	void Matrix4x4<T>::operator+=(const Matrix4x4<T>& aMatrix)
	{
		*this = *this + aMatrix;
	}

	template <class T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T>& aSecondMatrix)
	{
		Matrix4x4<T> temp;

		for (int i = 1; i <= 4; ++i)
		{
			for (int j = 1; j <= 4; ++j)
			{
				temp(i, j) = aFirstMatrix(i, j) - aSecondMatrix(i, j);
			}
		}

		return temp;
	}

	template <class T>
	void Matrix4x4<T>::operator-=(const Matrix4x4<T>& aMatrix)
	{
		*this = *this - aMatrix;
	}

	template <class T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T>& aSecondMatrix)
	{
		Matrix4x4<T> temp;

		for (int i = 1; i <= 4; ++i)
		{
			for (int j = 1; j <= 4; ++j)
			{
				temp(i, j) = aFirstMatrix.GetRowVector(i).Dot(aSecondMatrix.GetColumnVector(j));
			}
		}

		return temp;
	}

	template <class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		Vector4<T> temp;

		temp.x = aVector.Dot(aMatrix.GetColumnVector(1));
		temp.y = aVector.Dot(aMatrix.GetColumnVector(2));
		temp.z = aVector.Dot(aMatrix.GetColumnVector(3));
		temp.w = aVector.Dot(aMatrix.GetColumnVector(4));

		return temp;
	}

	template <class T>
	void Matrix4x4<T>::operator*=(const Matrix4x4<T>& aMatrix)
	{
		*this = *this * aMatrix;
	}

	template <class T>
	Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4<T>& aMatrix)
	{
		memcpy_s(this, sizeof(Matrix4x4<T>), &aMatrix, sizeof(Matrix4x4<T>));
		return *this;
	}

	template <class T>
	bool operator==(const Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T>& aSecondMatrix)
	{
		for (int i = 1; i <= 4; ++i)
		{
			for (int j = 1; j <= 4; ++j)
			{
				if (aFirstMatrix(i, j) != aSecondMatrix(i, j))
				{
					return false;
				}
			}
		}

		return true;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		Matrix4x4<T> temp;

		T sine = sin(aAngleInRadians);
		T cosine = cos(aAngleInRadians);

		temp(1, 1) = 1;
		//temp(1, 2) = 0;
		//temp(1, 3) = 0;
		//temp(1, 4) = 0;

		//temp(2, 1) = 0;
		temp(2, 2) = cosine;
		temp(2, 3) = sine;
		//temp(2, 4) = 0;

		//temp(3, 1) = 0;
		temp(3, 2) = -sine;
		temp(3, 3) = cosine;
		//temp(3, 4) = 0;

		//temp(4, 1) = 0;
		//temp(4, 2) = 0;
		//temp(4, 3) = 0;
		temp(4, 4) = 1;

		return temp;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		Matrix4x4<T> temp;

		T sine = sin(aAngleInRadians);
		T cosine = cos(aAngleInRadians);

		temp(1, 1) = cosine;
		//temp(1, 2) = 0;
		temp(1, 3) = -sine;
		//temp(1, 4) = 0;

		//temp(2, 1) = 0;
		temp(2, 2) = 1;
		//temp(2, 3) = 0;
		//temp(2, 4) = 0;

		temp(3, 1) = sine;
		//temp(3, 2) = 0;
		temp(3, 3) = cosine;
		//temp(3, 4) = 0;

		//temp(4, 1) = 0;
		//temp(4, 2) = 0;
		//temp(4, 3) = 0;
		temp(4, 4) = 1;

		return temp;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		Matrix4x4<T> temp;

		T sine = sin(aAngleInRadians);
		T cosine = cos(aAngleInRadians);

		temp(1, 1) = cosine;
		temp(1, 2) = sine;
		//temp(1, 3) = 0;
		//temp(1, 4) = 0;

		temp(2, 1) = -sine;
		temp(2, 2) = cosine;
		//temp(2, 3) = 0;
		//temp(2, 4) = 0;

		//temp(3, 1) = 0;
		//temp(3, 2) = 0;
		temp(3, 3) = 1;
		//temp(3, 4) = 0;

		//temp(4, 1) = 0;
		//temp(4, 2) = 0;
		//temp(4, 3) = 0;
		temp(4, 4) = 1;

		return temp;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationMatrix(Vector3<T> aRotationVector)
	{
		Matrix4x4<T> rotationMatrix;

		rotationMatrix *= CreateRotationAroundX(DEGTORAD(aRotationVector.x));
		rotationMatrix *= CreateRotationAroundY(DEGTORAD(aRotationVector.y));
		rotationMatrix *= CreateRotationAroundZ(DEGTORAD(aRotationVector.z));

		return rotationMatrix;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::CreateProjectionMatrixPerspective(float aResolutionX, float aResolutionY, float aNearPlane, float aFarPlane, float someFovDegrees)
	{
		Matrix4x4<T> projectionMatrix;

		const float hFoVRad = DEGTORAD(someFovDegrees);
		const float hFovCalc = std::tanf(hFoVRad * 0.5f);
		const float vFoVRad = 2 * std::atan(hFovCalc * (aResolutionY / aResolutionX));

		const float xScale = 1.0f / hFovCalc;
		const float yScale = 1.0f / std::tanf(vFoVRad * 0.5f);

		const float Q = aFarPlane / (aFarPlane - aNearPlane);

		projectionMatrix(1, 1) = xScale;
		projectionMatrix(2, 2) = yScale;
		projectionMatrix(3, 3) = Q;
		projectionMatrix(4, 3) = -Q * aNearPlane;
		projectionMatrix(3, 4) = 1.0f / Q;
		projectionMatrix(4, 4) = 0;

		return projectionMatrix;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::CreateProjectionMatrixOrthographic(float aResolutionX, float aResolutionY, float aNearPlane, float aFarPlane)
	{
		Matrix4x4<T> orthographicProjection;

		orthographicProjection(1, 1) = 2.0f / aResolutionX;
		orthographicProjection(2, 2) = 2.0f / aResolutionY;
		orthographicProjection(3, 3) = 1.0f / (aFarPlane - aNearPlane);
		orthographicProjection(4, 3) = aNearPlane / (aNearPlane - aFarPlane);
		//orthographicProjection(4, 4) = 1.0f;

		return orthographicProjection;
	}

	template<class T>
	void Matrix4x4<T>::Decompose(const Matrix4x4<T>& aMatrix, Vector3<T>& aOutPosition, Vector3<T>& aOutRotation, Vector3<T>& aOutScale)
	{
		aOutPosition = Vector3<T>(aMatrix(4, 1), aMatrix(4, 2), aMatrix(4, 3));

		aOutScale.x = Vector3<T>(aMatrix(1, 1), aMatrix(1, 2), aMatrix(1, 3)).Length();
		aOutScale.y = Vector3<T>(aMatrix(2, 1), aMatrix(2, 2), aMatrix(2, 3)).Length();
		aOutScale.z = Vector3<T>(aMatrix(3, 1), aMatrix(3, 2), aMatrix(3, 3)).Length();

		if (aOutScale.x == 0 || aOutScale.y == 0 || aOutScale.z == 0)
		{
			//TODO: Fix Scale <= 0
			assert(L"Fix decompose if needed to support 0-scale or negative scale");
		}

		Matrix4x4<T> rotationMatrix;
		rotationMatrix(1, 1) = aMatrix(1, 1) / aOutScale.x;
		rotationMatrix(1, 2) = aMatrix(1, 2) / aOutScale.x;
		rotationMatrix(1, 3) = aMatrix(1, 3) / aOutScale.x;
		rotationMatrix(2, 1) = aMatrix(2, 1) / aOutScale.y;
		rotationMatrix(2, 2) = aMatrix(2, 2) / aOutScale.y;
		rotationMatrix(2, 3) = aMatrix(2, 3) / aOutScale.y;
		rotationMatrix(3, 1) = aMatrix(3, 1) / aOutScale.z;
		rotationMatrix(3, 2) = aMatrix(3, 2) / aOutScale.z;
		rotationMatrix(3, 3) = aMatrix(3, 3) / aOutScale.z;

		rotationMatrix = Transpose(rotationMatrix);

		float eulerX = atan2(rotationMatrix(3, 2), rotationMatrix(3, 3));
		float eulerY = atan2(-rotationMatrix(3, 1), sqrt(rotationMatrix(3, 2) * rotationMatrix(3, 2) + rotationMatrix(3, 3) * rotationMatrix(3, 3)));
		float eulerZ = atan2(rotationMatrix(2, 1), rotationMatrix(1, 1));

		aOutRotation.x = RADTODEG(eulerX);
		aOutRotation.y = RADTODEG(eulerY);
		aOutRotation.z = RADTODEG(eulerZ);
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> temp;

		for (int i = 1; i <= 4; ++i)
		{
			for (int j = 1; j <= 4; ++j)
			{
				temp(j, i) = aMatrixToTranspose(i, j);
			}
		}

		return temp;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::Lerp(const Matrix4x4<T>& aFirstMatrix, const Matrix4x4<T>& aSecondMatrix, float aFactor)
	{
		Matrix4x4<T> result = aFirstMatrix;

		Vector3f firstRight = { aFirstMatrix(1, 1), aFirstMatrix(1, 2), aFirstMatrix(1, 3) };
		Vector3f secondRight = { aSecondMatrix(1, 1), aSecondMatrix(1, 2), aSecondMatrix(1, 3) };
		Vector3f finalRight = Vector3<T>::NormalizedLerp(firstRight, secondRight, aFactor);

		result(1, 1) = finalRight.x;
		result(1, 2) = finalRight.y;
		result(1, 3) = finalRight.z;

		Vector3f firstUp = { aFirstMatrix(2, 1), aFirstMatrix(2, 2), aFirstMatrix(2, 3) };
		Vector3f secondUp = { aSecondMatrix(2, 1), aSecondMatrix(2, 2), aSecondMatrix(2, 3) };
		Vector3f finalUp = Vector3<T>::NormalizedLerp(firstUp, secondUp, aFactor);

		result(2, 1) = finalUp.x;
		result(2, 2) = finalUp.y;
		result(2, 3) = finalUp.z;

		Vector3f firstFront = { aFirstMatrix(3, 1), aFirstMatrix(3, 2), aFirstMatrix(3, 3) };
		Vector3f secondFront = { aSecondMatrix(3, 1), aSecondMatrix(3, 2), aSecondMatrix(3, 3) };
		Vector3f finalFront = Vector3<T>::NormalizedLerp(firstFront, secondFront, aFactor);

		result(3, 1) = finalFront.x;
		result(3, 2) = finalFront.y;
		result(3, 3) = finalFront.z;

		Vector3<T> pos = { aFirstMatrix(1, 4), aFirstMatrix(2, 4), aFirstMatrix(3, 4) };
		Vector3<T> secondPos = { aSecondMatrix(1, 4), aSecondMatrix(2, 4), aSecondMatrix(3, 4) };
		Vector3<T> lerpedPos = Vector3<T>::Lerp(pos, secondPos, aFactor);

		result(1, 4) = lerpedPos.x;
		result(2, 4) = lerpedPos.y;
		result(3, 4) = lerpedPos.z;

		return result;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::GetTranspose() const
	{
		Matrix4x4<T> transposed;

		for (int i = 1; i <= 4; ++i)
		{
			for (int j = 1; j <= 4; ++j)
			{
				transposed(j, i) = (*this)(i, j);
			}
		}

		return transposed;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4<T> tempTranslation;

		for (int i = 1; i <= 3; ++i)
		{
			tempTranslation(4, i) = aTransform(4, i) * -1;
		}

		Matrix4x4<T> tempRotation;

		for (int i = 1; i <= 3; ++i)
		{
			for (int j = 1; j <= 3; ++j)
			{
				tempRotation(j, i) = aTransform(i, j);
			}
		}

		return tempTranslation * tempRotation;
	}
}
