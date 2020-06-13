/*
 ============================================================================
 Name        : main.c
 Author      : Thierry
 Version     :
 Copyright   : Lycée A. Bourdelle Montauban
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>


#include "app.h"

int main(void) {
	fprintf(stdout, "================ENTER SpaceInvaders================\n");

	AppNew();
	AppRun();
	AppDel();

	fprintf(stdout, "================EXIT  SpaceInvaders================\n");
	return EXIT_SUCCESS;
}
