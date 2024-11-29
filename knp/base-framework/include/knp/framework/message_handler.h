/**
 * @file message_handler.h
 * @brief A class that processes a number of messages then sends messages of its own.
 * @kaspersky_support Vartenkov A.
 * @date 19.11.2024
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


#include <knp/core/message_endpoint.h>
#include <knp/core/messaging/messaging.h>

#include <algorithm>
#include <random>
#include <string>
#include <utility>
#include <vector>


namespace knp::framework::modifier
{

/**
 * @brief An object that receives and processes messages.
 */
class SpikeMessageHandler
{
public:
    /**
     * @brief Input message type.
     */
    using MessageIn = knp::core::messaging::SpikeMessage;

    /**
     * @brief Output message type.
     */
    using MessageOut = knp::core::messaging::SpikeMessage;

    /**
     * @brief Functor type.
     */
    using FunctionType = std::function<core::messaging::SpikeData(std::vector<MessageIn> &)>;

    /**
     * @brief Handler constructor.
     * @param function a function that takes a vector of spike messages and returns a vector of spikes.
     * @param endpoint message endpoint.
     * @param uid the uid of this object.
     */
    SpikeMessageHandler(FunctionType &&function, knp::core::MessageEndpoint &&endpoint, const knp::core::UID &uid = {})
        : message_handler_function_(std::move(function)), endpoint_(std::move(endpoint)), base_{uid}
    {
    }

    /**
     * @brief Default move constructor.
     * @param other object to move from.
     */
    SpikeMessageHandler(SpikeMessageHandler &&other) noexcept = default;


    /**
     * @brief Is not copyable.
     */
    SpikeMessageHandler(const SpikeMessageHandler &) = delete;


    /**
     * @brief Subscribe handler to a number of other entities.
     * @param entities network uids.
     * @note For internal use, don't try to call it manually.
     */
    void subscribe(const std::vector<core::UID> &entities) { endpoint_.subscribe<MessageIn>(base_.uid_, entities); }


    /**
     * @brief Read, process and send messages.
     * @param step current step.
     * @note for internal use, don't try to call it manually.
     */
    void update(size_t step);


    /**
     * @brief Get handler UID.
     * @return object UID.
     */
    [[nodiscard]] knp::core::UID get_uid() const { return base_.uid_; };


    /**
     * @brief Get a tag.
     * @param tag_name tag name.
     * @return tag value reference.
     */
    [[nodiscard]] std::any &get_tag(const std::string &tag_name) { return base_.tags_[tag_name]; };

private:
    FunctionType message_handler_function_;
    core::MessageEndpoint endpoint_;
    knp::core::BaseData base_;
};


/**
 * @brief A modifier functor to process spikes and select random K spikes out of the whole set.
 * @note Only processes a single message.
 */
class KWtaRandomHandler
{
public:
    /**
     * @brief Constructor
     * @param winners_number Max number of output spikes.
     * @param seed random generator seed.
     * @note uses mt19937 for random number generation.
     */
    explicit KWtaRandomHandler(size_t winners_number = 1, int seed = 0)
        : num_winners_(winners_number), random_engine_(seed)
    {
    }


    /**
     * @brief operator that takes a number of messages and returns a set of spikes.
     * @param messages spike messages.
     * @return spikes data containing no more than K spikes.
     * @note it's assumed that it gets no more than one message per step, so all messages except first are ignored.
     */
    knp::core::messaging::SpikeData operator()(std::vector<knp::core::messaging::SpikeMessage> &messages);

private:
    size_t num_winners_;
    std::mt19937 random_engine_;
    std::uniform_int_distribution<size_t> distribution_;
};


/**
 * @brief MessageHandler functor that only passes through spikes from no more than a fixed number of groups at once.
 * @note Group is considered to be winning if it is in the top K groups sorted by number of spikes in descending order.
 * @note If last place in the top K is shared between groups, the functor selects random ones among the sharing groups.
 */
class GroupWtaRandomHandler
{
public:
    /**
     * @brief Functor constructor.
     * @param group_borders right borders of the intervals.
     * @param num_winning_groups max number of groups that are allowed to pass their spikes further.
     * @param seed seed for internal random number generator.
     */
    explicit GroupWtaRandomHandler(
        const std::vector<size_t> &group_borders, size_t num_winning_groups = 1, int seed = 0)
        : group_borders_(group_borders), num_winners_(num_winning_groups), random_engine_(seed)
    {
        std::sort(group_borders_.begin(), group_borders_.end());
    }


    /**
     * @brief Functor operator.
     * @param messages input messages.
     * @return spikes from winning groups.
     */
    knp::core::messaging::SpikeData operator()(const std::vector<knp::core::messaging::SpikeMessage> &messages);

private:
    std::vector<size_t> group_borders_;
    size_t num_winners_;
    std::mt19937 random_engine_;
    std::uniform_int_distribution<size_t> distribution_;
};


/**
 * @brief Spike handler functor. An output vector has a spike if that spike was present in at least one input message.
 */
class SpikeUnionHandler
{
public:
    /**
     * @brief Functor operator, receives a vector of messages, returns a union of all spike sets from those messages.
     * @param messages incoming spike messages.
     * @return spikes vector containing the union of input message spike sets.
     */
    knp::core::messaging::SpikeData operator()(const std::vector<knp::core::messaging::SpikeMessage> &messages);
};


}  // namespace knp::framework::modifier
