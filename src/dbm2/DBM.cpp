/*
 * Copyright Thomas M. Grosen
 * Created on 13/10/2021.
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

#include "DBM.h"

namespace dbm2 {
    DBM::DBM(dim_t number_of_clocks) : _bounds_table(number_of_clocks) {}

    bool DBM::is_empty() const {
        // Check if 0 - 0 is less than 0 (used for quicker identification of empty zone
        if (_bounds_table.at(0, 0) < bound_t().zero())
            return true;

        // The DBM has to be closed for this to actually work
        for (dim_t i = 0; i < this->_bounds_table._number_of_clocks; ++i) {
            for (dim_t j = 0; j < this->_bounds_table._number_of_clocks; ++j) {
                bound_t i_to_j_to_i = this->_bounds_table.at(i, j) + this->_bounds_table.at(j, i);

                if (i_to_j_to_i < bound_t(0, false))
                    return true;
            }
        }

        return false;
    }

    bool DBM::is_included_in(const DBM &d) const {
        for (dim_t i = 0; i < _bounds_table._number_of_clocks; ++i)
            for (dim_t j = 0; j < _bounds_table._number_of_clocks; ++j)
                if (d._bounds_table.at(i, j) < _bounds_table.at(i, j))
                    return false;

        return true;
    }

    bool DBM::is_satisfied(dim_t x, dim_t y, bound_t g) const {
        return bound_t::zero() <= (this->_bounds_table.at(y, x) + g);
    }

    void DBM::close() {
        const dim_t size = _bounds_table._number_of_clocks;

        for(dim_t k = 0; k < size; ++k)
            for(dim_t i = 0; i < size; ++i)
                for(dim_t j = 0; j < size; ++j)
                    _bounds_table.get(i, j) = bound_t::min(_bounds_table.get(i, j),
                                                           _bounds_table.get(i, k) + _bounds_table.get(k, j));
    }

    void DBM::future() {
        for (dim_t i = 1; i < _bounds_table._number_of_clocks; ++i)
            _bounds_table.get(i, 0) = bound_t::inf();
    }

    void DBM::past() {
        for (dim_t i = 1; i < this->_bounds_table._number_of_clocks; ++i) {
            this->_bounds_table.get(0, i) = bound_t::zero();
            for (dim_t j = 1; j < this->_bounds_table._number_of_clocks; ++j) {
                if (this->_bounds_table.at(j, i) < this->_bounds_table.at(0, i)) {
                    this->_bounds_table.get(0, i) = this->_bounds_table.at(j, i);
                }
            }
        }
    }

    void DBM::restrict(dim_t x, dim_t y, bound_t g) {
        if ((_bounds_table.at(y, x) + g) < bound_t::zero()) // In this case the zone is now empty
            _bounds_table.get(0, 0) = bound_t(-1, false);
        else if (g < _bounds_table.get(x, y)) {
            _bounds_table.get(x, y) = g;
            for (dim_t i = 0; i < _bounds_table._number_of_clocks; ++i) {
                for (dim_t j = 0; j < _bounds_table._number_of_clocks; ++j) {
                    if (_bounds_table.at(i, x) + _bounds_table.at(x, j) < _bounds_table.at(i, j))
                        _bounds_table.get(i, j) = _bounds_table.at(i, x) + _bounds_table.at(x, j);

                    if (_bounds_table.at(i, y) + _bounds_table.at(y, j) < _bounds_table.at(i, j))
                        _bounds_table.get(i, j) = _bounds_table.at(i, y) + _bounds_table.at(y, j);
                }
            }
        }

    }

    void DBM::free(dim_t x) {
        for (dim_t i = 0; i < _bounds_table._number_of_clocks; ++i) {
            if (i != x) {
                _bounds_table.get(x, i) = bound_t::inf();
                _bounds_table.get(i, x) = _bounds_table.get(i, 0);
            }
        }
    }

    // x := m
    void DBM::assign(dim_t x, val_t m) {
        for (dim_t i = 0; i < _bounds_table._number_of_clocks; ++i) {
            _bounds_table.get(x, i) = bound_t(m, false) + _bounds_table.get(0, i);
            _bounds_table.get(i, x) = bound_t(-m, false) + _bounds_table.get(i, 0);
        }
    }

    // x := y
    void DBM::copy(dim_t x, dim_t y) {
        for (dim_t i = 0; i < _bounds_table._number_of_clocks; ++i) {
            if (i != x) {
                _bounds_table.get(x, i) = _bounds_table.at(y, i);
                _bounds_table.get(i, x) = _bounds_table.at(i, y);
            }
        }
        _bounds_table.get(x, y) = bound_t::zero();
        _bounds_table.get(y, x) = bound_t::zero();
    }

    void DBM::shift(dim_t x, val_t n) {
        for (dim_t i = 0; i < this->_bounds_table._number_of_clocks; ++i) {
            if (i != x) {
                this->_bounds_table.get(x, i) = this->_bounds_table.at(x, i) + bound_t(n, false);
                this->_bounds_table.get(i, x) = this->_bounds_table.at(i, x) + bound_t(-n, false);
            }
        }
        this->_bounds_table.get(x, 0) = bound_t::max(this->_bounds_table.at(x, 0), bound_t::zero());
        this->_bounds_table.get(0, x) = bound_t::min(this->_bounds_table.at(0, x), bound_t::zero());
    }

    // Simple normalisation by a ceiling for all clocks.
    void DBM::norm(const std::vector<val_t> &ceiling) {
        if (this->_bounds_table._number_of_clocks != ceiling.size())
            return; //Todo: throw error or something

        for (dim_t i = 0; i < this->_bounds_table._number_of_clocks; ++i) {
            for (dim_t j = 0; j < this->_bounds_table._number_of_clocks; ++j) {
                if (!this->_bounds_table.at(i, j)._inf && this->_bounds_table.at(i, j) > bound_t(ceiling[i], false)){
                    this->_bounds_table.get(i, j) = bound_t::inf();
                }
                else if (!this->_bounds_table.at(i, j)._inf && this->_bounds_table.at(i, j) < bound_t(-ceiling[j], true)) {
                    this->_bounds_table.get(i, j) = bound_t(-ceiling[j], true);
                }
            }
        }

        close();
    }

    void DBM::diagonal_extrapolation(const std::vector<val_t> &ceiling) {
        DBM D(*this);

        for (dim_t i = 0; i < D._bounds_table._number_of_clocks; ++i) {
            for (dim_t j = 0; j < D._bounds_table._number_of_clocks; ++j) {
                if (i == j) continue;
                if ((D._bounds_table.at(i, j)._n > ceiling[i]) ||
                    (-D._bounds_table.at(0, i)._n > ceiling[i]) ||
                    (-D._bounds_table.at(0, j)._n > ceiling[j] && i != 0)){

                    this->_bounds_table.get(i, j) = bound_t::inf();
                }
                else if (-D._bounds_table.at(i, j)._n > ceiling[j] && i == 0)
                    this->_bounds_table.get(i, j) = bound_t(-ceiling[j], true);

                // Make sure we don't set 0, j to positive bound or i, 0 to a negative one
                //TODO: We only do this because regular close() does not catch these.
                // We should propably use a smarter close()
                if (i == 0 && this->_bounds_table.at(i, j) > bound_t::zero()) {
                    this->_bounds_table.get(i, j) = bound_t::zero();
                }
                if (j == 0 && this->_bounds_table.at(i, j) < bound_t::zero()) {
                    this->_bounds_table.get(i, j) = bound_t::zero();
                }

            }
        }

        //TODO: Do something smart where we only close if something changes
        this->close();
    }

    std::ostream& operator<<(std::ostream& out, const DBM& D) {
        return out << D._bounds_table;
    }

    std::vector<dim_t> DBM::resize(const std::vector<bool>& src_bits, const std::vector<bool>& dst_bits) {
        /* assume number of '1' bits in src_bits and dst_bits match, and
         * that the length of src_bits is the same as _number_of_clocks
         */
        assert(src_bits.size() == this->_bounds_table._number_of_clocks);
        assert(std::count_if(dst_bits.begin(), dst_bits.end(), [](bool b){return b;}) ==
               std::count_if(src_bits.begin(), src_bits.end(), [](bool b){return b;}));

        DBM dest_dbm(dst_bits.size());

        std::vector<dim_t> src_indir(src_bits.size(), 0);
        dim_t dst_cnt = 0;

        for (dim_t i = 0; i < src_bits.size(); ++i) {
            if (src_bits[i]) {
                while (not dst_bits[dst_cnt]) ++dst_cnt; // increment to first used position
                src_indir[i] = dst_cnt++;
            }
            else
                src_indir[i] = -1;
        }

        // dest(src_indir[i], src_indir[j] = src(i, j);
        for (dim_t i = 0; i < src_indir.size(); ++i) {
            for (dim_t j = 0; j < src_indir.size(); ++j) {
                if (src_indir[i] != -1 && src_indir[j] != -1)
                    dest_dbm._bounds_table.get(src_indir[i], src_indir[j]) = this->_bounds_table.at(i, j);
            }
        }

        // Free new clocks
        for (dim_t i = 0; i < dst_bits.size(); ++i)
            if (not dst_bits[i])
                dest_dbm.free(i);

        *this = dest_dbm;

        return src_indir;
    }

    void DBM::reorder(std::vector<dim_t> order, dim_t new_size) {
        DBM D(new_size);

        for (dim_t i = 0; i < this->_bounds_table._number_of_clocks; ++i) {
            for (dim_t j = 0; j < this->_bounds_table._number_of_clocks; ++j) {
                if (order[i] != ~0 && order[j] != ~0)
                    D._bounds_table.get(order[i], order[j]) = this->_bounds_table.at(i, j);
            }
        }

        *this = D;
    }
}
