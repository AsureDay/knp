/**
 * @brief Input channel header
 * @author Vartenkov Andrey
 * @date 04.05.2023
 */
#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "input_converter.h"


/**
 * @brief Input channel namespace.
 */
namespace knp::framework::input
{

/**
 * @brief The InputChannel class is a base class for input channels.
 */
class InputChannel
{
public:
    /**
     * @brief Input channel constructor.
     * @param channel_uid sender UID to put into the message header.
     * @param endpoint endpoint used to send messages.
     */
    InputChannel(core::MessageEndpoint &&endpoint, core::UID channel_uid)
        : endpoint_(std::move(endpoint)), uid_(channel_uid)
    {
    }

    virtual ~InputChannel() = default;

    /**
     * @brief Get input channel UID.
     * @return input channel UID.
     */
    [[nodiscard]] const core::UID &get_uid() const { return uid_; }

    /**
     * @brief Read data from input stream, form a spike message and send it to an endpoint.
     * @note The method throws exceptions if an input stream is set to throw exceptions.
     * @param step current step.
     * @return true if message was sent, false if no message was sent.
     */
    virtual bool send(core::messaging::Step step) = 0;

protected:
    /**
     * @brief Send spikes to the endpoint.
     * @param spikes spikes to send.
     * @param step current step.
     * @return true if message was sent, false otherwise.
     */
    bool send_data(const core::messaging::SpikeData &spikes, core::messaging::Step step)
    {
        if (spikes.empty()) return false;

        core::messaging::SpikeMessage message{{uid_, step}, std::move(spikes)};
        endpoint_.send_message(message);
        return true;
    }

private:
    /**
     * @brief A reference to the endpoint used by channel to send messages.
     */
    core::MessageEndpoint endpoint_;

    /**
     * @brief Channel own UID, used as a sender UID for messages.
     */
    const core::UID uid_;
};


/**
 * @brief The InputStreamChannel defines an input channel that provides a connected entity (for example, an input
 * projection) with spike messages based on the input stream data.
 * @details You need to create an input stream channel, associate it with an input stream and provide the stream with
 * input data. After constructing an input channel, connect it with an input entity (for example, a projection) using
 * the `connect_input(channel, target_endpoint, receiver_uid)` method. To send the input data to a connected entity,
 * call the `send(time)` method.
 */
class InputStreamChannel : public InputChannel
{
public:
    /**
     * @brief Input stream channel constructor.
     * @param stream stream from which to receive data.
     * @param converter functor that generates spike messages based on data from the input stream.
     * @param channel_uid sender UID to put into the message header.
     * @param endpoint endpoint used to send messages.
     */
    InputStreamChannel(
        std::unique_ptr<std::istream> &&stream, core::MessageEndpoint &&endpoint, DataConverter converter,
        core::UID channel_uid = core::UID{true})
        : InputChannel(std::move(endpoint), channel_uid), stream_(std::move(stream)), converter_(std::move(converter))
    {
    }

    /**
     * @brief Get input stream.
     * @return stream.
     */
    [[nodiscard]] std::istream &get_stream() { return *stream_; }

    /**
     * @brief Read data from input stream, form a spike message and send it to an endpoint.
     * @note The method throws exceptions if an input stream is set to throw exceptions.
     * @param step current step.
     * @return true if message was sent, false if no message was sent.
     */
    bool send(core::messaging::Step step) override { return send_data(converter_(*stream_), step); }

private:
    /**
     * @brief A reference to the stream from which to read data.
     */
    std::unique_ptr<std::istream> stream_;

    /**
     * @brief Converter functor: gets stream, returns a list of spikes.
     */
    DataConverter converter_;
};


/**
 * @brief The InputStreamChannel defines an input channel that provides a connected entity (for example, an input
 * projection) with spike messages based on the input stream data
 */
class InputGenChannel : public InputChannel
{
public:
    /**
     * @brief Input generator channel constructor.
     * @param generator functor that generates spike messages.
     * @param channel_uid sender UID to put into the message header.
     * @param endpoint endpoint used to send messages.
     */
    InputGenChannel(core::MessageEndpoint &&endpoint, DataGenerator generator, core::UID channel_uid = core::UID{true})
        : InputChannel(std::move(endpoint), channel_uid), generator_(std::move(generator))
    {
    }

    /**
     * @brief Generate spike message and send it to an endpoint.
     * @param step current step.
     * @return true if message was sent, false if no message was sent.
     */
    bool send(core::messaging::Step step) override { return send_data(generator_(), step); }

private:
    /**
     * @brief Generator functor
     * @return list of spikes.
     */
    DataGenerator generator_;
};


/**
 * @brief Connect input channel to a target entity.
 * @note Target entity should be able to receive spikes.
 * @param channel input channel.
 * @param target_endpoint endpoint to use for message exchange.
 * @param receiver_uid UID of an entity that receives spike messages.
 */
inline void connect_input(
    const InputChannel &channel, core::MessageEndpoint &target_endpoint, const core::UID &receiver_uid)
{
    target_endpoint.subscribe<core::messaging::SpikeMessage>(receiver_uid, {channel.get_uid()});
}

}  // namespace knp::framework::input
