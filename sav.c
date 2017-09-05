#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sav.h"

const unsigned int BLOCKS_START = 0x8000;
const unsigned int BLOCKS_SIZE = 0x200;

struct lsdj_error_t
{
	const char* message;
};

const char* lsdj_get_error_c_str(lsdj_error_t* error)
{
	if (error == NULL)
		return NULL;
	else
		return error->message;
}

void lsdj_free_error(lsdj_error_t* error)
{
	free(error);
}

typedef struct
{
	char project_names[32 * 8];
	char versions[32 * 1];
	char empty[30];
	char init[2];
	int active_project;
} header_t;

void create_error(lsdj_error_t** error, const char* message)
{
	if (error == NULL)
		return;

	*error = malloc(sizeof(lsdj_error_t));
	(*error)->message = "could not open file";
}

lsdj_sav_t* lsdj_open(const char* path, lsdj_error_t** error)
{
    // Try to open the sav file at the given path
	FILE* file = fopen(path, "r");
	if (!file)
	{
		create_error(error, "could not open file");
		return NULL;
	}

    // Skip memory representing the currently open song
	fseek(file, BLOCKS_START, SEEK_SET);

	// Read the header block, before we start processing each song
	header_t header;
	fread(&header, sizeof(header), 1, file);

	// Check the initialization characters. If they're not 'jk', we're
	// probably not dealing with an actual LSDJ sav format file.
	if (header.init[0] != 'j' || header.init[1] != 'k')
	{
		create_error(error, "SRAM initialization check wasn't 'jk'");
		fclose(file);
		return NULL;
	}

	// Create the sav file structure to return from this function
	lsdj_sav_t* sav = malloc(sizeof(lsdj_sav_t));

	// Find out how many projects there are
	sav->project_count = 0;
	char* ptr = header.project_names;
	while (1)
	{
		size_t length = strlen(ptr);
		if (length == 0)
			break;

		sav->project_count += 1;
		ptr += length + 1;
	}
    
    // Allocate data for all the projects
    sav->projects = malloc(sizeof(lsdj_project_t) * sav->project_count);

	// Store the active project index
	sav->active_project = header.active_project == 0xFF ? -1 : header.active_project;

    // Clean-up and close the file
	fclose(file);

	return sav;
}

void lsdj_close(lsdj_sav_t* sav)
{
    free(sav->projects);
	free(sav);
}