/*
 * Copyright Thomas M. Grosen
 * Created on 21/10/2021.
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

#include <vector>
#include <ostream>
#include <iomanip>

#include "bounds_table_t.h"
#include "bound_t.h"
#include "errors.h"

namespace pardibaal {
    bounds_table_t::bounds_table_t(dim_t number_of_clocks) : _number_of_clocks(number_of_clocks) {
        _memory = (bound_t*) malloc(_number_of_clocks * _number_of_clocks * sizeof(bound_t));
        _bounds = (bound_t**) malloc(_number_of_clocks * sizeof(bound_t*));
        
        for (dim_t i = 0; i < _number_of_clocks * _number_of_clocks; ++i)
            _memory[i] = bound_t::le_zero();

        for (dim_t i = 0; i < _number_of_clocks; ++i)
            _bounds[i] = _memory + (i * _number_of_clocks);
    }

    dim_t bounds_table_t::number_of_clocks() const {return this->_number_of_clocks;}

//     bound_t bounds_table_t::at(dim_t i, dim_t j) const {
// #ifndef NEXCEPTIONS
//         if (i >= _number_of_clocks || j >= _number_of_clocks)
//             throw base_error("ERROR: Out of bounds access on coordinate: ", i, ", ", j, " with dimensions: ",
//                              _number_of_clocks);
// #endif

//         return _bounds[i * _number_of_clocks + j];
//     }

//     void bounds_table_t::set(dim_t i, dim_t j, bound_t bound) {
// #ifndef NEXCEPTIONS
//         if (i >= _number_of_clocks || j >= _number_of_clocks)
//             throw base_error("ERROR: Out of bounds access on coordinate: ", i, ", ", j, " with dimensions: ",
//                              _number_of_clocks);
// #endif
//         this->_bounds[i * _number_of_clocks + j] = bound;
//     }

    std::ostream& operator<<(std::ostream& out, const bounds_table_t& table) {
        out << '\n';
        for (dim_t i = 0; i < table._number_of_clocks; ++i) {
            for (dim_t j = 0; j < table._number_of_clocks; ++j) {
                bound_t b = table.at(i, j);
                out << std::left << std::setw(8);
                out << b;
            }
            out << '\n';
        }

        return out;
    }
}
