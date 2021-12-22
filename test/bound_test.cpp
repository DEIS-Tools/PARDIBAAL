/*
 * Copyright Thomas Møller Grosen
 * Created on 03/11/2021.
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
#include <dbm2/bound_t.h>

using namespace dbm;

BOOST_AUTO_TEST_CASE(Inf_Test_1) {
    bound_t b = bound_t::inf();
    BOOST_CHECK(b._inf);
}

BOOST_AUTO_TEST_CASE(Zero_Test_1) {
    bound_t b = bound_t::zero();
    BOOST_CHECK(!b._inf);
    BOOST_CHECK(!b._strict);
    BOOST_CHECK(b._n == 0);
}

BOOST_AUTO_TEST_CASE(Equal_Test_1) {
    bound_t a = bound_t::zero();
    bound_t b = bound_t::inf();

    BOOST_CHECK(a != b);
}

BOOST_AUTO_TEST_CASE(Equal_Test_2) {
    bound_t a(5, false);
    bound_t b(5, false);

    BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(Comp_Test_1) {
    bound_t a = bound_t::zero();
    bound_t b = bound_t::inf();

    BOOST_CHECK(a == bound_t::min(a, b));
    BOOST_CHECK(b == bound_t::max(a, b));
    BOOST_CHECK(a < b);
    BOOST_CHECK(a <= b);
    BOOST_CHECK(b > a);
    BOOST_CHECK(b >= a);
}

BOOST_AUTO_TEST_CASE(Comp_Test_2) {
    bound_t a(100, false);
    bound_t b = bound_t::inf();

    BOOST_CHECK(a == bound_t::min(a, b));
    BOOST_CHECK(b == bound_t::max(a, b));
    BOOST_CHECK(a < b);
    BOOST_CHECK(a <= b);
    BOOST_CHECK(b > a);
    BOOST_CHECK(b >= a);
}

BOOST_AUTO_TEST_CASE(Comp_Test_3) {
    bound_t a(1, true);
    bound_t b(1, false);

    BOOST_CHECK(a == bound_t::min(a, b));
    BOOST_CHECK(b == bound_t::max(a, b));
    BOOST_CHECK(a < b);
    BOOST_CHECK(a <= b);
    BOOST_CHECK(b > a);
    BOOST_CHECK(b >= a);
}

BOOST_AUTO_TEST_CASE(Comp_Test_4) {
    bound_t a = bound_t::inf();
    bound_t b = bound_t::inf();

    BOOST_CHECK(a == bound_t::min(a, b));
    BOOST_CHECK(a == bound_t::max(a, b));
    BOOST_CHECK(!(a < b));
    BOOST_CHECK(a <= b);
    BOOST_CHECK(!(b > a));
    BOOST_CHECK(b >= a);
}

BOOST_AUTO_TEST_CASE(Comp_Test_5) {
    bound_t a = bound_t::zero();
    bound_t b = bound_t::zero();

    BOOST_CHECK(a == bound_t::min(a, b));
    BOOST_CHECK(a == bound_t::max(a, b));
    BOOST_CHECK(!(a < b));
    BOOST_CHECK(a <= b);
    BOOST_CHECK(!(b > a));
    BOOST_CHECK(b >= a);
}

BOOST_AUTO_TEST_CASE(Add_Test_1) {
    bound_t a(1, true);
    bound_t b(3, false);

    BOOST_CHECK(a + b == bound_t(4, true));
}

BOOST_AUTO_TEST_CASE(Add_Test_2) {
    bound_t a(1, false);
    bound_t b(3, false);

    BOOST_CHECK(a + b == bound_t(4, false));
}

BOOST_AUTO_TEST_CASE(Add_Test_3) {
    bound_t a(1, true);
    bound_t b = bound_t::inf();

    BOOST_CHECK(a + b == bound_t::inf());
}

BOOST_AUTO_TEST_CASE(Add_Test_4) {
    bound_t a = bound_t::zero();
    bound_t b = bound_t::zero();

    BOOST_CHECK(a + b == bound_t::zero());
}