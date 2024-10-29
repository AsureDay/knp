/**
 * @file convert_bitwise.h
 * @brief Header for bitwise converter.
 * @kaspersky_support Vartenkov Andrey
 * @date 01.06.2023
 * @license Apache 2.0
 * @copyright © 2024 AO Kaspersky Lab
 */

#pragma once

#include <knp/core/impexp.h>
#include <knp/core/messaging/spike_message.h>

#include <vector>


/**
 * @brief Output channel namespace.
 */
namespace knp::framework::io::output
{
/**
 * @brief Convert a set of spike messages to a bool vector where an element value is `true` if a neuron with the
 * corresponding index sent at least one spike.
 * @details For example, a method, where `output_size` equals 6 and `message_list` contains messages `{0, 2}`, `{2, 4}`,
 * `{1, 2}`, returns a boolean vector `{true, true, true, false, true, false}`.
 * @param message_list list of spike messages that contain indexes of spiked neurons.
 * @param output_size output vector size (usually corresponds to the size of an output population).
 * @return bool vector.
 */
std::vector<bool> KNP_DECLSPEC
converter_bitwise(const std::vector<core::messaging::SpikeMessage> &message_list, size_t output_size)
{
    std::vector<bool> result(output_size, false);
    for (const auto &message : message_list)
    {
        for (auto index : message.neuron_indexes_)
        {
            if (index < output_size) result[index] = true;
        }
    }
    return result;
}
}  // namespace knp::framework::io::output
