#pragma once

#include "common.h"

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <vector>


namespace bbp {
namespace sonata {

struct EdgeSelection
{
    std::vector<std::pair<uint64_t, uint64_t>> ranges;
};


class EdgePopulation
{
public:
    EdgePopulation(const std::string& h5FilePath, const std::string& csvFilePath, const std::string& name);

    EdgePopulation(const EdgePopulation&) = delete;

    ~EdgePopulation();

    std::string name() const;

    /**
     * Total number of edges
     */
    uint64_t size() const;

    /**
     * Name of source population extracted from 'source_node_id' dataset
     */
    std::string sourcePopulation() const;

    /**
     * Name of target population extracted from 'source_node_id' dataset
     */
    std::string targetPopulation() const;

    std::vector<NodeID> sourceNodeIDs(const EdgeSelection& selection) const;

    std::vector<NodeID> targetNodeIDs(const EdgeSelection& selection) const;

    // ------------------------------------------------------------------------
    // Attributes
    // ------------------------------------------------------------------------

    /**
     * All attribute names (CSV columns + required attributes + union of attributes in groups)
     */
    const std::vector<std::string>& attributeNames() const;

    /**
     * If getAttribute() will work for a given edgeID.
     */
    std::vector<bool> hasAttribute(const std::string& name, const EdgeSelection& selection) const;

    /**
     * Get attribute values for given EdgeSelection
     *
     * @param name is a string to allow attributes not defined in spec
     * @throw if there is no such attribute for the population
     * @throw if the attribute is not defined for _any_ edge from the edge selection
     */
    template <typename T>
    std::vector<T> getAttribute(const std::string& name, const EdgeSelection& selection) const;

    // ...
    // Access to dynamics params analogous to attributes

    // ------------------------------------------------------------------------
    // Connectivity
    // ------------------------------------------------------------------------
    /**
     * Find inbound edges for a given node ID.
     */
    EdgeSelection afferentEdges(const std::vector<NodeID>& target) const;

    /**
     * Find outbound edges for a given node ID.
     */
    EdgeSelection efferentEdges(const std::vector<NodeID>& source) const;

    /**
     * Find edges connecting two given nodes.
     */
    EdgeSelection connectingEdges(const std::vector<NodeID>& source, const std::vector<NodeID>& target) const;

private:
    struct Impl;
    const std::unique_ptr<Impl> impl_;
};


class EdgeStorage
{
public:
    EdgeStorage(const std::string& h5FilePath, const std::string& csvFilePath = "");

    EdgeStorage(const EdgeStorage&) = delete;

    ~EdgeStorage();

    /**
     * List all populations.
     *
     */
    std::set<std::string> populationNames() const;

    /**
     * Open specific population.
     * @throw if no population with such a name exists
     */
    std::shared_ptr<EdgePopulation> openPopulation(const std::string& name) const;

private:
    struct Impl;
    const std::unique_ptr<Impl> impl_;
};
}
} // namespace bbp::sonata