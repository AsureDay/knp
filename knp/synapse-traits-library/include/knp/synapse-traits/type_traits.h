/**
 * @file type_traits.h
 * @brief Common synapse type traits.
 * @author Artiom N.
 * @date 26.01.2023
 */

#pragma once

/**
 * @brief Namespace for synapse traits.
 */
namespace knp::synapse_traits
{

/**
 * @brief Structure for synapse parameters.
 * @tparam SynapseType type of synapses.
 */
template <typename SynapseType>
struct synapse_parameters;


/**
 * @brief Structure for parameters shared between synapses.
 * @tparam SynapseType type of synapses.
 */
template <typename SynapseType>
struct shared_synapse_parameters
{
};
}  // namespace knp::synapse_traits
