#pragma once
#include "AABB3D.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "../UtilityFunctions.hpp"

namespace Utility
{
	// If the ray is parallel to the plane, aOutIntersectionPoint remains unchanged. If
	// the ray is in the plane, true is returned, if not, false is returned. If the ray
	// isn't parallel to the plane, the intersection point is stored in
	// aOutIntersectionPoint and true returned.
	template <class T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		const T directionDot = aPlane.GetNormal().Dot(aRay.GetDirection());

		const T tolerance = 0.000001;

		const T positionalDot = (aRay.GetOrigin() - aPlane.GetOrigin()).Dot(aPlane.GetNormal());

		// Check if ray is in the plane
		if (positionalDot > -tolerance && positionalDot < tolerance)
		{
			return true;
		}

		// if dot product close to 0, basically parallel, no intersection
		if (directionDot > -tolerance && directionDot < tolerance)
		{
			return false;
		}

		// ray originates from above plane & ray points down || ray originates from under plane & ray points up
		if (positionalDot > 0 && directionDot < 0 || positionalDot < 0 && directionDot > 0)
		{
			aOutIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection() * (positionalDot / directionDot);
			return true;
		}

		return false;
	}

	// If the ray intersects the AABB, true is returned, if not, false is returned.
	// A ray in one of the AABB's sides is counted as intersecting it.
	template <class T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay)
	{
		Vector3<T> min = aAABB.GetMin();
		Vector3<T> max = aAABB.GetMax();
		Vector3<T> dir = aRay.GetDirection();
		Vector3<T> origin = aRay.GetOrigin();

		T xMinInt = (min.x - origin.x) / dir.x;
		T xMaxInt = (max.x - origin.x) / dir.x;

		if (xMinInt > xMaxInt)
		{
			Utility::Swap(xMinInt, xMaxInt);
		}

		T yMinInt = (min.y - origin.y) / dir.y;
		T yMaxInt = (max.y - origin.y) / dir.y;

		if (yMinInt > yMaxInt)
		{
			Utility::Swap(yMinInt, yMaxInt);
		}

		if (xMinInt > yMaxInt || yMinInt > xMaxInt)
		{
			return false;
		}

		if (yMinInt > xMinInt)
		{
			xMinInt = yMinInt;
		}

		if (yMaxInt < xMaxInt)
		{
			xMaxInt = yMaxInt;
		}

		T zMinInt = (min.z - origin.z) / dir.z;
		T zMaxInt = (max.z - origin.z) / dir.z;

		if (zMinInt > zMaxInt)
		{
			Utility::Swap(zMinInt, zMaxInt);
		}

		if (xMinInt > zMaxInt || zMinInt > xMaxInt)
		{
			return false;
		}

		if (zMinInt > xMinInt)
		{
			xMinInt = zMinInt;
		}

		if (zMaxInt < xMaxInt)
		{
			xMaxInt = zMaxInt;
		}

		return true;
	}

	// If the ray intersects the sphere, true is returned, if not, false is returned.
	// A rat intersecting the surface of the sphere is considered as intersecting it.
	template <class T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		Vector3<T> rayToSphere = aSphere.GetOrigin() - aRay.GetOrigin();

		// Ray origin inside sphere
		if (rayToSphere.LengthSqr() < aSphere.GetRadius() * aSphere.GetRadius())
		{
			return true;
		}

		T t = rayToSphere.Dot(aRay.GetDirection());

		// Ray origin is in front of sphere origin
		if (t < 0)
		{
			return false;
		}

		// Ray origin is behind sphere origin

		Vector3<T> sphereProjRay = aRay.GetOrigin() + aRay.GetDirection() * t;

		T projDistanceSquared = (sphereProjRay - aSphere.GetOrigin()).LengthSqr();

		// sphere proj is inside sphere
		if (projDistanceSquared <= aSphere.GetRadius() * aSphere.GetRadius())
		{
			return true;
		}

		return false;
	}
}