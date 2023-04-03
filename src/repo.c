/* repo.c
 *
 * Copyright 2023 Unknown
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
#include "repo.h"

json_object
*parse_repo() {
    FILE *fp;
    char *buffer = malloc(5145416);
    struct json_object *parsed_json;
    fp = fopen("index-v2.json", "r");
    fread( buffer, 5145416, 1, fp);
    fclose(fp);
    parsed_json = json_tokener_parse(buffer);
    return parsed_json;
}

const char
*find_package_name(char *name) {
    json_object *parsed_json = parse_repo();
    json_object *packages, *package, *metadata, *pkgnames, *pkgname;
    json_object_object_get_ex(parsed_json, "packages", &packages);
    json_object_object_get_ex(packages, name, &package);
    json_object_object_get_ex(package, "metadata", &metadata);
    json_object_object_get_ex(metadata, "name", &pkgnames);
    json_object_object_get_ex(pkgnames, "en-US", &pkgname);
    return json_object_get_string(pkgname);
}

void
find_package_id(char *name, package_key_T *matches) {
    json_object *parsed_json = parse_repo();
    json_object *packages;
    json_object_object_get_ex(parsed_json, "packages", &packages);
    int i = 0;
    package_key_T *temp = matches;
    package_key_T *prev = NULL;
    json_object_object_foreach(packages, key, value) {
        json_object *metadata, *pkgnames, *pkgname;
        json_object_object_get_ex(value, "metadata", &metadata);
        json_object_object_get_ex(metadata, "name", &pkgnames);
        json_object_object_get_ex(pkgnames, "en-US", &pkgname);
        if (json_object_is_type(pkgname, json_type_string)) {
            if (strstr(json_object_get_string(pkgname), name) != NULL) {
                temp->key=(char*) malloc(strlen(key));
                strcpy(temp->key, key);
                temp->next = (package_key_T*) malloc(sizeof(package_key_T));
                if (temp->next == NULL) {
                    printf("Error allocating memory for package_key_T");
                    exit(1);
                }
                //printf("%s\n", temp->key);
                prev = temp;
                temp = temp->next;
                i++;
            }
        }
    }
    if (prev != NULL) {
        prev->next = NULL;
        free(temp);
    }
    json_object_put(parsed_json);
}


int
repo() {
    printf("repo");
    return 0;
}