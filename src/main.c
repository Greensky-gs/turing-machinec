#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "cl.h"

int main() {
	TuringMachine machine = create_machine();

	TuringState move_til_end_of_ones = machine_create_state(machine);
	TuringState end = machine_create_state(machine);

	state_edit_omv(move_til_end_of_ones, 1);
	state_edit_oed(move_til_end_of_ones, 1);
	state_edit_zmv(move_til_end_of_ones, 1);
	state_edit_zed(move_til_end_of_ones, 0);

	state_edit_reds(move_til_end_of_ones, end, move_til_end_of_ones);

	machine_set_initial(machine, move_til_end_of_ones);
	machine_set_stop(machine, end);

	printf("Machine validi : \x1b[31m%d\x1b[0m\n", machine_valid(machine));

	char * input = malloc(10);
	int j = 0;
	while (j < 9) input[j++] = '1';
	input[j++] = '0';
	input[j++] = 0;

	long int last;
	long int res = machine_evaluate(machine, input, &last);

	printf("res = %ld\nPosition de la tête de lecture sur le ruban : %ld\n", res, last);

	destroy_machine(&machine);
	
	free(input);
}
