/*
 * Copyright Thomas M. Grosen
 * Created on 13/10/2021.
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

#include "DBM.h"
#include "errors.h"

namespace pardibaal {

    relation_t relation_t::equal() {return relation_t(true, true, true, false);}
    relation_t relation_t::subset() {return relation_t(false, true, false, false);}
    relation_t relation_t::superset() {return relation_t(false, false, true, false);}
    relation_t relation_t::different() {return relation_t(false, false, false, true);}

    DBM::DBM(dim_t number_of_clocks) : _bounds_table(number_of_clocks) {}

    bound_t DBM::at(dim_t i, dim_t j) const {return this->_bounds_table.at(i, j);}
    void DBM::set(dim_t i, dim_t j, bound_t bound) {this->_bounds_table.set(i, j, bound);}
    dim_t DBM::dimension() const {return this->_bounds_table.number_of_clocks();}

    bool DBM::is_empty() const {
        // Check if 0 - 0 is less than 0 (used for quicker identification of empty zone
        if (_bounds_table.at(0, 0) < bound_t().zero())
            return true;

        // The DBM has to be closed for this to actually work
        for (dim_t i = 0; i < this->dimension(); ++i) {
            for (dim_t j = 0; j < this->dimension(); ++j) {
                bound_t i_to_j_to_i = this->_bounds_table.at(i, j) + this->_bounds_table.at(j, i);

                if (i_to_j_to_i < bound_t::non_strict(0))
                    return true;
            }
        }

        return false;
    }

    bool DBM::is_satisfied(dim_t x, dim_t y, bound_t g) const {
        return bound_t::zero() <= (this->_bounds_table.at(y, x) + g);
    }

    relation_t DBM::relation(const DBM &dbm) const {
        if (this->dimension() != dbm.dimension())
            return relation_t::different();

        bool eq, sub = true, super = true;

        for (dim_t i = 0; i < dimension(); ++i)
            for (dim_t j = 0; j < dimension(); ++j) {
                sub &= this->at(i, j) <= dbm.at(i, j);
                super &= this->at(i, j) >= dbm.at(i, j);
                if (!sub && !super) return relation_t::different();
            }

        eq = sub && super;

        if (eq) return relation_t::equal();
        if (sub) return relation_t::subset();
        if (super) return relation_t::superset();

        return relation_t::different();
    }

    bool DBM::equal(const DBM& dbm) const {return this->relation(dbm)._equal;}
    bool DBM::subset(const DBM& dbm) const {return this->relation(dbm)._subset;}
    bool DBM::superset(const DBM& dbm) const {return this->relation(dbm)._superset;}

    bool DBM::is_unbounded() const {
        for (dim_t i = 1; i < dimension(); ++i)
            if (not this->at(i, 0).is_inf())
                return false;

        return true;
    }

    void DBM::close() {
        const dim_t size = this->dimension();

        for(dim_t k = 0; k < size; ++k)
            for(dim_t i = 0; i < size; ++i)
                for(dim_t j = 0; j < size; ++j)
                    _bounds_table.set(i, j, bound_t::min(_bounds_table.at(i, j),
                                                         _bounds_table.at(i, k) + _bounds_table.at(k, j)));
    }

    void DBM::future() {
        for (dim_t i = 1; i < this->dimension(); ++i)
            _bounds_table.set(i, 0, bound_t::inf());
    }

    void DBM::past() {
        for (dim_t i = 1; i < this->dimension(); ++i) {
            this->_bounds_table.set(0, i, bound_t::zero());
            for (dim_t j = 1; j < this->dimension(); ++j) {
                if (this->_bounds_table.at(j, i) < this->_bounds_table.at(0, i)) {
                    this->_bounds_table.set(0, i, this->_bounds_table.at(j, i));
                }
            }
        }
    }

    void DBM::restrict(dim_t x, dim_t y, bound_t g) {
        if ((_bounds_table.at(y, x) + g) < bound_t::zero()) // In this case the zone is now empty
            _bounds_table.set(0, 0, bound_t::non_strict(-1));
        else if (g < _bounds_table.at(x, y)) {
            _bounds_table.set(x, y, g);
            for (dim_t i = 0; i < this->dimension(); ++i) {
                for (dim_t j = 0; j < this->dimension(); ++j) {
                    if (_bounds_table.at(i, x) + _bounds_table.at(x, j) < _bounds_table.at(i, j))
                        _bounds_table.set(i, j, _bounds_table.at(i, x) + _bounds_table.at(x, j));

                    if (_bounds_table.at(i, y) + _bounds_table.at(y, j) < _bounds_table.at(i, j))
                        _bounds_table.set(i, j, _bounds_table.at(i, y) + _bounds_table.at(y, j));
                }
            }
        }

    }

    void DBM::free(dim_t x) {
        for (dim_t i = 0; i < dimension(); ++i) {
            if (i != x) {
                _bounds_table.set(x, i, bound_t::inf());
                _bounds_table.set(i, x, _bounds_table.at(i, 0));
            }
        }
    }

    // x := m
    void DBM::assign(dim_t x, val_t m) {
        for (dim_t i = 0; i < this->dimension(); ++i) {
            _bounds_table.set(x, i, bound_t::non_strict(m) + _bounds_table.at(0, i));
            _bounds_table.set(i, x, bound_t::non_strict(-m) + _bounds_table.at(i, 0));
        }
    }

    // x := y
    void DBM::copy(dim_t x, dim_t y) {
        for (dim_t i = 0; i < this->dimension(); ++i) {
            if (i != x) {
                _bounds_table.set(x, i, _bounds_table.at(y, i));
                _bounds_table.set(i, x, _bounds_table.at(i, y));
            }
        }
        _bounds_table.set(x, y, bound_t::zero());
        _bounds_table.set(y, x, bound_t::zero());
    }

    void DBM::shift(dim_t x, val_t n) {
        for (dim_t i = 0; i < this->dimension(); ++i) {
            if (i != x) {
                this->_bounds_table.set(x, i, this->_bounds_table.at(x, i) + bound_t::non_strict(n));
                this->_bounds_table.set(i, x, this->_bounds_table.at(i, x) + bound_t::non_strict(-n));
            }
        }
        this->_bounds_table.set(x, 0, bound_t::max(this->_bounds_table.at(x, 0), bound_t::zero()));
        this->_bounds_table.set(0, x, bound_t::min(this->_bounds_table.at(0, x), bound_t::zero()));
    }

    // Simple extrapolation from a ceiling for all clocks.
    void DBM::extrapolate(const std::vector<val_t> &ceiling) {
#ifndef NEXCEPTIONS
        if (this->dimension() != ceiling.size())
            throw base_error("ERROR: Got max constants vector of size ", ceiling.size(), " but the DBM has ",
                             this->dimension(), " clocks");
#endif

        for (dim_t i = 0; i < this->dimension(); ++i) {
            for (dim_t j = 0; j < this->dimension(); ++j) {
                if (!this->_bounds_table.at(i, j).is_inf() && this->_bounds_table.at(i, j) > bound_t::non_strict(ceiling[i])){
                    this->_bounds_table.set(i, j, bound_t::inf());
                }
                else if (!this->_bounds_table.at(i, j).is_inf() && this->_bounds_table.at(i, j) < bound_t::strict(-ceiling[j])) {
                    this->_bounds_table.set(i, j, bound_t::strict(-ceiling[j]));
                }
            }
        }

        close();
    }

    void DBM::extrapolate_diagonal(const std::vector<val_t> &ceiling) {
#ifndef NEXCEPTIONS
        if (this->dimension() != ceiling.size())
            throw base_error("ERROR: Got max constants vector of size ", ceiling.size(), " but the DBM has ",
                             this->dimension(), " clocks");
#endif
        DBM D(*this);

        for (dim_t i = 0; i < D.dimension(); ++i) {
            for (dim_t j = 0; j < D.dimension(); ++j) {
                if (i == j) continue;
                if ((D.at(i, j) > bound_t::non_strict(ceiling[i])) ||
                    (D.at(0, i) < bound_t::non_strict(-ceiling[i])) ||
                    (D.at(0, j) < bound_t::non_strict(-ceiling[j]) && i != 0)){

                    this->set(i, j, bound_t::inf());
                }
                else if (D.at(i, j) < bound_t::non_strict(-ceiling[j]) && i == 0)
                    this->at(i, j) = bound_t::strict(-ceiling[j]);

                // Make sure we don't set 0, j to positive bound or i, 0 to a negative one
                //TODO: We only do this because regular close() does not catch these.
                // We should propably use a smarter close()
                if (i == 0 && this->at(i, j) > bound_t::zero()) {
                    this->set(i, j, bound_t::zero());
                }
                if (j == 0 && this->at(i, j) < bound_t::zero()) {
                    this->set(i, j, bound_t::zero());
                }

            }
        }

        //TODO: Do something smart where we only close if something changes
        this->close();
    }

    void DBM::extrapolate_lu(const std::vector<val_t> &lower, const std::vector<val_t> &upper) {
#ifndef NEXCEPTIONS
        if (this->dimension() != lower.size() || this->dimension() != upper.size())
            throw base_error("ERROR: Got LU constants vector of size ", lower.size(), " and ", upper.size(),
                             " but the DBM has ", this->dimension(), " clocks");
#endif
        DBM D(*this);

        for (dim_t i = 0; i < D.dimension(); ++i) {
            for (dim_t j = 0; j < D.dimension(); ++j) {
                if (i == j) continue;
                else if (D.at(i, j) > bound_t::non_strict(lower[i]))
                    this->at(i, j) = bound_t::inf();
                else if (D.at(i, j) < bound_t::non_strict(-upper[j]))
                    this->at(i, j) = bound_t::strict(-upper[j]);

                // Make sure we don't set 0, j to positive bound or i, 0 to a negative one
                //TODO: We only do this because regular close() does not catch these.
                // We should propably use a smarter close()
                if (i == 0 && this->at(i, j) > bound_t::zero())
                    this->set(i, j, bound_t::zero());
                if (j == 0 && this->at(i, j) < bound_t::zero())
                    this->set(i, j, bound_t::zero());
            }
        }

        //TODO: Do something smart where we only close if something changes
        this->close();
    }

    void DBM::extrapolate_lu_diagonal(const std::vector<val_t> &lower, const std::vector<val_t> &upper) {
#ifndef NEXCEPTIONS
        if (this->dimension() != lower.size() || this->dimension() != upper.size())
            throw base_error("ERROR: Got LU constants vector of size ", lower.size(), " and ", upper.size(),
                             " but the DBM has ", this->dimension(), " clocks");
#endif
        DBM D(*this);

        for (dim_t i = 0; i < D.dimension(); ++i) {
            for (dim_t j = 0; j < D.dimension(); ++j) {
                if (i == j) continue;
                else if (D.at(i, j) > bound_t::non_strict(lower[i]) ||
                         D.at(0, i) < bound_t::non_strict(-lower[i]) ||
                         (D.at(0, j) < bound_t::non_strict(-upper[j]) && i != 0))
                    this->at(i, j) = bound_t::inf();
                else if (D.at(0, j) < bound_t::non_strict(-upper[j]) && i == 0)
                    this->at(i, j) = bound_t::strict(-upper[j]);

                // Make sure we don't set 0, j to positive bound or i, 0 to a negative one
                //TODO: We only do this because regular close() does not catch these.
                // We should propably use a smarter close()
                if (i == 0 && this->at(i, j) > bound_t::zero())
                    this->set(i, j, bound_t::zero());
                if (j == 0 && this->at(i, j) < bound_t::zero())
                    this->set(i, j, bound_t::zero());
            }
        }

        //TODO: Do something smart where we only close if something changes
        this->close();
    }

    void DBM::remove_clock(dim_t c) {
#ifndef NEXCEPTIONS
        if (c == 0)
            throw base_error("ERROR: Cannot remove the zero clock");
        if (c >= this->dimension())
            throw base_error("ERROR: Removing clock ", c, " but the DBM only has clocks from 0 to ", dimension() - 1);
#endif
        DBM D(dimension() - 1);

        for (dim_t i = 0, i2 = 0; i < dimension(); ++i, ++i2) {
            for (dim_t j = 0, j2 = 0; j < dimension(); ++j) {
                if (i == c && i < dimension() -1) {++i;}
                if (j == c || i == c) continue;

                D.set(i2, j2++, this->at(i, j));
            }
        }

        *this = std::move(D);
    }

    void DBM::swap_clocks(dim_t a, dim_t b) {
#ifndef NEXCEPTIONS
        if (a == 0 || b == 0)
            throw base_error("ERROR: Cannot swap the zero clock");
        if (a >= this->dimension() || b >= this->dimension())
            throw base_error("ERROR: Swapping clock ", a, " and ", b, " but the DBM only has clocks from 0 to ", dimension() - 1);
#endif
        if (a == b) return;

        bound_t tmp;
        for (dim_t i = 0; i < dimension(); ++i) {
            if (!(i == a || i == b)) {
                tmp = at(i, a);
                set(i, a, at(i, b));
                set(i, b, tmp);

                tmp = at(a, i);
                set(a, i, at(b, i));
                set(b, i, tmp);
            }
        }

        tmp = at(a, b);
        set(a, b, at(b, a));
        set(b, a, tmp);
    }

    void DBM::add_clock_at(dim_t c) {
#ifndef NEXCEPTIONS
        if (c == 0)
            throw base_error("ERROR: Cannot add a new clock at index zero");
        if (c > this->dimension())
            throw base_error("ERROR: Adding clock at index", c, " but the DBM only has clocks from 0 to ", dimension() - 1);
#endif

        DBM D(dimension() + 1);

        for (dim_t i = 0, i2 = 0; i < D.dimension(); ++i, ++i2) {
            for (dim_t j = 0, j2 = 0; j < D.dimension(); ++j) {
                if (i == c && i < D.dimension() - 1) {++i;}
                if (j == c || i == c) continue;
                D.set(i, j, this->at(i2, j2++));
            }
        }

        *this = std::move(D);
        free(c);
    }

    std::vector<dim_t> DBM::resize(const std::vector<bool>& src_bits, const std::vector<bool>& dst_bits) {
        /* assume number of '1' bits in src_bits and dst_bits match, and
         * that the length of src_bits is the same as _number_of_clocks
         */
#ifndef NEXCEPTIONS
        if (src_bits.size() != this->dimension())
            throw base_error("ERROR: src_bits has size: ", src_bits.size(), " but the dimension of the DBM is: ",
                             this->dimension(), " but they must be equal");

        int src = std::count_if(dst_bits.begin(), dst_bits.end(), [](bool b){return b;});
        int dst = std::count_if(src_bits.begin(), src_bits.end(), [](bool b){return b;});
        if (src != dst)
            throw base_error("ERROR: Mismatch in number of 1 bits/true values in src ", src, " and dst ", dst);
#endif

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
                    dest_dbm.set(src_indir[i], src_indir[j], this->_bounds_table.at(i, j));
            }
        }

        // Free new clocks
        for (dim_t i = 0; i < dst_bits.size(); ++i)
            if (not dst_bits[i])
                dest_dbm.free(i);

        *this = std::move(dest_dbm);

        return src_indir;
    }

    void DBM::reorder(std::vector<dim_t> order, dim_t new_size) {
#ifndef NEXCEPTIONS
        if (order.size() != this->dimension())
            throw base_error("ERROR: Order vector has size: ", order.size(), " but the dimension of the DBM is: ",
                             this->dimension(), " They must be equal");

        int clocks_removed = std::count_if(order.begin(), order.end(), [](dim_t i){return i == ~0;});
        if (this->dimension() - clocks_removed != new_size)
            throw base_error("ERROR: new_size does not match the number of clocks removed. new_size is: ", new_size,
                             " current size: ", this->dimension(), " Clocks removed: ", clocks_removed);
        for (const dim_t& i : order)
            if (i >= new_size && i != (dim_t) -1)
                throw base_error("ERROR: order has value ", order[i], " on index ", i,
                                 " which is outside of the new dimension of ", new_size);
#endif

        DBM D(new_size);

        for (dim_t i = 0; i < this->dimension(); ++i) {
            for (dim_t j = 0; j < this->dimension(); ++j) {
                if (order[i] != ~0 && order[j] != ~0)
                    D.set(order[i], order[j], this->_bounds_table.at(i, j));
            }
        }

        *this = std::move(D);
    }

    std::ostream& operator<<(std::ostream& out, const DBM& D) {
        return out << D._bounds_table;
    }
}
