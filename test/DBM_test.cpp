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

#define BOOST_TEST_MODULE PARDIBAAL

#include <boost/test/unit_test.hpp>
#include "pardibaal/DBM.h"
#include "errors.h"

using namespace pardibaal;

BOOST_AUTO_TEST_CASE(close_test_1) {
    DBM D(3);

    D.future();
    D.assign(1, 0);
    D.restrict(2, 0, bound_t::non_strict(10));
    D.restrict(0, 2, bound_t::strict(-5));
    D.free(2);
    D.copy(1, 2);

    DBM Q = D;
    D.close();

    for (dim_t i = 0; i < 3; i++)
        for (dim_t j = 0; j < 3; j++)
            BOOST_CHECK(D.at(i, j) == Q.at(i, j));
}

BOOST_AUTO_TEST_CASE(future_test_1) {
    DBM D(10);

    D.future();

    for (dim_t i = 0; i < 10; i++)
        for (dim_t j = 0; j < 10; j++) {
            if (i == 0 || j != 0)
                BOOST_CHECK(D.at(i, j) == bound_t::le_zero());
            else
                BOOST_CHECK(D.at(i, j) == bound_t::inf());
        }
}

BOOST_AUTO_TEST_CASE(bounded_future_test_1) {
    DBM D(10);

    D.future(5);

    for (dim_t i = 0; i < 10; i++)
        for (dim_t j = 0; j < 10; j++) {               
            if (j == 0 && i != 0)
                BOOST_CHECK(D.at(i, j) == bound_t::non_strict(5));
            else
                BOOST_CHECK(D.at(i, j) == bound_t::le_zero());
        }
}

BOOST_AUTO_TEST_CASE(bounded_future_test_2) {
    DBM D(10), Z(10);

    D.future(0);

    BOOST_CHECK(D.is_equal(Z));
    BOOST_CHECK(Z.is_equal(D));
}

BOOST_AUTO_TEST_CASE(bounded_future_test_3) {
    DBM D(10);

    D.free(2);
    D.future(5);

    for (dim_t i = 0; i < 10; i++)
        for (dim_t j = 0; j < 10; j++) {               
            if (j == 0 && i != 0) {
                if (i == 2)
                    BOOST_CHECK(D.at(i, j) == bound_t::inf() && D.at(i,j).get_bound() == 0);
                else
                    BOOST_CHECK(D.at(i, j) == bound_t::non_strict(5));
            } else {
                if (i == 2 && j != 2)
                    BOOST_CHECK(D.at(i, j) == bound_t::inf() && D.at(i, j).get_bound() == 0);
                else
                    BOOST_CHECK(D.at(i, j) == bound_t::le_zero());
            }
        }
}

BOOST_AUTO_TEST_CASE(delay_test_1) {
    DBM D(10);

    D.delay(5);

    for (dim_t i = 0; i < 10; i++)
        for (dim_t j = 0; j < 10; j++) {               
            if (j == 0 && i != 0)
                BOOST_CHECK(D.at(i, j) == bound_t::non_strict(5));
            else if (i == 0 && j != 0)
                BOOST_CHECK(D.at(i, j) == bound_t::non_strict(-5));
            else
                BOOST_CHECK(D.at(i, j) == bound_t::le_zero());
        }
}

BOOST_AUTO_TEST_CASE(delay_test_2) {
    DBM D(10), Z(10);

    D.delay(0);

    BOOST_CHECK(D.is_equal(Z));
    BOOST_CHECK(Z.is_equal(D));
}

BOOST_AUTO_TEST_CASE(interval_delay_test_1) {
    DBM D(10), Z(10);

    D.interval_delay(0, 5);
    Z.future(5);

    BOOST_CHECK(D.is_equal(Z));
    BOOST_CHECK(Z.is_equal(D));
}

BOOST_AUTO_TEST_CASE(interval_delay_test_2) {
    DBM D(10), Z(10);

    D.interval_delay(2, 5);
    Z.delay(2);
    Z.future(3);

    BOOST_CHECK(D.is_equal(Z));
    BOOST_CHECK(Z.is_equal(D));
}

BOOST_AUTO_TEST_CASE(restrict_test_1) {
    DBM D(3);
    bound_t g(5, NON_STRICT);

    D.future();
    D.restrict(1, 0, g);
    BOOST_CHECK(D.at(1, 0) == g);

    DBM Q = D;
    D.close();

    for (dim_t i = 0; i < 3; i++)
        for (dim_t j = 0; j < 3; j++)
            BOOST_CHECK(D.at(i, j) == Q.at(i, j));
}

BOOST_AUTO_TEST_CASE(restrict_test_2) {
    DBM D(3);
    bound_t g(-5, NON_STRICT);

    D.future();
    D.restrict(1, 0, g);

    BOOST_CHECK(D.is_empty());
}

BOOST_AUTO_TEST_CASE(subtract_test_1) {
    DBM D(3);
    bound_t g = bound_t::inf();

    D.future();
    D.subtract(1, 0, g);

    BOOST_CHECK(D.is_empty());
}

BOOST_AUTO_TEST_CASE(subtract_test_2) {
    DBM D = DBM::unconstrained(3);
    bound_t g = bound_t::strict(1);

    D.subtract(1, 2, g);

    BOOST_CHECK(not D.is_satisfying(1, 2, g));
    BOOST_CHECK(D.is_satisfying(1, 2, bound_t::non_strict(1)));
    BOOST_CHECK(D.is_satisfying(2, 1, bound_t::inf()));
}

BOOST_AUTO_TEST_CASE(trace_test_1) {
    DBM D(4);
    dim_t x = 1, y = 2, z = 3;
    std::vector<val_t> ceiling{0, 2, 5, 3};

    D.future();
    D.restrict(y, 0, bound_t::non_strict(5));

    // x = y = z
    BOOST_CHECK(D.at(x, y) == bound_t::le_zero());
    BOOST_CHECK(D.at(y, x) == bound_t::le_zero());
    BOOST_CHECK(D.at(x, z) == bound_t::le_zero());
    BOOST_CHECK(D.at(z, x) == bound_t::le_zero());
    BOOST_CHECK(D.at(y, z) == bound_t::le_zero());
    BOOST_CHECK(D.at(z, y) == bound_t::le_zero());

    // x,y,z in [0, 5]
    BOOST_CHECK(D.at(x, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(y, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(z, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(0, x) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, y) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, z) == bound_t::le_zero());

    // Reset x
    D.assign(x, 0);
    D.future();
    D.restrict(y, 0, bound_t::non_strict(5));

    // y = z
    BOOST_CHECK(D.at(y, z) == bound_t::le_zero());
    BOOST_CHECK(D.at(z, y) == bound_t::le_zero());

    // x - y <= 0, y - x <= 5
    BOOST_CHECK(D.at(x, y) == bound_t::le_zero());
    BOOST_CHECK(D.at(y, x) == bound_t::non_strict(5));

    // x,y,z in [0, 5]
    BOOST_CHECK(D.at(x, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(y, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(z, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(0, x) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, y) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, z) == bound_t::le_zero());

    // Copy z to be the value of x
    D.copy(z, x);
    D.future();
    D.restrict(y, 0, bound_t::non_strict(5));

    // x = z
    BOOST_CHECK(D.at(x, z) == bound_t::le_zero());
    BOOST_CHECK(D.at(z, x) == bound_t::le_zero());

    // x - y <= 0, y - x <= 5 also for z instead of x
    BOOST_CHECK(D.at(x, y) == bound_t::le_zero());
    BOOST_CHECK(D.at(y, x) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(z, y) == bound_t::le_zero());
    BOOST_CHECK(D.at(y, z) == bound_t::non_strict(5));

    // x,y,z in [0, 5]
    BOOST_CHECK(D.at(x, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(y, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(z, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(0, x) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, y) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, z) == bound_t::le_zero());

    // Reset x
    D.assign(x, 0);
    D.future();
    D.restrict(y, 0, bound_t::non_strict(5));

    // x - z <= 0 and z - x <= 5
    BOOST_CHECK(D.at(x, z) == bound_t::le_zero());
    BOOST_CHECK(D.at(z, x) == bound_t::non_strict(5));

    // z - y <= 0 and y - z <= 5
    BOOST_CHECK(D.at(z, y) == bound_t::le_zero());
    BOOST_CHECK(D.at(y, z) == bound_t::non_strict(5));

    // x,y,z in [0, 5]
    BOOST_CHECK(D.at(x, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(y, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(z, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(0, x) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, y) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, z) == bound_t::le_zero());

    // To goal
    BOOST_CHECK(D.is_satisfying(x, 0, bound_t::non_strict(2)));
    BOOST_CHECK(D.is_satisfying(0, x, bound_t::non_strict(-2)));
    BOOST_CHECK(D.is_satisfying(y, 0, bound_t::non_strict(4)));
    BOOST_CHECK(D.is_satisfying(0, y, bound_t::non_strict(-4)));
    BOOST_CHECK(D.is_satisfying(z, 0, bound_t::non_strict(3)));
    BOOST_CHECK(D.is_satisfying(0, z, bound_t::non_strict(-3)));

    D.restrict(x, 0, bound_t::non_strict(2));
    D.restrict(0, x, bound_t::non_strict(-2));
    D.restrict(y, 0, bound_t::non_strict(4));
    D.restrict(0, y, bound_t::non_strict(-4));
    D.restrict(z, 0, bound_t::non_strict(3));
    D.restrict(0, z, bound_t::non_strict(-3));
    D.future();

    // y - x = 2
    BOOST_CHECK(D.at(y, x) == bound_t::non_strict(2));
    BOOST_CHECK(D.at(x, y) == bound_t::non_strict(-2));

    // y - z = 1
    BOOST_CHECK(D.at(y, z) == bound_t::non_strict(1));
    BOOST_CHECK(D.at(z, y) == bound_t::non_strict(-1));

    // z - x = 1
    BOOST_CHECK(D.at(z, x) == bound_t::non_strict(1));
    BOOST_CHECK(D.at(x, z) == bound_t::non_strict(-1));

    // x >= 2, y >= 4, z >= 3
    BOOST_CHECK(D.at(x, 0) == bound_t::inf());
    BOOST_CHECK(D.at(y, 0) == bound_t::inf());
    BOOST_CHECK(D.at(z, 0) == bound_t::inf());
    BOOST_CHECK(D.at(0, x) == bound_t::non_strict(-2));
    BOOST_CHECK(D.at(0, y) == bound_t::non_strict(-4));
    BOOST_CHECK(D.at(0, z) == bound_t::non_strict(-3));
}

BOOST_AUTO_TEST_CASE(remove_clock_test_1) {
    DBM D(4);
    D.set(1, 0, bound_t::inf());
    D.set(3, 0, bound_t::inf());
    D.set(1, 2, bound_t::inf());
    D.set(3, 2, bound_t::inf());

    D.remove_clock(2);

    BOOST_CHECK(D.dimension() == 3);

    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(2, 0) == bound_t::inf());

    BOOST_CHECK(D.at(0, 0) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, 2) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 2) == bound_t::le_zero());
    BOOST_CHECK(D.at(2, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(2, 2) == bound_t::le_zero());
}

BOOST_AUTO_TEST_CASE(remove_clock_test_2) {
    DBM D(10);

    BOOST_CHECK_THROW(D.remove_clock(0), base_error);
    BOOST_CHECK_THROW(D.remove_clock(10), base_error);
}

BOOST_AUTO_TEST_CASE(swap_clocks_test_1) {
    dim_t size = 5, a = 2, b = 4;
    DBM D(size);

    for (dim_t i = 0; i < size; ++i)
        for (dim_t j = 0; j < size; ++j) {
            if (i == j) continue;
            D.set(i, j, bound_t::non_strict(j + (i * size)));
        }

    D.swap_clocks(a, b);

    for (dim_t i = 0; i < size; ++i)
        for (dim_t j = 0; j < size; ++j) {
            bound_t bound;
            if (i == j)
                bound = bound_t::le_zero();
            else if ((i == a && j == b) || (i == b && j == a))
                bound = bound_t::non_strict(i + (j * size));
            else if (i == a)
                bound = bound_t::non_strict(j + (b * size));
            else if (i == b)
                bound = bound_t::non_strict(j + (a * size));
            else if (j == a)
                bound = bound_t::non_strict(b + (i * size));
            else if (j == b)
                bound = bound_t::non_strict(a + (i * size));
            else
                bound = bound_t::non_strict(j + (i * size));

            BOOST_CHECK(D.at(i, j) == bound);
        }
}

BOOST_AUTO_TEST_CASE(swap_clocks_test_2) {
    DBM D(7);

    BOOST_CHECK_THROW(D.swap_clocks(0, 5), base_error);
    BOOST_CHECK_THROW(D.swap_clocks(1, 0), base_error);
    BOOST_CHECK_THROW(D.swap_clocks(2, 7), base_error);
    BOOST_CHECK_THROW(D.swap_clocks(7, 5), base_error);
    BOOST_CHECK_THROW(D.swap_clocks(0, 7), base_error);
}

BOOST_AUTO_TEST_CASE(add_clock_test_1) {
    DBM D(4);
    D.set(1, 0, bound_t::inf());
    D.set(3, 0, bound_t::inf());
    D.set(1, 2, bound_t::inf());
    D.set(3, 2, bound_t::inf());

    D.add_clock_at(3);

    BOOST_CHECK(D.dimension() == 5);

    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(1, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 2) == bound_t::inf());
    BOOST_CHECK(D.at(1, 3) == D.at(1, 0));
    BOOST_CHECK(D.at(1, 4) == bound_t::le_zero());

    BOOST_CHECK(D.at(2, 0) == bound_t::le_zero());
    BOOST_CHECK(D.at(2, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(2, 2) == bound_t::le_zero());
    BOOST_CHECK(D.at(2, 3) == D.at(2, 0));
    BOOST_CHECK(D.at(2, 4) == bound_t::le_zero());

    BOOST_CHECK(D.at(3, 0) == bound_t::inf());
    BOOST_CHECK(D.at(3, 1) == bound_t::inf());
    BOOST_CHECK(D.at(3, 2) == bound_t::inf());
    BOOST_CHECK(D.at(3, 3) == bound_t::le_zero());
    BOOST_CHECK(D.at(3, 4) == bound_t::inf());

    BOOST_CHECK(D.at(4, 0) == bound_t::inf());
    BOOST_CHECK(D.at(4, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(4, 2) == bound_t::inf());
    BOOST_CHECK(D.at(4, 3) == D.at(4, 0));
    BOOST_CHECK(D.at(4, 4) == bound_t::le_zero());
}

BOOST_AUTO_TEST_CASE(add_clock_test_2) {
    DBM D(9);

    BOOST_CHECK_THROW(D.add_clock_at(0), base_error);
    BOOST_CHECK_THROW(D.add_clock_at(10), base_error);
}

BOOST_AUTO_TEST_CASE(resize_test_1) {
    DBM D(5);
    D.future();
    D.assign(1, 1);
    D.assign(2, 2);
    D.assign(4, 4);

    std::vector<bool> src {true, true, false, true, true};
    std::vector<bool> dst {true, false, true, true, true};
    std::vector<dim_t> indir = D.resize(src, dst);


    BOOST_CHECK(D.is_satisfying(1, 0, bound_t::le_zero()));
    BOOST_CHECK(D.is_satisfying(0, 1, bound_t::le_zero()));

    BOOST_CHECK(D.is_satisfying(2, 0, bound_t::non_strict(1)));
    BOOST_CHECK(D.is_satisfying(0, 2, bound_t::non_strict(1)));

    BOOST_CHECK(D.is_satisfying(3, 0, bound_t::inf()));
    BOOST_CHECK(D.is_satisfying(0, 3, bound_t::le_zero()));

    BOOST_CHECK(D.is_satisfying(4, 0, bound_t::non_strict(4)));
    BOOST_CHECK(D.is_satisfying(0, 4, bound_t::non_strict(4)));
}

BOOST_AUTO_TEST_CASE(resize_test_2) {
    DBM D(4);
    std::vector<bool> src1{true, true, false, true, false};
    std::vector<bool> dst1{true, true, true, true};
    std::vector<bool> src2{true, true, true, false};
    std::vector<bool> dst2{true, true, true, true};

    BOOST_CHECK_THROW(D.resize(src1, dst1), base_error);
    BOOST_CHECK_THROW(D.resize(src2, dst2), base_error);

}

BOOST_AUTO_TEST_CASE(reorder_test_1) {
    DBM D(5);
    std::vector<dim_t> order = {0, 2,(dim_t) ~0, 1, 3};

    D.future();
    D.assign(1, 0);
    D.assign(2, 3);
    D.assign(4, 10);

    D.reorder(order, 4);

    BOOST_CHECK(D.dimension() == 4);

    BOOST_CHECK(D.is_satisfying(1, 0, bound_t::non_strict(3)));
    BOOST_CHECK(D.is_satisfying(0, 1, bound_t::non_strict(3)));

    BOOST_CHECK(D.is_satisfying(2, 0, bound_t::le_zero()));
    BOOST_CHECK(D.is_satisfying(0, 2, bound_t::le_zero()));

    BOOST_CHECK(D.is_satisfying(3, 0, bound_t::inf()));
    BOOST_CHECK(D.is_satisfying(0, 3, bound_t::le_zero()));
}

BOOST_AUTO_TEST_CASE(reorder_test_2) {
    DBM D(4);
    std::vector<dim_t> order1{0, 1, 2, 3, 2};
    std::vector<dim_t> order2{0, (dim_t) ~0, 1, 2};
    std::vector<dim_t> order3{0, (dim_t) ~0, 3, 2};
    DBM D1 = D;
    BOOST_CHECK_THROW(D1.reorder(order1, 4), base_error);
    D1 = D;
    BOOST_CHECK_THROW(D1.reorder(order2, 2), base_error);
    D1 = D;
    BOOST_CHECK_THROW(D1.reorder(order2, 4), base_error);
    D1 = D;
    BOOST_CHECK_NO_THROW(D1.reorder(order2, 3));
    D1 = D;
    BOOST_CHECK_THROW(D1.reorder(order3, 3), base_error);
}

BOOST_AUTO_TEST_CASE(extrapolate_test_1) {
    DBM D(10);
    std::vector ceiling{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    BOOST_CHECK_THROW(D.extrapolate(ceiling), base_error);
}

BOOST_AUTO_TEST_CASE(extrapolate_test_2) {
    /* Classical Extrapolation
     * Example from Behrmann, Gerd & Bouyer, Patricia & Larsen, Kim & Pelánek, Radek. (2004).
     * Lower and Upper Bounds in Zone Based Abstractions of Timed Automata. 312-326. 10.1007/978-3-540-24730-2_25.
     *
     *   M 8-|                               M 8-|
     *     7-|              @                  7-|              @  .  .
     *     6-|           @  @                  6-|           @  @  .
     *     5-|        @  @  @         ----\    5-|        @  @  @
     *     4-|     @  @  @            ----/    4-|     @  @  @
     *     3-|     @  @                        3-|  .  @  @
     *     2-|     @                           2-|  .  @
     *     1-|                                 1-|
     *       |_____________________              |_____________________
     *          |  |  |  |  |  |  |                 |  |  |  |  |  |  |
     *          1  2  3  4  5  6  7                 1  2  3  4  5  6  7
     *          M                                   M
     */

    DBM D(3);
    D.set(0, 1, bound_t::non_strict(-2));
    D.set(0, 2, bound_t::non_strict(-2));
    D.set(1, 0, bound_t::non_strict(5));
    D.set(1, 2, bound_t::le_zero());
    D.set(2, 0, bound_t::non_strict(7));
    D.set(2, 1, bound_t::non_strict(2));

    std::vector<val_t> max {0, 1, 7};

    D.extrapolate(max);

    BOOST_CHECK(D.at(0, 0) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, 1) == bound_t::strict(-1));
    BOOST_CHECK(D.at(0, 2) == bound_t::non_strict(-2));
    BOOST_CHECK(D.at(1, 0) == bound_t::non_strict(7));
    BOOST_CHECK(D.at(1, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 2) == bound_t::le_zero());
    BOOST_CHECK(D.at(2, 0) == bound_t::non_strict(7));
    BOOST_CHECK(D.at(2, 1) == bound_t::non_strict(2));
    BOOST_CHECK(D.at(2, 2) == bound_t::le_zero());
}

BOOST_AUTO_TEST_CASE(extrapolate_diagonal_test_1) {
    /* Diagonal Extrapolation
     * Example from Behrmann, Gerd & Bouyer, Patricia & Larsen, Kim & Pelánek, Radek. (2004).
     * Lower and Upper Bounds in Zone Based Abstractions of Timed Automata. 312-326. 10.1007/978-3-540-24730-2_25.
     *   M 8-|                                M 8-|
     *     7-|              @                   7-|  .  .  .  .  @  .  .  ..
     *     6-|           @  @                   6-|  .  .  .  @  @  .  .  ..
     *     5-|        @  @  @         ----\     5-|  .  .  @  @  @  .  .  ..
     *     4-|     @  @  @            ----/     4-|  .  @  @  @  .  .  .  ..
     *     3-|     @  @                         3-|  .  @  @  .  .  .  .  ..
     *     2-|     @                            2-|  .  @  .  .  .  .  .  ..
     *     1-|                                  1-|
     *       |_____________________               |_____________________
     *          |  |  |  |  |  |  |                  |  |  |  |  |  |  |
     *          1  2  3  4  5  6  7                  1  2  3  4  5  6  7
     *          M                                    M
     */
    DBM D(3);
    D.set(0, 1, bound_t::non_strict(-2));
    D.set(0, 2, bound_t::non_strict(-2));
    D.set(1, 0, bound_t::non_strict(5));
    D.set(1, 2, bound_t::le_zero());
    D.set(2, 0, bound_t::non_strict(7));
    D.set(2, 1, bound_t::non_strict(2));

    std::vector<val_t> max {0, 1, 7};

    D.extrapolate_diagonal(max);

    BOOST_CHECK(D.at(0, 0) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, 1) == bound_t::strict(-1));
    BOOST_CHECK(D.at(0, 2) == bound_t::non_strict(-2));
    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(1, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 2) == bound_t::inf());
    BOOST_CHECK(D.at(2, 0) == bound_t::non_strict(7));
    BOOST_CHECK(D.at(2, 1) == bound_t::strict(6));
    BOOST_CHECK(D.at(2, 2) == bound_t::le_zero());
}

BOOST_AUTO_TEST_CASE(extrapolate_diagonal_test_2) {
    DBM D(3);
    D.set(1, 0, bound_t::inf());
    D.set(2, 0, bound_t::inf());
    std::vector<val_t> ceiling = {0, -1073741823, -1073741823};

    D.extrapolate_diagonal(ceiling);

    BOOST_CHECK(!D.is_empty());
    BOOST_CHECK(D.at(0, 0) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(2, 2) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, 2) == bound_t::le_zero());

    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(2, 0) == bound_t::inf());
    BOOST_CHECK(D.at(1, 2) == bound_t::inf());
    BOOST_CHECK(D.at(2, 1) == bound_t::inf());
}

BOOST_AUTO_TEST_CASE(extrapolate_diagonal_test_3) {
    DBM D(4);
//  Extrapolating: [0, 1, -1073741823, 3]
//  <=0     <=0     <=0     <=0
//  <=1     <=0     <=1     <=0
//  INF     INF     <=0     INF
//  <=1     <=0     <=1     <=0
    std::vector<val_t> ceiling = {0, 1, -1073741823, 3};

    D.set(1, 0, bound_t::non_strict(1));
    D.set(1, 2, bound_t::non_strict(1));
    D.set(3, 0, bound_t::non_strict(1));
    D.set(3, 2, bound_t::non_strict(1));
    D.set(2, 0, bound_t::inf());
    D.set(2, 1, bound_t::inf());
    D.set(2, 3, bound_t::inf());

    D.extrapolate_diagonal(ceiling);

//  <=0     <=0     <=0     <=0
//  <=1     <=0     <=1     <=0
//  INF     INF     <=0     INF
//  <=1     <=0     <=1     <=0
    BOOST_CHECK(D.at(1, 0) == bound_t::non_strict(1));
    BOOST_CHECK(D.at(1, 2) == bound_t::non_strict(1));
    BOOST_CHECK(D.at(3, 0) == bound_t::non_strict(1));
    BOOST_CHECK(D.at(3, 2) == bound_t::non_strict(1));
    BOOST_CHECK(D.at(2, 0) == bound_t::inf());
    BOOST_CHECK(D.at(2, 1) == bound_t::inf());
    BOOST_CHECK(D.at(2, 3) == bound_t::inf());
}

BOOST_AUTO_TEST_CASE(extrapolate_diagonal_test_4) {
//    <=0     <=-6    <=0     <=0     <=0
//    INF     <=0     INF     INF     INF
//    INF     INF     <=0     INF     INF
//    INF     <=-6    <=0     <=0     <=0
//    INF     <=-6    <=0     <=0     <=0
    DBM D(5);
    std::vector<val_t> ceiling = {0, 3, -1073741823, 3, 3};

    D.set(0, 1, bound_t::non_strict(-6));
    D.set(3, 1, bound_t::non_strict(-6));
    D.set(4, 1, bound_t::non_strict(-6));

    D.set(1, 0, bound_t::inf());
    D.set(2, 0, bound_t::inf());
    D.set(3, 0, bound_t::inf());
    D.set(4, 0, bound_t::inf());

    D.set(2, 1, bound_t::inf());
    D.set(1, 2, bound_t::inf());
    D.set(1, 3, bound_t::inf());
    D.set(2, 3, bound_t::inf());
    D.set(1, 4, bound_t::inf());
    D.set(2, 4, bound_t::inf());

    D.extrapolate_diagonal(ceiling);

//    <=0     <-3     <=0     <=0     <=0
//    INF     <=0     INF     INF     INF
//    INF     INF     <=0     INF     INF
//    INF     INF     INF     <=0     <=0
//    INF     INF     INF     <=0     <=0

    BOOST_CHECK(D.at(0, 0) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(2, 0) == bound_t::inf());
    BOOST_CHECK(D.at(3, 0) == bound_t::inf());
    BOOST_CHECK(D.at(4, 0) == bound_t::inf());

    BOOST_CHECK(D.at(0, 1) == bound_t::strict(-3));
    BOOST_CHECK(D.at(1, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(2, 1) == bound_t::inf());
    BOOST_CHECK(D.at(3, 1) == bound_t::inf());
    BOOST_CHECK(D.at(4, 1) == bound_t::inf());

    BOOST_CHECK(D.at(0, 2) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 2) == bound_t::inf());
    BOOST_CHECK(D.at(2, 2) == bound_t::le_zero());
    BOOST_CHECK(D.at(3, 2) == bound_t::inf());
    BOOST_CHECK(D.at(4, 2) == bound_t::inf());

    BOOST_CHECK(D.at(0, 3) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 3) == bound_t::inf());
    BOOST_CHECK(D.at(2, 3) == bound_t::inf());
    BOOST_CHECK(D.at(3, 3) == bound_t::le_zero());
    BOOST_CHECK(D.at(4, 3) == bound_t::le_zero());

    BOOST_CHECK(D.at(0, 4) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 4) == bound_t::inf());
    BOOST_CHECK(D.at(2, 4) == bound_t::inf());
    BOOST_CHECK(D.at(3, 4) == bound_t::le_zero());
    BOOST_CHECK(D.at(4, 4) == bound_t::le_zero());

}

BOOST_AUTO_TEST_CASE(extrapolate_diagonal_test_5) {
    DBM D(10);
    std::vector ceiling{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    BOOST_CHECK_THROW(D.extrapolate_diagonal(ceiling), base_error);
}

BOOST_AUTO_TEST_CASE(extrapolate_diagonal_test_6) {
    DBM dbm = DBM::unconstrained(4);
    std::vector ceiling{0, 2, 2, 6};

    dbm.set(0, 1, {-2, STRICT});
    dbm.set(2, 1, {-2, STRICT});
    dbm.set(3, 2, {2, NON_STRICT});
    dbm.set(0, 2, bound_t::le_zero());
    dbm.set(0, 3, bound_t::le_zero());
    dbm.set(2, 3, bound_t::le_zero());
    dbm.set(3, 1, {0, STRICT});

    BOOST_CHECK(!dbm.is_empty());

    auto cpy = dbm;

    dbm.extrapolate_diagonal(ceiling);

    BOOST_CHECK(dbm.is_equal(cpy));
}

BOOST_AUTO_TEST_CASE(extrapolate_lu_test_1) {
    /* LU Extrapolation
     * Example from Behrmann, Gerd & Bouyer, Patricia & Larsen, Kim & Pelánek, Radek. (2004).
     * Lower and Upper Bounds in Zone Based Abstractions of Timed Automata. 312-326. 10.1007/978-3-540-24730-2_25.
     *                                               :  :  :  :  :  :  :  :
     *   U 8-|                                U 8-|  .  .  .  .  .  .  .  .
     *     7-|              @                   7-|  .  .  .  .  @  .  .
     *     6-|           @  @                   6-|  .  .  .  @  @  .
     *     5-|        @  @  @         ----\     5-|  .  .  @  @  @
     *     4-|     @  @  @            ----/     4-|  .  @  @  @
     *     3-|     @  @                         3-|  .  @  @
     *     2-|     @                            2-|  .  @
     *   L 1-|                                L 1-|
     *       |_____________________               |_____________________
     *          |  |  |  |  |  |  |                  |  |  |  |  |  |  |
     *          1  2  3  4  5  6  7                  1  2  3  4  5  6  7
     *         L/U                                  L/U
     */
    DBM D(3);
    D.set(0, 1, bound_t::non_strict(-2));
    D.set(0, 2, bound_t::non_strict(-2));
    D.set(1, 0, bound_t::non_strict(5));
    D.set(1, 2, bound_t::le_zero());
    D.set(2, 0, bound_t::non_strict(7));
    D.set(2, 1, bound_t::non_strict(2));

    std::vector<val_t> upper {0, 1, 7};
    std::vector<val_t> lower {0, 1, 1};

    D.extrapolate_lu(lower, upper);

    BOOST_CHECK(D.at(0, 0) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, 1) == bound_t::strict(-1));
    BOOST_CHECK(D.at(0, 2) == bound_t::non_strict(-2));
    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(1, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 2) == bound_t::le_zero());
    BOOST_CHECK(D.at(2, 0) == bound_t::inf());
    BOOST_CHECK(D.at(2, 1) == bound_t::inf());
    BOOST_CHECK(D.at(2, 2) == bound_t::le_zero());
}

BOOST_AUTO_TEST_CASE(extrapolate_lu_diagonal_test_1) {
    /* Diagonal LU Extrapolation
     * Example from Behrmann, Gerd & Bouyer, Patricia & Larsen, Kim & Pelánek, Radek. (2004).
     * Lower and Upper Bounds in Zone Based Abstractions of Timed Automata. 312-326. 10.1007/978-3-540-24730-2_25.
     *                                               :  :  :  :  :  :  :  :.
     *   U 8-|                                U 8-|  .  .  .  .  .  .  .  ..
     *     7-|              @                   7-|  .  .  .  .  @  .  .  ..
     *     6-|           @  @                   6-|  .  .  .  @  @  .  .  ..
     *     5-|        @  @  @         ----\     5-|  .  .  @  @  @  .  .  ..
     *     4-|     @  @  @            ----/     4-|  .  @  @  @  .  .  .  ..
     *     3-|     @  @                         3-|  .  @  @  .  .  .  .  ..
     *     2-|     @                            2-|  .  @  .  .  .  .  .  ..
     *   L 1-|                                L 1-|
     *       |_____________________               |_____________________
     *          |  |  |  |  |  |  |                  |  |  |  |  |  |  |
     *          1  2  3  4  5  6  7                  1  2  3  4  5  6  7
     *         L/U                                  L/U
     */
    DBM D(3);
    D.set(0, 1, bound_t::non_strict(-2));
    D.set(0, 2, bound_t::non_strict(-2));
    D.set(1, 0, bound_t::non_strict(5));
    D.set(1, 2, bound_t::le_zero());
    D.set(2, 0, bound_t::non_strict(7));
    D.set(2, 1, bound_t::non_strict(2));

    std::vector<val_t> upper {0, 1, 7};
    std::vector<val_t> lower {0, 1, 1};

    D.extrapolate_lu_diagonal(lower, upper);

    BOOST_CHECK(D.at(0, 0) == bound_t::le_zero());
    BOOST_CHECK(D.at(0, 1) == bound_t::strict(-1));
    BOOST_CHECK(D.at(0, 2) == bound_t::non_strict(-2));
    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(1, 1) == bound_t::le_zero());
    BOOST_CHECK(D.at(1, 2) == bound_t::inf());
    BOOST_CHECK(D.at(2, 0) == bound_t::inf());
    BOOST_CHECK(D.at(2, 1) == bound_t::inf());
    BOOST_CHECK(D.at(2, 2) == bound_t::le_zero());
}

BOOST_AUTO_TEST_CASE(intersection_test_1) {
    auto dbm1 = DBM::zero(3);
    auto dbm2 = DBM::zero(3);

    dbm1.intersection(dbm2);

    BOOST_CHECK(dbm1.is_equal(dbm2));
}

BOOST_AUTO_TEST_CASE(intersection_test_2) {
    auto dbm1 = DBM::unconstrained(3);
    auto dbm2 = DBM::zero(3);

    dbm1.restrict(0, 1, bound_t::strict(-2));
    dbm2.future();

    BOOST_CHECK(dbm1.is_intersecting(dbm2));

    dbm1.intersection(dbm2);

    BOOST_CHECK(dbm1.is_satisfying(0, 1, bound_t::strict(-2)));
    BOOST_CHECK(not dbm1.is_satisfying(1, 0, bound_t::non_strict(-2)));
    BOOST_CHECK(dbm1.is_satisfying(1, 0, bound_t::inf()));
}

BOOST_AUTO_TEST_CASE(intersection_test_3) {
    auto dbm1 = DBM::unconstrained(3);
    auto dbm2 = DBM::unconstrained(3);

    dbm1.restrict(1, 0, bound_t::non_strict(10));
    dbm2.restrict(0, 1, bound_t::strict(-10));

    BOOST_CHECK(not dbm1.is_intersecting(dbm2));

    dbm1.intersection(dbm2);

    BOOST_CHECK(dbm1.is_empty());
    BOOST_CHECK(not dbm1.is_intersecting(dbm2));
    BOOST_CHECK(not dbm2.is_intersecting(dbm1));

    dbm2.intersection(dbm1);

    BOOST_CHECK(dbm2.is_empty());
    BOOST_CHECK(not dbm1.is_intersecting(dbm2));
    BOOST_CHECK(not dbm2.is_intersecting(dbm1));
}

BOOST_AUTO_TEST_CASE(is_unbounded_test_1) {
    DBM D(3);
    BOOST_CHECK(!D.is_unbounded());

    D.future();
    BOOST_CHECK(D.is_unbounded());
}

BOOST_AUTO_TEST_CASE(is_unbounded_test_2) {
    DBM dbm(1);

    BOOST_CHECK(dbm.is_unbounded());

    dbm.future();

    BOOST_CHECK(dbm.is_unbounded());
}

BOOST_AUTO_TEST_CASE(relation_test_1) {
    DBM D1(2);
    DBM D2(3);

    BOOST_CHECK(D1.relation(D2).is_different());
    BOOST_CHECK(not D1.is_equal(D2));
    BOOST_CHECK(not D1.is_subset(D2));
    BOOST_CHECK(not D1.is_superset(D2));
    BOOST_CHECK(D2.relation(D1).is_different());
    BOOST_CHECK(not D2.is_equal(D1));
    BOOST_CHECK(not D2.is_subset(D1));
    BOOST_CHECK(not D2.is_superset(D1));
}

BOOST_AUTO_TEST_CASE(relation_test_2) {
    DBM D1(2);
    DBM D2(2);

    D2.future();

    BOOST_CHECK(D1.relation(D2).is_subset());
    BOOST_CHECK(D1.is_subset(D2));
    BOOST_CHECK(D2.relation(D1).is_superset());
    BOOST_CHECK(D2.is_superset(D1));

    D1.free(1);

    BOOST_CHECK(D1.relation(D2).is_equal());
    BOOST_CHECK(D1.is_equal(D2));
    BOOST_CHECK(D2.relation(D1).is_equal());
    BOOST_CHECK(D2.is_equal(D1));
}

BOOST_AUTO_TEST_CASE(relation_test_3) {
    DBM D1(2);
    DBM D2(2);

    D1.assign(1, 10);
    D2.assign(1, 5);

    BOOST_CHECK(D1.relation(D2).is_different());
    BOOST_CHECK(not D1.is_equal(D2));
    BOOST_CHECK(not D1.is_subset(D2));
    BOOST_CHECK(not D1.is_superset(D2));

    D1.future();
    D2.future();

    BOOST_CHECK(D1.relation(D2).is_subset());
    BOOST_CHECK(D1.is_subset(D2));
    BOOST_CHECK(D2.relation(D1).is_superset());
    BOOST_CHECK(D2.is_superset(D1));
    BOOST_CHECK(not D2.relation(D1).is_equal());
    BOOST_CHECK(not D2.is_equal(D1));
    BOOST_CHECK(not D1.is_equal(D2));

    D1.assign(1, 10);

    BOOST_CHECK(D1.relation(D2).is_subset());
    BOOST_CHECK(D1.is_subset(D2));
    BOOST_CHECK(not D1.is_superset(D2));

    BOOST_CHECK(D2.relation(D1).is_superset());
    BOOST_CHECK(D2.is_superset(D1));
    BOOST_CHECK(not D2.is_subset(D1));

    BOOST_CHECK(!D2.relation(D1).is_equal());
    BOOST_CHECK(not D2.is_equal(D1));
    BOOST_CHECK(not D1.is_equal(D2));
}

BOOST_AUTO_TEST_CASE(relation_test_4) {
    DBM a(1), b(1);

    a.future();
    BOOST_CHECK(a.is_equal(b));
}

BOOST_AUTO_TEST_CASE(relation_t_test_1) {
    DBM a(10), b(10);

    a.future();

    BOOST_CHECK(a.relation(b).type() == SUPERSET);
    
    b.future();

    BOOST_CHECK(a.relation(b).type() == EQUAL);
}

BOOST_AUTO_TEST_CASE(intersects_test_1) {
    DBM dbm1(3);
    DBM dbm2(3);

    BOOST_CHECK(dbm1.is_intersecting(dbm2));
    BOOST_CHECK(dbm2.is_intersecting(dbm1));
    dbm1.future();

    BOOST_CHECK(dbm1.is_intersecting(dbm2));
    BOOST_CHECK(dbm2.is_intersecting(dbm1));

    dbm1.free(1);
    dbm1.free(2);

    BOOST_CHECK(dbm1.is_intersecting(dbm2));
    BOOST_CHECK(dbm2.is_intersecting(dbm1));

    dbm1.restrict(0, 1, bound_t::strict(-2));
    dbm1.restrict(0, 2, bound_t::strict(-3));

    BOOST_CHECK(not dbm1.is_intersecting(dbm2));
    BOOST_CHECK(not dbm2.is_intersecting(dbm1));
}

BOOST_AUTO_TEST_CASE(intersects_test_2) {
    DBM dbm1(3);
    DBM dbm2(3);

    dbm1.future();
    dbm2.restrict(0, 1, bound_t::strict(-1));

    BOOST_CHECK(not dbm1.is_intersecting(dbm2));
    BOOST_CHECK(not dbm2.is_intersecting(dbm1));
}

BOOST_AUTO_TEST_CASE(intersects_test_3) {
    auto dbm1 = DBM::unconstrained(3);
    auto dbm2 = DBM::unconstrained(3);

    dbm1.restrict(1, 0, bound_t::strict(1));
    dbm2.restrict(0, 1, bound_t::strict(-1));
    
    BOOST_CHECK(not dbm1.is_intersecting(dbm2));
    BOOST_CHECK(not dbm2.is_intersecting(dbm1));
}

BOOST_AUTO_TEST_CASE(zero_test_1) {
    dim_t dimension = 10;
    DBM dbm(dimension);
    for (dim_t i = 0; i < dimension; ++i)
        for (dim_t j = 0; j < dimension; ++j)
            dbm.set(i, j, bound_t::le_zero());

    BOOST_CHECK(DBM::zero(dimension).is_equal(dbm));
}

BOOST_AUTO_TEST_CASE(unconstrained_test_1) {
    dim_t dimension = 10;
    DBM dbm(dimension);
    for (dim_t i = 0; i < dimension; ++i) {
        for (dim_t j = 0; j < dimension; ++j) {
            if (j == i || i == 0)
                dbm.set(i, j, bound_t::le_zero());
            else
                dbm.set(i, j, bound_t::inf());
        }
    }

    BOOST_CHECK(DBM::unconstrained(dimension).is_equal(dbm));
}