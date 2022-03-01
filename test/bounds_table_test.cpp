/*
 * Copyright Thomas M. Grosen
 * Created on 14/12/2021.
 */

/*
 * This file is part of dbm2
 *
 * dbm2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dbm2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with dbm2.  If not, see <https://www.gnu.org/licenses/>.
 */

#define BOOST_TEST_MODULE DBM

#include <boost/test/unit_test.hpp>
#include <dbm2/DBM.h>

using namespace dbm2;

BOOST_AUTO_TEST_CASE(remove_clock_test_1) {
    DBM D(4);
    D._bounds_table.get(1, 0) = bound_t::inf();
    D._bounds_table.get(3, 0) = bound_t::inf();
    D._bounds_table.get(1, 2) = bound_t::inf();
    D._bounds_table.get(3, 2) = bound_t::inf();

    D._bounds_table.remove_clock(2);

    BOOST_CHECK(D._bounds_table._number_of_clocks == 3);

    BOOST_CHECK(D._bounds_table.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.at(2, 0) == bound_t::inf());

    BOOST_CHECK(D._bounds_table.at(0, 0) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, 1) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, 2) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(1, 1) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(1, 2) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(2, 1) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(2, 2) == bound_t::zero());
}

BOOST_AUTO_TEST_CASE(swap_clocks_test_1) {
    int check_cnt = 0;
    dim_t size = 5, a = 2, b = 4;
    DBM D(size);

    for (dim_t i = 0; i < size; ++i)
        for (dim_t j = 0; j < size; ++j) {
            if (i == j) continue;
            D._bounds_table.get(i, j) = bound_t(j + (i * size), false);
        }

    D._bounds_table.swap_clocks(a, b);

    for (dim_t i = 0; i < size; ++i)
        for (dim_t j = 0; j < size; ++j) {
            bound_t bound;
            if (i == j)
                bound = bound_t::zero();
            else if ((i == a && j == b) || (i == b && j == a))
                bound = bound_t(i + (j * size), false);
            else if (i == a)
                bound = bound_t(j + (b * size), false);
            else if (i == b)
                bound = bound_t(j + (a * size), false);
            else if (j == a)
                bound = bound_t(b + (i * size), false);
            else if (j == b)
                bound = bound_t(a + (i * size), false);
            else
                bound = bound_t(j + (i * size), false);

            BOOST_CHECK(D._bounds_table.at(i, j) == bound);
        }
}

BOOST_AUTO_TEST_CASE(add_clock_after_test_1) {
    DBM D(4);
    D._bounds_table.get(1, 0) = bound_t::inf();
    D._bounds_table.get(3, 0) = bound_t::inf();
    D._bounds_table.get(1, 2) = bound_t::inf();
    D._bounds_table.get(3, 2) = bound_t::inf();
    std::cout << D << std::endl;
    D._bounds_table.add_clock_at(3);
    std::cout << D << std::endl;
    BOOST_CHECK(D._bounds_table._number_of_clocks == 5);

    BOOST_CHECK(D._bounds_table.get(1, 0) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.get(1, 1) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.get(1, 2) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.get(1, 3) == D._bounds_table.get(1, 0));
    BOOST_CHECK(D._bounds_table.get(1, 4) == bound_t::zero());

    BOOST_CHECK(D._bounds_table.get(2, 0) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.get(2, 1) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.get(2, 2) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.get(2, 3) == D._bounds_table.get(2, 0));
    BOOST_CHECK(D._bounds_table.get(2, 4) == bound_t::zero());

    BOOST_CHECK(D._bounds_table.get(3, 0) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.get(3, 1) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.get(3, 2) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.get(3, 3) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.get(3, 4) == bound_t::inf());

    BOOST_CHECK(D._bounds_table.get(4, 0) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.get(4, 1) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.get(4, 2) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.get(4, 3) == D._bounds_table.get(4, 0));
    BOOST_CHECK(D._bounds_table.get(4, 4) == bound_t::zero());
}