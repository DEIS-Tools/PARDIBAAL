/*
 * Copyright Thomas M. Grosen
 * Created on 03/11/2021.
 */

/*
 * This file is part of PARDIBAAL
 *
 * PARDIBAAL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PARDIBAAL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with PARDIBAAL.  If not, see <https://www.gnu.org/licenses/>.
 */
#define BOOST_TEST_MODULE DBM

#include <boost/test/unit_test.hpp>
#include "pardibaal/bound_t.h"

using namespace pardibaal;

BOOST_AUTO_TEST_CASE(inf_test_1) {
    bound_t b = bound_t::inf();
    BOOST_CHECK(b.is_inf());
}

BOOST_AUTO_TEST_CASE(zero_test_1) {
    bound_t b = bound_t::zero();
    BOOST_CHECK(not b.is_inf());
    BOOST_CHECK(b.is_non_strict());
    BOOST_CHECK(b.get_bound() == 0);
}

BOOST_AUTO_TEST_CASE(equal_test_1) {
    bound_t a = bound_t::zero();
    bound_t b = bound_t::inf();

    BOOST_CHECK(a != b);
}

BOOST_AUTO_TEST_CASE(equal_test_2) {
    bound_t a(5, NON_STRICT);
    bound_t b(5, NON_STRICT);

    BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(comp_test_1) {
    bound_t a = bound_t::zero();
    bound_t b = bound_t::inf();

    BOOST_CHECK(a == bound_t::min(a, b));
    BOOST_CHECK(b == bound_t::max(a, b));
    BOOST_CHECK(a < b);
    BOOST_CHECK(a <= b);
    BOOST_CHECK(b > a);
    BOOST_CHECK(b >= a);
}

BOOST_AUTO_TEST_CASE(comp_test_2) {
    bound_t a(100, NON_STRICT);
    bound_t b = bound_t::inf();

    BOOST_CHECK(a == bound_t::min(a, b));
    BOOST_CHECK(b == bound_t::max(a, b));
    BOOST_CHECK(a < b);
    BOOST_CHECK(a <= b);
    BOOST_CHECK(b > a);
    BOOST_CHECK(b >= a);
}

BOOST_AUTO_TEST_CASE(comp_test_3) {
    bound_t a(1, STRICT);
    bound_t b(1, NON_STRICT);

    BOOST_CHECK(a == bound_t::min(a, b));
    BOOST_CHECK(b == bound_t::max(a, b));
    BOOST_CHECK(a < b);
    BOOST_CHECK(a <= b);
    BOOST_CHECK(b > a);
    BOOST_CHECK(b >= a);
}

BOOST_AUTO_TEST_CASE(comp_test_4) {
    bound_t a = bound_t::inf();
    bound_t b = bound_t::inf();

    BOOST_CHECK(a == bound_t::min(a, b));
    BOOST_CHECK(a == bound_t::max(a, b));
    BOOST_CHECK(!(a < b));
    BOOST_CHECK(a <= b);
    BOOST_CHECK(!(b > a));
    BOOST_CHECK(b >= a);
}

BOOST_AUTO_TEST_CASE(comp_test_5) {
    bound_t a = bound_t::zero();
    bound_t b = bound_t::zero();

    BOOST_CHECK(a == bound_t::min(a, b));
    BOOST_CHECK(a == bound_t::max(a, b));
    BOOST_CHECK(!(a < b));
    BOOST_CHECK(a <= b);
    BOOST_CHECK(!(b > a));
    BOOST_CHECK(b >= a);
}

BOOST_AUTO_TEST_CASE(add_test_1) {
    bound_t a(1, STRICT);
    bound_t b(3, NON_STRICT);

    BOOST_CHECK(a + b == bound_t::strict(4));
}

BOOST_AUTO_TEST_CASE(add_test_2) {
    bound_t a(1, NON_STRICT);
    bound_t b(3, NON_STRICT);

    BOOST_CHECK(a + b == bound_t::non_strict(4));
}

BOOST_AUTO_TEST_CASE(add_test_3) {
    bound_t a(1, STRICT);
    bound_t b = bound_t::inf();

    BOOST_CHECK(a + b == bound_t::inf());
}

BOOST_AUTO_TEST_CASE(add_test_4) {
    bound_t a = bound_t::zero();
    bound_t b = bound_t::zero();

    BOOST_CHECK(a + b == bound_t::zero());
}