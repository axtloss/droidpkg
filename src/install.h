/* install.h
 *
 * Copyright 2023 axtlos <axtlos@getcryst.al>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License only
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */
#ifndef DROIDPKG_INSTALL_H
#define DROIDPKG_INSTALL_H

#endif  /* //DROIDPKG_INSTALL_H */

void            fetch_package (char *url);

char            *check_package (char *name, int size);

void            install_package (char *name);
