//
// Created by Thomas Grosen on 13/10/2021.
//

#include "DBM.h"

namespace dbm {
    DBM::DBM(dim_t number_of_clocks) : _bounds_table(number_of_clocks) {}

    bool DBM::is_empty() const {
        // Check if 0 - 0 is less than 0 (used for quicker identification of empty zone
        if (_bounds_table.at(0, 0) < bound_t().zero())
            return true;

        // The DBM has to be closed for this to actually work
        for (int i = 0; i < this->_bounds_table._number_of_clocks; i++) {
            for (int j = 0; j < this->_bounds_table._number_of_clocks; j++) {
                bound_t i_to_j_to_i = this->_bounds_table.at(i, j) + this->_bounds_table.at(j, i);

                if (i_to_j_to_i < bound_t(0, false))
                    return true;
            }
        }

        return false;
    }

    bool DBM::is_included_in(const DBM &d) const {
        for (dim_t i = 0; i < _bounds_table._number_of_clocks; i++)
            for (dim_t j = 0; j < _bounds_table._number_of_clocks; j++)
                if (d._bounds_table.at(i, j) < _bounds_table.at(i, j))
                    return false;

        return true;
    }

    void DBM::close() {
        const int size = _bounds_table._number_of_clocks;

        for(dim_t k = 0; k < size; k++)
            for(dim_t i = 0; i < size; i++)
                for(dim_t j = 0; j < size; j++)
                    _bounds_table.get(i, j) = bound_t::min(_bounds_table.get(i, j),
                                                           _bounds_table.get(i, k) + _bounds_table.get(k, j));
    }

    void DBM::future() {
        for (dim_t i = 1; i < _bounds_table._number_of_clocks; i++)
            _bounds_table.get(i, 0) = bound_t::inf();
    }

    void DBM::restrict(dim_t x, dim_t y, bound_t g) {
        if ((_bounds_table.get(y, x) + g) < bound_t::zero()) // In this case the zone is now empty
            _bounds_table.get(0, 0) = bound_t(-1, false);
        else if (g < _bounds_table.get(x, y)) {
            _bounds_table.get(x, y) = g;
            for (dim_t i = 0; i < _bounds_table._number_of_clocks; i++) {
                for (dim_t j = 0; j < _bounds_table._number_of_clocks; ++j) {
                    if (_bounds_table.get(i, x) + _bounds_table.get(x, j) < _bounds_table.get(i, j))
                        _bounds_table.get(i, j) = _bounds_table.get(i, x) + _bounds_table.get(x, j);

                    if (_bounds_table.get(i, y) + _bounds_table.get(y, j) < _bounds_table.get(i, j))
                        _bounds_table.get(i, j) = _bounds_table.get(i, y) + _bounds_table.get(y, j);
                }
            }
        }

    }

    void DBM::free(dim_t x) {
        for (dim_t i = 0; i < _bounds_table._number_of_clocks; i++) {
            if (i != x) {
                _bounds_table.get(x, i) = bound_t::inf();
                _bounds_table.get(i, x) = _bounds_table.get(i, 0);
            }
        }
    }

    // x := m
    void DBM::assign(dim_t x, dim_t m) {
        for (dim_t i = 0; i < _bounds_table._number_of_clocks; i++) {
            _bounds_table.get(x, i) = bound_t(m, false) + _bounds_table.get(0, i);
            _bounds_table.get(i, x) = bound_t(-1 * m, false) + _bounds_table.get(i, 0);
        }
    }

    // x := y
    void DBM::copy(dim_t x, dim_t y) {
        for (dim_t i = 0; i < _bounds_table._number_of_clocks; i++) {
            if (i != x) {
                _bounds_table.get(x, i) = _bounds_table.at(y, i);
                _bounds_table.get(i, x) = _bounds_table.at(i, y);
            }
        }
        _bounds_table.get(x, y) = bound_t::zero();
        _bounds_table.get(y, x) = bound_t::zero();
    }


}
