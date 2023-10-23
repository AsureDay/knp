#include <knp/core-library/include/knp/core/message_bus.h>

#include <boost/python.hpp>


namespace py = boost::python;


BOOST_PYTHON_MODULE(MessageBus)
{
    py::class_<MessageBus>("MessageBus", "The MessageBus class is a definition of an interface to a message bus.")
        .def(py::init<>(), "Default message bus constructor.")
        .def(py::init<bool>(), "MessageBus with selection of implementation.")
        .def(
            py::init<std::unique_ptr<messaging::impl::MessageBusImpl>>(),
            "Message bus constructor with a specialized implementation.");
}
