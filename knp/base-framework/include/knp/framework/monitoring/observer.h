/**
 * @file observer.h
 * @brief Observer class that unloads messages and stores them.
 * @author Vartenkov A.
 * @date 23.08.2023
 */
#pragma once
#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/mp11.hpp>

#include "knp/core/message_endpoint.h"
#include "knp/core/messaging/messaging.h"

/**
 * @brief Monitoring namespace.
 */
namespace knp::framework::monitoring
{
/**
 * @brief Functor for message processing.
 * @tparam Message type of messages the functor processes.
 */
template <class Message>
using MessageProcessor = std::function<void(const std::vector<Message>)>;


/**
 * @brief The MessageObserver class is a definition of an observer that receives messages and processes them.
 * @tparam Message message type that is processed by an observer.
 * @note Use this class for statistics calculation or for information output.
 */
template <class Message>
class MessageObserver
{
public:
    /**
     * @brief Constructor.
     * @param endpoint endpoint from which to get messages.
     * @param processor functor to process messages.
     * @param uid observer UID.
     */
    MessageObserver(
        core::MessageEndpoint &&endpoint, MessageProcessor<Message> &&processor, core::UID uid = core::UID{true})
        : endpoint_(std::move(endpoint)), process_messages_(processor), uid_(uid)
    {
    }

    /**
     * @brief Subscribe to messages.
     * @param entities message senders.
     */
    void subscribe(const std::vector<core::UID> &entities) { endpoint_.subscribe<Message>(uid_, entities); }

    /**
     * @brief Receive and process messages.
     */
    void update()
    {
        endpoint_.receive_all_messages();
        auto messages_raw = endpoint_.unload_messages<Message>(uid_);
        process_messages_(messages_raw);
    }

private:
    core::MessageEndpoint endpoint_;
    MessageProcessor<Message> process_messages_;
    core::UID uid_;
};

/**
 * @brief List of all possible observers.
 */
using AllObservers = boost::mp11::mp_transform<MessageObserver, core::messaging::AllMessages>;

/**
 * @brief Observer variant that contains all possible observers.
 */
using AnyObserverVariant = boost::mp11::mp_rename<AllObservers, std::variant>;

#define INSTANCE_OBSERVERS(n, template_for_instance, message_type) template class MessageObserver<message_type>;
BOOST_PP_SEQ_FOR_EACH(INSTANCE_OBSERVERS, "", BOOST_PP_VARIADIC_TO_SEQ(ALL_MESSAGES))

}  // namespace knp::framework::monitoring
