//
// Created by Thomas Grosen on 13/10/2021.
//

#include "../src/dbm/DBM.h"
#include <cassert>
#include <chrono>
#include <iostream>

using namespace dbm;

bool random_gen_test01() {
    DBM d = DBM::gen_random(2, 100);

    return !d.is_empty();
}

bool canonical_test01() {
    DBM d(4);

    *d._bounds_table.get(0, 0) = bound_t::zero();
    *d._bounds_table.get(0, 1) = bound_t::zero();
    *d._bounds_table.get(0, 2) = bound_t::zero();
    *d._bounds_table.get(0, 3) = bound_t(5, true);

    *d._bounds_table.get(1, 0) = bound_t(20, true);
    *d._bounds_table.get(1, 1) = bound_t::zero();
    *d._bounds_table.get(1, 2) = bound_t(-10, false);
    *d._bounds_table.get(1, 3) = bound_t::inf();

    *d._bounds_table.get(2, 0) = bound_t(20, true);
    *d._bounds_table.get(2, 1) = bound_t(10, true);
    *d._bounds_table.get(2, 2) = bound_t::zero();
    *d._bounds_table.get(2, 0) = bound_t::inf();

    *d._bounds_table.get(3, 0) = bound_t::inf();
    *d._bounds_table.get(3, 1) = bound_t::inf();
    *d._bounds_table.get(3, 2) = bound_t::inf();
    *d._bounds_table.get(3, 3) = bound_t::zero();

    d.close_p_arr();

    //assert(!d.is_empty());

    return true;
}

bool canonical_test02() {
    DBM d(4);

    *d._bounds_table.get(0, 0) = bound_t::zero();
    *d._bounds_table.get(0, 1) = bound_t::zero();
    *d._bounds_table.get(0, 2) = bound_t::zero();
    *d._bounds_table.get(0, 3) = bound_t(5, true);

    *d._bounds_table.get(1, 0) = bound_t(20, true);
    *d._bounds_table.get(1, 1) = bound_t::zero();
    *d._bounds_table.get(1, 2) = bound_t(-10, false);
    *d._bounds_table.get(1, 3) = bound_t::inf();

    *d._bounds_table.get(2, 0) = bound_t(20, true);
    *d._bounds_table.get(2, 1) = bound_t(10, true);
    *d._bounds_table.get(2, 2) = bound_t::zero();
    *d._bounds_table.get(2, 0) = bound_t::inf();

    *d._bounds_table.get(3, 0) = bound_t::inf();
    *d._bounds_table.get(3, 1) = bound_t::inf();
    *d._bounds_table.get(3, 2) = bound_t::inf();
    *d._bounds_table.get(3, 3) = bound_t::zero();

    d.close_val_arr();

    //assert(!d.is_empty());

    return true;
}

bool canonical_test03() {
    DBM d(4);

    d._bounds_table_vec.at(0, 0) = bound_t::zero();
    d._bounds_table_vec.at(0, 1) = bound_t::zero();
    d._bounds_table_vec.at(0, 2) = bound_t::zero();
    d._bounds_table_vec.at(0, 3) = bound_t(5, true);

    d._bounds_table_vec.at(1, 0) = bound_t(20, true);
    d._bounds_table_vec.at(1, 1) = bound_t::zero();
    d._bounds_table_vec.at(1, 2) = bound_t(-10, false);
    d._bounds_table_vec.at(1, 3) = bound_t::inf();

    d._bounds_table_vec.at(2, 0) = bound_t(20, true);
    d._bounds_table_vec.at(2, 1) = bound_t(10, true);
    d._bounds_table_vec.at(2, 2) = bound_t::zero();
    d._bounds_table_vec.at(2, 0) = bound_t::inf();

    d._bounds_table_vec.at(3, 0) = bound_t::inf();
    d._bounds_table_vec.at(3, 1) = bound_t::inf();
    d._bounds_table_vec.at(3, 2) = bound_t::inf();
    d._bounds_table_vec.at(3, 3) = bound_t::zero();

    d.close_vec();

    //assert(!d.is_empty());

    return true;
}

bool canonical_benchmark01(int runs) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < runs; i++)
        canonical_test01();
    auto t1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t1 - t0;
    std::cout << "Closed " << runs << " in " << ms.count() << "ms\n";
}

bool canonical_benchmark02(int runs) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < runs; i++)
        canonical_test02();
    auto t1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t1 - t0;
    std::cout << "Closed " << runs << " in " << ms.count() << "ms\n";
}

bool canonical_benchmark03(int runs) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < runs; i++)
        canonical_test03();
    auto t1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t1 - t0;
    std::cout << "Closed " << runs << " in " << ms.count() << "ms\n";
}

int main() {
    std::cout << "Pointers: ";
    canonical_benchmark01(100000000);

    std::cout << "Values: ";
    canonical_benchmark02(100000000);

    std::cout << "Vector: ";
    canonical_benchmark03(100000000);

//    random_gen_test01();
    return 0;
};

