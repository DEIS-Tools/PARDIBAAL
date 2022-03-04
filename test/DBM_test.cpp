/*
 * Copyright Thomas M. Grosen
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
#include <dbm2/DBM.h>

using namespace dbm2;

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

BOOST_AUTO_TEST_CASE(delay_test_1) {
    DBM D(10);

    D.future();

    for (dim_t i = 0; i < 10; i++)
        for (dim_t j = 0; j < 10; j++) {
            if (i == 0 || j != 0)
                BOOST_CHECK(D.at(i, j) == bound_t::zero());
            else
                BOOST_CHECK(D.at(i, j) == bound_t::inf());
        }
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

BOOST_AUTO_TEST_CASE(trace_test_1) {
    DBM D(4);
    dim_t x = 1, y = 2, z = 3;
    std::vector<val_t> ceiling{0, 6, 10, 10};

    D.future();
    D.restrict(y, 0, bound_t::non_strict(10));

    // x = y = z
    BOOST_CHECK(D.at(x, y) == bound_t::zero());
    BOOST_CHECK(D.at(y, x) == bound_t::zero());
    BOOST_CHECK(D.at(x, z) == bound_t::zero());
    BOOST_CHECK(D.at(z, x) == bound_t::zero());
    BOOST_CHECK(D.at(y, z) == bound_t::zero());
    BOOST_CHECK(D.at(z, y) == bound_t::zero());

    // x, y, z in [0, 10]
    BOOST_CHECK(D.at(x, 0) == bound_t::non_strict(10));
    BOOST_CHECK(D.at(y, 0) == bound_t::non_strict(10));
    BOOST_CHECK(D.at(z, 0) == bound_t::non_strict(10));
    BOOST_CHECK(D.at(0, x) == bound_t::zero());
    BOOST_CHECK(D.at(0, y) == bound_t::zero());
    BOOST_CHECK(D.at(0, z) == bound_t::zero());


    // Reset x
    D.assign(x, 0);
    D.future();
    D.restrict(y, 0, bound_t::non_strict(10));

    // y = z
    BOOST_CHECK(D.at(y, z) == bound_t::zero());
    BOOST_CHECK(D.at(z, y) == bound_t::zero());

    // y - x in [0, 10]
    BOOST_CHECK(D.at(x, y) == bound_t::zero());
    BOOST_CHECK(D.at(y, x) == bound_t::non_strict(10));
    BOOST_CHECK(D.at(x, z) == bound_t::zero());
    BOOST_CHECK(D.at(z, x) == bound_t::non_strict(10));

    // Reset y
    D.assign(y, 0);
    D.future();
    D.restrict(y, 0, bound_t::non_strict(10));

    // x, z in [0, 20]
    BOOST_CHECK(D.at(x, 0) == bound_t::non_strict(20));
    BOOST_CHECK(D.at(z, 0) == bound_t::non_strict(20));
    BOOST_CHECK(D.at(0, x) == bound_t::zero());
    BOOST_CHECK(D.at(0, z) == bound_t::zero());

    // y in [0, 10]
    BOOST_CHECK(D.at(y, 0) == bound_t::non_strict(10));
    BOOST_CHECK(D.at(0, y) == bound_t::zero());

    // z - x in [0, 10]
    BOOST_CHECK(D.at(z, x) == bound_t::non_strict(10));
    BOOST_CHECK(D.at(x, z) == bound_t::zero());

    // x - y in [0, 10]
    BOOST_CHECK(D.at(x, y) == bound_t::non_strict(10));
    BOOST_CHECK(D.at(y, x) == bound_t::zero());

    // z - y in [0, 10]
    BOOST_CHECK(D.at(z, y) == bound_t::non_strict(10));
    BOOST_CHECK(D.at(y, z) == bound_t::zero());

    // Transition to Goal
    // guard: x > 6 && y == 2 && z - x >= 2
    BOOST_CHECK(D.is_satisfied(0, x, bound_t::strict(-6)));
    BOOST_CHECK(D.is_satisfied(y, 0, bound_t::non_strict(2)));
    BOOST_CHECK(D.is_satisfied(0, y, bound_t::non_strict(-2)));
    BOOST_CHECK(D.is_satisfied(x, z, bound_t::non_strict(-2)));

    D.restrict(0, x, bound_t::strict(-6));
    D.assign(y, 2);
    D.restrict(x, z, bound_t::non_strict(-2));
    D.future();
    D.norm(ceiling);
    //TODO: D.norm(ceiling, diff_bounds);

    //TODO: implement federations and normalization with clock differences
    // x > 6
    BOOST_CHECK(D.at(0, x) == bound_t::strict(-6));
    BOOST_CHECK(D.at(x, 0) == bound_t::inf());

    // y >= 2
    BOOST_CHECK(D.at(0, y) == bound_t::non_strict(-2));
    BOOST_CHECK(D.at(y, 0) == bound_t::inf());

    // z > 8
    BOOST_CHECK(D.at(0, z) == bound_t::strict(-8));
    BOOST_CHECK(D.at(z, 0) == bound_t::inf());

    // z - x in [2, 6)
    BOOST_CHECK(D.at(z, x) == bound_t::strict(6));
    BOOST_CHECK(D.at(x, z) == bound_t::non_strict(-2));

    // x - y in (4, 8]
    BOOST_CHECK(D.at(x, y) == bound_t::non_strict(8));
    BOOST_CHECK(D.at(y, x) == bound_t::strict(-4));

    // z - y in (6, 10]
    BOOST_CHECK(D.at(z, y) == bound_t::non_strict(10));
    BOOST_CHECK(D.at(x, z) == bound_t::strict(-6));
}

BOOST_AUTO_TEST_CASE(trace_test_2) {
    DBM D(4);
    dim_t x = 1, y = 2, z = 3;
    std::vector<val_t> ceiling{0, 2, 5, 3};

    D.future();
    D.restrict(y, 0, bound_t::non_strict(5));

    // x = y = z
    BOOST_CHECK(D.at(x, y) == bound_t::zero());
    BOOST_CHECK(D.at(y, x) == bound_t::zero());
    BOOST_CHECK(D.at(x, z) == bound_t::zero());
    BOOST_CHECK(D.at(z, x) == bound_t::zero());
    BOOST_CHECK(D.at(y, z) == bound_t::zero());
    BOOST_CHECK(D.at(z, y) == bound_t::zero());

    // x,y,z in [0, 5]
    BOOST_CHECK(D.at(x, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(y, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(z, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(0, x) == bound_t::zero());
    BOOST_CHECK(D.at(0, y) == bound_t::zero());
    BOOST_CHECK(D.at(0, z) == bound_t::zero());

    // Reset x
    D.assign(x, 0);
    D.future();
    D.restrict(y, 0, bound_t::non_strict(5));

    // y = z
    BOOST_CHECK(D.at(y, z) == bound_t::zero());
    BOOST_CHECK(D.at(z, y) == bound_t::zero());

    // x - y <= 0, y - x <= 5
    BOOST_CHECK(D.at(x, y) == bound_t::zero());
    BOOST_CHECK(D.at(y, x) == bound_t::non_strict(5));

    // x,y,z in [0, 5]
    BOOST_CHECK(D.at(x, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(y, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(z, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(0, x) == bound_t::zero());
    BOOST_CHECK(D.at(0, y) == bound_t::zero());
    BOOST_CHECK(D.at(0, z) == bound_t::zero());

    // Copy z to be the value of x
    D.copy(z, x);
    D.future();
    D.restrict(y, 0, bound_t::non_strict(5));

    // x = z
    BOOST_CHECK(D.at(x, z) == bound_t::zero());
    BOOST_CHECK(D.at(z, x) == bound_t::zero());

    // x - y <= 0, y - x <= 5 also for z instead of x
    BOOST_CHECK(D.at(x, y) == bound_t::zero());
    BOOST_CHECK(D.at(y, x) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(z, y) == bound_t::zero());
    BOOST_CHECK(D.at(y, z) == bound_t::non_strict(5));

    // x,y,z in [0, 5]
    BOOST_CHECK(D.at(x, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(y, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(z, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(0, x) == bound_t::zero());
    BOOST_CHECK(D.at(0, y) == bound_t::zero());
    BOOST_CHECK(D.at(0, z) == bound_t::zero());

    // Reset x
    D.assign(x, 0);
    D.future();
    D.restrict(y, 0, bound_t::non_strict(5));

    // x - z <= 0 and z - x <= 5
    BOOST_CHECK(D.at(x, z) == bound_t::zero());
    BOOST_CHECK(D.at(z, x) == bound_t::non_strict(5));

    // z - y <= 0 and y - z <= 5
    BOOST_CHECK(D.at(z, y) == bound_t::zero());
    BOOST_CHECK(D.at(y, z) == bound_t::non_strict(5));

    // x,y,z in [0, 5]
    BOOST_CHECK(D.at(x, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(y, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(z, 0) == bound_t::non_strict(5));
    BOOST_CHECK(D.at(0, x) == bound_t::zero());
    BOOST_CHECK(D.at(0, y) == bound_t::zero());
    BOOST_CHECK(D.at(0, z) == bound_t::zero());

    // To goal
    BOOST_CHECK(D.is_satisfied(x, 0, bound_t::non_strict(2)));
    BOOST_CHECK(D.is_satisfied(0, x, bound_t::non_strict(-2)));
    BOOST_CHECK(D.is_satisfied(y, 0, bound_t::non_strict(4)));
    BOOST_CHECK(D.is_satisfied(0, y, bound_t::non_strict(-4)));
    BOOST_CHECK(D.is_satisfied(z, 0, bound_t::non_strict(3)));
    BOOST_CHECK(D.is_satisfied(0, z, bound_t::non_strict(-3)));

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
    D.at(1, 0) = bound_t::inf();
    D.at(3, 0) = bound_t::inf();
    D.at(1, 2) = bound_t::inf();
    D.at(3, 2) = bound_t::inf();

    D.remove_clock(2);

    BOOST_CHECK(D.dimension() == 3);

    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(2, 0) == bound_t::inf());

    BOOST_CHECK(D.at(0, 0) == bound_t::zero());
    BOOST_CHECK(D.at(0, 1) == bound_t::zero());
    BOOST_CHECK(D.at(0, 2) == bound_t::zero());
    BOOST_CHECK(D.at(1, 1) == bound_t::zero());
    BOOST_CHECK(D.at(1, 2) == bound_t::zero());
    BOOST_CHECK(D.at(2, 1) == bound_t::zero());
    BOOST_CHECK(D.at(2, 2) == bound_t::zero());
}

BOOST_AUTO_TEST_CASE(swap_clocks_test_1) {
    int check_cnt = 0;
    dim_t size = 5, a = 2, b = 4;
    DBM D(size);

    for (dim_t i = 0; i < size; ++i)
        for (dim_t j = 0; j < size; ++j) {
            if (i == j) continue;
            D.at(i, j) = bound_t::non_strict(j + (i * size));
        }

    D.swap_clocks(a, b);

    for (dim_t i = 0; i < size; ++i)
        for (dim_t j = 0; j < size; ++j) {
            bound_t bound;
            if (i == j)
                bound = bound_t::zero();
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

BOOST_AUTO_TEST_CASE(add_clock_after_test_1) {
    DBM D(4);
    D.at(1, 0) = bound_t::inf();
    D.at(3, 0) = bound_t::inf();
    D.at(1, 2) = bound_t::inf();
    D.at(3, 2) = bound_t::inf();
    std::cout << D << std::endl;
    D.add_clock_at(3);
    std::cout << D << std::endl;
    BOOST_CHECK(D.dimension() == 5);

    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(1, 1) == bound_t::zero());
    BOOST_CHECK(D.at(1, 2) == bound_t::inf());
    BOOST_CHECK(D.at(1, 3) == D.at(1, 0));
    BOOST_CHECK(D.at(1, 4) == bound_t::zero());

    BOOST_CHECK(D.at(2, 0) == bound_t::zero());
    BOOST_CHECK(D.at(2, 1) == bound_t::zero());
    BOOST_CHECK(D.at(2, 2) == bound_t::zero());
    BOOST_CHECK(D.at(2, 3) == D.at(2, 0));
    BOOST_CHECK(D.at(2, 4) == bound_t::zero());

    BOOST_CHECK(D.at(3, 0) == bound_t::inf());
    BOOST_CHECK(D.at(3, 1) == bound_t::inf());
    BOOST_CHECK(D.at(3, 2) == bound_t::inf());
    BOOST_CHECK(D.at(3, 3) == bound_t::zero());
    BOOST_CHECK(D.at(3, 4) == bound_t::inf());

    BOOST_CHECK(D.at(4, 0) == bound_t::inf());
    BOOST_CHECK(D.at(4, 1) == bound_t::zero());
    BOOST_CHECK(D.at(4, 2) == bound_t::inf());
    BOOST_CHECK(D.at(4, 3) == D.at(4, 0));
    BOOST_CHECK(D.at(4, 4) == bound_t::zero());
}

BOOST_AUTO_TEST_CASE(resize_test_2) {
    DBM D(5);
    D.future();
    D.assign(1, 1);
    D.assign(2, 2);
    D.assign(4, 4);

    std::vector<bool> src {true, true, false, true, true};
    std::vector<bool> dst {true, false, true, true, true};
    std::vector<dim_t> indir = D.resize(src, dst);


    BOOST_CHECK(D.is_satisfied(1, 0, bound_t::zero()));
    BOOST_CHECK(D.is_satisfied(0, 1, bound_t::zero()));

    BOOST_CHECK(D.is_satisfied(2, 0, bound_t::non_strict(1)));
    BOOST_CHECK(D.is_satisfied(0, 2, bound_t::non_strict(1)));

    BOOST_CHECK(D.is_satisfied(3, 0, bound_t::inf()));
    BOOST_CHECK(D.is_satisfied(0, 3, bound_t::zero()));

    BOOST_CHECK(D.is_satisfied(4, 0, bound_t::non_strict(4)));
    BOOST_CHECK(D.is_satisfied(0, 4, bound_t::non_strict(4)));
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

    BOOST_CHECK(D.is_satisfied(1, 0, bound_t::non_strict(3)));
    BOOST_CHECK(D.is_satisfied(0, 1, bound_t::non_strict(3)));

    BOOST_CHECK(D.is_satisfied(2, 0, bound_t::zero()));
    BOOST_CHECK(D.is_satisfied(0, 2, bound_t::zero()));

    BOOST_CHECK(D.is_satisfied(3, 0, bound_t::inf()));
    BOOST_CHECK(D.is_satisfied(0, 3, bound_t::zero()));
}

BOOST_AUTO_TEST_CASE(diagonal_extrapolation_test_1) {
    DBM D(3);
    D.at(1, 0) = bound_t::inf();
    D.at(2, 0) = bound_t::inf();
    std::vector<val_t> ceiling = {0, -1073741823, -1073741823};

    std::cout << D;
    D.diagonal_extrapolation(ceiling);
    std::cout << D;

    BOOST_CHECK(!D.is_empty());
    BOOST_CHECK(D.at(0, 0) == bound_t::zero());
    BOOST_CHECK(D.at(1, 1) == bound_t::zero());
    BOOST_CHECK(D.at(2, 2) == bound_t::zero());
    BOOST_CHECK(D.at(0, 1) == bound_t::zero());
    BOOST_CHECK(D.at(0, 2) == bound_t::zero());

    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(2, 0) == bound_t::inf());
    BOOST_CHECK(D.at(1, 2) == bound_t::inf());
    BOOST_CHECK(D.at(2, 1) == bound_t::inf());
}

BOOST_AUTO_TEST_CASE(diagonal_extrapolation_test_2) {
    DBM D(4);
//  Extrapolating: [0, 1, -1073741823, 3]
//  <=0     <=0     <=0     <=0
//  <=1     <=0     <=1     <=0
//  INF     INF     <=0     INF
//  <=1     <=0     <=1     <=0
    std::vector<val_t> ceiling = {0, 1, -1073741823, 3};

    D.at(1, 0) = bound_t::non_strict(1);
    D.at(1, 2) = bound_t::non_strict(1);
    D.at(3, 0) = bound_t::non_strict(1);
    D.at(3, 2) = bound_t::non_strict(1);
    D.at(2, 0) = bound_t::inf();
    D.at(2, 1) = bound_t::inf();
    D.at(2, 3) = bound_t::inf();

    D.diagonal_extrapolation(ceiling);

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

BOOST_AUTO_TEST_CASE(diagonal_extrapolation_test_3) {
//    <=0     <=-6    <=0     <=0     <=0
//    INF     <=0     INF     INF     INF
//    INF     INF     <=0     INF     INF
//    INF     <=-6    <=0     <=0     <=0
//    INF     <=-6    <=0     <=0     <=0
    DBM D(5);
    std::vector<val_t> ceiling = {0, 3, -1073741823, 3, 3};

    D.at(0, 1) = bound_t::non_strict(-6);
    D.at(3, 1) = bound_t::non_strict(-6);
    D.at(4, 1) = bound_t::non_strict(-6);

    D.at(1, 0) = bound_t::inf();
    D.at(2, 0) = bound_t::inf();
    D.at(3, 0) = bound_t::inf();
    D.at(4, 0) = bound_t::inf();

    D.at(2, 1) = bound_t::inf();
    D.at(1, 2) = bound_t::inf();
    D.at(1, 3) = bound_t::inf();
    D.at(2, 3) = bound_t::inf();
    D.at(1, 4) = bound_t::inf();
    D.at(2, 4) = bound_t::inf();

    D.diagonal_extrapolation(ceiling);

//    <=0     <-3     <=0     <=0     <=0
//    INF     <=0     INF     INF     INF
//    INF     INF     <=0     INF     INF
//    INF     INF     INF     <=0     <=0
//    INF     INF     INF     <=0     <=0

    BOOST_CHECK(D.at(0, 0) == bound_t::zero());
    BOOST_CHECK(D.at(1, 0) == bound_t::inf());
    BOOST_CHECK(D.at(2, 0) == bound_t::inf());
    BOOST_CHECK(D.at(3, 0) == bound_t::inf());
    BOOST_CHECK(D.at(4, 0) == bound_t::inf());

    BOOST_CHECK(D.at(0, 1) == bound_t::strict(-3));
    BOOST_CHECK(D.at(1, 1) == bound_t::zero());
    BOOST_CHECK(D.at(2, 1) == bound_t::inf());
    BOOST_CHECK(D.at(3, 1) == bound_t::inf());
    BOOST_CHECK(D.at(4, 1) == bound_t::inf());

    BOOST_CHECK(D.at(0, 2) == bound_t::zero());
    BOOST_CHECK(D.at(1, 2) == bound_t::inf());
    BOOST_CHECK(D.at(2, 2) == bound_t::zero());
    BOOST_CHECK(D.at(3, 2) == bound_t::inf());
    BOOST_CHECK(D.at(4, 2) == bound_t::inf());

    BOOST_CHECK(D.at(0, 3) == bound_t::zero());
    BOOST_CHECK(D.at(1, 3) == bound_t::inf());
    BOOST_CHECK(D.at(2, 3) == bound_t::inf());
    BOOST_CHECK(D.at(3, 3) == bound_t::zero());
    BOOST_CHECK(D.at(4, 3) == bound_t::zero());

    BOOST_CHECK(D.at(0, 4) == bound_t::zero());
    BOOST_CHECK(D.at(1, 4) == bound_t::inf());
    BOOST_CHECK(D.at(2, 4) == bound_t::inf());
    BOOST_CHECK(D.at(3, 4) == bound_t::zero());
    BOOST_CHECK(D.at(4, 4) == bound_t::zero());

}