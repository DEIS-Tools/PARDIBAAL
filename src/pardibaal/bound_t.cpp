/*
 * Copyright Thomas M. Grosen
 * Created on 26/10/2021.
 */

/*
 * This file is part of PARDIBAAL
 *
 * PARDIBAAL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PARDIBAAL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with PARDIBAAL.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "bound_t.h"

namespace pardibaal {

    bound_t::bound_t(val_t n, strict_t strictness) : _n(n) {_strict = strictness == STRICT ? true : false;}

    bound_t::bound_t(val_t n, bool strict) : _n(n), _strict(strict) {}

    bound_t bound_t::strict(val_t n) {return bound_t(n, STRICT);}

    bound_t bound_t::non_strict(val_t n) {return bound_t(n, NON_STRICT);}

    bound_t bound_t::inf() {
        bound_t b;
        b._inf = true;
        b._strict = true;

        return b;
    }

    bound_t bound_t::zero() {
        return non_strict(0);
    }

    const bound_t &bound_t::max(const bound_t &a, const bound_t &b) {
        return a < b ? b : a;
    }

    const bound_t bound_t::max(bound_t &&a, bound_t &&b) {
        return max(a, b);
    }

    const bound_t bound_t::max(const bound_t &a, bound_t &&b) {
        return max(a, b);
    }

    const bound_t bound_t::max(bound_t &&a, const bound_t &b) {
        return max(a, b);
    }

    const bound_t &bound_t::min(const bound_t &a, const bound_t &b) {
        return a <= b ? a : b;
    }

    const bound_t bound_t::min(bound_t &&a, bound_t &&b) {
        return bound_t::min(a, b);
    }

    const bound_t bound_t::min(const bound_t &a, bound_t &&b) {
        return bound_t::min(a, b);
    }

    const bound_t bound_t::min(bound_t &&a, const bound_t &b) {
        return bound_t::min(a, b);
    }

    const bound_t bound_t::operator+(bound_t rhs) const {
        if (this->is_inf() || rhs.is_inf())
            return bound_t::inf();

        return bound_t(this->get_bound() + rhs.get_bound(), this->is_strict() || rhs.is_strict());
    }

    const bound_t bound_t::operator+(val_t rhs) const {
        return bound_t(this->get_bound() + rhs, this->is_strict());
    }

    const bound_t bound_t::operator*(val_t rhs) const {
        return bound_t(this->get_bound() * rhs, this->is_strict());
    }

    bool bound_t::operator<(bound_t rhs) const {
        if (this->is_inf()) return false;
        if (rhs.is_inf()) return true;

        if (this->get_bound() == rhs.get_bound()) {
            if (rhs.is_strict())
                return false;

            return this->is_strict();
        }

        return this->get_bound() < rhs.get_bound();
    }

    bool bound_t::operator==(bound_t rhs) const {
        if (this->is_inf() || rhs.is_inf())
            return this->is_inf() && rhs.is_inf();

        return (this->get_bound() == rhs.get_bound()) && (this->is_strict() == rhs.is_strict());
    }

    bool bound_t::operator!=(bound_t rhs) const {
        return not (*this == rhs);
    }

    bool bound_t::operator>(bound_t rhs) const {
        return rhs < *this;
    }

    bool bound_t::operator>=(bound_t rhs) const {
        return not (*this < rhs);
    }

    bool bound_t::operator<=(bound_t rhs) const {
        return not (rhs < *this);
    }

    const bound_t operator+(val_t val, const bound_t bound) {
        return bound + val;
    }

    const bound_t operator*(val_t val, const bound_t bound) {
        return bound * val;
    }

    std::ostream& operator<<(std::ostream& out, const bound_t& bound) {
        if (bound.is_inf()) {
            out << "INF";
        }
        else {
            out << (bound.is_strict() ? "<" : "<=") + std::to_string(bound.get_bound());
        }

        return out;
    }
}