/**
 * @file input_converter.cpp
 * @brief Input converter routines.
 * @author Artiom N.
 * @date 14.07.2023
 * @license Apache 2.0
 * @copyright © 2024 AO Kaspersky Lab
 */

#include <knp/framework/io/in_converters/index_converter.h>
#include <knp/framework/io/input_converter.h>

// #include <spdlog/spdlog.h>

namespace knp::framework::io::input
{

core::messaging::SpikeData IndexConverter::operator()(core::Step) const
{
    core::messaging::SpikeData result;

    std::string buffer_string;
    std::getline(*stream_, buffer_string);

    auto iter_first = buffer_string.begin();
    auto iter_second = iter_first;

    while (iter_first != buffer_string.end())
    {
        iter_second = std::find(iter_second + 1, buffer_string.end(), delim_);
        result.push_back(
            std::stoul(buffer_string.substr(iter_first - buffer_string.begin(), iter_second - buffer_string.begin())));

        if (iter_second == buffer_string.end()) break;
        iter_first = std::next(iter_second);
    }
    return result;
}

}  // namespace knp::framework::io::input
