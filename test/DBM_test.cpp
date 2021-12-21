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
#include <dbm/DBM.h>

using namespace dbm;

BOOST_AUTO_TEST_CASE(Close_Test_1) {
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
    bound_t g(5, false);

    D.future();
    D.restrict(1, 0, g);
    BOOST_CHECK(D._bounds_table.get(1, 0) == g);

    DBM Q = D;
    D.close();

    for (dim_t i = 0; i < 3; i++)
        for (dim_t j = 0; j < 3; j++)
            BOOST_CHECK(D._bounds_table.get(i, j) == Q._bounds_table.get(i, j));
}

BOOST_AUTO_TEST_CASE(Restrict_Test_2) {
    DBM D(3);
    bound_t g(-5, false);

    D.future();
    D.restrict(1, 0, g);

    BOOST_CHECK(D.is_empty());
}

BOOST_AUTO_TEST_CASE(Trace_1) {
    DBM D(4);
    dim_t x = 1, y = 2, z = 3;
    std::vector<val_t> ceiling{0, 6, 10, 10};

    D.future();
    D.restrict(y, 0, bound_t(10, false));

    // x = y = z
    BOOST_CHECK(D._bounds_table.at(x, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(y, x) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(x, z) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(z, x) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(y, z) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(z, y) == bound_t::zero());

    // x, y, z in [0, 10]
    BOOST_CHECK(D._bounds_table.at(x, 0) == bound_t(10, false));
    BOOST_CHECK(D._bounds_table.at(y, 0) == bound_t(10, false));
    BOOST_CHECK(D._bounds_table.at(z, 0) == bound_t(10, false));
    BOOST_CHECK(D._bounds_table.at(0, x) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, z) == bound_t::zero());


    // Reset x
    D.assign(x, 0);
    D.future();
    D.restrict(y, 0, bound_t(10, false));

    // y = z
    BOOST_CHECK(D._bounds_table.at(y, z) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(z, y) == bound_t::zero());

    // y - x in [0, 10]
    BOOST_CHECK(D._bounds_table.at(x, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(y, x) == bound_t(10, false));
    BOOST_CHECK(D._bounds_table.at(x, z) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(z, x) == bound_t(10, false));

    // Reset y
    D.assign(y, 0);
    D.future();
    D.restrict(y, 0, bound_t(10, false));

    // x, z in [0, 20]
    BOOST_CHECK(D._bounds_table.at(x, 0) == bound_t(20, false));
    BOOST_CHECK(D._bounds_table.at(z, 0) == bound_t(20, false));
    BOOST_CHECK(D._bounds_table.at(0, x) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, z) == bound_t::zero());

    // y in [0, 10]
    BOOST_CHECK(D._bounds_table.at(y, 0) == bound_t(10, false));
    BOOST_CHECK(D._bounds_table.at(0, y) == bound_t::zero());

    // z - x in [0, 10]
    BOOST_CHECK(D._bounds_table.at(z, x) == bound_t(10, false));
    BOOST_CHECK(D._bounds_table.at(x, z) == bound_t::zero());

    // x - y in [0, 10]
    BOOST_CHECK(D._bounds_table.at(x, y) == bound_t(10, false));
    BOOST_CHECK(D._bounds_table.at(y, x) == bound_t::zero());

    // z - y in [0, 10]
    BOOST_CHECK(D._bounds_table.at(z, y) == bound_t(10, false));
    BOOST_CHECK(D._bounds_table.at(y, z) == bound_t::zero());

    // Transition to Goal
    // guard: x > 6 && y == 2 && z - x >= 2
    BOOST_CHECK(D.is_satisfied(0, x, bound_t(-6, true)));
    BOOST_CHECK(D.is_satisfied(y, 0, bound_t(2, false)));
    BOOST_CHECK(D.is_satisfied(0, y, bound_t(-2, false)));
    BOOST_CHECK(D.is_satisfied(x, z, bound_t(-2, false)));

    D.restrict(0, x, bound_t(-6, true));
    D.assign(y, 2);
    D.restrict(x, z, bound_t(-2, false));
    D.future();
    D.norm(ceiling);
    //TODO: D.norm(ceiling, diff_bounds);

    //TODO: implement federations and normalization with clock differences
    // x > 6
    BOOST_CHECK(D._bounds_table.at(0, x) == bound_t(-6, true));
    BOOST_CHECK(D._bounds_table.at(x, 0) == bound_t::inf());

    // y >= 2
    BOOST_CHECK(D._bounds_table.at(0, y) == bound_t(-2, false));
    BOOST_CHECK(D._bounds_table.at(y, 0) == bound_t::inf());

    // z > 8
    BOOST_CHECK(D._bounds_table.at(0, z) == bound_t(-8, true));
    BOOST_CHECK(D._bounds_table.at(z, 0) == bound_t::inf());

    // z - x in [2, 6)
    BOOST_CHECK(D._bounds_table.at(z, x) == bound_t(6, true));
    BOOST_CHECK(D._bounds_table.at(x, z) == bound_t(-2, false));

    // x - y in (4, 8]
    BOOST_CHECK(D._bounds_table.at(x, y) == bound_t(8, false));
    BOOST_CHECK(D._bounds_table.at(y, x) == bound_t(-4, true));

    // z - y in (6, 10]
    BOOST_CHECK(D._bounds_table.at(z, y) == bound_t(10, false));
    BOOST_CHECK(D._bounds_table.at(x, z) == bound_t(-6, true));
}

BOOST_AUTO_TEST_CASE(Trace_2) {
    DBM D(4);
    dim_t x = 1, y = 2, z = 3;
    std::vector<val_t> ceiling{0, 2, 5, 3};

    D.future();
    D.restrict(y, 0, bound_t(5, false));

    // x = y = z
    BOOST_CHECK(D._bounds_table.at(x, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(y, x) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(x, z) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(z, x) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(y, z) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(z, y) == bound_t::zero());

    // x,y,z in [0, 5]
    BOOST_CHECK(D._bounds_table.at(x, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(y, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(z, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(0, x) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, z) == bound_t::zero());

    // Reset x
    D.assign(x, 0);
    D.future();
    D.restrict(y, 0, bound_t(5, false));

    // y = z
    BOOST_CHECK(D._bounds_table.at(y, z) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(z, y) == bound_t::zero());

    // x - y <= 0, y - x <= 5
    BOOST_CHECK(D._bounds_table.at(x, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(y, x) == bound_t(5, false));

    // x,y,z in [0, 5]
    BOOST_CHECK(D._bounds_table.at(x, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(y, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(z, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(0, x) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, z) == bound_t::zero());

    // Copy z to be the value of x
    D.copy(z, x);
    D.future();
    D.restrict(y, 0, bound_t(5, false));

    // x = z
    BOOST_CHECK(D._bounds_table.at(x, z) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(z, x) == bound_t::zero());

    // x - y <= 0, y - x <= 5 also for z instead of x
    BOOST_CHECK(D._bounds_table.at(x, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(y, x) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(z, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(y, z) == bound_t(5, false));

    // x,y,z in [0, 5]
    BOOST_CHECK(D._bounds_table.at(x, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(y, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(z, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(0, x) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, z) == bound_t::zero());

    // Reset x
    D.assign(x, 0);
    D.future();
    D.restrict(y, 0, bound_t(5, false));

    // x - z <= 0 and z - x <= 5
    BOOST_CHECK(D._bounds_table.at(x, z) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(z, x) == bound_t(5, false));

    // z - y <= 0 and y - z <= 5
    BOOST_CHECK(D._bounds_table.at(z, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(y, z) == bound_t(5, false));

    // x,y,z in [0, 5]
    BOOST_CHECK(D._bounds_table.at(x, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(y, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(z, 0) == bound_t(5, false));
    BOOST_CHECK(D._bounds_table.at(0, x) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, y) == bound_t::zero());
    BOOST_CHECK(D._bounds_table.at(0, z) == bound_t::zero());

    // To goal
    BOOST_CHECK(D.is_satisfied(x, 0, bound_t(2, false)));
    BOOST_CHECK(D.is_satisfied(0, x, bound_t(-2, false)));
    BOOST_CHECK(D.is_satisfied(y, 0, bound_t(4, false)));
    BOOST_CHECK(D.is_satisfied(0, y, bound_t(-4, false)));
    BOOST_CHECK(D.is_satisfied(z, 0, bound_t(3, false)));
    BOOST_CHECK(D.is_satisfied(0, z, bound_t(-3, false)));

    D.restrict(x, 0, bound_t(2, false));
    D.restrict(0, x, bound_t(-2, false));
    D.restrict(y, 0, bound_t(4, false));
    D.restrict(0, y, bound_t(-4, false));
    D.restrict(z, 0, bound_t(3, false));
    D.restrict(0, z, bound_t(-3, false));
    D.future();

    // y - x = 2
    BOOST_CHECK(D._bounds_table.at(y, x) == bound_t(2, false));
    BOOST_CHECK(D._bounds_table.at(x, y) == bound_t(-2, false));

    // y - z = 1
    BOOST_CHECK(D._bounds_table.at(y, z) == bound_t(1, false));
    BOOST_CHECK(D._bounds_table.at(z, y) == bound_t(-1, false));

    // z - x = 1
    BOOST_CHECK(D._bounds_table.at(z, x) == bound_t(1, false));
    BOOST_CHECK(D._bounds_table.at(x, z) == bound_t(-1, false));

    // x >= 2, y >= 4, z >= 3
    BOOST_CHECK(D._bounds_table.at(x, 0) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.at(y, 0) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.at(z, 0) == bound_t::inf());
    BOOST_CHECK(D._bounds_table.at(0, x) == bound_t(-2, false));
    BOOST_CHECK(D._bounds_table.at(0, y) == bound_t(-4, false));
    BOOST_CHECK(D._bounds_table.at(0, z) == bound_t(-3, false));
}