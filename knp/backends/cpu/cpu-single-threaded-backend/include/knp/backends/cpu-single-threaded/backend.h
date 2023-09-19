/**
 * @file backend.h
 * @brief Single threaded CPU backend class definition.
 * @author Artiom N.
 * @date 30.01.2023
 */

#pragma once

#include <knp/core/backend.h>
#include <knp/core/population.h>
#include <knp/core/projection.h>
#include <knp/devices/cpu.h>
#include <knp/neuron-traits/all_traits.h>
#include <knp/synapse-traits/all_traits.h>

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <boost/config.hpp>
#include <boost/dll/alias.hpp>
#include <boost/mp11.hpp>


/**
 * @brief Namespace for single-threaded backend.
 */
namespace knp::backends::single_threaded_cpu
{
/**
 * @brief The SingleThreadedCPUBackend class is a definition of an interface to the single-threaded CPU backend.
 */
class SingleThreadedCPUBackend : public knp::core::Backend
{
public:
    /**
     * @brief List of neuron types supported by the single-threaded CPU backend.
     */
    using SupportedNeurons = boost::mp11::mp_list<knp::neuron_traits::BLIFATNeuron>;

    /**
     * @brief List of synapse types supported by the single-threaded CPU backend.
     */
    using SupportedSynapses = boost::mp11::mp_list<
        knp::synapse_traits::DeltaSynapse, knp::synapse_traits::AdditiveSTDPDeltaSynapse,
        knp::synapse_traits::SynapticResourceSTDPDeltaSynapse>;

    /**
     * @brief List of supported population types based on neuron types specified in `SupportedNeurons`.
     */
    using SupportedPopulations = boost::mp11::mp_transform<knp::core::Population, SupportedNeurons>;

    /**
     * @brief List of supported projection types based on synapse types specified in `SupportedSynapses`.
     */
    using SupportedProjections = boost::mp11::mp_transform<knp::core::Projection, SupportedSynapses>;
    /**
     * @brief Population variant that contains any population type specified in `SupportedPopulations`.
     * @details `PopulationVariants` takes the value of `std::variant<PopulationType_1,..., PopulationType_n>`, where
     * `PopulationType_[1..n]` is the population type specified in `SupportedPopulations`. \n
     * For example, if `SupportedPopulations` contains BLIFATNeuron and IzhikevichNeuron types,
     * then `PopulationVariants = std::variant<BLIFATNeuron, IzhikevichNeuron>`. \n
     * `PopulationVariants` retains the same order of message types as defined in `SupportedPopulations`.
     * @see ALL_NEURONS.
     */
    using PopulationVariants = boost::mp11::mp_rename<SupportedPopulations, std::variant>;

    /**
     * @brief Projection variant that contains any projection type specified in `SupportedProjections`.
     * @details `ProjectionVariants` takes the value of `std::variant<ProjectionType_1,..., ProjectionType_n>`, where
     * `ProjectionType_[1..n]` is the projection type specified in `SupportedProjections`. \n
     * For example, if `SupportedProjections` contains DeltaSynapse and AdditiveSTDPSynapse types,
     * then `ProjectionVariants = std::variant<DeltaSynapse, AdditiveSTDPSynapse>`. \n
     * `ProjectionVariants` retains the same order of message types as defined in `SupportedProjections`.
     * @see ALL_SYNAPSES.
     */
    using ProjectionVariants = boost::mp11::mp_rename<SupportedProjections, std::variant>;

private:
    struct ProjectionWrapper
    {
        ProjectionVariants arg_;
        core::messaging::SynapticMessageQueue messages_;
    };

public:
    /**
     * @brief Type of population container.
     */
    using PopulationContainer = std::vector<PopulationVariants>;
    /**
     * @brief Type of projection container.
     */
    using ProjectionContainer = std::vector<ProjectionWrapper>;

    // TODO: Make custom iterators.

    /**
     * @brief Types of population iterators.
     */
    using PopulationIterator = typename PopulationContainer::iterator;
    /**
     * @brief Types of constant population iterators.
     */
    using PopulationConstIterator = typename PopulationContainer::const_iterator;

    /**
     * @brief Types of projection iterators.
     */
    using ProjectionIterator = typename ProjectionContainer::iterator;
    /**
     * @brief Types of constant projection iterators.
     */
    using ProjectionConstIterator = typename ProjectionContainer::const_iterator;

public:
    /**
     * @brief Default constructor for single-threaded CPU backend.
     */
    SingleThreadedCPUBackend();
    /**
     * @brief Destructor for single-threaded CPU backend.
     */
    ~SingleThreadedCPUBackend() = default;

public:
    /**
     * @brief Create an object of the single-threaded CPU backend.
     */
    static std::shared_ptr<SingleThreadedCPUBackend> create();

public:
    /**
     * @brief Define if plasticity is supported.
     * @return `true` if plasticity is supported, `false` if plasticity is not supported.
     */
    [[nodiscard]] bool plasticity_supported() const override { return true; }
    /**
     * @brief Get type names of supported neurons.
     * @return vector of supported neuron type names.
     */
    [[nodiscard]] std::vector<std::string> get_supported_neurons() const override;
    /**
     * @brief Get type names of supported synapses.
     * @return vector of supported synapse type names.
     */
    [[nodiscard]] std::vector<std::string> get_supported_synapses() const override;
    /**
     * @brief Get indexes of supported projections.
     */
    [[nodiscard]] std::vector<size_t> get_supported_projection_indexes() const override;
    /**
     * @brief Get indexes of supported populations.
     */
    [[nodiscard]] std::vector<size_t> get_supported_population_indexes() const override;

public:
    /**
     * @brief Load populations to the backend.
     * @param populations vector of populations to load.
     */
    void load_populations(const std::vector<PopulationVariants> &populations);

    /**
     * @brief Load projections to the backend.
     * @param projections vector of projections to load.
     */
    void load_projections(const std::vector<ProjectionVariants> &projections);

    /**
     * @brief Add projections to backend.
     * @throw exception if the `projections` parameter contains unsupported projection types.
     * @param projections projections to add.
     */
    void load_all_projections(const std::vector<knp::core::AllProjectionsVariant> &projections) override;

    /**
     * @brief Add populations to backend.
     * @throw exception if the `populations` parameter contains unsupported population types.
     * @param populations populations to add.
     */
    void load_all_populations(const std::vector<knp::core::AllPopulationsVariant> &populations) override;

public:
    /**
     * @brief Get an iterator pointing to the first element of the population loaded to backend.
     * @return population iterator.
     */
    PopulationIterator begin_populations();

    /**
     * @brief Get an iterator pointing to the first element of the population loaded to backend.
     * @return constant population iterator.
     */
    PopulationConstIterator begin_populations() const;
    /**
     * @brief Get an iterator pointing to the last element of the population.
     * @return iterator.
     */
    PopulationIterator end_populations();
    /**
     * @brief Get a constant iterator pointing to the last element of the population.
     * @return iterator.
     */
    PopulationConstIterator end_populations() const;

    // TODO: make iterator, which is returns projections, but not a wrapper.
    /**
     * @brief Get an iterator pointing to the first element of the projection loaded to backend.
     * @return projection iterator.
     */
    ProjectionIterator begin_projections();
    /**
     * @brief Get an iterator pointing to the first element of the projection loaded to backend.
     * @return constant projection iterator.
     */
    ProjectionConstIterator begin_projections() const;
    /**
     * @brief Get an iterator pointing to the last element of the projection.
     * @return iterator.
     */
    ProjectionIterator end_projections();
    /**
     * @brief Get a constant iterator pointing to the last element of the projection.
     * @return iterator.
     */
    ProjectionConstIterator end_projections() const;

public:
    /**
     * @brief Remove projections with given UIDs from the backend.
     * @param uids UIDs of projections to remove.
     */
    void remove_projections(const std::vector<knp::core::UID> &uids) override {}

    /**
     * @brief Remove populations with given UIDs from the backend.
     * @param uids UIDs of populations to remove.
     */
    void remove_populations(const std::vector<knp::core::UID> &uids) override {}

public:
    /**
     * @brief Get a list of devices supported by the backend.
     * @return list of devices.
     * @see Device.
     */
    [[nodiscard]] std::vector<std::unique_ptr<knp::core::Device>> get_devices() const override;

public:
    /**
     * @copydoc knp::core::Backend::step()
     */
    void step() override;

    /**
     * @brief Subscribe internal endpoint to messages.
     * @details The method is used to get a subscription necessary for receiving messages of the specified type.
     * @tparam MessageType message type.
     * @param receiver receiver UID.
     * @param senders list of possible sender UIDs.
     * @return subscription.
     */
    template <typename MessageType>
    knp::core::Subscription<MessageType> &subscribe(
        const knp::core::UID &receiver, const std::vector<knp::core::UID> &senders)
    {
        return message_endpoint_.subscribe<MessageType>(receiver, senders);
    }

    /**
     * @brief Get message endpoint.
     * @return message endpoint.
     */
    const core::MessageEndpoint &get_message_endpoint() const override { return message_endpoint_; }
    core::MessageEndpoint &get_message_endpoint() override { return message_endpoint_; }

protected:
    /**
     * @copydoc knp::core::Backend::init()
     */
    void init() override;

    /**
     * @brief Calculate population of BLIFAT neurons.
     * @note Population will be changed during calculation.
     * @param population population of BLIFAT neurons to calculate.
     */
    void calculate_population(knp::core::Population<knp::neuron_traits::BLIFATNeuron> &population);
    /**
     * @brief Calculate projection of Delta synapses.
     * @note Projection will be changed during calculation.
     * @param projection projection of Delta synapses to calculate.
     * @param message_queue message queue to send to projection for calculation.
     */
    void calculate_projection(
        knp::core::Projection<knp::synapse_traits::DeltaSynapse> &projection,
        core::messaging::SynapticMessageQueue &message_queue);
    /**
     * @brief Calculate projection of AdditiveSTDPDelta synapses.
     * @note Projection will be changed during calculation.
     * @param projection projection to calculate.
     * @param message_queue message queue to send to projection for calculation.
     */
    void calculate_projection(
        knp::core::Projection<knp::synapse_traits::AdditiveSTDPDeltaSynapse> &projection,
        core::messaging::SynapticMessageQueue &message_queue);
    /**
     * @brief Calculate projection of STDPSynapticResourceSynapse synapses.
     * @note Projection will be changed during calculation.
     * @param projection projection to calculate.
     * @param message_queue message queue to send to projection for calculation.
     */
    void calculate_projection(
        knp::core::Projection<knp::synapse_traits::SynapticResourceSTDPDeltaSynapse> &projection,
        core::messaging::SynapticMessageQueue &message_queue);

private:
    // cppcheck-suppress unusedStructMember
    PopulationContainer populations_;
    // cppcheck-suppress unusedStructMember
    ProjectionContainer projections_;
    core::MessageEndpoint message_endpoint_;
};

BOOST_DLL_ALIAS(knp::backends::single_threaded_cpu::SingleThreadedCPUBackend::create, create_knp_backend)

}  // namespace knp::backends::single_threaded_cpu
