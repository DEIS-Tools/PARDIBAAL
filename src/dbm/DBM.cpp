//
// Created by Thomas Grosen on 13/10/2021.
//

#include "DBM.h"
#include "bounds_table_t.h"
#include <iostream>

namespace dbm {
    DBM::DBM(dim_t size) : _bounds_table(size) {}

    DBM DBM::gen_random(int size, int max) {
        DBM d(size);

        do {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (i != j)
                        d._bounds_table.get(i, j) = bound_t::gen_random(max);
                }
            }
            d.close();
        } while (!d.is_empty());

        return d;
    }

    bool DBM::is_empty() const {
        // The DBM has to be closed for this to actually work
        for (int i = 0; i < this->_bounds_table._number_of_clocks; i++) {
            for (int j = 0; j < this->_bounds_table._number_of_clocks; j++) {
                bound_t i_to_j_to_i = bound_t::add(this->_bounds_table.at(i, j), this->_bounds_table.at(j, i));

                if (bound_t::less(i_to_j_to_i, bound_t(0, false)))
                    return true;
            }
        }

        return false;
    }

    //TODO
    bool DBM::relation(const DBM &d) const {
        return false;
    }

    //TODO
    bool DBM::satisfy(const bound_t &g) const {
        return false;
    }

    void DBM::close() {
        const int size = _bounds_table._number_of_clocks;
        for(int k = 0; k < size; k++) {
            for(int i = 0; i < size; i++) {
                for(int j = 0; j < size; j++) {
                    _bounds_table.get(i, j) = bound_t::min(_bounds_table.get(i, j),
                                                           bound_t::add(_bounds_table.get(i, k),
                                                                        _bounds_table.get(k, j)));
                }
            }
        }
    }

    //TODO
    void DBM::delay() {

    }

    //TODO
    void DBM::restrict(const bound_t &g) {

    }

    //TODO
    void DBM::free(const int &clock) {

    }

    //TODO
    void DBM::reset(const int &clock, const int &k) {

    }

    //TODO
    void DBM::copy(const int &i, const int &j) {

    }


}
