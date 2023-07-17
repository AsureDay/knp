/**
 * @file model.h
 * @brief Model interface.
 * @author Artiom N.
 * @date 30.03.2023
 */

#pragma once

#include <knp/core/core.h>
#include <knp/core/impexp.h>
#include <knp/framework/backend_loader.h>
#include <knp/framework/io/input_channel.h>
#include <knp/framework/io/output_channel.h>
#include <knp/framework/network.h>

#include <memory>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>


/**
 * @brief Framework namespace.
 */
namespace knp::framework
{

/**
 * @brief The Model class is a definition of a model that contains a network, input and output channels, a monitor and a
 * backend.
 */
class Model
{
public:
    /**
     * @brief Model constructor.
     * @param network network to use in the model.
     */
    explicit Model(knp::framework::Network &&network) : network_(std::move(network)) {}

public:
    /**
     * @brief Get model UID.
     * @return model UID.
     */
    [[nodiscard]] const knp::core::UID &get_uid() const { return base_.uid_; }
    /**
     * @brief Get tags used by the model.
     * @return model tag map.
     * @see TagMap.
     */
    [[nodiscard]] auto &get_tags() { return base_.tags_; }

public:
    /**
     * @brief Get network associated with the model.
     * @return network.
     */
    [[nodiscard]] knp::framework::Network &get_network() { return network_; }
    /**
     * @brief Get network associated with the model.
     * @note Constant method.
     * @return network.
     */
    [[nodiscard]] const knp::framework::Network &get_network() const { return network_; }

    /**
     * @brief Add an input channel to the network.
     * @param channel input channel to be added.
     * @param projection_uid UID of the projection which will be connected to the channel.
     * @return channel UID.
     */
    core::UID add_input_channel(
        std::unique_ptr<knp::framework::input::InputChannel> &&channel, const core::UID &projection_uid);
    /**
     * @brief Add an output channel to the network.
     * @param channel_ptr pointer to the channel object.
     * @param population_uid UID of the population which will be connected to the channel.
     * @return channel UID.
     */
    core::UID add_output_channel(
        std::unique_ptr<knp::framework::output::OutputChannelBase> &&channel_ptr, const core::UID &population_uid);
    /**
     * @brief Get output channel reference. Cast it to the type you need.
     * @param channel_uid channel UID.
     * @return base output channel class reference. It should be cast to the right type before extracting data from it.
     * @throw std::runtime_error if there is no channel with a given UID.
     */
    output::OutputChannelBase *get_output_channel(const core::UID &channel_uid);
    /**
     * @brief Get an input channel reference by its UID.
     * @param channel_uid channel UID.
     * @return reference to a channel.
     * @throw std::runtime_error if no channel with that UID exists.
     */
    input::InputChannel *get_input_channel(const core::UID &channel_uid);
    /**
     * @brief Return all input channels.
     * @return input channels vector.
     */
    std::vector<std::tuple<input::InputChannel *, core::UID>> get_input_channels() const;
    /**
     * @brief Return all output channels.
     * @return output channels bases vector.
     */
    std::vector<std::tuple<output::OutputChannelBase *, core::UID>> get_output_channels() const;

private:
    knp::core::BaseData base_;
    knp::framework::Network network_;
    std::unordered_map<
        core::UID, std::tuple<std::unique_ptr<knp::framework::input::InputChannel>, core::UID>, core::uid_hash>
        // cppcheck-suppress unusedStructMember
        in_channels_;
    std::unordered_map<
        core::UID, std::tuple<std::unique_ptr<knp::framework::output::OutputChannelBase>, core::UID>, core::uid_hash>
        // cppcheck-suppress unusedStructMember
        out_channels_;
};

}  // namespace knp::framework
