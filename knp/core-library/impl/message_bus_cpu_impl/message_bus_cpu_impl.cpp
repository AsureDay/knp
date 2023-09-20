/**
 * @file message_bus_single_cpu_impl.cpp
 * @brief Single-threaded CPU message bus implementation.
 * @author Vartenkov A.
 * @date 18.09.2023
 */

#include "message_bus_cpu_impl.h"

#include <utility>

#include "endpoint_cpu_impl.h"

/**
 * @brief Core library namespace.
 */
namespace knp::core
{

class MessageEndpointCPU : public MessageEndpoint
{
public:
    explicit MessageEndpointCPU(std::shared_ptr<MessageEndpointCPUImpl> &&ptr) { impl_ = std::move(ptr); }
};

void MessageBusCPUImpl::update()
{
    std::lock_guard lock(mutex_);
    // This function is called before routing messages.
    auto iter = endpoints_.begin();
    while (iter != endpoints_.end())
    {
        auto endpoint_ptr = iter->lock();
        // Clear up all pointers to expired endpoints.
        if (!endpoint_ptr)
        {
            auto new_iter = iter;
            ++new_iter;
            endpoints_.erase(iter);
            iter = new_iter;
            continue;
        }

        // Read all sent messages to an internal buffer.
        auto new_messages = endpoint_ptr->unload_sent_messages();
        messages_to_route_.insert(
            messages_to_route_.end(), std::make_move_iterator(new_messages.begin()),
            std::make_move_iterator(new_messages.end()));
        ++iter;
    }
}


bool MessageBusCPUImpl::step()
{
    const std::lock_guard lock(mutex_);
    if (messages_to_route_.empty()) return false;  // no more messages left for endpoints to receive.
    // Sending a message to every endpoint.
    auto message = std::move(messages_to_route_.back());
    for (auto endpoint_ptr : endpoints_)
    {
        auto shared_endpoint_ptr = endpoint_ptr.lock();
        // Skipping all endpoints deleted after previous update(). Will delete them at the next update().
        if (!shared_endpoint_ptr) continue;
        shared_endpoint_ptr->add_received_message(message);
    }
    // Remove message from container.
    messages_to_route_.pop_back();
    return true;
}


core::MessageEndpoint MessageBusCPUImpl::create_endpoint()
{
    auto endpoint_impl = std::make_shared<MessageEndpointCPUImpl>();
    const std::lock_guard lock(mutex_);
    endpoints_.push_back(std::weak_ptr(endpoint_impl));
    return MessageEndpointCPU{std::move(endpoint_impl)};
}

}  // namespace knp::core
