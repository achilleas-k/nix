// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/FeatureHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


string linkTypeToString(LinkType link_type) {
    static vector<string> type_names = {"tagged", "untagged", "indexed"};
    return type_names[static_cast<int>(link_type)];
}


LinkType linkTypeFromString(const string &str) {
    if (str == "tagged")
        return LinkType::Tagged;
    else if (str == "untagged")
        return LinkType::Untagged;
    else if (str == "indexed")
        return LinkType::Indexed;
    else
        throw runtime_error("Unable to create a LinkType from the string: " + str);
}


FeatureHDF5::FeatureHDF5(const FeatureHDF5 &feature)
    : EntityHDF5(feature.file(), feature.group(), feature.id()),
      block(feature.block)
{}


FeatureHDF5::FeatureHDF5(const File &file, const Block &block, const Group &group,
                                       const string &id, DataArray data, LinkType link_type)
    : FeatureHDF5(file, block, group, id, data, link_type, util::getTime())
{
}


FeatureHDF5::FeatureHDF5(const File &file, const Block &block, const Group &group,
                                       const string &id, DataArray data, LinkType link_type, time_t time)
    : EntityHDF5(file, group, id, time), block(block)
{
    linkType(link_type);
    // TODO: the line below currently throws an exception if the DataArray
    // is not in block - to consider if we prefer copying it to the block
    this->data(data.id()); 
}


void FeatureHDF5::linkType(LinkType link_type) {
    // linkTypeToString will generate an error if link_type is invalid
    group().setAttr("link_type", linkTypeToString(link_type));
    forceUpdatedAt();
}


void FeatureHDF5::data(const std::string &data_array_id) {
    if(data_array_id.empty()) {
        throw EmptyString("data DataArray id");
    }
    else {
        if(!block.hasDataArray(data_array_id)) {
            throw runtime_error("FeatureHDF5::data: cannot set Feature data because referenced DataArray does not exist!");
        } else {
            group().setAttr("data", data_array_id);
            forceUpdatedAt();
        }
    }
}


DataArray FeatureHDF5::data() const {
    if(group().hasAttr("data")) {
        string dataId;
        group().getAttr("data", dataId);
        if(block.hasDataArray(dataId)) {
            return block.getDataArray(dataId);
        } else {
            throw std::runtime_error("Data array not found by id in Block");
        }
    } else {
        throw MissingAttr("data");
    }
}


LinkType FeatureHDF5::linkType() const {
    if(group().hasAttr("link_type")) {
        string link_type;
        group().getAttr("link_type", link_type);
        return linkTypeFromString(link_type);
    } else {
        throw MissingAttr("data");
    }
}


FeatureHDF5::~FeatureHDF5() {}


} // namespace hdf5
} // namespace nix
