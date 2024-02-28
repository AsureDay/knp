/**
 * @file spike_message.cpp
 * @brief SpikeMessage Python bindings.
 * @author Artiom N.
 * @date 01.02.2024
 */

#if defined(_KNP_IN_CORE)

#    include "spike_message.h"

#    include "common.h"


py::class_<core::messaging::SpikeData>("SpikeData").def(py::vector_indexing_suite<core::messaging::SpikeData>());
//    .def("__str__", static_cast<std::ostream &(std::ostream &, const
//    core::messaging::SpikeData&)>(&core::messaging::operator<<));


py::class_<core::messaging::SpikeMessage>("SpikeMessage", "Structure of the spike message.")
    .def("__init__", py::make_constructor(&spike_message_constructor))
    .add_property("header", &core::messaging::SpikeMessage::header_, "Message header")
    .add_property(
        "neuron_indexes", &core::messaging::SpikeMessage::neuron_indexes_, "Indexes of the recently spiked neurons")
    .def(py::self_ns::str(py::self));

#endif
