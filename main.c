#include "imagewriter.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// globals shared with imagewriter.cpp
const char * g_imagewriter_fixed_font = "letgothl.ttf";
const char * g_imagewriter_prop_font = "letgothl.ttf";
const int iw_scc_write = 0;

// convert str to unsigned short, return LONG_MIN on errors
static long strtohu(const char * name, const char * val)
{
	char * endptr;

	errno = 0;
	long number = strtol(val, &endptr, 10);

	// test return to number and errno values
	if (! *val || *endptr || errno || number < 0 || number > USHRT_MAX) {
		fprintf(stderr, "Invalid value '%s' for %s: must be a number in range 0 to %hu (inclusive)\n", val, name, USHRT_MAX);
		return LONG_MIN;
	}

	return number;
}

int main(int argc, char * argv[])
{
	// settings
	long dpi = 144;
	long paperSize = 0;
	long bannerSize = 0;
	int multipageOutput = 0;
	char * output = "";

	// parse options
	int opt;
	while ((opt = getopt(argc, argv, "d:p:b:mo:")) != -1) {
		switch (opt) {

		// dpi / paperSize / bannerSize / multiPageOutput
		case 'd':
			dpi = strtohu("DPI", optarg);

			if (dpi < 0)
				return EXIT_FAILURE;

			break;

		case 'p':
			paperSize = strtohu("Paper Size", optarg);

			if (paperSize < 0)
				return EXIT_FAILURE;

			break;

		case 'b':
			bannerSize = strtohu("Banner Size", optarg);

			if (bannerSize < 0)
				return EXIT_FAILURE;

			break;

		case 'm':
			multipageOutput = 1;
			break;

		// Output format
		case 'o':
			output = optarg;
			break;

		case '?':
		default:
			return EXIT_FAILURE;
		}
	}

	// before we call this, set some SDL env vars to not actually output anything
	setenv("SDL_VIDEODRIVER", "dummy", 1);
	setenv("SDL_AUDIODRIVER", "dummy", 1);

	// ready to print!
	printf("Initializing virtual ImageWriter [dpi=%ld, paperSize=%ld, bannerSize=%ld, output='%s', multipageOutput=%d]\n",
		dpi, paperSize, bannerSize, output, multipageOutput);
	imagewriter_init(dpi, paperSize, bannerSize, output, multipageOutput);

	if (optind == argc - 1 &&
		strcmp(argv[optind], "-") == 0) {
		printf("Parsing STDIN...\n");

		int c;
		while ((c = fgetc(stdin)) != -1)
			imagewriter_loop(c);

		if (! feof(stdin)) {
			perror("Error in reading STDIN");
			return EXIT_FAILURE;
		}

		imagewriter_feed();
	} else {
		for (int index = optind; index < argc; index++) {
			printf("Parsing %s...\n", argv[index]);

			FILE * file = fopen(argv[index], "rb");
			if (file == NULL) {
				perror("Failed to open file");
				return EXIT_FAILURE;
			}

			int c;
			while ((c = fgetc(file)) != -1)
				imagewriter_loop(c);

			if (! feof(file)) {
				perror("Error in reading file");
				return EXIT_FAILURE;
			}

			imagewriter_feed();
			fclose(file);
		}
	}

	printf("Closing ImageWriter.\n");
	imagewriter_close();

	return EXIT_SUCCESS;
}
