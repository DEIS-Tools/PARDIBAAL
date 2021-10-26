//
// Created by Thomas Grosen on 26/10/2021.
//

#include "bound_t.h"

#include <cstdlib>
#include <ctime>

namespace dbm {

    bound_t::bound_t(int n, bool strict) : _n(n), _strict(strict) {}

    bound_t bound_t::zero() {
        return bound_t(0, false);
    }

    bound_t bound_t::inf() {
        bound_t b;
        b._inf = true;

        return b;
    }

    bound_t bound_t::gen_random(const int &max) {
        srand((int) time(0));
        int n = rand() % (max + 1);
        bool strict = rand() % 2;

        n = rand() % 2 ? n : n * (-1);

        return bound_t(n, strict);
    }

    // Returns const ref to the lowest bound
    const bound_t &bound_t::min(const bound_t &a, const bound_t &b) {
        if (bound_t::less_eq(a, b))
            return a;
        return b;
    }

    //TODO: I hope this is a cool way to do it :S
    bound_t bound_t::min(bound_t &&a, bound_t &&b) {
        return bound_t::min(a, b);
    }

    bound_t bound_t::min(const bound_t &a, bound_t &&b) {
        return bound_t::min(a, b);
    }

    bound_t bound_t::min(bound_t &&a, const bound_t &b) {
        return bound_t::min(a, b);
    }

    bound_t bound_t::add(const bound_t &a, const bound_t &b) {
        if (a._inf || b._inf)
            return bound_t::inf();

        return bound_t(a._n + b._n, a._strict || b._strict);
    }

    bound_t bound_t::add(bound_t &&a, bound_t &&b) {
        return bound_t::add(a, b);
    }

    bound_t bound_t::add(const bound_t &a, bound_t &&b) {
        return bound_t::add(a, b);
    }

    bound_t bound_t::add(bound_t &&a, const bound_t &b) {
        return bound_t::add(a, b);
    }

    bool bound_t::less(const bound_t &lhs, const bound_t &rhs) {
        if (lhs._inf) return false;
        if (rhs._inf) return true;

        if (lhs._n == rhs._n) {
            if (rhs._strict)
                return false;

            return lhs._strict;
        }

        return lhs._n < rhs._n;
    }

    bool bound_t::less(bound_t &&lhs, bound_t &&rhs) {
        return bound_t::less(lhs, rhs);
    }

    bool bound_t::less(const bound_t &lhs, bound_t &&rhs) {
        return bound_t::less(lhs, rhs);
    }

    bool bound_t::less(bound_t &&lhs, const bound_t &rhs) {
        return bound_t::less(lhs, rhs);
    }

    bool bound_t::less_eq(const bound_t &lhs, const bound_t &rhs) {
        if (lhs._inf)
            return rhs._inf;

        if (lhs._n == rhs._n) {
            if (rhs._strict)
                return lhs._strict;

            return true;
        }

        return lhs._n < rhs._n;
    }

    bool bound_t::less_eq(bound_t &&lhs, bound_t &&rhs) {
        return bound_t::less_eq(lhs, rhs);
    }

    bool bound_t::less_eq(const bound_t &lhs, bound_t &&rhs) {
        return bound_t::less_eq(lhs, rhs);
    }

    bool bound_t::less_eq(bound_t &&lhs, const bound_t &rhs) {
        return bound_t::less_eq(lhs, rhs);
    }
}