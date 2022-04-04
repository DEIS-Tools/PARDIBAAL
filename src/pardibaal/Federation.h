/*
 * Copyright Thomas Møller Grosen
 * Created on 01/04/2022
 */

/*
 * This file is part of pardibaal
 *
 * pardibaal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pardibaal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with pardibaal. If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef PARDIBAAL_FEDERATION_H
#define PARDIBAAL_FEDERATION_H

#include "DBM.h"
namespace pardibaal {

    class Federation {
        std::vector<DBM> zones;

        void compute_on_zones(void (*comp) (DBM&));

    public:
        Federation(dim_t dimension){zones = std::vector<DBM>{DBM(dimension)};}

        auto begin() const;
        auto end() const;

        [[nodiscard]] const DBM& at(dim_t index) const;
        void add(DBM dbm);
        void remove(dim_t index);

        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] bool satisfies(dim_t x, dim_t y, bound_t g) const;
        [[nodiscard]] relation_t relation(const DBM& dbm) const;
        [[nodiscard]] relation_t relation(const Federation& fed) const;

        [[nodiscard]] bool equal(const DBM& dbm) const;
        [[nodiscard]] bool equal(const Federation& fed) const;
        [[nodiscard]] bool subset(const DBM& dbm) const;
        [[nodiscard]] bool subset(const Federation& fed) const;
        [[nodiscard]] bool superset(const DBM& dbm) const;
        [[nodiscard]] bool superset(const Federation& fed) const;

        /** Is unbounded
         * @return true if one of the zones in the federation is unbounded
         */
        [[nodiscard]] bool is_unbounded() const;

        void future();
        void past();
        void restrict(dim_t x, dim_t y, bound_t g);
        void free(dim_t x);
        void assign(dim_t x, val_t m);
        void copy(dim_t x, dim_t y);
        void shift(dim_t x, val_t n);

        void extrapolate(const std::vector<val_t> &ceiling);
        void extrapolate_diagonal(const std::vector<val_t> &ceiling);
        void extrapolate_lu(const std::vector<val_t> &lower, const std::vector<val_t> &upper);
        void extrapolate_lu_diagonal(const std::vector<val_t> &lower, const std::vector<val_t> &upper);

        void remove_clock(dim_t c);
        void swap_clocks(dim_t a, dim_t b);
        void add_clock_at(dim_t c);

        std::vector<dim_t> resize(const std::vector<bool>& src_bits, const std::vector<bool>& dst_bits);
        void reorder(std::vector<dim_t> order, dim_t new_size);

        friend std::ostream& operator<<(std::ostream& out, const Federation& fed);
    };

    std::ostream& operator<<(std::ostream& out, const Federation& fed);
}

#endif //PARDIBAAL_FEDERATION_H
