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


BOOST_AUTO_TEST_CASE(at_test_1){
    Federation fed(3);

//    BOOST_CHECK_THROW(fed.at(1), base_error);
//    BOOST_CHECK_THROW(fed.at(-1), base_error);

    BOOST_CHECK(fed.at(0).equal(DBM(3)));
}

BOOST_AUTO_TEST_CASE(add_test_1){
    Federation fed(3);
    DBM dbm(3);

    fed.add(dbm);
}

BOOST_AUTO_TEST_CASE(remove_test_1){
    Federation fed(3);

    BOOST_CHECK(fed.size() == 1);

    fed.remove(0);

    BOOST_CHECK(fed.size() == 0);
    BOOST_CHECK(fed.is_empty());
}

BOOST_AUTO_TEST_CASE(is_empty_test_1){
    auto empty_fed = Federation();
    Federation fed(10);

    BOOST_CHECK(empty_fed.is_empty());
    BOOST_CHECK(not fed.is_empty());
}

BOOST_AUTO_TEST_CASE(satisfies_test_1){
    Federation fed(3);
    DBM dbm(3);

    fed.future();
    fed.restrict(1, 0, bound_t::non_strict(5));

    BOOST_CHECK(fed.satisfies(1, 0, bound_t::non_strict(5)));
    BOOST_CHECK(fed.satisfies(1, 0, bound_t::strict(5)));
    BOOST_CHECK(fed.satisfies(0, 1, bound_t::non_strict(-3)));
    BOOST_CHECK(not fed.satisfies(0, 1, bound_t::non_strict(-6)));

}

BOOST_AUTO_TEST_CASE(relation_test_1){
    Federation fed(3);
    DBM dbm(3);

    dbm.set(0, 0, bound_t::strict(-1));

    auto relation = fed.relation(dbm);

    BOOST_CHECK(relation._superset);
    BOOST_CHECK(not relation._equal);
    BOOST_CHECK(not relation._subset);
    BOOST_CHECK(not relation._different);

    BOOST_CHECK(fed.superset(dbm));
    BOOST_CHECK(not fed.equal(dbm));
    BOOST_CHECK(not fed.subset(dbm));
}

BOOST_AUTO_TEST_CASE(relation_test_2){
    DBM dbm(3);
    auto fed = Federation();

    auto relation = fed.relation(dbm);

    BOOST_CHECK(not relation._superset);
    BOOST_CHECK(not relation._equal);
    BOOST_CHECK(relation._subset);
    BOOST_CHECK(not relation._different);

    BOOST_CHECK(not fed.superset(dbm));
    BOOST_CHECK(not fed.equal(dbm));
    BOOST_CHECK(fed.subset(dbm));
}

BOOST_AUTO_TEST_CASE(relation_test_3){
    Federation fed1(3);
    auto fed2 = Federation();

    auto relation = fed1.relation(fed2);

    BOOST_CHECK(relation._superset);
    BOOST_CHECK(not relation._equal);
    BOOST_CHECK(not relation._subset);
    BOOST_CHECK(not relation._different);

    BOOST_CHECK(fed1.superset(fed2));
    BOOST_CHECK(not fed1.equal(fed2));
    BOOST_CHECK(not fed1.subset(fed2));
}

BOOST_AUTO_TEST_CASE(is_unbounded_test_1){
    Federation fed(3);
    DBM dbm(3);

    dbm.future();

    BOOST_CHECK(not fed.is_unbounded());

    fed.add(dbm);

    BOOST_CHECK(not fed.is_unbounded());

    fed.remove(0);

    BOOST_CHECK(fed.is_unbounded());
}

BOOST_AUTO_TEST_CASE(restrict_test_1){
    Federation fed(3);
    fed.future();
    fed.restrict(0, 1, bound_t::strict(-5));

    fed.add(DBM(3));

    BOOST_CHECK(fed.size() == 2);

    fed.restrict(0, 1, bound_t::strict(-5));

    BOOST_CHECK(fed.size() == 1);
}

BOOST_AUTO_TEST_CASE(remove_clock_test_1){
    Federation fed(3);

    BOOST_CHECK(fed.dimension() == 3);

    fed.remove_clock(2);

    BOOST_CHECK(fed.dimension() == 2);
}

BOOST_AUTO_TEST_CASE(add_clock_at_test_1){
    Federation fed(3);

    BOOST_CHECK(fed.dimension() == 3);

    fed.add_clock_at(2);

    BOOST_CHECK(fed.dimension() == 4);
}
