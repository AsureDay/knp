/**
 * @file message_endpoint_cpu_impl.h
 * @brief CPU endpoint implementation header.
 * @kaspersky_support Vartenkov A.
 * @date 18.09.2023
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
#pragma once

#include <message_endpoint_impl.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

/**
 * @brief Namespace for implementations of message bus.
 */
namespace knp::core::messaging::impl
{

/**
 * @brief Endpoint implementation class for CPU message bus.
 * @note It should never be used explicitly.
 */
class MessageEndpointCPUImpl : public MessageEndpointImpl
{
public:
    MessageEndpointCPUImpl(
        std::shared_ptr<std::vector<messaging::MessageVariant>> messages_to_send,
        std::shared_ptr<std::vector<messaging::MessageVariant>> received_messages)
        : messages_to_send_(std::move(messages_to_send)), received_messages_(std::move(received_messages))
    {
    }

    void send_message(const knp::core::messaging::MessageVariant &message) override
    {
        const std::lock_guard lock(mutex_);

        messages_to_send_->push_back(message);
        SPDLOG_TRACE("Message was sent, type index = {}.", message.index());
    }

    ~MessageEndpointCPUImpl() override = default;

    /**
     * @brief Read all the messages queued to be sent, then clear message container.
     * @return vector of messages to be sent to other endpoints.
     */
    [[nodiscard]] std::vector<knp::core::messaging::MessageVariant> unload_sent_messages()
    {
        const std::lock_guard lock(mutex_);
        auto result = std::move(*messages_to_send_);

        messages_to_send_->clear();
        return result;
    }

    // TODO: Embarrassingly inefficient: all endpoints basically receive all messages by copying them. It needs to be
    // optimized.
    void add_received_messages(const std::vector<knp::core::messaging::MessageVariant> &incoming_messages)
    {
        const std::lock_guard lock(mutex_);

        received_messages_->insert(received_messages_->end(), incoming_messages.begin(), incoming_messages.end());
    }

    void add_received_message(knp::core::messaging::MessageVariant &&incoming)
    {
        const std::lock_guard lock(mutex_);

        received_messages_->emplace_back(incoming);
    }

    void add_received_message(const knp::core::messaging::MessageVariant &incoming)
    {
        const std::lock_guard lock(mutex_);

        received_messages_->push_back(incoming);
    }

    std::optional<knp::core::messaging::MessageVariant> receive_message() override
    {
        const std::lock_guard lock(mutex_);

        if (received_messages_->empty())
        {
            return {};
        }

        auto result = std::move(received_messages_->back());
        received_messages_->pop_back();
        return result;
    }

private:
    std::shared_ptr<std::vector<messaging::MessageVariant>> messages_to_send_;
    std::shared_ptr<std::vector<messaging::MessageVariant>> received_messages_;
    std::mutex mutex_;
};

}  // namespace knp::core::messaging::impl
