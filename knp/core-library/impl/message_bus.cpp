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
MessageBus::~MessageBus() {}

MessageBus::MessageBus(MessageBus &&) = default;

MessageBus MessageBus::construct_cpu_bus()
{
    return MessageBus(std::make_unique<messaging::impl::MessageBusCPUImpl>());
}


MessageBus MessageBus::construct_zmq_bus()
{
    return MessageBus(std::make_unique<messaging::impl::MessageBusZMQImpl>());
}


MessageBus MessageBus::construct_bus()
{
    return construct_zmq_bus();
}


MessageBus::MessageBus(std::unique_ptr<messaging::impl::MessageBusImpl> &&impl) : impl_(std::move(impl))
{
    if (!impl_) throw std::runtime_error("Unavailable message bus implementation");
}


MessageEndpoint MessageBus::create_endpoint()
{
    return impl_->create_endpoint();
}


size_t MessageBus::step()
{
    return impl_->step();
}


size_t MessageBus::route_messages()
{
    SPDLOG_DEBUG("Message routing cycle started...");
    size_t count = 0;
    impl_->update();
    size_t num_messages;
    while ((num_messages = step())) count += num_messages;

    return count;
}

}  // namespace knp::core
