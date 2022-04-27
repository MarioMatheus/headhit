#include <CppUTest/TestHarness.h>

extern "C"
{
	#include "../libs/hello_world.h"
}

TEST_GROUP(helloWorld)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(helloWorld, test1)
{
	STRCMP_EQUAL("Koe!", hello_world());
}
