#include <cassert>
#include <cmath>
#include <limits>

#include "UnitTest.h"
#include "Vector2.h"
#include "hRectangle.h"

#ifndef EPSILON
#define EPSILON (0.001f)
#endif 

namespace catInWonderland
{
	void UnitTest::Test()
	{
		UnitTest::Vector2_DistanceTest();
		UnitTest::hRectangle_Rotate();
		UnitTest::hRectangle_GetCenter();
		UnitTest::hRectangle_GetSize();
		UnitTest::hRectangle_IsCollison();
		UnitTest::hRectangle_IsContained();
		UnitTest::hRectangle_GetBoundingRectangle();
		UnitTest::hRectangle_GetIntersection();
	}
	
	void UnitTest::Vector2_DistanceTest()
	{
		// distance test
		{
			catInWonderland::Vector2 vec0(10.f, 10.f);
			catInWonderland::Vector2 vec1(20.f, 20.f);

			float distance = Vector2::GetDistance(vec0, vec1);

			assert(equalFloat(distance * distance, 200.f));
		}
	}

	void UnitTest::hRectangle_Rotate()
	{
		// rotate test
		{
			hRectangle rec0(0.f, 0.f, 100.f, 100.f);
			hRectangle rec1 = rec0;

			Vector2 center = rec0.GetCenter();

			float radian = 1.5708;
			rec0.Rotate(center.GetX(), center.GetY(), radian);

			assert(rec1.GetTopLeft() == rec0.GetBottomLeft());
			assert(rec1.GetTopRight() == rec0.GetTopLeft());
			assert(rec1.GetBottomLeft() == rec0.GetBottomRight());
			assert(rec1.GetBottomRight() == rec0.GetTopRight());
		}
	}

	void UnitTest::hRectangle_GetCenter()
	{
		// getcenter test
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);
			hRectangle hRecTest0(rec0);

			Vector2 vec = hRecTest0.GetCenter();
			assert(equalFloat(vec.GetX(), 15.f) && equalFloat(vec.GetY(), 15.f));
		}
	}

	void UnitTest::hRectangle_GetSize()
	{
		// getsize test
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);
			hRectangle hRecTest0(rec0);

			Vector2 vec = hRecTest0.GetSize();
			assert(equalFloat(vec.GetX(), 10.f) && equalFloat(vec.GetY(), 10.f));
		}
	}

	void UnitTest::hRectangle_IsCollison()
	{
		// iscollison test
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);
			hRectangle rec1(15.f, 15.f, 25.f, 25.f);

			// 충돌이 되면 true
			assert(hRectangle::IsCollision(rec0, rec1));
		}
	}

	void UnitTest::hRectangle_IsContained()
	{
		// iscontained test
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);
			hRectangle rec1(15.f, 15.f, 18.f, 18.f);

			// 포함이 되면 true
			assert(hRectangle::IsContained(rec0, rec1));
		}
	}

	void UnitTest::hRectangle_GetBoundingRectangle()
	{
		// getboundingrectangle test1 90도 돌리기
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);

			float radian = 1.5708;

			rec0.Rotate(rec0.GetCenter().GetX(), rec0.GetCenter().GetY(), radian);
			hRectangle rec1(10.f, 10.f, 20.f, 20.f);

			assert(hRectangle::GetBoundingRectangle(rec1).GetTopLeft() == hRectangle::GetBoundingRectangle(rec0).GetTopLeft());
			assert(hRectangle::GetBoundingRectangle(rec1).GetTopRight() == hRectangle::GetBoundingRectangle(rec0).GetTopRight());
			assert(hRectangle::GetBoundingRectangle(rec1).GetBottomLeft() == hRectangle::GetBoundingRectangle(rec0).GetBottomLeft());
			assert(hRectangle::GetBoundingRectangle(rec1).GetBottomRight() == hRectangle::GetBoundingRectangle(rec0).GetBottomRight());
		}

		// getboundingrectangle test2 45도 돌리기
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);

			float radian = 0.785398;

			rec0.Rotate(rec0.GetCenter().GetX(), rec0.GetCenter().GetY(), radian);
			hRectangle rec1(7.92893, 7.92893, 22.0711, 22.0711);

			hRectangle boundingRec0 = hRectangle::GetBoundingRectangle(rec0);
			hRectangle boundingRec1 = hRectangle::GetBoundingRectangle(rec1);

			assert(equalFloat(boundingRec1.GetTopLeft().GetX(), boundingRec0.GetTopLeft().GetX()));
			assert(equalFloat(boundingRec1.GetTopLeft().GetY(), boundingRec0.GetTopLeft().GetY()));

			assert(equalFloat(boundingRec1.GetTopRight().GetX(), boundingRec0.GetTopRight().GetX()));
			assert(equalFloat(boundingRec1.GetTopRight().GetY(), boundingRec0.GetTopRight().GetY()));

			assert(equalFloat(boundingRec1.GetBottomLeft().GetX(), boundingRec0.GetBottomLeft().GetX()));
			assert(equalFloat(boundingRec1.GetBottomLeft().GetY(), boundingRec0.GetBottomLeft().GetY()));
			
			assert(equalFloat(boundingRec1.GetBottomRight().GetX(), boundingRec0.GetBottomRight().GetX()));
			assert(equalFloat(boundingRec1.GetBottomRight().GetY(), boundingRec0.GetBottomRight().GetY()));
		}
	}

	// 어떻게 판단하는거지?
	void UnitTest::hRectangle_GetIntersection()
	{
		// getintersection test
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);
			hRectangle rec1(15.f, 15.f, 25.f, 25.f);

			hRectangle interSectionRec = hRectangle::GetIntersection(rec0, rec1);
		}
	}

	bool UnitTest::equalFloat(float num1, float num2)
	{
		return fabs(num1 - num2) < EPSILON;
	}
}