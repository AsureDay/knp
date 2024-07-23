/**
 * @brief Tests runner.
 * @license Apache 2.0
 * @copyright © 2024 AO Kaspersky Lab
 */

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <tests_common.h>

#include <exception>
#include <iostream>


int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::trace);
    testing::InitGoogleTest(&argc, argv);

    try
    {
        return RUN_ALL_TESTS();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }

    return EXIT_SUCCESS;
}
