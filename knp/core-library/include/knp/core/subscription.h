/**
 * @file subscription.h
 * @brief Subscription class that determines message exchange between entities in the network.
 * @author Vartenkov A.
 * @date 15.03.2023
 */

#pragma once
#include <knp/core/uid.h>

#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>

#include <boost/functional/hash.hpp>


namespace knp::core
{

/**
 * @brief The Subscription class is used for message exchange between the network entities.
 * @tparam MessageT type of messages that are exchanged via the subscription.
 */
template <class MessageT>
class Subscription
{
public:
    /**
     * @brief Message type.
     */
    using MessageType = MessageT;
    /**
     * @brief Internal container for messages of the specified message type.
     */
    using MessageContainerType = std::vector<MessageType>;

    /**
     * @brief Internal container for UIDs.
     */
    using UidSet = std::unordered_set<::boost::uuids::uuid, boost::hash<boost::uuids::uuid>>;
    // Subscription(const Subscription &) = delete;

public:
    /**
     * @brief Subscription constructor.
     * @param receiver receiver UID.
     * @param senders list of sender UIDs.
     */
    Subscription(const UID &receiver, const std::vector<UID> &senders) : receiver_(receiver) { add_senders(senders); }

    /**
     * @brief Get list of sender UIDs.
     * @return senders UIDs.
     */
    [[nodiscard]] const UidSet &get_senders() const { return senders_; }

    /**
     * @brief Get UID of the entity that receives messages via the subscription.
     * @return UID.
     */
    [[nodiscard]] UID get_receiver_uid() const { return receiver_; }

    /**
     * @brief Unsubscribe from a sender.
     * @details If a sender is not associated with the subscription, the method does not do anything.
     * @param uid sender UID.
     * @return number of senders deleted from subscription.
     */
    size_t remove_sender(const UID &uid) { return senders_.erase(static_cast<boost::uuids::uuid>(uid)); }

    /**
     * @brief Add a sender with the given UID to the subscription.
     * @details If a sender is already associated with the subscription, the method does not do anything.
     * @param uid UID of the new sender.
     * @return number of senders added.
     */
    size_t add_sender(const UID &uid) { return senders_.insert(static_cast<boost::uuids::uuid>(uid)).second; }

    /**
     * @brief Add several senders to the subscription.
     * @param senders vector of sender UIDs.
     * @return number of senders added.
     */
    size_t add_senders(const std::vector<UID> &senders)
    {
        size_t size_before = senders_.size();
        std::copy(senders.begin(), senders.end(), std::inserter(senders_, senders_.end()));
        return senders_.size() - size_before;
    }

    /**
     * @brief Check if a sender with the given UID exists.
     * @param uid sender UID.
     * @return `true` if the sender with the given UID exists, `false` if the sender with the given UID doesn't exist.
     */
    [[nodiscard]] bool has_sender(const UID &uid) const
    {
        return senders_.find(static_cast<boost::uuids::uuid>(uid)) != senders_.end();
    }

public:
    /**
     * @brief Add a message to the subscription.
     * @param message message to add.
     */
    void add_message(MessageType &&message) { messages_.push_back(message); }
    /**
     * @brief Add a message to the subscription.
     * @param message constant message to add.
     */
    void add_message(const MessageType &message) { messages_.push_back(message); }

    /**
     * @brief Get all messages.
     * @return reference to message container.
     */
    MessageContainerType &get_messages() { return messages_; }
    /**
     * @brief Get all messages.
     * @return constant reference to message container.
     */
    const MessageContainerType &get_messages() const { return messages_; }

    /**
     * @brief Remove all stored messages.
     */
    void clear_messages() { messages_.clear(); }

private:
    /**
     * @brief Receiver UID.
     */
    const UID receiver_;

    /**
     * @brief Set of sender UIDs.
     */
    std::unordered_set<::boost::uuids::uuid, boost::hash<boost::uuids::uuid>> senders_;
    /**
     * @brief Message storage.
     */
    MessageContainerType messages_;
};

}  // namespace knp::core
