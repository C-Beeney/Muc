#include "muc_copystring.h"

#include <stddef.h>
#include <assert.h>
#include <string.h>

void muc_copystring(char *destination, size_t destination_size, const char *source)
{	
	size_t source_length = strlen(source); 
	size_t total_needed = source_length + 1;

	assert(destination != NULL);
	assert(source != NULL);
	assert(destination_size != 0);
	assert(destination_size >= total_needed &&
		"Buffer overflow: destination_size too small for source");

	memcpy(destination, source, total_needed);
}
