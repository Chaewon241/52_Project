#include <cassert>
#include <limits>

#include "UnitTest.h"
#include "Vector2.h"
#include "hRectangle.h"

namespace catInWonderland
{
	void UnitTest::Vector2_DistanceTest()
	{
		// distance test
		{
			catInWonderland::Vector2 vec0(10.f, 10.f);
			catInWonderland::Vector2 vec1(20.f, 20.f);

			float distance = Vector2::GetDistance(vec0, vec1);

			assert(equalFloat(distance* distance, 200.f));
		}
	}

	void UnitTest::hRectangle_Rotate()
	{
		// rotate test
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);
			hRectangle* hRecTest0 = new hRectangle(rec0);

			float radian = 3.14 / 2;
			hRecTest0->Rotate(rec0.GetTopLeft().GetX(), rec0.GetTopLeft().GetY(), radian);

			assert(equalFloat(rec0.GetTopLeft().GetX(), 15.f));
		}

	}

	void UnitTest::hRectangle_GetCenter()
	{
		// getcenter test
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);
			hRectangle* hRecTest0 = new hRectangle(rec0);

			Vector2 vec = hRecTest0->GetCenter();
			assert(equalFloat(vec.GetX(), 15.f) && equalFloat(vec.GetY(), 15.f));
		}
	}

	void UnitTest::hRectangle_GetSize()
	{
		// getsize test
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);
			hRectangle* hRecTest0 = new hRectangle(rec0);

			Vector2 vec = hRecTest0->GetSize();
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
		// getboundingrectangle test
		{
			hRectangle rec0(10.f, 10.f, 20.f, 20.f);
			// 이거 rotate 해서 하기
			hRectangle tmp_rec = hRectangle::GetBoundingRectangle(rec0);
			hRectangle rec1(10.f, 10.f, 20.f, 20.f);

			assert(equalFloat(tmp_rec.GetTopLeft().GetX(), rec1.GetTopLeft().GetX())
				&& equalFloat(tmp_rec.GetBottomRight().GetX(), rec1.GetBottomRight().GetX()));
		}
	}

	// 어떻게 작동하는지..!
	//void UnitTest::hRectangle_GetIntersection()
	//{
	//	// getintersection test
	//	{
	//		hRectangle rec0(10.f, 10.f, 20.f, 20.f);
	//		hRectangle rec1(15.f, 15.f, 18.f, 18.f);

	//		// 포함이 되면 true
	//		assert(hRectangle::IsContained(rec0, rec1));
	//	}
	//}

	bool UnitTest::equalFloat(float num1, float num2)
	{
		if (num1 <= num2 + FLT_EPSILON)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
}

