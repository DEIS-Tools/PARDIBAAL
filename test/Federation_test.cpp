/*
 * Copyright Thomas M. Grosen
 * Created on 04/04/2022
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
#include "pardibaal/Federation.h"
#include "errors.h"

using namespace pardibaal;

BOOST_AUTO_TEST_CASE(at_test_1) {
    Federation fed(3);

    BOOST_CHECK_THROW(fed.at(1), base_error);
    BOOST_CHECK_THROW(fed.at(-1), base_error);

    BOOST_CHECK(fed.at(0).equal(DBM(3)));
}

BOOST_AUTO_TEST_CASE(add_test_1) {
    Federation fed(3);
    DBM dbm(3);

    fed.add(dbm);

    BOOST_CHECK(fed.size() == 1);
}

BOOST_AUTO_TEST_CASE(add_test_2) {
    auto fed = Federation::unconstrained(3);
    auto fed2 = Federation::zero(3);
    auto fed3 = Federation::zero(3);
    DBM dbm(3);
    dbm.assign(2, 5);

    fed2.add(dbm);

    BOOST_CHECK(fed.size() == 1);
    BOOST_CHECK(fed2.size() == 2);
    BOOST_CHECK(fed3.size() == 1);

    fed3.add(fed2);
    BOOST_CHECK(fed3.size() == 2);

    fed.add(fed2);
    BOOST_CHECK(fed.size() == 1);

    fed2.add(fed);
    BOOST_CHECK(fed2.size() == 1);
}

BOOST_AUTO_TEST_CASE(add_test_3) {
    auto fed = Federation::zero(3);
    auto dbm = DBM::zero(3);
    dbm.delay(2);

    BOOST_CHECK(fed.size() == 1);
    fed.add(dbm);
    BOOST_CHECK(fed.size() == 2);
    fed.add(dbm);
    BOOST_CHECK(fed.size() == 2);
}

BOOST_AUTO_TEST_CASE(subtract_test_1) {
    auto fed = Federation::unconstrained(3);
    auto dbm = DBM::unconstrained(3);

    dbm.restrict(1, 0, bound_t::strict(10));

    BOOST_CHECK(fed.satisfies(1, 0, bound_t::non_strict(11)));
    BOOST_CHECK(fed.satisfies(1, 0, bound_t::non_strict(10)));
    BOOST_CHECK(fed.satisfies(1, 0, bound_t::strict(10)));
    BOOST_CHECK(fed.satisfies(1, 0, bound_t::non_strict(5)));

    fed.subtract(dbm);

    BOOST_CHECK(fed.satisfies(1, 0, bound_t::non_strict(11)));
    BOOST_CHECK(fed.satisfies(1, 0, bound_t::non_strict(10)));
    BOOST_CHECK(not fed.satisfies(1, 0, bound_t::strict(10)));
    BOOST_CHECK(not fed.satisfies(1, 0, bound_t::non_strict(5)));
}

BOOST_AUTO_TEST_CASE(remove_test_1) {
    Federation fed(3);

    BOOST_CHECK(fed.size() == 1);

    fed.remove(0);

    BOOST_CHECK(fed.size() == 0);
    BOOST_CHECK(fed.is_empty());
}

BOOST_AUTO_TEST_CASE(is_empty_test_1) {
    auto empty_fed = Federation();
    Federation fed(10);

    BOOST_CHECK(empty_fed.is_empty());
    BOOST_CHECK(not fed.is_empty());
}

BOOST_AUTO_TEST_CASE(satisfies_test_1) {
    Federation fed(3);
    DBM dbm(3);

    fed.future();
    fed.restrict(1, 0, bound_t::non_strict(5));

    BOOST_CHECK(fed.satisfies(1, 0, bound_t::non_strict(5)));
    BOOST_CHECK(fed.satisfies(1, 0, bound_t::strict(5)));
    BOOST_CHECK(fed.satisfies(0, 1, bound_t::non_strict(-3)));
    BOOST_CHECK(not fed.satisfies(0, 1, bound_t::non_strict(-6)));

}

BOOST_AUTO_TEST_CASE(relation_test_1) {
    Federation fed(3);
    DBM dbm(3);

    dbm.set(0, 0, bound_t::strict(-1));

    auto relation1 = fed.relation(dbm);
    auto relation2 = dbm.relation(fed);

    BOOST_CHECK(relation1._superset);
    BOOST_CHECK(not relation1._equal);
    BOOST_CHECK(not relation1._subset);
    BOOST_CHECK(not relation1._different);

    BOOST_CHECK(not relation2._superset);
    BOOST_CHECK(not relation2._equal);
    BOOST_CHECK(relation2._subset);
    BOOST_CHECK(not relation2._different);

    BOOST_CHECK(fed.superset(dbm));
    BOOST_CHECK(not fed.equal(dbm));
    BOOST_CHECK(not fed.subset(dbm));

    BOOST_CHECK(not dbm.superset(fed));
    BOOST_CHECK(not dbm.equal(fed));
    BOOST_CHECK(dbm.subset(fed));
}

BOOST_AUTO_TEST_CASE(relation_test_2) {
    DBM dbm(3);
    auto fed = Federation();

    auto relation1 = fed.relation(dbm);
    auto relation2 = dbm.relation(fed);

    BOOST_CHECK(not relation1._superset);
    BOOST_CHECK(not relation1._equal);
    BOOST_CHECK(relation1._subset);
    BOOST_CHECK(not relation1._different);

    BOOST_CHECK( relation2._superset);
    BOOST_CHECK(not relation2._equal);
    BOOST_CHECK(not relation2._subset);
    BOOST_CHECK(not relation2._different);

    BOOST_CHECK(not fed.superset(dbm));
    BOOST_CHECK(not fed.equal(dbm));
    BOOST_CHECK(fed.subset(dbm));

    BOOST_CHECK(dbm.superset(fed));
    BOOST_CHECK(not dbm.equal(fed));
    BOOST_CHECK(not dbm.subset(fed));
}

BOOST_AUTO_TEST_CASE(relation_test_3) {
    Federation fed1(3);
    auto fed2 = Federation();

    auto relation1 = fed1.relation(fed2);
    auto relation2 = fed2.relation(fed1);

    BOOST_CHECK(relation1._superset);
    BOOST_CHECK(not relation1._equal);
    BOOST_CHECK(not relation1._subset);
    BOOST_CHECK(not relation1._different);

    BOOST_CHECK(not relation2._superset);
    BOOST_CHECK(not relation2._equal);
    BOOST_CHECK(relation2._subset);
    BOOST_CHECK(not relation2._different);

    BOOST_CHECK(fed1.superset(fed2));
    BOOST_CHECK(not fed1.equal(fed2));
    BOOST_CHECK(not fed1.subset(fed2));

    BOOST_CHECK(not fed2.superset(fed1));
    BOOST_CHECK(not fed2.equal(fed1));
    BOOST_CHECK(fed2.subset(fed1));
}

BOOST_AUTO_TEST_CASE(relation_test_4) {
    auto fed = Federation::zero(3);
    auto dbm = DBM::zero(3);
    dbm.delay(2);

    fed.add(dbm);

    BOOST_CHECK(fed.superset(dbm));
}

BOOST_AUTO_TEST_CASE(relation_test_5) {
    auto fed1 = Federation::zero(3),
         fed2 = Federation::zero(3);
    auto dbm = DBM::zero(3);

    dbm.delay(2);

    fed1.add(dbm);

    BOOST_CHECK(fed1.superset(fed2));
    BOOST_CHECK(fed2.subset(fed1));
}

BOOST_AUTO_TEST_CASE(intersects_test_1) {
    auto fed = Federation::zero(3);
    auto dbm = DBM::zero(3);

    BOOST_CHECK(fed.intersects(dbm));
    BOOST_CHECK(dbm.intersects(fed));

    fed.future();

    BOOST_CHECK(fed.intersects(dbm));
    BOOST_CHECK(dbm.intersects(fed));

    auto dbm2 = DBM::unconstrained(3);
    dbm2.restrict(0, 1, bound_t::strict(-2));
    dbm2.restrict(0, 2, bound_t::strict(-3));
    fed.add(dbm2);

    BOOST_CHECK(fed.size() == 2);
    BOOST_CHECK(fed.intersects(dbm));
    BOOST_CHECK(dbm.intersects(fed));

    dbm.future();

    BOOST_CHECK(fed.intersects(dbm));
    BOOST_CHECK(dbm.intersects(fed));

    dbm.shift(1, 1);

    BOOST_CHECK(fed.intersects(dbm));
    BOOST_CHECK(dbm.intersects(fed));

    dbm.restrict(1, 0, bound_t::strict(2));

    BOOST_CHECK(not fed.intersects(dbm));
    BOOST_CHECK(not dbm.intersects(fed));
}

BOOST_AUTO_TEST_CASE(intersects_test_2) {
    auto fed1 = Federation::zero(3);
    auto fed2 = Federation::zero(3);

    BOOST_CHECK(fed1.intersects(fed2));
    BOOST_CHECK(fed2.intersects(fed1));

    fed1.future();

    BOOST_CHECK(fed1.intersects(fed2));
    BOOST_CHECK(fed2.intersects(fed1));

    auto dbm = DBM::unconstrained(3);
    dbm.restrict(0, 1, bound_t::strict(-2));
    dbm.restrict(0, 2, bound_t::strict(-3));
    fed1.add(dbm);

    BOOST_CHECK(fed1.size() == 2);
    BOOST_CHECK(fed1.intersects(fed2));
    BOOST_CHECK(fed2.intersects(fed1));

    fed2.future();

    BOOST_CHECK(fed1.intersects(fed2));
    BOOST_CHECK(fed2.intersects(fed1));

    fed2.shift(1, 1);

    BOOST_CHECK(fed1.intersects(fed2));
    BOOST_CHECK(fed2.intersects(fed1));
    BOOST_CHECK(fed1.intersects(fed2.at(0)));

    fed2.restrict(1, 0, bound_t::strict(2));

    BOOST_CHECK(not fed1.intersects(fed2));
    BOOST_CHECK(not fed2.intersects(fed1));
}

BOOST_AUTO_TEST_CASE(is_unbounded_test_1) {
    Federation fed(3);
    DBM dbm(3);

    dbm.assign(2, 5);
    dbm.future();

    BOOST_CHECK(not fed.is_unbounded());

    fed.add(dbm);

    BOOST_CHECK(not fed.is_unbounded());

    fed.remove(0);

    BOOST_CHECK(fed.is_unbounded());
}

BOOST_AUTO_TEST_CASE(is_unbounded_test_2) {
    Federation fed(3);
    DBM dbm(3);

    dbm.future();

    BOOST_CHECK(not fed.is_unbounded());

    fed.add(dbm);

    BOOST_CHECK(fed.is_unbounded());
}

BOOST_AUTO_TEST_CASE(restrict_test_1) {
    Federation fed(3);
    fed.future();
    fed.restrict(0, 1, bound_t::strict(-5));

    fed.add(DBM(3));

    BOOST_CHECK(fed.size() == 2);

    fed.restrict(0, 1, bound_t::strict(-5));

    BOOST_CHECK(fed.size() == 1);
}

BOOST_AUTO_TEST_CASE(restrict_test_2) {
    Federation fed1(3), fed2(3);
    fed1.future(); fed2.future();
    fed1.restrict(0, 1, bound_t::strict(-5));
    fed2.restrict(0, 1, bound_t::strict(-5));

    fed1.restrict({});

    BOOST_CHECK(fed1.equal(fed2));
}

BOOST_AUTO_TEST_CASE(intersection_test_1) {
    auto fed = Federation::unconstrained(3);
    auto dbm = DBM::zero(3);

    fed.intersection(dbm);

    BOOST_CHECK(fed.equal(dbm));
}

BOOST_AUTO_TEST_CASE(intersection_test_2) {
    auto fed = Federation::unconstrained(3);
    auto dbm = DBM::zero(3);

    dbm.restrict(0, 1, bound_t::strict(-1));
    fed.intersection(dbm);

    BOOST_CHECK(fed.equal(dbm));
    BOOST_CHECK(fed.is_empty());
}

BOOST_AUTO_TEST_CASE(intersection_test_3) {
    auto fed = Federation::unconstrained(3);
    auto dbm1 = DBM::unconstrained(3);
    auto dbm2 = DBM::unconstrained(3);

    dbm1.restrict(0, 1, bound_t::strict(-2));
    fed.restrict(1, 0, bound_t::strict(1));
    fed.add(dbm1);

    dbm2.restrict(2, 0, bound_t::strict(1));

    BOOST_CHECK(fed.satisfies(0, 2, bound_t::non_strict(-1)));

    fed.intersection(dbm2);

    BOOST_CHECK(not fed.equal(dbm2));
    BOOST_CHECK(not fed.is_empty());
    BOOST_CHECK(fed.size() == 2);
    BOOST_CHECK(fed.satisfies(1, 0, bound_t::strict(2)));
    BOOST_CHECK(fed.satisfies(0, 1, bound_t::strict(-2)));
    BOOST_CHECK(not fed.satisfies(0, 2, bound_t::non_strict(-1)));
    BOOST_CHECK(fed.satisfies(2, 0, bound_t::non_strict(1)));
}

BOOST_AUTO_TEST_CASE(intersection_test_4) {
    auto fed = Federation::unconstrained(3);
    auto dbm1 = DBM::unconstrained(3);
    auto dbm2 = DBM::unconstrained(3);

    dbm1.restrict(0, 1, bound_t::strict(-2));
    fed.restrict(1, 0, bound_t::strict(1));
    fed.add(dbm1);

    dbm2.restrict(1, 0, bound_t::strict(2));
    dbm2.restrict(0, 1, bound_t::strict(-1));

    fed.intersection(dbm2);

    BOOST_CHECK(fed.is_empty());
    BOOST_CHECK(fed.size() <= 1);
}

BOOST_AUTO_TEST_CASE(intersection_test_5) {
    auto fed1 = Federation::unconstrained(3);
    auto fed2 = Federation::unconstrained(3);

    fed1.restrict(1, 0, bound_t::strict(1));
    fed2.restrict(0, 1, bound_t::strict(-1));

    BOOST_CHECK(not fed1.intersects(fed2));
    BOOST_CHECK(not fed1.is_empty());

    fed1.intersection(fed2);

    BOOST_CHECK(fed1.is_empty());
}

BOOST_AUTO_TEST_CASE(intersection_test_6) {
    auto fed1 = Federation::unconstrained(3);
    auto fed2 = Federation::unconstrained(3);
    auto dbm = DBM::unconstrained(3);

    dbm.restrict(0, 1, bound_t::strict(-2));
    fed1.restrict(1, 0, bound_t::strict(1));
    fed1.add(dbm);

    fed2.restrict(0, 1, bound_t::strict(-1));

    BOOST_CHECK(fed1.intersects(fed2));
    BOOST_CHECK(not fed1.is_empty());
    BOOST_CHECK(fed1.satisfies(1, 0, bound_t::non_strict(2)));
    BOOST_CHECK(fed1.satisfies(1, 0, bound_t::strict(3)));

    fed1.intersection(fed2);

    BOOST_CHECK(not fed1.is_empty());
    BOOST_CHECK(fed1.size() == 1);
    BOOST_CHECK(not fed1.satisfies(1, 0, bound_t::non_strict(2)));
    BOOST_CHECK(fed1.satisfies(1, 0, bound_t::strict(3)));
}

BOOST_AUTO_TEST_CASE(remove_clock_test_1) {
    Federation fed(3);

    BOOST_CHECK(fed.dimension() == 3);

    fed.remove_clock(2);

    BOOST_CHECK(fed.dimension() == 2);
}

BOOST_AUTO_TEST_CASE(add_clock_at_test_1) {
    Federation fed(3);

    BOOST_CHECK(fed.dimension() == 3);

    fed.add_clock_at(2);

    BOOST_CHECK(fed.dimension() == 4);
}

BOOST_AUTO_TEST_CASE(zero_test_1) {
    dim_t dim = 10;
    auto fed = Federation();
    fed.add(DBM::zero(dim));

    BOOST_CHECK(fed.equal(Federation::zero(dim)));
}

BOOST_AUTO_TEST_CASE(unconstrained_test_1) {
    dim_t dim = 10;
    auto fed = Federation();
    fed.add(DBM::unconstrained(dim));

    BOOST_CHECK(fed.equal(Federation::unconstrained(dim)));
}
