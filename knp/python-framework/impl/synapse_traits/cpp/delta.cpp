/**
 * @file delta.cpp
 * @brief Python bindings for delta synapse.
 * @kaspersky_support Artiom N.
 * @date 21.02.24
 * @license Apache 2.0
 * @copyright © 2024 AO Kaspersky Lab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(_KNP_IN_SYNAPSE_TRAITS)

#    include "common.h"

py::enum_<knp::synapse_traits::OutputType>("OutputType")
    .value("EXCITATORY", knp::synapse_traits::OutputType::EXCITATORY)  // Excitatory synapse type.
    .value(
        "INHIBITORY_CURRENT",
        knp::synapse_traits::OutputType::INHIBITORY_CURRENT)  // Inhibitory by current synapse type.
    .value(
        "INHIBITORY_CONDUCTANCE",
        knp::synapse_traits::OutputType::INHIBITORY_CONDUCTANCE)   // Inhibitory by conductance synapse type.
    .value("DOPAMINE", knp::synapse_traits::OutputType::DOPAMINE)  // Dopamine synapse type.
    // Neuron-blocking synapse type.
    .value("BLOCKING", knp::synapse_traits::OutputType::BLOCKING);

using ds_params = knp::synapse_traits::synapse_parameters<knp::synapse_traits::DeltaSynapse>;

py::class_<ds_params>("DeltaSynapseParameters", "Structure for BLIFAT neuron parameters")
    .def(py::init<>())
    .def(py::init<float, uint32_t, knp::synapse_traits::OutputType>())
    .add_property("weight", &ds_params::weight_, "Synaptic weight.")
    .add_property(
        "delay", &ds_params::delay_,
        "Synaptic delay. Delay of `N` means that a spike sent on step `X` will be received on step `X + N`.")
    .add_property(
        "output_type", &ds_params::output_type_,
        "Synapse type. Various types have different influences on neuron parameters as defined by neuron function.");

#endif
