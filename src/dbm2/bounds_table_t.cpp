/*
 * Copyright Thomas M. Grosen
 * Created on 21/10/2021.
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

#include <iomanip>
#include "bounds_table_t.h"

namespace dbm2 {

    bounds_table_t::bounds_table_t(dim_t number_of_clocks) : _number_of_clocks(number_of_clocks) {
        _bounds = std::vector<bound_t>(number_of_clocks * number_of_clocks);
    }

    bound_t &bounds_table_t::at(dim_t i, dim_t j) {
#ifdef DBUG_BOUNDS
        if (i >= _number_of_clocks || j >= _number_of_clocks) {
            std::cout << "Out of bounds access on coordinate: " << i << ", " << j << " with max size: " <<
                    _number_of_clocks * _number_of_clocks << "\n";
            abort();
        }
#endif

        return _bounds[i * _number_of_clocks + j];
    }

    bound_t bounds_table_t::at(dim_t i, dim_t j) const {
#ifdef DBUG_BOUNDS
        if (i >= _number_of_clocks || j >= _number_of_clocks) {
            std::cout << "Out of bounds access on coordinate: " << i << ", " << j << " with max size: " <<
                    _number_of_clocks * _number_of_clocks << "\n";
            abort();
        }
#endif

        return _bounds[i * _number_of_clocks + j];
    }

    std::ostream& operator<<(std::ostream& out, const bounds_table_t& table) {
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

    void bounds_table_t::remove_clock(dim_t c) {
#ifdef DBUG_BOUNDS
        if (c >= _number_of_clocks) {
            std::cout << "Out of bounds access on coordinate: " << c << " with max size: " <<
                    _number_of_clocks * _number_of_clocks << "\n";
            abort();
        }
#endif
        std::vector<bound_t> new_bounds((_number_of_clocks-1) * (_number_of_clocks-1));

        dim_t i2 = 0;
        for (dim_t i = 0; i < ((_number_of_clocks) * (_number_of_clocks)); ++i) {
            if ((i >= c && (i - c) % _number_of_clocks == 0) ||
                (i >= (c * _number_of_clocks) && i < ((c+1) * _number_of_clocks)))
                continue; //This index is deleted, so skip

            new_bounds[i2] = _bounds[i];
            ++i2;
        }

        _bounds = new_bounds;
        --_number_of_clocks;
    }

    void bounds_table_t::swap_clocks(dim_t a, dim_t b) {
        if (a == b || a >= _number_of_clocks || b >= _number_of_clocks)
            return; //TODO: throw error?
        bound_t tmp;
        for (dim_t i = 0; i < _number_of_clocks; ++i) {
            if (!(i == a || i == b)) {
                tmp = at(i, a);
                at(i, a) = at(i, b);
                at(i, b) = tmp;

                tmp = at(a, i);
                at(a, i) = at(b, i);
                at(b, i) = tmp;
            }
        }
        tmp = at(a, b);
        at(a, b) = at(b, a);
        at(b, a) = tmp;
    }

    void bounds_table_t::add_clock_at(dim_t c) {
        ++_number_of_clocks;
        std::vector<bound_t> new_bounds(_number_of_clocks * _number_of_clocks);
        dim_t src = 0;

        for (dim_t dst = 0; dst < _number_of_clocks * _number_of_clocks; ++dst) {
            if ((dst >= c && (dst - c) % _number_of_clocks == 0) ||
                (dst >= (c * _number_of_clocks) && dst < ((c+1) * _number_of_clocks)))
                continue;
            new_bounds[dst] = _bounds[src++];
        }

        _bounds = new_bounds;

        //Free the new clock
        for (dim_t i = 0; i < _number_of_clocks; ++i) {
            if (i != c) {
                this->at(c, i) = bound_t::inf();
                this->at(i, c) = this->at(i, 0);
            }
        }
    }
}
