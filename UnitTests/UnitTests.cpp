#include "pch.h"
#include "CppUnitTest.h"
#include "../LearnOpenGL/Physics.h"
#include "../LearnOpenGL/Graphics.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		
		TEST_METHOD(TestCubeVsCube)
		{
			Cube b1 = Cube(glm::vec3(-10.0f, 0.0f, -3.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
			Cube b2 = Cube(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

			Physics::CubeVsCube();
		}
	};
}
