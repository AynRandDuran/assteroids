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
// Some vector math that Raylib didn't give me
//

float v2_magnitude(Vector2 v) {
	return sqrt((v.x*v.x)+(v.y*v.y));
}

Vector2 v2_normal(Vector2 v) {
	float len = v2_magnitude(v);
	Vector2 normal = {v.x/len, v.y/len};
	return normal;
}
