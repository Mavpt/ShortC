/* See LICENSE file for copyright and license details
 *
 * MinMaxCol finds the minimum and maximum of each collumn in a TSV file
 */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT_IGNOREFIRSTLINE 0
#define SIZEOFLINE 256

FILE* OpenFile(const char* FilePath) {
	FILE* File = fopen(FilePath, "r");
	if(!File) {
		printf("Error opening %s", FilePath);
		exit(1);
	}

	return File;
}

const double GetCol(char* line, int Col) { 
	const char* tok;
	for (tok = strtok(line, "\t"); tok && *tok; tok = strtok(NULL, "\t\n")) {
		if (!--Col) {
			//printf("%s : %d : %s : %lf\n", line, Col, tok, atof(tok));
			return atof(tok);
		}
	}

	printf("Error finding collumn #%d in \"%s\"\n\n", Col, line);
	exit(1);
}

void PrintOutput(const int NCol, const double* Min, const double* Max) {
	printf("Col\tMin\t\tMax\n");
	for(int i = 0; i < NCol; i++) printf("%d\t%lf\t\%lf\n", i, Min[i], Max[i]);
}

int main(int argc, char* argv[]) {
	bool ignorefirstline = DEFAULT_IGNOREFIRSTLINE;
	switch(argc) {
	case 3:
		break;
	
	case 4:
		ignorefirstline = strcmp(argv[3], "0");
		//printf("%s : %d\n", argv[3], ignorefirstline);
		break;

	default:
		printf("Usage: MinMaxCol [FILE] [COLLUMNS] [IGNOREFIRSTLINE]");
		exit(1);
	}

	FILE* Input = OpenFile(argv[1]);
	const unsigned int NCol = atoi(argv[2]);
	
	double *Min = (double*) alloca(NCol*sizeof(double));
	double *Max = (double*) alloca(NCol*sizeof(double));

	for(int i = 0; i < NCol; i++) {
		Min[i] = DBL_MAX;
		Max[i] = -DBL_MAX;
	}

	char line[SIZEOFLINE];
	double x = 0;

	if(ignorefirstline) fgets(line, sizeof(line), Input);

	while(fgets(line, SIZEOFLINE, Input)) {
		for(int i = 0; i < NCol; i++) {
			char* tmp = strdup(line);
			x = GetCol(tmp, i+1);
			Min[i] = (Min[i] > x)? x : Min[i];
			Max[i] = (Max[i] < x)? x : Max[i];
		}
	}

	PrintOutput(NCol, Min, Max);
	if(fclose(Input)) {
		printf("ERROR: Could not close %s", argv[1]);
		exit(1);
	}

	return 0;
}
