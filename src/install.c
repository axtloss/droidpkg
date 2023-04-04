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
#include <openssl/evp.h>
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

char
*verify_package(char *file, int size) {
    FILE                 *fp;
    unsigned char        *buffer = malloc(size);
    unsigned char        *hash= malloc(EVP_MAX_MD_SIZE + 1);
    unsigned int          hash_len;
    char                 *hash_str = malloc (EVP_MAX_MD_SIZE * 2 + 1);
    size_t                bytes_read;
    EVP_MD_CTX           *mdctx = EVP_MD_CTX_new ();
    const EVP_MD         *md = EVP_sha256 ();

    EVP_DigestInit_ex (mdctx, md, NULL);
    fp = fopen (file, "rb");
    if (fp == NULL) {
        printf ("File not found");
        return false;
    }
    while ((bytes_read = fread (buffer, 1, size, fp)) != 0)
        EVP_DigestUpdate (mdctx, buffer, bytes_read);
    EVP_DigestFinal_ex (mdctx, hash, &hash_len);

    for (int i = 0; i < hash_len; i++)
        sprintf (&hash_str[i*2], "%02x", hash[i]);

    printf ("Hash: %s\n", hash_str);
    fclose (fp);
    EVP_MD_CTX_free (mdctx);
    return hash_str;
}

void
install_package(char *name)
{
    json_object    *releases;
    json_object    *package = get_package (name);
    json_object    *file = NULL;
    json_object    *filename;
    char           *url = malloc (strlen ("https://apt.izzysoft.de/fdroid/repo/") + 1);
    json_object    *checksum;
    json_object    *size;
    char           *calculated_checksum;
    char           *expected_checksum;
    if (package == NULL)
    {
        printf ("Package not found\n");
        return;
    }
    json_object_object_get_ex (package, "versions", &releases);
    json_object_object_foreach (releases, key, val)
    {
        json_object_object_get_ex (val, "file", &file);
        break;
    }
    json_object_get_string (file); /* Crashes without this :shipit: */
    json_object_object_get_ex (file, "name", &filename);
    strcpy (url, "https://apt.izzysoft.de/fdroid/repo/");
    url = (char *)realloc (url, sizeof (url) + strlen (json_object_get_string (filename)) + 1);
    if (url == NULL)
    {
        printf ("Error (re)allocating memory\n");
        return;
    }
    strcat (url, (const char *)json_object_get_string (filename));
    json_object_object_get_ex (file, "sha256", &checksum);
    json_object_object_get_ex (file, "size", &size);
    printf ("fetching: %s\n", url);
    fetch_package (url);
    expected_checksum = (char *)json_object_get_string (checksum);
    calculated_checksum = verify_package ("test.apk", json_object_get_int (size));
    if (strcmp (expected_checksum, calculated_checksum) == 0) {
        printf ("File OK\n");
    } else {
        printf ("Calculated checksum does NOT match expected checksum!\n");
        printf ("Expected: %s\n", expected_checksum);
        printf ("Calculated: %s\n", calculated_checksum);
    }
}
