/**
 * @file uid_marshal.h
 * @brief Implementation of UID marshalling.
 * @kaspersky_support Artiom N.
 * @date 12.10.2024
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


namespace knp::core::messaging
{

inline marshal::UID get_marshaled_uid(const knp::core::UID& uid)
{
    return flatbuffers::span<const uint8_t, decltype(uid.tag)::static_size()>(uid.tag.data, uid.tag.size());
}

}  // namespace knp::core::messaging
