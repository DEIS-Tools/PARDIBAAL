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

#include "difference_bound_t.h"
#include "errors.h"

namespace pardibaal {

    std::ostream& operator<<(std::ostream& out, const difference_bound_t& constraint) {
        out << '[' << constraint._i << "] - [" << constraint._j << "] ";
        if (constraint._bound.is_inf())
            out << "INF";
        else
            out << (constraint._bound.is_strict() ? "< " : "<= ") << constraint._bound.get_bound();

        return out;
    }
}
