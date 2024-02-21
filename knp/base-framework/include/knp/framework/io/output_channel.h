/**
 * @file output_channel.h
 * @brief Header for output channel.
 * @author Vartenkov Andrey
 * @date 11.05.2023
 */
#pragma once

#include <knp/core/core.h>
#include <knp/core/message_endpoint.h>
#include <knp/core/messaging/messaging.h>

#include <algorithm>
#include <utility>
#include <vector>

#include "output_converter.h"


/**
 * @brief Output channel namespace.
 */
namespace knp::framework::output
{
/**
 * @brief The OutputChannel class is a definition of an output channel.
 */
class OutputChannel
{
public:
    /**
     * @brief Base output channel constructor.
     * @param channel_uid output channel UID.
     * @param endpoint endpoint to use for message exchange.
     */
    OutputChannel(const core::UID &channel_uid, core::MessageEndpoint &&endpoint)
        : base_{channel_uid}, endpoint_(std::move(endpoint))
    {
    }

    /**
     * @brief Move constructor.
     */
    OutputChannel(OutputChannel &&) = default;

public:
    /**
     * @brief Get backend UID.
     * @return backend UID.
     */
    [[nodiscard]] const auto &get_uid() const { return base_.uid_; }
    /**
     * @brief Get tags used by the backend.
     * @return backend tag map.
     * @see TagMap.
     */
    [[nodiscard]] auto &get_tags() { return base_.tags_; }

public:
    /**
     * @brief Unload spike messages from the endpoint into the message buffer.
     * @details You should call the method before reading data from the channel.
     */
    std::vector<core::messaging::SpikeMessage> update();

    /**
     * @brief Read a specified interval of messages from sorted internal message buffer.
     * @param starting_step step from which the method starts reading spike messages.
     * @param final_step step after which the method stops reading spike messages.
     * @return vector of messages sent on the specified interval of steps.
     */
    std::vector<core::messaging::SpikeMessage> read_some_from_buffer(core::Step starting_step, core::Step final_step);

protected:
    /**
     * @brief Base data.
     */
    core::BaseData base_;

    /**
     * @brief Endpoint used for message exchange.
     */
    core::MessageEndpoint endpoint_;

    /**
     * @brief Messages received from output population.
     */
    std::vector<core::messaging::SpikeMessage> message_buffer_;
};


/**
 * @brief Read all accumulated spike messages from subscription and convert them to output data.
 * @param output_channel output channel object.
 * @param converter data converter.
 * @param step_from network step from which the method starts reading spike messages.
 * @param step_to network step after which the method stops reading spike messages.
 * @return output data in the required format.
 */
template <typename ResultType>
[[nodiscard]] ResultType output_channel_get(
    OutputChannel &output_channel, OutputConverter<ResultType> converter, core::Step step_from, core::Step step_to)
{
    output_channel.update();
    return converter(output_channel.read_some_from_buffer(step_from, step_to));
}

}  // namespace knp::framework::output
