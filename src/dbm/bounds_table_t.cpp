//
// Created by Thomas Grosen on 21/10/2021.
//

#include <iomanip>
#include "bounds_table_t.h"

namespace dbm {

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

}
