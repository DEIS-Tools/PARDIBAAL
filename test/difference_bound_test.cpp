/*
 * Copyright Thomas M. Grosen 
 * Created on 08/06/2022
 */

/*
 * This file is part of pardibaal
 *
 * pardibaal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pardibaal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with pardibaal. If not, see <https://www.gnu.org/licenses/>.
 */

#define BOOST_TEST_MODULE PARDIBAAL

#include <boost/test/unit_test.hpp>
#include "pardibaal/difference_bound_t.h"
#include "errors.h"

using namespace pardibaal;

BOOST_AUTO_TEST_CASE(zero_test_1) {
    auto c = difference_bound_t::zero(1, 0);

    BOOST_CHECK(c._i == 1);
    BOOST_CHECK(c._j == 0);
    BOOST_CHECK(c._bound.is_non_strict());
    BOOST_CHECK(not c._bound.is_inf());
    BOOST_CHECK(c._bound.get_bound() == 0);
}

BOOST_AUTO_TEST_CASE(inf_test_1) {
    auto c = difference_bound_t::inf(10, 3);

    BOOST_CHECK(c._i == 10);
    BOOST_CHECK(c._j == 3);
    BOOST_CHECK(c._bound.is_strict());
    BOOST_CHECK(c._bound.is_inf());
}

BOOST_AUTO_TEST_CASE(upper_strict_test_1) {
    auto c = difference_bound_t::upper_strict(10, 0);

    BOOST_CHECK(c._i == 10);
    BOOST_CHECK(c._j == 0);
    BOOST_CHECK(c._bound.is_strict());
    BOOST_CHECK(not c._bound.is_inf());
    BOOST_CHECK(c._bound.get_bound() == 0);
}

BOOST_AUTO_TEST_CASE(upper_non_strict_test_1) {
    auto c = difference_bound_t::upper_non_strict(10, 3);

    BOOST_CHECK(c._i == 10);
    BOOST_CHECK(c._j == 0);
    BOOST_CHECK(c._bound.is_non_strict());
    BOOST_CHECK(not c._bound.is_inf());
    BOOST_CHECK(c._bound.get_bound() == 3);
}

BOOST_AUTO_TEST_CASE(lower_strict_test_1) {
    auto c = difference_bound_t::lower_strict(10, 3);

    BOOST_CHECK(c._i == 0);
    BOOST_CHECK(c._j == 10);
    BOOST_CHECK(c._bound.is_strict());
    BOOST_CHECK(not c._bound.is_inf());
    BOOST_CHECK(c._bound.get_bound() == -3);
}

BOOST_AUTO_TEST_CASE(lower_non_strict_test_1) {
    auto c = difference_bound_t::lower_non_strict(10, 0);

    BOOST_CHECK(c._i == 0);
    BOOST_CHECK(c._j == 10);
    BOOST_CHECK(c._bound.is_non_strict());
    BOOST_CHECK(not c._bound.is_inf());
    BOOST_CHECK(c._bound.get_bound() == 0);
}

BOOST_AUTO_TEST_CASE(lower_non_strict_test_2) {
    //BOOST_CHECK_THROW(difference_bound_t::lower_non_strict(10, -1), base_error);
}





