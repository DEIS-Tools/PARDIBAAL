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

#include <ostream>

#include "bound_t.h"

namespace pardibaal {

    // val_t bound_t::get_bound()    const {return this->_n;}
    // bool bound_t::is_strict()     const {return this->_strict;}
    // bool bound_t::is_non_strict() const {return not this->_strict;}
    // bool bound_t::is_inf()        const {return this->_inf;}

    const bound_t &bound_t::max(const bound_t &a, const bound_t &b) {return a < b ? b : a;}
    bound_t bound_t::max(bound_t &&a, bound_t &&b) {return max(a, b);}
    bound_t bound_t::max(const bound_t &a, bound_t &&b) {return max(a, b);}
    bound_t bound_t::max(bound_t &&a, const bound_t &b) {return max(a, b);}

    const bound_t &bound_t::min(const bound_t &a, const bound_t &b) {return a <= b ? a : b;}
    bound_t bound_t::min(bound_t &&a, bound_t &&b) {return bound_t::min(a, b);}
    bound_t bound_t::min(const bound_t &a, bound_t &&b) {return bound_t::min(a, b);}
    bound_t bound_t::min(bound_t &&a, const bound_t &b) {return bound_t::min(a, b);}

    bound_t bound_t::operator+(bound_t rhs) const {
        if (this->is_inf() || rhs.is_inf())
            return bound_t::inf();

        return bound_t(this->get_bound() + rhs.get_bound(), this->is_strict() || rhs.is_strict());
    }

    bound_t bound_t::operator+(val_t rhs) const {
        if (not this->is_inf())
            return bound_t(this->get_bound() + rhs, this->is_strict());
        return *this;
    }

    bound_t bound_t::operator-(val_t rhs) const {
        if (not this->is_inf())
            return bound_t(this->get_bound() - rhs, this->is_strict());
        return *this;
    }

    bound_t bound_t::operator*(val_t rhs) const {
        if (not this->is_inf())
            return bound_t(this->get_bound() * rhs, this->is_strict());
        return *this;
    }

    bool bound_t::operator<(bound_t rhs) const {
        if (this->is_inf()) return false;
        if (rhs.is_inf()) return true;

        if (this->get_bound() == rhs.get_bound())
            return !rhs.is_strict() && this->is_strict();

        return this->get_bound() < rhs.get_bound();
    }

    bool bound_t::operator==(bound_t rhs) const {
        if (this->is_inf() || rhs.is_inf())
            return this->is_inf() && rhs.is_inf();

        return (this->get_bound() == rhs.get_bound()) && (this->is_strict() == rhs.is_strict());
    }

    bool bound_t::operator!=(bound_t rhs) const {return not (*this == rhs);}
    bool bound_t::operator>(bound_t rhs)  const {return rhs < *this;}
    bool bound_t::operator>=(bound_t rhs) const {return not (*this < rhs);}
    bool bound_t::operator<=(bound_t rhs) const {return not (rhs < *this);}

    bool bound_t::operator==(val_t rhs) const {return *this == bound_t::non_strict(rhs);}
    bool bound_t::operator!=(val_t rhs) const {return *this != bound_t::non_strict(rhs);}
    bool bound_t::operator<(val_t rhs) const {return *this < bound_t::non_strict(rhs);}
    bool bound_t::operator>(val_t rhs) const {return *this > bound_t::non_strict(rhs);}
    bool bound_t::operator<=(val_t rhs) const {return *this <= bound_t::non_strict(rhs);}
    bool bound_t::operator>=(val_t rhs) const {return *this >= bound_t::non_strict(rhs);}

    bool lt(bound_t lhs, bound_t rhs) {return lhs < rhs;}
    bool le(bound_t lhs, bound_t rhs) {return lhs <= rhs;}
    bool gt(bound_t lhs, bound_t rhs) {return lhs > rhs;}
    bool ge(bound_t lhs, bound_t rhs) {return lhs >= rhs;}

    bound_t operator+(val_t val, bound_t bound) {return bound + val;}
    bound_t operator-(val_t val, bound_t bound) {return bound - val;}
    bound_t operator*(val_t val, bound_t bound) {return bound * val;}

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