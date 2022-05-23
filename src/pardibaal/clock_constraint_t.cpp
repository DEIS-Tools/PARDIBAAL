/*
 * Copyright Thomas M. Grosen 
 * Created on 19/05/2022
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
#include "clock_constraint_t.h"
namespace pardibaal {

    clock_constraint_t::clock_constraint_t(dim_t i, dim_t j, const bound_t &bound) : _i(i), _j(j), _bound(bound) {}

    clock_constraint_t clock_constraint_t::zero(dim_t i, dim_t j) {return clock_constraint_t(i, j, bound_t::zero());}

    clock_constraint_t clock_constraint_t::inf(dim_t i, dim_t j) {return clock_constraint_t(i, j, bound_t::inf());}

    clock_constraint_t clock_constraint_t::upper_strict(dim_t x, val_t n) {
        return clock_constraint_t(x, 0, bound_t::strict(n));
    }

    clock_constraint_t clock_constraint_t::upper_non_strict(dim_t x, val_t n) {
        return clock_constraint_t(x, 0, bound_t::non_strict(n));
    }

    clock_constraint_t clock_constraint_t::lower_strict(dim_t x, val_t n) {
        return clock_constraint_t(0, x, bound_t::strict(-n));
    }

    clock_constraint_t clock_constraint_t::lower_non_strict(dim_t x, val_t n) {
        return clock_constraint_t(0, x, bound_t::non_strict(-n));
    }

    std::ostream& operator<<(std::ostream& out, const clock_constraint_t& constraint) {
        out << '[' << constraint._i << "] - [" << constraint._j << "] ";
        if (constraint._bound.is_inf())
            out << "INF";
        else
            out << (constraint._bound.is_strict() ? "< " : "<= ") << constraint._bound.get_bound();

        return out;
    }
}
