// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_TESTDATAARRAYHDF5_HPP
#define NIX_TESTDATAARRAYHDF5_HPP

#include "BaseTestDataArray.hpp"

class TestDataArrayHDF5 : public BaseTestDataArray {

    CPPUNIT_TEST_SUITE(TestDataArrayHDF5);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);
    CPPUNIT_TEST(testData);
    CPPUNIT_TEST(testPolynomial);
    CPPUNIT_TEST(testPolynomialSetter);
    CPPUNIT_TEST(testLabel);
    CPPUNIT_TEST(testUnit);
    CPPUNIT_TEST(testDimension);
    CPPUNIT_TEST(testAliasRangeDimension);
    CPPUNIT_TEST(testDataFrameDimension);
    CPPUNIT_TEST(testOperator);
    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_DataArray.h5", nix::FileMode::Overwrite);

        block = file.createBlock("block_one", "dataset");
        array1 = block.createDataArray("array_one",
                                       "testdata",
                                       nix::DataType::Double,
                                       nix::NDSize({ 0, 0, 0 }));
        array2 = block.createDataArray("random",
                                       "double",
                                       nix::DataType::Double,
                                       nix::NDSize({ 20, 20 }));
        array3 = block.createDataArray("one_d",
                                       "double",
                                       nix::DataType::Double,
                                       nix::NDSize({ 20 }));
        std::vector<double> t;
        for (size_t i = 0; i < 20; i++)
            t.push_back(1.3 * i);
        array3.setData(nix::DataType::Double, t.data(), nix::NDSize({ 20 }), nix::NDSize({ 0 }));
        array3.label("label");
        array3.unit("Hz");
    }

    void tearDown() {
        file.close();
    }

};

#endif //NIX_TESTDATAARRAYHDF5_HPP
