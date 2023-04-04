/* repo.h
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

#include <stdbool.h>
#include <json.h>

#ifndef DROIDPKG_REPO_H
#define DROIDPKG_REPO_H

#endif  /* DROIDPKG_REPO_H */

typedef struct package_key
{
    char           *key;
    struct package_key *next;
}               package_key_T;

json_object    *parse_repo (void);

const char     *find_package_name (char *name);

void            find_package_id (char *name, package_key_T * matches);

const char     *get_value (char *name, char *key);


json_object    *get_package (char *name);