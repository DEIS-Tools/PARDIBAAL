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
#ifndef PARDIBAAL_DIFFERENCE_BOUND_T_H
#define PARDIBAAL_DIFFERENCE_BOUND_T_H

#include "bound_t.h"

namespace pardibaal {

    struct difference_bound_t {
        dim_t _i, _j;
        bound_t _bound;

        difference_bound_t(dim_t i, dim_t j, const bound_t &bound);

        static difference_bound_t zero(dim_t i, dim_t j);
        static difference_bound_t inf(dim_t i, dim_t j);

        static difference_bound_t upper_strict(dim_t x, val_t n);
        static difference_bound_t upper_non_strict(dim_t x, val_t n);
        static difference_bound_t lower_strict(dim_t x, val_t n);
        static difference_bound_t lower_non_strict(dim_t x, val_t n);

        friend std::ostream& operator<<(std::ostream& out, const difference_bound_t& constraint);
    };

    std::ostream& operator<<(std::ostream& out, const difference_bound_t& constraint);
}


#endif //PARDIBAAL_DIFFERENCE_BOUND_T_H
