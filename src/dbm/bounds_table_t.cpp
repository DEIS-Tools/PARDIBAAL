//
// Created by Thomas Grosen on 21/10/2021.
//

#include "bounds_table_t.h"
#include "bound_t.h"

#include <vector>

namespace dbm {

    bounds_table_t::bounds_table_t(int size) : _number_of_clocks(size) {
        _bounds = std::vector<bound_t>(size * size);
    }

    bound_t &bounds_table_t::get(int i, int j) {
#ifdef DBUG_BOUNDS
        if (i >= _number_of_clocks || j >= _number_of_clocks) {
            std::cout << "Out of bounds access on coordinate: " << i << ", " << j << " with max size: " <<
                    _number_of_clocks * _number_of_clocks << "\n";
            abort();
        }
#endif

        return _bounds[i * _number_of_clocks + j];
    }

    bound_t bounds_table_t::at(int i,  int j) const {
#ifdef DBUG_BOUNDS
        if (i >= _number_of_clocks || j >= _number_of_clocks) {
            std::cout << "Out of bounds access on coordinate: " << i << ", " << j << " with max size: " <<
                    _number_of_clocks * _number_of_clocks << "\n";
            abort();
        }
#endif

        return _bounds[i * _number_of_clocks + j];
    }

}
