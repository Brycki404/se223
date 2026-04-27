#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char name[64];
	char species[128];
} monster;

int main(void) {
	monster *m1 = (monster *)malloc(sizeof(monster));
	monster *m2 = (monster *)malloc(sizeof(monster));

	if (m1 == NULL || m2 == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		free(m1);
		free(m2);
		return 1;
	}

	strcpy(m1->name, "Frankenstein");
	strcpy(m1->species, "human machine hybrid");

	strcpy(m2->name, "Dracula");
	strcpy(m2->species, "vampire");

	printf("Monster 1: name = %s, species = %s\n", m1->name, m1->species);
	printf("Monster 2: name = %s, species = %s\n", m2->name, m2->species);

	free(m1);
	free(m2);

	return 0;
}
