//--------------------------------------------------
// Unit Tests for class ParameterFactory
//
// @author: Wild Boar
//
// @date: 2025-09-17
//--------------------------------------------------

#include <gtest/gtest.h>

#include <MemeFinderLib/ParameterFactory.h>
using namespace NVL_App;

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Confirm various aspects of parameter generation
 */
TEST(ParameterFactory_Test, generation_test)
{
	auto factory = ParameterFactory(Point2d(640, 480));

	for (auto i = 0; i < 100; i++)
	{
		// Get a random parameter
		auto parameters = factory.Generate();

		// Test that two appropriate parameter indices are generated
		auto active = parameters->GetIndices();
		ASSERT_EQ(active.size(), 2);
		ASSERT_TRUE(active[0] >= 0 && active[0] < 6);
		ASSERT_TRUE(active[1] >= 0 && active[1] < 6);
		ASSERT_NE(active[0], active[1]);

		// Test that all other parameters are delta zero
		for (int j = 0; j < 6; j++)
		{
			if (j != active[0] && j != active[1])
			{
				ASSERT_EQ(parameters->GetValueDelta(j), 0);
			}
		}

		// Test that the update parameters are within bounds
		ASSERT_GE(parameters->GetValueDelta(active[0]), -2.0);
		ASSERT_LE(parameters->GetValueDelta(active[0]), 2.0);
		ASSERT_GE(parameters->GetValueDelta(active[1]), -2.0);
		ASSERT_LE(parameters->GetValueDelta(active[1]), 2.0);
	}
}
