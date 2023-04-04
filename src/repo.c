/* repo.c
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

#include <stdio.h>
#include <json.h>
#include <string.h>
#include <sys/stat.h>
#include "repo.h"

/* Parse the repository json */
json_object
* parse_repo (void)
{
    struct stat     st = {0};
    stat ("index-v2.json", &st);
    off_t           size = st.st_size;
    FILE           *fp;
    char           *buffer = malloc (size);
    struct json_object *parsed_json;
    fp = fopen ("index-v2.json", "r");
    fread (buffer, 5145416, 1, fp);
    fclose (fp);
    parsed_json = json_tokener_parse (buffer);
    return parsed_json;
}

/* Find the name of a package by its ID */
const char
* find_package_name(char *name)
{
    json_object    *parsed_json = parse_repo ();
    json_object    *packages, *package, *metadata, *pkgnames, *pkgname;
    json_object_object_get_ex (parsed_json, "packages", &packages);
    json_object_object_get_ex (packages, name, &package);
    json_object_object_get_ex (package, "metadata", &metadata);
    json_object_object_get_ex (metadata, "name", &pkgnames);
    json_object_object_get_ex (pkgnames, "en-US", &pkgname);
    return json_object_get_string (pkgname);
}

/* Find IDs matching a package name and store them in a linked list */
void
find_package_id(char *name, package_key_T * matches)
{
    json_object    *parsed_json = parse_repo ();
    json_object    *packages;
    json_object_object_get_ex (parsed_json, "packages", &packages);
    package_key_T  *temp = matches;
    package_key_T  *prev = NULL;
    json_object_object_foreach (packages, key, value)
    {
        json_object    *metadata, *pkgnames, *pkgname;
        json_object_object_get_ex (value, "metadata", &metadata);
        json_object_object_get_ex (metadata, "name", &pkgnames);
        json_object_object_get_ex (pkgnames, "en-US", &pkgname);
        if (json_object_is_type (pkgname, json_type_string))
        {
            if (strstr (json_object_get_string (pkgname), name) != NULL && strlen(key) != 0)
            {
                temp->key = (char *)malloc (strlen (key)+1);
                if (temp->key == NULL)
                {
                    printf ("Error allocating memory for package_key_T");
                    exit (1);
                }
                strcpy (temp->key, key);
                temp->next = (package_key_T *) malloc (sizeof (package_key_T)+1);
                if (temp->next == NULL)
                {
                    printf ("Error allocating memory for package_key_T");
                    exit (1);
                }
                prev = temp;
                temp = temp->next;
            }
        }
    }
    if (prev != NULL)
    {
        prev->next = NULL;
        free (temp);
    }
    json_object_put (parsed_json);
}

/* Get a value of a given key */
const char
* get_value(char *name, char *key)
{
    const char     *result = NULL;
    json_object    *parsed_json = parse_repo ();
    json_object    *packages, *package, *metadata, *value;
    json_object_object_get_ex (parsed_json, "packages", &packages);
    json_object_object_get_ex (packages, name, &package);
    json_object_object_get_ex (package, "metadata", &metadata);
    json_object_object_get_ex (metadata, key, &value);
    result = json_object_get_string (value);
    return result;
}

/* Get the entire object of a given package */
json_object
* get_package (char *name)
{
    json_object    *parsed_json = parse_repo ();
    json_object    *packages, *package;
    json_object_object_get_ex (parsed_json, "packages", &packages);
    json_object_object_get_ex (packages, name, &package);
    return package;
}
