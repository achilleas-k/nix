// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DATA_ARRAY_H
#define NIX_DATA_ARRAY_H

#include <nix/DataSet.hpp>

#include <nix/base/EntityWithSources.hpp>
#include <nix/base/IDataArray.hpp>
#include <nix/Dimensions.hpp>
#include <nix/Hydra.hpp>
#include <nix/DataFrame.hpp>

#include <nix/Platform.hpp>
#include <nix/util/util.hpp>


namespace nix {

// TODO add documentation for undocumented methods.

/**
 * @brief A class that can store arbitrary n-dimensional data along with further
 * information.
 *
 * The {@link DataArray} is the core entity of the NIX data model, its purpose is to
 * store arbitrary n-dimensional data. In addition to the common fields id, name, type, and definition
 * the DataArray stores sufficient information to understand the physical nature of the stored data.
 *
 * A guiding principle of the data model is provides enough information to create a
 * plot of the stored data. In order to do so, the DataArray defines a property
 * {@link dataType} which provides the physical type of the stored data (for example
 * 16 bit integer or double precision IEEE floatingpoint number).
 * The property {@link unit} specifies the SI unit of the values stored in the
 * DataArray{} whereas the {@link label} defines what is given in this units.
 * Together, both specify what corresponds to the the y-axis of a plot.
 *
 * In some cases it is much more efficient or convenient to store data not as
 * floating point numbers but rather as (16 bit) integer values as, for example
 * read from a data acquisition board.
 * In order to convert such data to the correct values, we follow the approach
 * taken by the comedi data-acquisition library (http://www.comedi.org)
 * and provide {@link polynomCoefficients} and an {@link expansionOrigin}.
 *
 * ### Create a new data array with a 10 x 10 float matrix as data
 *
 * A DataArray can only be created at an existing block. Do not use the
 * DataArrays constructors for this purpose.
 *
 * ~~~
 * Block b = ...;
 * DataArray da = b.crateDataArray("matrix_10_10", "data");
 * da.createData(DataType::Float, {10, 10});
 * ~~~
 *
 * ### Remove a data array from a file
 *
 * ~~~
 * Block b = ...;
 * bool deleted = da.deleteDataArray(some_id);
 * ~~~
 */
class NIXAPI DataArray : public base::EntityWithSources<base::IDataArray>, public DataSet {

public:

    /**
     * @brief Constructor that creates an uninitialized DataArray.
     *
     * Calling any method on an uninitialized data array will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a data array is initialized:
     *
     * ~~~
     * DataArray e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    DataArray()
        : EntityWithSources()
    {}

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like DataArray is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The data array to copy.
     */
    DataArray(const DataArray &other)
        : EntityWithSources(other.impl())
    {
    }

    /**
     * @brief Constructor that creates a new data array from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    DataArray(const std::shared_ptr<base::IDataArray> &p_impl)
        : EntityWithSources(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new data array from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    DataArray(std::shared_ptr<base::IDataArray> &&ptr)
        : EntityWithSources(std::move(ptr))
    {
    }

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

    /**
     * @brief Get the label for the values stored in the DataArray.
     *
     * @return The label of the data array.
     */
    boost::optional<std::string> label() const {
        return backend()->label();
    }

    /**
     * @brief Set the label for the data stored.
     *
     * @param label     The label of the data array.
     */
    void label(const std::string &label);

    /**
     * @brief Deleter for the label attribute.
     *
     * @param t         None
     */
    void label(const none_t t) {
        backend()->label(t);
    }

    /**
     * @brief Get the unit of the data stored in this data array.
     *
     * @return The unit of the data array.
     */
    boost::optional<std::string> unit() const {
        return backend()->unit();
    }

    /**
     * @brief Deleter for the unit attribute.
     *
     * @param t         None
     */
    void unit(const none_t t) {
        backend()->unit(t);
    }

    /**
     * @brief Set the unit for the values stored in this DataArray.
     *
     * @param unit      The unit of the data array.
     */
    void unit(const std::string &unit);

    /**
     * @brief Returns the expansion origin of the calibration polynom.
     *
     * The expansion origin is 0.0 by default.
     *
     * @return The expansion origin.
     */
    boost::optional<double> expansionOrigin() const {
        return backend()->expansionOrigin();
    }

    /**
     * @brief Set the expansion origin for the calibration.
     *
     * @param expansion_origin  The expansion origin for the calibration.
     */
    void expansionOrigin(double expansion_origin) {
        backend()->expansionOrigin(expansion_origin);
    }

    /**
     * @brief Deleter for the expansion origin.
     *
     * This will reset the expansion origin for the calibration to its
     * default value which is given with 0.0.
     *
     * @param t         None
     */
    void expansionOrigin(const none_t t) {
        backend()->expansionOrigin(t);
    }

    /**
     * @brief Set the polynom coefficients for the calibration.
     *
     * By default this is set to a two element vector of [0.0, 1.0] for a linear calibration
     * with zero offset.
     *
     * @param polynom_coefficients      The new polynom coefficients for the calibration.
     * @param compression               The nix::Compression flag defining the compression of the dataset.
     */
    void polynomCoefficients(const std::vector<double> &polynom_coefficients,
                             const Compression &compression=Compression::None) {
        backend()->polynomCoefficients(polynom_coefficients, compression);
    }

    /**
     * @brief Returns the polynom coefficients.
     *
     * @return The polynom coefficients for the calibration.
     */
    std::vector<double> polynomCoefficients() const {
        return backend()->polynomCoefficients();
    }

    /**
     * @brief Deleter for the `polynomCoefficients` attribute.
     *
     * @param t         None
     */
    void polynomCoefficients(const none_t t) {
        backend()->polynomCoefficients(t);
    }

    //--------------------------------------------------
    // Methods concerning dimensions
    //--------------------------------------------------

    /**
     * @brief Get all dimensions associated with this data array.
     *
     * The parameter filter can be used to filter sources by various
     * criteria.
     *
     * @param filter    A filter function ({@link nix::util::Filter::type})
     *
     * @return The filtered dimensions as a vector
     */
    std::vector<Dimension> dimensions(const util::Filter<Dimension>::type &filter) const;

    /**
     * @brief Get all dimensions associated with this data array.
     *
     * Always uses filter that accepts all sources.
     *
     * @return The filtered dimensions as a vector
     */
    std::vector<Dimension> dimensions() const {
        return dimensions(util::AcceptAll<Dimension>());
    }

    /**
     * @brief Returns the number of dimensions stored in the DataArray.
     *
     * This matches the dimensionality of the data stored in this property.
     *
     * @return The number of dimensions.
     */
    ndsize_t dimensionCount() const {
        return backend()->dimensionCount();
    }

    /**
     * @brief Returns the Dimension object for the specified dimension of the data.
     *
     * @param id        The index of the respective dimension.
     *
     * @return The dimension object.
     */
    Dimension getDimension(ndsize_t id) const {
        return backend()->getDimension(id);
    }

    /**
     * @brief Append a new SetDimension to the list of existing dimension descriptors.
     *
     * @param    labels       The category labels, default: empty std::vector of std::string
     * @return   The newly created SetDimension.
     */
    SetDimension appendSetDimension(const std::vector<std::string> &labels={}) {
        SetDimension dim = backend()->createSetDimension(backend()->dimensionCount() + 1);
        if (labels.size() > 0 )
            dim.labels(labels);
        return dim;
    }

    /**
     * @brief Append a new RangeDimension to the list of existing dimension descriptors.
     *
     * @param ticks     The ticks of the RangeDimension to create.
     * @param label     The label of the dimension (e.g. 'time'), default: empty
     * @param unit      The unit of the ticks, default: empty
     *
     * @return The newly created RangeDimension
     */
    RangeDimension appendRangeDimension(const std::vector<double> &ticks, const std::string &label="",
                                        const std::string &unit="") {
        if (ticks.size() == 0) {
            throw nix::InvalidDimension("The ticks of a range dimension must not be empty!",
                                        "DataArray::appendRangeDimension");
        }
        RangeDimension dim = backend()->createRangeDimension(backend()->dimensionCount() + 1, ticks);
        if (label.size() > 0)
            dim.label(label);
        if (unit.size() > 0)
            dim.unit(unit);
        return dim;
    }

    /**
     * @brief Append a new RangeDimension that uses the data stored in this DataArray as ticks.
     * This works only(!) if the DataArray in 1D and the stored data is numeric. An Exception of the
     * type {@link nix::InvalidDimension} will be thrown otherwise.
     *
     * @return The created RangeDimension
     */
    RangeDimension appendAliasRangeDimension() {
        if (this->dataExtent().size() > 1) {
            throw nix::InvalidDimension("AliasRangeDimensions only allowed for 1D numeric DataArrays!",
                                        "DataArray::appendAliasRangeDimension");
        }
        if (!nix::data_type_is_numeric(this->dataType())) {
            throw nix::InvalidDimension("AliasRangeDimensions are only allowed for 1D numeric DataArrays!",
                                        "DataArray::appendAliasRangeDimension");
        }
        if (dimensionCount() > 0) {
            throw nix::InvalidDimension("Cannot append additional alias dimension. There must only be one!",
                                        "DataArray::appendAliasRangeDimension");
        }
        if (this->unit()) {
            std::string unit = *this->unit();
            if(!(util::isSIUnit(unit) || util::isCompoundSIUnit(unit))) {
                throw nix:: InvalidUnit("AliasRangeDimensions are only allowed when SI or composites of SI units are used.",
                                        "DataArray::appendAliasRangeDimension");
            }
        }
        return backend()->createAliasRangeDimension();
    }


    /**
     * @brief Append a new SampledDimension to the list of existing dimension descriptors.
     *
     * @param sampling_interval         The sampling interval of the SetDimension to create.
     * @param label                     The label of the dimension (e.g. 'time'), default: empty
     * @param unit                      The unit of the dimension, default: empty
     * @param offset                    The offset with which this dimension starts, default: 0.0
     *
     * @return The newly created SampledDimension.
     */
    SampledDimension appendSampledDimension(double sampling_interval, const std::string &label="",
                                            const std::string &unit="", double offset=0.0) {
        SampledDimension dim = backend()->createSampledDimension(backend()->dimensionCount() + 1,
                                                                 sampling_interval);
        if (label.size() > 0)
            dim.label(label);
        if (unit.size() > 0)
            dim.unit(unit);
        if (offset > 0.0)
            dim.offset(offset);
        return dim;
    }

    /**
     * @brief Append a new {@link nix::DataFrameDimension} to the list of
     *        dimension descriptors.  A DataFrameDimension can be used to
     *        specify a dimension of a DataArray as specified by one (or all)
     *        column(s) of the linked DataFrame. That is, for each entry in the
     *        DataArray there must be a respecive row in the DataFrame. If no
     *        column index is specified that whole DataFrame is assumed to
     *        describe the DataArray entries along this dimension.
     *
     * @param frame: The DataFrame
     * @param column_index: The index of the column that should be used.
     *
     * @return The new DataFrameDimension
     */
    DataFrameDimension appendDataFrameDimension(const DataFrame &frame, unsigned column_index) {
        if (column_index > frame.columns().size()) {
            throw nix::OutOfBounds("DataArray::appendDataFrameDimensios: Invalid columnIndex ", column_index);
        }
        if (!frame) {
            throw nix::UninitializedEntity();
        }

        DataFrameDimension dim = backend()->createDataFrameDimension(backend()->dimensionCount() + 1, frame, column_index);
        return dim;
    };

    /**
     * @brief Append a new {@link nix::DataFrameDimension} to the list of
     *        dimension descriptors.  A DataFrameDimension can be used to
     *        specify a dimension of a DataArray as specified by one (or all)
     *        column(s) of the linked DataFrame. That is, for each entry in the
     *        DataArray there must be a respecive row in the DataFrame. If no
     *        column index is specified that whole DataFrame is assumed to
     *        describe the DataArray entries along this dimension.
     *
     * @param frame: The DataFrame
     * @param column_name: The name of the column that should be used.
     *
     * @return The new DataFrameDimension
     */
    DataFrameDimension appendDataFrameDimension(const DataFrame &frame, const std::string &column_name) {
        if (!frame) {
            throw nix::UninitializedEntity();
        }
        unsigned index;
        try {
            index = frame.colIndex(column_name);
        } catch ( ... ) {
            throw nix::OutOfBounds("DataArray::appendDataFrameDimension: provided column name does not exist in the DataFrame!");
        }
        DataFrameDimension dim = backend()->createDataFrameDimension(backend()->dimensionCount() + 1, frame, index);
        return dim;
    };

     /**
     * @brief Append a new {@link nix::DataFrameDimension} to the list of
     *        dimension descriptors.  A DataFrameDimension can be used to
     *        specify a dimension of a DataArray as specified by one (or all)
     *        column(s) of the linked DataFrame. That is, for each entry in the
     *        DataArray there must be a respecive row in the DataFrame. If no
     *        column index is specified that whole DataFrame is assumed to
     *        describe the DataArray entries along this dimension.
     *
     * @param frame: The DataFrame
     *
     * @return The new DataFrameDimension
     */
    DataFrameDimension appendDataFrameDimension(const DataFrame &frame) {
        if (!frame) {
            throw nix::UninitializedEntity();
        }
        DataFrameDimension dim = backend()->createDataFrameDimension(backend()->dimensionCount() + 1, frame);
        return dim;
    };


    /**
     * @brief Create a new SetDimension at a specified dimension index.
     *
     * This adds a new dimension descriptor of the type {@link nix::SetDimension} that describes the dimension
     * of the data at the specified index.
     *
     * @param id        The index of the dimension. Must be a value > 0 and <= `dimensionCount + 1`.
     *
     * @return The created dimension descriptor.
     * @deprecated This function is deprecated and ignores the id argument!
     */
    DEPRECATED SetDimension createSetDimension(ndsize_t id) {
        return appendSetDimension();
    }

    /**
     * @brief Create a new RangeDimension at a specified dimension index.
     *
     * This adds a new dimension descriptor of the type {@link nix::RangeDimension} that describes the dimension
     * of the data at the specified index.
     *
     * @param id        The index of the dimension. Must be a value > 0 and <= `dimensionCount + 1`.
     * @param ticks     Vector with {@link nix::RangeDimension::ticks}.
     *
     * @return The created dimension descriptor.
     * @deprecated This function is deprecated and ignores the id argument!
     */
    DEPRECATED RangeDimension createRangeDimension(ndsize_t id, const std::vector<double> &ticks) {
        return appendRangeDimension(ticks);
    }

    /**
     * @brief Create a new RangeDimension that uses the data stored in this DataArray as ticks.
     *
     * @return The created dimension descriptor.
     * @deprecated This function is deprecated and will be removed. Use appendAliasRangeDimension instead!
     */
    DEPRECATED RangeDimension createAliasRangeDimension() {
        return appendAliasRangeDimension();
    }

    /**
     * @brief Create a new SampledDimension at a specified dimension index.
     *
     * This adds a new dimension descriptor of the type {@link nix::SampledDimension} that describes the dimension
     * of the data at the specified index.
     *
     * @param id                The index of the dimension. Must be a value > 0 and <= `dimensionCount + 1`.
     * @param sampling_interval  The sampling interval of the dimension.
     *
     * @return The created dimension descriptor.
     * @deprecated This function is deprecated and ignores the id argument!
     */
    DEPRECATED SampledDimension createSampledDimension(ndsize_t id, double sampling_interval) {
        return appendSampledDimension(sampling_interval);
    }

    /**
     * @brief Remove all dimension descriptors of the {@link nix::DataArray}.
     *
     * @return bool indicating if operation succeeded.
     */
    bool deleteDimensions() {
        return backend()->deleteDimensions();
    }

    //--------------------------------------------------
    // Methods concerning data access.
    //--------------------------------------------------

    void getDataDirect(DataType dtype,
                       void *data,
                       const NDSize &count,
                       const NDSize &offset) const {
        backend()->read(dtype, data, count, offset);
    }

    void setDataDirect(DataType dtype,
                       const void *data,
                       const NDSize &count,
                       const NDSize &offset)
    {
        backend()->write(dtype, data, count, offset);
    }


    /**
     * @brief Get the extent of the data of the DataArray entity.
     *
     * @return The data extent.
     */
    NDSize dataExtent() const {
        return backend()->dataExtent();
    }

    /**
     * @brief Set the data extent of the DataArray entity.
     *
     * @param extent    The extent of the data.
     */
    void dataExtent(const NDSize &extent) {
        backend()->dataExtent(extent);
    }

    /**
     * @brief Get the data type of the data stored in the DataArray entity.
     *
     * @return The data type of the DataArray.
     */
    DataType dataType(void) const {
        return backend()->dataType();
    }

    void appendData(DataType dtype, const void *data, const NDSize &count, size_t axis);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
     * @brief Assignment operator for none.
     */
    DataArray &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

    /**
     * @brief Copy-assignment operator.
     */
    DataArray &operator=(const DataArray &other)  {
        ImplContainer::operator=(other);
        return *this;
    }

    /**
     * @brief Output operator
     */
    NIXAPI friend std::ostream& operator<<(std::ostream &out, const DataArray &ent);

    //
protected:
    //implementation of the DataIO interface
    void ioRead(DataType dtype,
                void *data,
                const NDSize &count,
                const NDSize &offset) const;

    void ioWrite(DataType dtype,
                 const void *data,
                 const NDSize &count,
                 const NDSize &offset);
};


template<>
struct objectToType<nix::DataArray> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::DataArray;
    typedef nix::base::IDataArray backendType;
};

NIXAPI std::ostream& operator<<(std::ostream &out, const DataArray &ent);

} // namespace nix

#endif // NIX_DATA_ARRAY_H
