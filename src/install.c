/* install.c
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
#include <curl/curl.h>
#include <string.h>
#include "repo.h"
#include "install.h"

void
fetch_package(char *url)
{
    CURL           *curl;
    FILE           *fp;
    CURLcode        res;
    curl = curl_easy_init ();
    if (curl)
    {
        fp = fopen ("test.apk", "wb");
        curl_easy_setopt (curl, CURLOPT_URL, url);
        curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt (curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform (curl);
        if (res != CURLE_OK)
            fprintf (stderr, "curl_easy_perform() failed: %s",
                     curl_easy_strerror (res));

        curl_easy_cleanup (curl);
        fclose (fp);
    }
}


void
install_package(char *name)
{
    json_object    *releases;
    json_object    *package = get_package (name);
    json_object    *file;
    json_object    *filename;
    char           *url = malloc (strlen ("https://apt.izzysoft.de/fdroid/repo/") + 1);
    if (package == NULL)
    {
        printf ("Package not found");
        return;
    }
    json_object_object_get_ex (package, "versions", &releases);
    json_object_object_foreach (releases, key, val)
    {
        json_object_object_get_ex (val, "file", &file);
    }
    printf ("%s\n", json_object_get_string (file));
    json_object_object_get_ex (file, "name", &filename);
    strcpy (url, "https://apt.izzysoft.de/fdroid/repo/");
    url = (char *)realloc (url, sizeof (url) + strlen (json_object_get_string (filename)) + 1);
    if (url == NULL)
    {
        printf ("Error (re)allocating memory");
        return;
    }
    strcat (url, (const char *)json_object_get_string (filename));
    printf ("fetching: %s\n", url);
    fetch_package (url);
    /* free(url); gives an error "invalid next size (fast)" */
}
