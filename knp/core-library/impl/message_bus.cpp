/**
 * @file message_bus.cpp
 * @brief Message bus implementation.
 * @author Artiom N.
 * @date 21.02.2023
 */

#include <knp/core/message_bus.h>

#include <spdlog/spdlog.h>

#include <zmq.hpp>

#include "message_bus_cpu_impl/message_bus_cpu_impl.h"
#include "message_bus_zmq_impl/message_bus_zmq_impl.h"


namespace knp::core
{

std::unique_ptr<MessageBusImpl> MessageBus::make_cpu_implementation()
{
    return std::make_unique<MessageBusCPUImpl>();
}


std::unique_ptr<MessageBusImpl> MessageBus::make_zmq_implementation()
{
    return std::make_unique<MessageBusZMQImpl>();
}


MessageBus::MessageBus() : impl_(make_zmq_implementation())
{
    assert(impl_.get());
}


MessageBus::MessageBus(std::unique_ptr<MessageBusImpl> &&impl) : impl_(std::move(impl))
{
    if (!impl_) throw std::runtime_error("Unavailable message bus implementation");
}


MessageEndpoint MessageBus::create_endpoint()
{
    return impl_->create_endpoint();
}


bool MessageBus::step()
{
    return impl_->step();
}


size_t MessageBus::route_messages()
{
    SPDLOG_DEBUG("Message routing cycle started...");
    size_t count = 0;
    impl_->update();
    while (step())
    {
        ++count;
    }
    return count;
}

}  // namespace knp::core
