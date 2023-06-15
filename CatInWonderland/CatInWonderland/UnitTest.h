#pragma once

namespace catInWonderland
{
	class UnitTest
	{
	public:
		static void Test();

		static void Vector2_DistanceTest();
		static void hRectangle_Rotate();
		static void hRectangle_GetCenter();
		static void hRectangle_GetSize();
		static void hRectangle_IsCollison();
		static void hRectangle_IsContained();
		static void hRectangle_GetBoundingRectangle();
		static void hRectangle_GetIntersection();

	private:
		static bool equalFloat(float num1, float num2);		
	};
}

