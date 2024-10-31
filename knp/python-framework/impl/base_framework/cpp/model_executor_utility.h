/**
 * @file model_executor_utility.h
 * @brief Auxiliary functions for ModelExecutor class bindings.
 * @kaspersky_support Vartenkov A.
 * @date 31.10.2024
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
#include <knp/core/backend.h>
#include <knp/framework/model_executor.h>

#include <memory>
#include <utility>
#include <vector>


auto create_model_executor(
    knp::framework::Model &model, std::shared_ptr<knp::core::Backend> &backend,
    const knp::framework::ModelLoader::InputChannelMap &i_map)
{
    return std::make_shared<knp::framework::ModelExecutor>(model, backend, i_map);
}

void start_model_executor(knp::framework::ModelExecutor &self)
{
    self.start();
}


void start_model_executor_predicate(
    knp::framework::ModelExecutor &self, const knp::core::Backend::RunPredicate &predicate)
{
    self.start(predicate);
}


void add_executor_spike_observer(
    knp::framework::ModelExecutor &self,
    knp::framework::monitoring::MessageProcessor<knp::core::messaging::SpikeMessage> &message_processor,
    const std::vector<knp::core::UID> &senders)
{
    self.add_observer(std::move(message_processor), senders);
}


void add_executor_impact_observer(
    knp::framework::ModelExecutor &self,
    knp::framework::monitoring::MessageProcessor<knp::core::messaging::SynapticImpactMessage> &message_processor,
    const std::vector<knp::core::UID> &senders)
{
    self.add_observer(std::move(message_processor), senders);
}
