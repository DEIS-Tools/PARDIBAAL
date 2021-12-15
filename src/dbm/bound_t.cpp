//
// Created by Thomas Grosen on 26/10/2021.
//

#include "bound_t.h"

namespace dbm {

    bound_t::bound_t(guard_t n, bool strict) : _n(n), _strict(strict) {}

    bound_t bound_t::zero() {
        return bound_t(0, false);
    }

    bound_t bound_t::inf() {
        bound_t b;
        b._inf = true;

        return b;
    }

    const bound_t &bound_t::max(const bound_t &a, const bound_t &b) {
        return a < b ? b : a;
    }

    bound_t bound_t::max(bound_t &&a, bound_t &&b) {
        return max(a, b);
    }

    bound_t bound_t::max(const bound_t &a, bound_t &&b) {
        return max(a, b);
    }

    bound_t bound_t::max(bound_t &&a, const bound_t &b) {
        return max(a, b);
    }

    const bound_t &bound_t::min(const bound_t &a, const bound_t &b) {
        return a <= b ? a : b;
    }

    bound_t bound_t::min(bound_t &&a, bound_t &&b) {
        return bound_t::min(a, b);
    }

    bound_t bound_t::min(const bound_t &a, bound_t &&b) {
        return bound_t::min(a, b);
    }

    bound_t bound_t::min(bound_t &&a, const bound_t &b) {
        return bound_t::min(a, b);
    }

    bound_t bound_t::operator+(bound_t rhs) const {
        if (this->_inf || rhs._inf)
            return bound_t::inf();

        return bound_t(this->_n + rhs._n, this->_strict || rhs._strict);
    }

    bool bound_t::operator<=(bound_t rhs) const {
        if (this->_inf) return rhs._inf;
        if (rhs._inf) return true;

        if (this->_n == rhs._n) {
            if (rhs._strict)
                return this->_strict;

            return true;
        }

        return this->_n < rhs._n;
    }

    bool bound_t::operator<(bound_t rhs) const {
        if (this->_inf) return false;
        if (rhs._inf) return true;

        if (this->_n == rhs._n) {
            if (rhs._strict)
                return false;

            return this->_strict;
        }

        return this->_n < rhs._n;
    }

    bool bound_t::operator==(bound_t rhs) const {
        if (this->_inf || rhs._inf)
            return this->_inf && rhs._inf;

        return (this->_n == rhs._n) && (this->_strict == rhs._strict);
    }

    bool bound_t::operator!=(bound_t rhs) const {
        return !(*this == rhs);
    }

    bool bound_t::operator>(bound_t rhs) const {
        return rhs < *this;
    }

    bool bound_t::operator>=(bound_t rhs) const {
        return rhs <= *this;
    }
}