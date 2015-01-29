/*
	This file is part of CoDExtended.

    CoDExtended is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CoDExtended is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CoDExtended.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "shared.h"

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

static size_t callback(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

static int progress(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded) {
    // how wide you want the progress meter to be
    int totaldotz=40;
    double fractiondownloaded = NowDownloaded / TotalToDownload;
    // part of the progressmeter that's already "full"
    int dotz = round(fractiondownloaded * totaldotz);

    // create the "meter"
    int ii=0;
    printf("%3.0f%% [",fractiondownloaded*100);
    // part  that's full already
    for ( ; ii < dotz;ii++) {
        printf("=");
    }
    // remaining part (spaces)
    for ( ; ii < totaldotz;ii++) {
        printf(" ");
    }
    // and back to line begin - do not forget the fflush to avoid output buffering problems!
    printf("]\r");
    fflush(stdout);
    // if you don't return 0, the transfer will be aborted - see the documentation
    return 0; 
}

int download_file(const char* remoteName, const char* localName) {
	printf("\r\n");
	CURL *curl;
	FILE *fp;
	curl = curl_easy_init();
	if(curl != NULL) {
		fp = fopen(localName, "wb");
		
		if(!fp) {
			Com_Printf("download_file: failed to open local file '%s'\n", localName);
			return 0;
		}
		
		curl_easy_setopt(curl, CURLOPT_URL, remoteName);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); //Prevent "longjmp causes uninitialized stack frame" bug
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        if(curl_easy_perform(curl) != CURLE_OK)
			Com_Printf("curl_easy_perform failed!\n");
        curl_easy_cleanup(curl);
		
		fclose(fp);
		printf("\r\n");
		return 1;
	}
	printf("\r\n");
	return 0;
}