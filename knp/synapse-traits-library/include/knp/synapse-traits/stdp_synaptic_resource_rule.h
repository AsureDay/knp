/**
 * @file stdp_synaptic_resource_rule.h
 * @brief STDP rule with synaptic resource.
 * @author Artiom N.
 * @date 05.09.2023
 */

#pragma once

#include <cinttypes>
#include <vector>

#include "stdp_common.h"


/**
 * @brief Synapse traits namespace.
 */
namespace knp::synapse_traits
{

/**
 * @brief STDP rule with synaptic resource parameters.
 * @tparam SynapseType synapse type linked with rule.
 * @note Author of the synaptic resource - Mikhail Kiselev.
 */
template <typename SynapseType>
struct STDPSynapticResourceRule
{
    /**
     * @brief Type of the synapse linked with rule.
     */
    using LinkedSynapseType = SynapseType;

    /**
     * @brief Synaptic resource.
     */
    float synaptic_resource_ = 0;
    /**
     * @brief Minimal weight value.
     */
    float w_min_ = 0;
    /**
     * @brief Maximal weight value.
     */
    float w_max_ = 1;
    /**
     * @brief Resource decreasing constant.
     * @note The `d_u_` value must be equal or greater than `0`.
     */
    float d_u_ = 0;

    /**
     * @brief Dopamine plasticity period. If a neuron is rewarded during this period, then the synapse weight changes.
     */
    uint32_t dopamine_plasticity_period_ = 0;

    /**
     * @brief Step number of when the synapse got a spike last.
     */
    uint64_t last_spike_step_ = 0;

    /**
     * @brief `true` if the synapse was updated during the current spike sequence.
     */
    bool had_hebbian_update_ = false;
};


/**
 * @brief Shared parameters for the resource STDP.
 * @tparam SynapseType synapse type linked with resource-based STDP rule.
 */
template <typename SynapseType>
struct shared_synapse_parameters<STDP<STDPSynapticResourceRule, SynapseType>>
{
};

}  // namespace knp::synapse_traits
