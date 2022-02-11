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

BOOST_AUTO_TEST_CASE(Remove_Clock_Test) {
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