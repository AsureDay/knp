/**
 * @file network_io.cpp
 * @brief Python bindings for saving and loading network.
 * @kaspersky_support Vartenkov A.
 * @date 05.06.2024
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
#include "common.h"

#ifdef KNP_IN_BASE_FW

py::def("save_network", &knp::framework::sonata::save_network, "Save network to disk.");

py::def("load_network", &knp::framework::sonata::load_network, "Load network from disk.");

#endif  // KNP_IN_BASE_FW
