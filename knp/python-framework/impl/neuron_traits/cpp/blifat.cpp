/**
 * @file blifat.cpp
 * @brief BLIFAT neuron Python bindings.
 * @author Artiom N.
 * @date 20.02.24
 */

#if defined(_KNP_IN_NEURON_TRAITS)

#    include "common.h"

using bn_params = knp::neuron_traits::neuron_parameters<knp::neuron_traits::BLIFATNeuron>;

py::class_<bn_params>("BLIFATNeuronParameters", "Structure for BLIFAT neuron parameters")
    .def(py::init<>())
    .add_property(
        "n_time_steps_since_last_firing", &bn_params::n_time_steps_since_last_firing_,
        "The parameter defines a number of network steps since the last spike.")
    .add_property(
        "activation_threshold", &bn_params::activation_threshold_,
        "The parameter defines a threshold for membrane potential.")
    .add_property(
        "dynamic_threshold", &bn_params::dynamic_threshold_,
        "The parameter defines a dynamic threshold for membrane potential after reaching which a neuron generates a "
        "spike.")
    .add_property(
        "threshold_decay", &bn_params::threshold_decay_,
        "The parameter defines a time constant during which the `dynamic_threshold_` parameter tends to its base value "
        "if nothing happens.")
    .add_property(
        "threshold_increment", &bn_params::threshold_increment_,
        "The parameter defines a value that increases the `dynamic_threshold_` value if a neuron generates a spike.")
    .add_property(
        "postsynaptic_trace", &bn_params::postsynaptic_trace_,
        "The parameter defines a threshold after reaching which a neuron generates spikes.")
    .add_property(
        "postsynaptic_trace_decay", &bn_params::postsynaptic_trace_decay_,
        "The parameter defines a time constant during which the `postsynaptic_trace_` parameter tends to zero if "
        "nothing happens.")
    .add_property(
        "postsynaptic_trace_increment", &bn_params::postsynaptic_trace_increment_,
        "The parameter defines a value that increases the `postsynaptic_trace_` value if a neuron generates a spike.")
    .add_property(
        "inhibitory_conductance", &bn_params::inhibitory_conductance_,
        "The parameter defines speed with which a potential tends to the `reversal_inhibitory_potential` value.")
    .add_property(
        "inhibitory_conductance_decay", &bn_params::inhibitory_conductance_decay_,
        "The parameter defines a time constant during which the `inhibitory_conductance_` value decreases.")
    .add_property("potential", &bn_params::potential_, "The parameter defines the current membrane potential.")
    .add_property(
        "pre_impact_potential", &bn_params::pre_impact_potential_,
        "This parameter is used if there was a blocking signal. Then all potential changes due to synapses are "
        "ignored.")
    .add_property(
        "potential_decay", &bn_params::potential_decay_,
        "The parameter defines a time constant during which the `potential_` value tends to zero.")
    .add_property(
        "bursting_phase", &bn_params::bursting_phase_,
        "The parameter defines a counter for the `bursting_period_` value.")
    .add_property(
        "bursting_period", &bn_params::bursting_period_,
        "The parameter defines a number of network steps after reaching which a neuron generates a spike.")
    .add_property(
        "reflexive_weight", &bn_params::reflexive_weight_,
        "The parameter defines a value that increases the membrane potential after a neuron generates a spike.")
    .add_property(
        "reversal_inhibitory_potential", &bn_params::reversal_inhibitory_potential_,
        "The parameter takes the default value of `reversal_inhibitory_potential` defined for a BLIFAT neuron.")
    .add_property(
        "absolute_refractory_period", &bn_params::absolute_refractory_period_,
        "The parameter defines a minimum number of network steps before a neuron can generate the next spike.")
    .add_property(
        "potential_reset_value", &bn_params::potential_reset_value_,
        "The parameter defines a potential value after a neuron generates a spike.")
    .add_property(
        "min_potential", &bn_params::min_potential_,
        "The parameter takes the default value of `min_potential` defined for a BLIFAT neuron.")
    .add_property(
        "total_blocking_period", &bn_params::total_blocking_period_,
        "The parameter defines the number of network execution steps, during which the neuron activity is totally "
        "blocked.")
    .add_property(
        "dopamine_value", &bn_params::dopamine_value_,
        "The parameter defines a dopamine value used to sum up all incoming dopamine synapse impacts.");

#endif
