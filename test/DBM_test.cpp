//
// Created by Thomas Grosen on 03/11/2021.
//

#define BOOST_TEST_MODULE DBM

#include <boost/test/unit_test.hpp>
#include <dbm/DBM.h>
#include <ostream>

using namespace dbm;

BOOST_AUTO_TEST_CASE(Close_Test_1)
{
    DBM D(3);

    D.future();
    D.assign(1, 0);
    D.restrict(2, 0, bound_t(10, false));
    D.restrict(0, 2, bound_t(-5, true));
    D.free(2);
    D.copy(1, 2);

    DBM Q = D;
    D.close();

    for (dim_t i = 0; i < 3; i++)
        for (dim_t j = 0; j < 3; j++)
            BOOST_CHECK(D._bounds_table.get(i, j) == Q._bounds_table.get(i, j));
}

BOOST_AUTO_TEST_CASE(Delay_Test_1) {
    DBM D(10);

    D.future();

    for (dim_t i = 0; i < 10; i++)
        for (dim_t j = 0; j < 10; j++) {
            if (i == 0 || j != 0)
                BOOST_CHECK(D._bounds_table.get(i, j) == bound_t::zero());
            else
                BOOST_CHECK(D._bounds_table.get(i, j) == bound_t::inf());
        }
}

BOOST_AUTO_TEST_CASE(Restrict_Test_1) {
    DBM D(3);

    D.future();
//    D.restrict()
}