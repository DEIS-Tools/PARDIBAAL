//
// Created by Thomas Grosen on 13/10/2021.
//

#include "../src/dbm/DBM.h"
#include <cassert>
#include <chrono>
#include <iostream>

using namespace dbm;

bool random_gen_test01() {
    DBMPtr *d = DBMPtr::gen_random(2, 100);

    return !d->is_empty();
}

bool canonical_test_ptr() {
    DBMPtr d(4);

//    *d._bounds_table.get(0, 0) = bound_t::zero();
//    *d._bounds_table.get(0, 1) = bound_t::zero();
//    *d._bounds_table.get(0, 2) = bound_t::zero();
    *d._bounds_table.get(0, 3) = bound_t(5, true);

    *d._bounds_table.get(1, 0) = bound_t(20, true);
//    *d._bounds_table.get(1, 1) = bound_t::zero();
    *d._bounds_table.get(1, 2) = bound_t(-10, false);
    *d._bounds_table.get(1, 3) = bound_t::inf();

    *d._bounds_table.get(2, 0) = bound_t(20, true);
    *d._bounds_table.get(2, 1) = bound_t(10, true);
//    *d._bounds_table.get(2, 2) = bound_t::zero();
    *d._bounds_table.get(2, 0) = bound_t::inf();

    *d._bounds_table.get(3, 0) = bound_t::inf();
    *d._bounds_table.get(3, 1) = bound_t::inf();
    *d._bounds_table.get(3, 2) = bound_t::inf();
//    *d._bounds_table.get(3, 3) = bound_t::zero();

    d.close_p();

    //assert(!d.is_empty());

    return true;
}

bool canonical_test_val() {
    DBMPtr d(4);

//    *d._bounds_table.get(0, 0) = bound_t::zero();
//    *d._bounds_table.get(0, 1) = bound_t::zero();
//    *d._bounds_table.get(0, 2) = bound_t::zero();
    d._bounds_table.ref(0, 3) = bound_t(5, true);

    d._bounds_table.ref(1, 0) = bound_t(20, true);
//    *d._bounds_table.get(1, 1) = bound_t::zero();
    d._bounds_table.ref(1, 2) = bound_t(-10, false);
    d._bounds_table.ref(1, 3) = bound_t::inf();

    d._bounds_table.ref(2, 0) = bound_t(20, true);
    d._bounds_table.ref(2, 1) = bound_t(10, true);
//    *d._bounds_table.get(2, 2) = bound_t::zero();
    d._bounds_table.ref(2, 0) = bound_t::inf();

    d._bounds_table.ref(3, 0) = bound_t::inf();
    d._bounds_table.ref(3, 1) = bound_t::inf();
    d._bounds_table.ref(3, 2) = bound_t::inf();
//    *d._bounds_table.get(3, 3) = bound_t::zero();

    d.close_val();

    //assert(!d.is_empty());

    return true;
}

bool canonical_test_vec() {
    DBM d(4);

//    d._bounds_table.get(0, 0) = bound_t::zero();
//    d._bounds_table.get(0, 1) = bound_t::zero();
//    d._bounds_table.get(0, 2) = bound_t::zero();
    d._bounds_table.get(0, 3) = bound_t(5, true);

    d._bounds_table.get(1, 0) = bound_t(20, true);
//    d._bounds_table.get(1, 1) = bound_t::zero();
    d._bounds_table.get(1, 2) = bound_t(-10, false);
    d._bounds_table.get(1, 3) = bound_t::inf();

    d._bounds_table.get(2, 0) = bound_t(20, true);
    d._bounds_table.get(2, 1) = bound_t(10, true);
//    d._bounds_table.get(2, 2) = bound_t::zero();
    d._bounds_table.get(2, 3) = bound_t::inf();

    d._bounds_table.get(3, 0) = bound_t::inf();
    d._bounds_table.get(3, 1) = bound_t::inf();
    d._bounds_table.get(3, 2) = bound_t::inf();
//    d._bounds_table.get(3, 3) = bound_t::zero();

    d.close();

    //assert(!d.is_empty());

    return true;
}

bool canonical_benchmark_ptr(int runs) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < runs; i++)
        canonical_test_ptr();
    auto t1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t1 - t0;
    std::cout << "Closed " << runs << " in " << ms.count() << "ms\n";

    return true;
}

bool canonical_benchmark_val(int runs) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < runs; i++)
        canonical_test_val();
    auto t1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t1 - t0;
    std::cout << "Closed " << runs << " in " << ms.count() << "ms\n";

    return true;
}

bool canonical_benchmark_vec(int runs) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < runs; i++)
        canonical_test_vec();
    auto t1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms = t1 - t0;
    std::cout << "Closed " << runs << " in " << ms.count() << "ms\n";

    return true;
}

int main() {

    std::cout << "Vector: ";
    canonical_benchmark_vec(10000000);

    std::cout << "Pointers: ";
    canonical_benchmark_ptr(10000000);

    std::cout << "Values: ";
    canonical_benchmark_val(10000000);

    return 0;
};

