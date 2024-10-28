/**
 * @file visualize_network.h
 * @brief Functions for graph visualization. 
 * @warning Most of the functions are not well-tested or stable yet.
 * @date 26.07.2024
 * @license Apache 2.0
 * @copyright © 2024 AO Kaspersky Lab
 */

#pragma once
#include <knp/framework/network.h>

#include <string>
#include <vector>

#include <opencv2/core/types.hpp>


/**
 * @brief Network description structure for drawing. 
 * @note You can use this to check network structure.
 */
struct NetworkGraph
{
public:
    /**
     * @brief Node description structure.
     */
    struct Node
    {
        /**
         * @brief Population size.
         */
        // cppcheck-suppress unusedStructMember
        size_t size_;

        /**
         * @brief Population UID.
         */
        knp::core::UID uid_;

        /**
         * @brief Population name.
         */
        std::string name_;

        /**
         * @brief Neuron type.
         */
        // cppcheck-suppress unusedStructMember
        size_t type_;
    };

    /**
     * @brief Node vector.
     */
    std::vector<Node> nodes_;

    /**
     * @brief Edge description structure.
     */
    struct Edge
    {
        /**
         * @brief Projection size.
         */
        // cppcheck-suppress unusedStructMember
        size_t size_;

        /**
         * @brief Index of the source population.
         */
        // cppcheck-suppress unusedStructMember
        int index_from_;

        /**
         * @brief Index of the target population.
         */
        // cppcheck-suppress unusedStructMember
        int index_to_;

        /**
         * @brief Projection UID.
         */
        knp::core::UID uid_;

        /**
         * @brief Projection name.
         */
        std::string name_;

        /**
         * @brief Synapse type.
         */
        // cppcheck-suppress unusedStructMember
        size_t type_;
    };

    /**
     * @brief Edge vector.
     */
    std::vector<Edge> edges_;

    /**
     * @brief Build network graph from a network.
     * @param network source network for a graph.
     */
    explicit NetworkGraph(const knp::framework::Network &network);
};


/**
 * @brief Print node and edge connections.
 * @param graph network graph.
 */
void print_network_description(const NetworkGraph &graph);


/**
 * @brief Print whole network information.
 * @note The function prints information not in a human-friendly manner.
 * @param graph network graph.
 */
void print_modified_network_description(const NetworkGraph &graph);


/**
 * @brief Divide network graph into subgraphs.
 * @param graph network graph.
 * @return vector of subgraphs presented by node indexes.
 */
std::vector<std::vector<int>> divide_graph_by_connectivity(const NetworkGraph &graph);


/**
 * @brief Position a subgraph for drawing.
 * @param graph network graph.
 * @param nodes connected subgraph nodes.
 * @param screen_size output window size.
 * @param margin border size for network graph.
 * @param num_iterations number of iterations for the positioning algorithm.
 * @return node positions.
 */
std::vector<cv::Point2i> position_network(
    const NetworkGraph &graph, const std::vector<int> &nodes, cv::Size screen_size, int margin, int num_iterations);


/**
 * @brief Draw a subgraph in process of its positioning.
 * @param graph base network graph.
 * @param nodes connected subgraph nodes.
 * @param screen_size output image size.
 * @param margin size of borders in pixels.
 */
void position_network_test(
    const NetworkGraph &graph, const std::vector<int> &nodes, const cv::Size &screen_size, int margin = 50);
