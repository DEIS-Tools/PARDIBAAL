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

    bound_t &bounds_table_t::get(dim_t i, dim_t j) {
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
        for (int i = 0; i < table._number_of_clocks; i++) {
            for (int j = 0; j < table._number_of_clocks; j++) {
                bound_t b = table.at(i, j);
                out << std::left << std::setw(15);
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
        for (int64_t i = _number_of_clocks-1; i >= 0; i--) {
            if (i == c)
                _bounds.erase(std::next(_bounds.begin(), _number_of_clocks * i), std::next(_bounds.begin(), _number_of_clocks * (i + 1) - 1));
            else
                _bounds.erase(std::next(_bounds.begin(), c + _number_of_clocks * i));
        }
        _number_of_clocks -= 1;
    }

    void bounds_table_t::swap_clocks(dim_t a, dim_t b) {
        if (a == b || a >= _number_of_clocks || b >= _number_of_clocks)
            return; //TODO: throw error?
        bound_t tmp;
        for (int i = 0; i < _number_of_clocks; i++) {
            if (!(i == a || i == b)) {
                tmp = at(i, a);
                get(i, a) = at(i, b);
                get(i, b) = tmp;

                tmp = at(a, i);
                get(a, i) = at(b, i);
                get(b, i) = tmp;
            }
        }
        tmp = at(a, b);
        get(a, b) = at(b, a);
        get(b, a) = tmp;
    }

    void bounds_table_t::add_clock_after(dim_t c) {
        c += 1;
        for (dim_t i = _number_of_clocks-1; i >= 0; i--) {
            if (i == c)
                _bounds.insert(std::next(_bounds.begin(), _number_of_clocks * i + 1), _number_of_clocks, bound_t::zero());
            else
                _bounds.insert(std::next(_bounds.begin(), _number_of_clocks * i + c), bound_t::zero());
        }
    }
}
