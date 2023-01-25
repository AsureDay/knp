/**
 * @file population.h
 * @brief General Population Interface.
 * @author Artiom N.
 * @date 18.01.2023
 */

#pragma once

#include <knp/core.h>
#include <knp/uid.h>

#include <vector>


namespace knp::core
{


/**
 * @brief The Population class is a container of neurons of the same model.
 * @tparam NeuronType type of the population neurons.
 * @see Neuron.
 */
template <typename NeuronType>
class Population
{
public:
    get_neurons_parameters()->std::vector<NeuronType>;

public:
    UID presynaptic_population_uid_;
    UID postsynaptic_population_uid_;
    BaseData base_;
};

}  // namespace knp::core
