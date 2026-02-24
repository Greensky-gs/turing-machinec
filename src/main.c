#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "cl.h"

int main() {
	TuringMachine machine = create_machine();

	int i = 0;

	while (i < 20) {
		printf("Creating state %d\n", i);
		machine_create_state(machine);
		i++;
	
	}

	TuringState zeroth = machine_find_state(machine, 0);

	printf("zeroth = \x1b[31m%p\x1b[0m\n", zeroth);
	printf("ID = %d\n", zeroth->id);

	machine_set_initial(machine, zeroth);
	destroy_machine(&machine);
	
	printf("Hello world!\n");
}
