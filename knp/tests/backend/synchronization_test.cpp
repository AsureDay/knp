/**
 * @brief Synchronization tests for different backends.
 * @author Vartenkov An.
 * @date 17.05.24.
 */

#include <knp/backends/cpu-multi-threaded/backend.h>
#include <knp/backends/cpu-single-threaded/backend.h>
#include <knp/core/population.h>
#include <knp/framework/synchronization.h>

#include <generators.h>
#include <spdlog/spdlog.h>
#include <tests_common.h>

#include <vector>


namespace knp::testing
{
class STestingBack : public knp::backends::single_threaded_cpu::SingleThreadedCPUBackend
{
public:
    STestingBack() = default;
    void _init() override { knp::backends::single_threaded_cpu::SingleThreadedCPUBackend::_init(); }
};


class MTestingBack : public knp::backends::multi_threaded_cpu::MultiThreadedCPUBackend
{
public:
    MTestingBack() = default;
    void _init() override { knp::backends::multi_threaded_cpu::MultiThreadedCPUBackend::_init(); }
};

}  // namespace knp::testing


TEST(SynchronizationSuite, SingleThreadCpuTest)
{
    // Create a single neuron network: input -> input_projection -> population <=> loop_projection
    knp::testing::STestingBack backend;
    knp::testing::BLIFATPopulation population{knp::testing::neuron_generator, 1};
    auto loop_projection =
        knp::testing::DeltaProjection{population.get_uid(), population.get_uid(), knp::testing::synapse_generator, 1};
    auto input_projection = knp::testing::DeltaProjection{
        knp::core::UID{false}, population.get_uid(), knp::testing::input_projection_gen, 1};

    backend.load_populations({population});
    backend.load_projections({input_projection, loop_projection});
    backend._init();

    auto network = knp::framework::synchronization::get_network_copy(backend);
    ASSERT_EQ(network.get_projections().size(), 2);
    ASSERT_EQ(network.get_populations().size(), 1);
    auto &proj0 = std::get<knp::testing::DeltaProjection>(network.get_projections()[0]);
    auto &proj1 = std::get<knp::testing::DeltaProjection>(network.get_projections()[1]);
    auto &pop = std::get<knp::testing::BLIFATPopulation>(network.get_populations()[0]);
    ASSERT_EQ(proj0.size(), 1);
    ASSERT_EQ(proj1.size(), 1);
    ASSERT_EQ(pop.size(), 1);
}


TEST(SynchronizationSuite, MultiThreadCpuTest)
{
    // Create a single neuron network: input -> input_projection -> population <=> loop_projection
    knp::testing::MTestingBack backend;
    knp::testing::BLIFATPopulation population{knp::testing::neuron_generator, 1};
    auto loop_projection =
        knp::testing::DeltaProjection{population.get_uid(), population.get_uid(), knp::testing::synapse_generator, 1};
    auto input_projection = knp::testing::DeltaProjection{
        knp::core::UID{false}, population.get_uid(), knp::testing::input_projection_gen, 1};

    backend.load_populations({population});
    backend.load_projections({input_projection, loop_projection});
    backend._init();

    auto network = knp::framework::synchronization::get_network_copy(backend);
    ASSERT_EQ(network.get_projections().size(), 2);
    ASSERT_EQ(network.get_populations().size(), 1);
    auto &proj0 = std::get<knp::testing::DeltaProjection>(network.get_projections()[0]);
    auto &proj1 = std::get<knp::testing::DeltaProjection>(network.get_projections()[1]);
    auto &pop = std::get<knp::testing::BLIFATPopulation>(network.get_populations()[0]);
    ASSERT_EQ(proj0.size(), 1);
    ASSERT_EQ(proj1.size(), 1);
    ASSERT_EQ(pop.size(), 1);
}
