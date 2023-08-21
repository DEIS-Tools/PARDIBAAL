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

#ifndef PARDIBAAL_BOUNDS_TABLE_T_H
#define PARDIBAAL_BOUNDS_TABLE_T_H

#include <vector>
#include <ostream>

#include "bound_t.h"


namespace pardibaal {
    struct bounds_table_t {
    public:
        explicit bounds_table_t(dim_t number_of_clocks);

        bounds_table_t(const bounds_table_t& other) : _number_of_clocks(other._number_of_clocks) {
            _memory = (bound_t*) malloc(_number_of_clocks * _number_of_clocks * sizeof(bound_t));
            _bounds = (bound_t**) malloc(_number_of_clocks * sizeof(bound_t*));
            
            for (dim_t i = 0; i < _number_of_clocks * _number_of_clocks; ++i)
                _memory[i] = other._memory[i];

            for (dim_t i = 0; i < _number_of_clocks; ++i)
                _bounds[i] = _memory + (i * _number_of_clocks);
        }

        bounds_table_t(bounds_table_t&& other) : _number_of_clocks(other._number_of_clocks) {
            if (_memory == other._memory) return;
            free(_bounds);
            free(_memory);
            
            _memory = other._memory;
            _bounds = other._bounds;

            other._memory = nullptr;
            other._bounds = nullptr;
        }

        bounds_table_t& operator=(const bounds_table_t& other) {
            if (_memory == other._memory) return *this;
            free(_bounds);
            free(_memory);

            _number_of_clocks = other._number_of_clocks;

            _memory = (bound_t*) malloc(_number_of_clocks * _number_of_clocks * sizeof(bound_t));
            _bounds = (bound_t**) malloc(_number_of_clocks * sizeof(bound_t*));

            for (dim_t i = 0; i < _number_of_clocks * _number_of_clocks; ++i)
                _memory[i] = other._memory[i];

            for (dim_t i = 0; i < _number_of_clocks; ++i)
                _bounds[i] = _memory + (i * _number_of_clocks);

            return *this;
        }

        bounds_table_t& operator=(bounds_table_t&& other) {
            if (_memory == other._memory) return *this;
            free(_bounds);
            free(_memory);

            _number_of_clocks = other._number_of_clocks;
            
            _memory = other._memory;
            _bounds = other._bounds;

            other._memory = nullptr;
            other._bounds = nullptr;

            return *this;
        }

        /** number of clock including the zero clock
         * same as the dimension of the "matrix"
         * @return number of clocks including the zero clock
         */
        [[nodiscard]] dim_t number_of_clocks() const;

        [[nodiscard]] inline bound_t at(dim_t i, dim_t j) const {
            return _bounds[i][j];
        }

        inline void set(dim_t i, dim_t j, bound_t bound) {
            this->_bounds[i][j] = bound;
        }

        /** Used for iteration, where indexes does not matter
         * Ordered as (0, 0), (0, 1), ..., (1, 0), (1, 1), ..., (n -1, n -1).
         */
        [[nodiscard]] const bound_t* raw_begin() const { return _memory; }
        [[nodiscard]] bound_t*       begin() {return _memory;}
        [[nodiscard]] const bound_t* begin() const {return _memory;}

        [[nodiscard]] const bound_t* raw_end() const { return _memory + _number_of_clocks*_number_of_clocks; }
        [[nodiscard]] bound_t*       end()       {return _memory + _number_of_clocks*_number_of_clocks;}
        [[nodiscard]] const bound_t* end() const {return _memory + _number_of_clocks*_number_of_clocks;}

        friend std::ostream& operator<<(std::ostream& out, const bounds_table_t& table);

        ~bounds_table_t() {
            free(_bounds);
            free(_memory);
        }

    private:
        dim_t _number_of_clocks;
        bound_t* _memory = nullptr;
        bound_t** _bounds = nullptr;
    };

    std::ostream& operator<<(std::ostream& out, const bounds_table_t& table);
}

#endif //PARDIBAAL_BOUNDS_TABLE_T_H
