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
#ifndef PARDIBAAL_CLOCK_CONSTRAINT_T_H
#define PARDIBAAL_CLOCK_CONSTRAINT_T_H

#include "bound_t.h"

namespace pardibaal {

    struct clock_constraint_t {
        dim_t _i, _j;
        bound_t _bound;

        clock_constraint_t(dim_t i, dim_t j, const bound_t &bound);

        static clock_constraint_t zero(dim_t i, dim_t j);
        static clock_constraint_t inf(dim_t i, dim_t j);

        static clock_constraint_t upper_strict(dim_t x, val_t n);
        static clock_constraint_t upper_non_strict(dim_t x, val_t n);
        static clock_constraint_t lower_strict(dim_t x, val_t n);
        static clock_constraint_t lower_non_strict(dim_t x, val_t n);
    };
}


#endif //PARDIBAAL_CLOCK_CONSTRAINT_T_H
