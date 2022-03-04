/*
 * Copyright Thomas M. Grosen
 * Created on 26/10/2021.
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

#include "bound_t.h"

namespace dbm2 {

    bound_t::bound_t(val_t n, strict_t strictness) : _n(n) {_strict = strictness == STRICT ? true : false;}

    bound_t::bound_t(val_t n, bool strict) : _n(n), _strict(strict) {}

    bound_t bound_t::strict(val_t n) {return bound_t(n, STRICT);}

    bound_t bound_t::non_strict(val_t n) {return bound_t(n, NON_STRICT);}

    bound_t bound_t::zero() {
        return non_strict(0);
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

    std::ostream& operator<<(std::ostream& out, const bound_t& bound) {
        if (bound._inf) {
            out << "INF";
        }
        else {
            out << (bound._strict ? "<" : "<=") + std::to_string(bound._n);
        }

        return out;
    }
}