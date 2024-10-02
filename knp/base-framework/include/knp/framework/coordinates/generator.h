/**
 * @file generator.h
 * @brief Coordinate generator.
 * @author Artiom N.
 * @date 22.03.2023
 * @license Apache 2.0
 * @copyright © 2024 AO Kaspersky Lab
 */

#pragma once

#include <any>
#include <functional>


/**
 * @brief Namespace for framework coordinates.
 */
namespace knp::framework::coordinates
{
/**
 * @brief Type of coordinate generator.
 * @param index neuron index.
 */
using CoordinateGenerator = std::function<std::any(size_t index)>;

}  // namespace knp::framework::coordinates
