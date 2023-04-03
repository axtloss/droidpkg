/* main.c
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
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <json.h>
#include "main.h"
#include "repo.h"

int
main(int argc, char *argv[]) {
    int opt;
    int option_index = 0;
    package_key_T *package_key_start;
    static struct option long_options[] = {
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {"search", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    while((opt = getopt_long(argc, argv, "vh:s:", long_options, &option_index)) != -1)
    {
        switch(opt)
        {
            case 'v':
                printf("version 1.0");
                break;
            case 'h':
                printf("help");
                break;
            case 's':
                printf("searching for %s\n", optarg);
                package_key_start = (package_key_T*) malloc(sizeof(package_key_T));
                if (package_key_start == NULL) {
                    printf("malloc failed");
                    exit(1);
                }
                package_key_start->key = NULL;
                package_key_start->next = NULL;
                find_package_id(optarg, package_key_start);
                package_key_T *temp = package_key_start;
                package_key_T *prev = NULL;
                while (temp->next != NULL || temp->key != NULL) {
                    printf("%s\n", temp->key);
                    prev = temp;
                    temp = temp->next;
                    free(prev);
                }
                free(package_key_start);
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
            default:
                printf("help");
                break;
        }
    }
    return 0;
}
