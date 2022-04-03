/*
 * Randy's Rowdy Rocket Roundup

 * Copyright (C) 2022 Patrick Letts

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

int enable_shotgun() {
    shotgun_blasts = 6;
    active_powerups |= 1;
    return 1;
}

int disable_shotgun() {
    active_powerups &= 0;
    shotgun_box.w = 0;
    shotgun_blasts = 0;
    return 0;
}
