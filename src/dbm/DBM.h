/*
 * Copyright Thomas Møller Grosen
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

#ifndef BDM_DBM_H
#define BDM_DBM_H

#include "bounds_table_t.h"

namespace dbm {
    class DBM {
    public:
        bounds_table_t _bounds_table;

        DBM(dim_t number_of_clocks);

        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] bool is_included_in(const DBM &d) const;
        [[nodiscard]] bool is_satisfied(dim_t x, dim_t y, bound_t g) const;

        void close();

        void future();
        void past();
        void restrict(dim_t x, dim_t y, bound_t g);
        void free(dim_t x);
        void assign(dim_t x, val_t m);
        void copy(dim_t x, dim_t y);
        void shift(dim_t x, val_t n);
        void norm(const std::vector<val_t> &ceiling);

        friend std::ostream& operator<<(std::ostream& out, const DBM& D);
    };

    std::ostream& operator<<(std::ostream& out, const DBM& D);

}

#endif //BDM_DBM_H
