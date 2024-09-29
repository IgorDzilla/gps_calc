#ifndef SPLITTER_H
#define SPLITTER_H

#include <stdlib.h>

/*
 * Splits string into NULL-terminated array of strings by given separator
 * sep - separator
 * If substring in given string is empty, susbtring in split will be an empty
 * string
 * */
char **strsplit(const char *string, const char sep);

/*
 * Frees all the memory allocated for the split
 */
void free_split(char **splits);

/*
 * Returns number of strings in split. NULL-terminator is not counted.
 */
size_t splitsize(char **splits);

char **split_pop(char ***split);

#endif // !SPLITTER_H
