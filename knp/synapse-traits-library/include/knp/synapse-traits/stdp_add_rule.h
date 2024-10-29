/**
 * @file stdp_add_rule.h
 * @brief Rule for additive STDP.
 * @kaspersky_support Artiom N.
 * @date 09.08.2023
 * @license Apache 2.0
 * @copyright © 2024 AO Kaspersky Lab
 */

#pragma once

#include <cinttypes>
#include <vector>

#include "stdp_common.h"


/**
 * @brief Namespace for synapse traits.
 */
namespace knp::synapse_traits
{

/**
 * @brief STDP additive rule parameters.
 * @note Parameters for the `W(x)` function by Zhang et al. 1998.
 */
template <typename SynapseType>
struct STDPAdditiveRule
{
    /**
     * @brief Type of the synapse linked with rule.
     */
    using LinkedSynapseType = SynapseType;

    /**
     * @brief Time constant in milliseconds intended to increase the weight.
     */
    // cppcheck-suppress unusedStructMember
    float tau_plus_ = 10;

    /**
     * @brief Time constant in milliseconds intended to decrease the weight.
     */
    // cppcheck-suppress unusedStructMember
    float tau_minus_ = 10;

    /**
     * @brief Indexes of network execution steps on which spikes on the synapse were generated.
     */
    // cppcheck-suppress unusedStructMember
    std::vector<uint32_t> presynaptic_spike_times_;

    /**
     * @brief Indexes of network execution steps on which spikes on the axon were generated.
     */
    // cppcheck-suppress unusedStructMember
    std::vector<uint32_t> postsynaptic_spike_times_;
};

}  // namespace knp::synapse_traits
