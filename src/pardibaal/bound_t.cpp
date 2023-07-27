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
#include <cassert>

#include "bound_t.h"

namespace pardibaal {

    bound_t bound_t::operator+(bound_t rhs) const {

        if (this->is_inf() || rhs.is_inf())
            return bound_t::inf();

        assert(get_bound() < BOUND_VAL_MAX - rhs.get_bound() && "Overflow");

        return bound_t(this->get_bound() + rhs.get_bound(), this->is_strict() || rhs.is_strict());
    }

    bound_t bound_t::operator+(val_t rhs) const {
        if (this->is_inf())
            return *this;

        assert(get_bound() < BOUND_VAL_MAX - rhs && "Overflow");

        return bound_t(this->get_bound() + rhs, this->is_strict());
    }

    bound_t bound_t::operator-(val_t rhs) const {
        if (this->is_inf())
            return *this;

        assert(get_bound() > BOUND_VAL_MIN + rhs && rhs > BOUND_VAL_MIN && rhs < BOUND_VAL_MAX && "Underflow");

        return bound_t(this->get_bound() - rhs, this->is_strict());
    }

    bound_t bound_t::operator*(val_t rhs) const {
        if (this->is_inf())
            return *this;
        
        auto prod = this->get_bound() * rhs;

        assert((!(this->get_bound() != 0) || (prod / this->get_bound() == rhs))
                && prod > BOUND_VAL_MIN && prod < BOUND_VAL_MAX
                && rhs > BOUND_VAL_MIN && rhs < BOUND_VAL_MAX && ("Overflow or Underflow"));

        return bound_t(this->get_bound() * rhs, this->is_strict());
    }

    bool bound_t::operator==(val_t rhs) const {return this->_data == bound_t::non_strict(rhs)._data;}
    bool bound_t::operator!=(val_t rhs) const {return this->_data != bound_t::non_strict(rhs)._data;}
    bool bound_t::operator<(val_t rhs) const {return this->_data < bound_t::non_strict(rhs)._data;}
    bool bound_t::operator>(val_t rhs) const {return this->_data > bound_t::non_strict(rhs)._data;}
    bool bound_t::operator<=(val_t rhs) const {return this->_data <= bound_t::non_strict(rhs)._data;}
    bool bound_t::operator>=(val_t rhs) const {return this->_data >= bound_t::non_strict(rhs)._data;}

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