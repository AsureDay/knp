/**
 * @file model_loader.h
 * @brief Model loader interface.
 * @author Artiom N.
 * @date 21.04.2023
 */

#pragma once

#include <knp/core/impexp.h>
#include <knp/framework/io/input_converter.h>
#include <knp/framework/model.h>
#include <knp/framework/monitoring/observer.h>

#include <filesystem>
#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>


/**
 * @brief Framework namespace.
 */
namespace knp::framework
{
/**
 * @brief The ModelLoader class is a definition of a loader that uploads the model to the specified backend.
 */
class ModelLoader
{
public:
    /**
     * @brief Type of the input channel map.
     */
    using InputChannelMap = std::unordered_map<core::UID, io::input::DataGenerator, core::uid_hash>;

public:
    /**
     * @brief Model loader constructor.
     * @param backend pointer to backend to which you want to load the model.
     * @param i_map input channel map.
     */
    ModelLoader(std::shared_ptr<core::Backend> backend, InputChannelMap i_map)
        : backend_(backend), i_map_(std::move(i_map))
    {
    }

public:
    /**
     * @brief Write model to backend.
     * @param model model to load.
     */
    void load(knp::framework::Model &model);

public:
    /**
     * @brief Get reference to output channel.
     * @param channel_uid channel UID.
     * @return reference to output channel.
     * @throw std::runtime_error if there is no channel with a given UID.
     */
    io::output::OutputChannel &get_output_channel(const core::UID &channel_uid);
    /**
     * @brief Get reference to output channel.
     * @note Constant method.
     * @param channel_uid channel UID.
     * @return reference to output channel.
     * @throw std::runtime_error if there is no channel with a given UID.
     */
    const io::output::OutputChannel &get_output_channel(const core::UID &channel_uid) const;

    /**
     * @brief Get reference to input channel by its UID.
     * @param channel_uid channel UID.
     * @return reference to input channel.
     * @throw std::runtime_error if no channel with the given UID exists.
     */
    io::input::InputChannel &get_input_channel(const core::UID &channel_uid);
    /**
     * @brief Get reference to input channel by its UID.
     * @note Constant method.
     * @param channel_uid channel UID.
     * @return reference to input channel.
     * @throw std::runtime_error if no channel with the given UID exists.
     */
    const io::input::InputChannel &get_input_channel(const core::UID &channel_uid) const;

    /**
     * @brief Get input channels.
     * @return tuple of input channel vectors.
     */
    auto &get_inputs() noexcept { return in_channels_; }

    /**
     * @brief Get output channels.
     * @return tuple of output channel vectors.
     */
    auto &get_outputs() noexcept { return out_channels_; }

    /**
     * @brief Get pointer to backend object.
     * @return shared pointer to `Backend` object.
     */
    std::shared_ptr<core::Backend> get_backend() { return backend_; }

private:
    template <typename GenType>
    void init_channels(
        knp::framework::Model &model, const std::unordered_multimap<core::UID, core::UID, core::uid_hash> &channels,
        GenType channel_gen);

    void gen_input_channel(knp::framework::Model &model, const core::UID &, const std::vector<core::UID> &);
    void gen_output_channel(knp::framework::Model &model, const core::UID &, const std::vector<core::UID> &);

private:
    knp::core::BaseData base_;
    std::shared_ptr<core::Backend> backend_;

    InputChannelMap i_map_;
    // cppcheck-suppress unusedStructMember
    std::vector<knp::framework::io::input::InputChannel> in_channels_;
    // cppcheck-suppress unusedStructMember
    std::vector<knp::framework::io::output::OutputChannel> out_channels_;
};
}  // namespace knp::framework
