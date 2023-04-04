/* main.c
 *
 * Copyright 2023 axtlos <axtlos@getcryst.al
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
#include "main.h"
#include "repo.h"
#include "install.h"

int
main(int argc, char *argv[])
{
    int             opt;
    int             option_index = 0;
    package_key_T  *package_key_start;
    package_key_T  *temp;
    package_key_T  *prev;
    char           *search_key = NULL;
    const char     *result = NULL;

    /* Declare available options */
    static struct option long_options[] = {
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {"ID", required_argument, 0, 'i'},
        {"name", required_argument, 0, 'n'},
        {"search", required_argument, 0, 's'},
        {"key", required_argument, 0, 'k'},
        {"install", required_argument, 0, 'I'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long (argc, argv, "vh:k:i:n:s:I:", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
            case 'v':
                printf ("version 1.0");
                break;
            case 'h':
                printf ("help");
                break;
            case 'k':
                search_key = optarg;
                break;
            case 'i':
                printf ("searching for %s\n", optarg);
                package_key_start = (package_key_T *) malloc (sizeof (package_key_T));
                if (package_key_start == NULL)
                {
                    printf ("malloc failed");
                    exit (1);
                }
                package_key_start->key = NULL;  /* Set to NULL to avoid any
                                                errors when the search fails */
                package_key_start->next = NULL;
                find_package_id (optarg, package_key_start);
                temp = package_key_start;
                prev = NULL;
                if (temp->key == NULL)
                {
                    printf ("No results found\n");
                    free(package_key_start);
                    break;
                }
                while (temp->next != NULL) /* Loop through the linked list and print the results */
                {
                    printf ("%s\n", temp->key);
                    prev = temp;
                    temp = temp->next;
                    free (prev);
                }
                free (package_key_start);
                break;
            case 'n':
                printf ("searching for %s\n", optarg);
                printf ("%s\n", find_package_name (optarg));
                break;
            case 's':
                if (search_key == NULL)
                {
                    printf ("Please specify a key to search for with -k\n");
                    exit (1);
                }
                printf ("searching for %s: %s\n", optarg, search_key);
                result = get_value (optarg, search_key);
                if (result == NULL)
                {
                    printf ("No results found");
                }
                else
                {
                    printf ("%s", result);
                }
                break;
            case 'I':
                printf ("installing %s\n", optarg);
                install_package (optarg);
                break;
            case '?':
                printf ("unknown option: %c\n", optopt);
                break;
            default:
                printf ("help");
                break;
        }
    }
    return 0;
}
