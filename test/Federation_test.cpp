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

    BOOST_CHECK(fed.at(0).equal<false>(DBM(3)));
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

    auto relation1 = fed.approx_relation(dbm);
    auto relation2 = dbm.approx_relation(fed);

    BOOST_CHECK(relation1._superset);
    BOOST_CHECK(not relation1._equal);
    BOOST_CHECK(not relation1._subset);
    BOOST_CHECK(not relation1._different);

    BOOST_CHECK(not relation2._superset);
    BOOST_CHECK(not relation2._equal);
    BOOST_CHECK(relation2._subset);
    BOOST_CHECK(not relation2._different);

    BOOST_CHECK(fed.approx_superset(dbm));
    BOOST_CHECK(not fed.approx_equal(dbm));
    BOOST_CHECK(not fed.approx_subset(dbm));

    BOOST_CHECK(not dbm.approx_superset(fed));
    BOOST_CHECK(not dbm.approx_equal(fed));
    BOOST_CHECK(dbm.approx_subset(fed));
}

BOOST_AUTO_TEST_CASE(relation_test_2) {
    DBM dbm(3);
    auto fed = Federation();

    auto relation1 = fed.approx_relation(dbm);
    auto relation2 = dbm.approx_relation(fed);

    BOOST_CHECK(not relation1._superset);
    BOOST_CHECK(not relation1._equal);
    BOOST_CHECK(relation1._subset);
    BOOST_CHECK(not relation1._different);

    BOOST_CHECK( relation2._superset);
    BOOST_CHECK(not relation2._equal);
    BOOST_CHECK(not relation2._subset);
    BOOST_CHECK(not relation2._different);

    BOOST_CHECK(not fed.approx_superset(dbm));
    BOOST_CHECK(not fed.approx_equal(dbm));
    BOOST_CHECK(fed.approx_subset(dbm));

    BOOST_CHECK(dbm.approx_superset(fed));
    BOOST_CHECK(not dbm.approx_equal(fed));
    BOOST_CHECK(not dbm.approx_subset(fed));
}

BOOST_AUTO_TEST_CASE(relation_test_3) {
    Federation fed1(3);
    auto fed2 = Federation();

    auto relation1 = fed1.approx_relation(fed2);
    auto relation2 = fed2.approx_relation(fed1);

    BOOST_CHECK(relation1._superset);
    BOOST_CHECK(not relation1._equal);
    BOOST_CHECK(not relation1._subset);
    BOOST_CHECK(not relation1._different);

    BOOST_CHECK(not relation2._superset);
    BOOST_CHECK(not relation2._equal);
    BOOST_CHECK(relation2._subset);
    BOOST_CHECK(not relation2._different);

    BOOST_CHECK(fed1.approx_superset(fed2));
    BOOST_CHECK(not fed1.approx_equal(fed2));
    BOOST_CHECK(not fed1.approx_subset(fed2));

    BOOST_CHECK(not fed2.approx_superset(fed1));
    BOOST_CHECK(not fed2.approx_equal(fed1));
    BOOST_CHECK(fed2.approx_subset(fed1));
}

BOOST_AUTO_TEST_CASE(relation_test_4) {
    auto fed = Federation::zero(3);
    auto dbm = DBM::zero(3);
    dbm.delay(2);

    fed.add(dbm);

    BOOST_CHECK(fed.approx_superset(dbm));
}

BOOST_AUTO_TEST_CASE(relation_test_5) {
    auto fed1 = Federation::zero(3),
         fed2 = Federation::zero(3);
    auto dbm = DBM::zero(3);

    dbm.delay(2);

    fed1.add(dbm);

    BOOST_CHECK(fed1.approx_superset(fed2));
    BOOST_CHECK(fed2.approx_subset(fed1));
}

BOOST_AUTO_TEST_CASE(relation_test_6) {
    auto fed1 = Federation::zero(3),
         fed2 = Federation::zero(3);
    auto dbm1 = DBM::unconstrained(3),
         dbm2 = DBM::unconstrained(3);
    
    dbm1.restrict(clock_constraint_t::upper_non_strict(1, 10));
    dbm2.restrict(clock_constraint_t::lower_strict(1, 10));

    dbm1.restrict(clock_constraint_t::upper_non_strict(2, 5));
    dbm2.restrict(clock_constraint_t::lower_strict(2, 5));

    BOOST_CHECK(dbm1.relation(dbm2)._different);
    BOOST_CHECK(dbm2.relation(dbm1)._different);

    fed1.add(dbm1);
    fed1.add(dbm2);
    fed2.add(dbm1);
    fed2.add(dbm2);

    auto approx1 = fed1.approx_relation(fed2);
    auto approx2 = fed2.approx_relation(fed1);

    BOOST_CHECK(fed1.approx_superset(dbm1));
    BOOST_CHECK(fed1.approx_superset(dbm2));
    BOOST_CHECK(fed2.approx_superset(dbm1));
    BOOST_CHECK(fed2.approx_superset(dbm2));
    BOOST_CHECK(approx1._superset);
    BOOST_CHECK(approx2._superset);
}

BOOST_AUTO_TEST_CASE(exact_relation_test_1) {
    Federation fed(3);
    DBM dbm(3);

    dbm.set(0, 0, bound_t::strict(-1));

    auto relation1 = fed.exact_relation(dbm);
    auto relation2 = dbm.exact_relation(fed);

    BOOST_CHECK(relation1._superset);
    BOOST_CHECK(not relation1._equal);
    BOOST_CHECK(not relation1._subset);
    BOOST_CHECK(not relation1._different);

    BOOST_CHECK(not relation2._superset);
    BOOST_CHECK(not relation2._equal);
    BOOST_CHECK(relation2._subset);
    BOOST_CHECK(not relation2._different);

    BOOST_CHECK(fed.exact_superset(dbm));
    BOOST_CHECK(not fed.exact_equal(dbm));
    BOOST_CHECK(not fed.exact_subset(dbm));

    BOOST_CHECK(not dbm.exact_superset(fed));
    BOOST_CHECK(not dbm.exact_equal(fed));
    BOOST_CHECK(dbm.exact_subset(fed));
}

BOOST_AUTO_TEST_CASE(exact_relation_test_2) {
    DBM dbm(3);
    auto fed = Federation();

    auto relation1 = fed.exact_relation(dbm);
    auto relation2 = dbm.exact_relation(fed);

    BOOST_CHECK(not relation1._superset);
    BOOST_CHECK(not relation1._equal);
    BOOST_CHECK(relation1._subset);
    BOOST_CHECK(not relation1._different);

    BOOST_CHECK(relation2._superset);
    BOOST_CHECK(not relation2._equal);
    BOOST_CHECK(not relation2._subset);
    BOOST_CHECK(not relation2._different);

    BOOST_CHECK(not fed.exact_superset(dbm));
    BOOST_CHECK(not fed.exact_equal(dbm));
    BOOST_CHECK(fed.exact_subset(dbm));

    BOOST_CHECK(dbm.exact_superset(fed));
    BOOST_CHECK(not dbm.exact_equal(fed));
    BOOST_CHECK(not dbm.exact_subset(fed));
}

BOOST_AUTO_TEST_CASE(exact_relation_test_3) {
    Federation fed1(3);
    auto fed2 = Federation();

    auto relation1 = fed1.exact_relation(fed2);
    auto relation2 = fed2.exact_relation(fed1);

    BOOST_CHECK(relation1._superset);
    BOOST_CHECK(not relation1._equal);
    BOOST_CHECK(not relation1._subset);
    BOOST_CHECK(not relation1._different);

    BOOST_CHECK(not relation2._superset);
    BOOST_CHECK(not relation2._equal);
    BOOST_CHECK(relation2._subset);
    BOOST_CHECK(not relation2._different);

    BOOST_CHECK(fed1.exact_superset(fed2));
    BOOST_CHECK(not fed1.exact_equal(fed2));
    BOOST_CHECK(not fed1.exact_subset(fed2));

    BOOST_CHECK(not fed2.exact_superset(fed1));
    BOOST_CHECK(not fed2.exact_equal(fed1));
    BOOST_CHECK(fed2.exact_subset(fed1));
}

BOOST_AUTO_TEST_CASE(exact_relation_test_4) {
    auto fed = Federation::zero(3);
    auto dbm = DBM::zero(3);
    dbm.delay(2);

    fed.add(dbm);

    BOOST_CHECK(fed.exact_superset(dbm));
}

BOOST_AUTO_TEST_CASE(exact_relation_test_5) {
    auto fed1 = Federation::zero(3),
         fed2 = Federation::zero(3);
    auto dbm = DBM::zero(3);

    dbm.delay(2);

    fed1.add(dbm);

    BOOST_CHECK(fed1.exact_superset(fed2));
    BOOST_CHECK(fed2.exact_subset(fed1));
}

BOOST_AUTO_TEST_CASE(exact_relation_test_6) {
    auto fed1 = Federation::zero(3),
         fed2 = Federation::zero(3);
    auto dbm1 = DBM::unconstrained(3),
         dbm2 = DBM::unconstrained(3),
         dbm3 = DBM::unconstrained(3),
         dbm4 = DBM::unconstrained(3);
    
    dbm1.restrict(clock_constraint_t::upper_non_strict(1, 10));
    dbm2.restrict(clock_constraint_t::lower_strict(1, 10));

    dbm3.restrict(clock_constraint_t::upper_non_strict(2, 10));
    dbm4.restrict(clock_constraint_t::lower_strict(2, 10));
    
    // dbm1, dbm2, dbm3, dbm4 are disjunct but 1+2 and 3+4 they are equal to unconstrained

    BOOST_CHECK(dbm1.relation(dbm2).different);
    BOOST_CHECK(dbm1.relation(dbm3).different);
    BOOST_CHECK(dbm1.relation(dbm4).different);
    BOOST_CHECK(dbm2.relation(dbm3).different);
    BOOST_CHECK(dbm2.relation(dbm4).different);
    BOOST_CHECK(dbm3.relation(dbm4).different);

    fed1.add(dbm1);
    fed1.add(dbm2);
    fed2.add(dbm3);
    fed2.add(dbm4);

    // auto approx1 = fed1.approx_relation(fed2);
    // auto approx2 = fed2.approx_relation(fed1);
    auto exact1 = fed1.exact_relation(fed2);
    auto exact2 = fed2.exact_relation(fed1);

    // BOOST_CHECK(approx1._different && "If this fails, then perhaps the approximate relation is more precise");
    // BOOST_CHECK(approx2._different && "If this fails, then perhaps the approximate relation is more precise");

    BOOST_CHECK(exact1._equal && exact2._equal);
}

BOOST_AUTO_TEST_CASE(exact_relation_test_7) {
    auto fed1 = Federation::zero(3),
         fed2 = Federation::zero(3);
    auto dbm1 = DBM::unconstrained(3),
         dbm2 = DBM::unconstrained(3);
    
    dbm1.restrict(clock_constraint_t::upper_non_strict(1, 10));
    dbm2.restrict(clock_constraint_t::lower_strict(1, 10));

    dbm1.restrict(clock_constraint_t::upper_non_strict(2, 5));
    dbm2.restrict(clock_constraint_t::lower_strict(2, 5));

    BOOST_CHECK(dbm1.relation(dbm2).different);
    BOOST_CHECK(dbm2.relation(dbm1).different);

    fed1.add(dbm1);
    fed1.add(dbm2);
    fed2.add(dbm1);
    fed2.add(dbm2);

    // auto approx1 = fed1.relation<false>(fed2);
    // auto approx2 = fed2.relation<false>(fed1);
    auto exact1 = fed1.exact_relation(fed2);
    auto exact2 = fed2.exact_relation(fed1);

    BOOST_CHECK(exact1._equal && exact2._equal);
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
