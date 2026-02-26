#include <stdlib.h>
#include <stdio.h>
#include "machine.h"

#define DEFAULT_STATES_SIZE 10

TuringState create_state(int id, TuringState zero, TuringState one, char zero_action[2], char one_action[2]) {
	TuringState state;
	if ((state = malloc(sizeof(struct sTuringMachine))) == NULL) return NULL;

	state->id = id;
	state->zero_state = zero;
	state->one_state = one;

	int i = 0;
	do {
		state->zero[i] = zero_action[i];
		state->one[i] = one_action[i];
	} while (i++ < 2);

	return state;
}
void destroy_state(TuringState state) {
	free(state);
}

TuringMachine create_machine() {
	TuringMachine machine;
	if ((machine = malloc(sizeof(struct sTuringMachine))) == NULL) return NULL;

	machine->initial = NULL;
	machine->id_gen = 0;

	if ((machine->states = cl_create()) == NULL) {
		free(machine);
		return NULL;
	}

	return machine;
}

void destroy_machine(TuringMachine * mc) {
	TuringMachine machine = *mc;

	cl_destroy(&(machine->states), (void (*)(void *))destroy_state);

	free(machine);
	*mc = NULL;
}

void state_edit_reds(TuringState source, TuringState zero, TuringState one) {
	if (zero != NULL) source->zero_state = zero;
	if (one != NULL) source->one_state = one;
}
void state_edit_oed(TuringState state, char val) {
	state->one[0] = val;
}
void state_edit_omv(TuringState st, char v) {
	st->one[1] = v;
}
void state_edit_zed(TuringState s, char v) {
	s->zero[0] = v;
}
void state_edit_zmv(TuringState s, char v) {
	s->zero[1] = v;
}
void state_edit_one(TuringState s, char w, char m) {
	state_edit_oed(s, w);
	state_edit_omv(s, m);
}
void state_edit_zer(TuringState s, char w, char m) {
	state_edit_zed(s, w);
	state_edit_zmv(s, m);
}

static void machine_append(TuringMachine machine, TuringState state) {
	cl_append(machine->states, state);
}

TuringState machine_create_state(TuringMachine machine) {
	TuringState state = create_state(machine->id_gen, NULL, NULL, "01", "11");
	if (state == NULL) return NULL;

	state_edit_reds(state, state, state);
	machine->id_gen++;

	machine_append(machine, state);
	
	return state;
}

static int compare_two(void * pa, void * pb) {
	TuringState a = (TuringState)pa;
	TuringState b = (TuringState)pb;
	return a->id == b->id;
}

TuringState machine_find_state(TuringMachine machine, int id) {
	TuringState template = create_state(id, NULL, NULL, "00", "00");
	if (template == NULL) return NULL;

	TuringState rep = cl_find_ref_template(machine->states, template, compare_two);

	free(template);
	return rep;
}

TuringState machine_set_initial(TuringMachine machine, TuringState state) {
	return machine->initial = state;
}
TuringState machine_set_stop(TuringMachine machine, TuringState state) {
	return machine->stop = state;
}

int machine_append_state(TuringMachine machine, TuringState state) {
	if (machine_find_state(machine, state->id) != NULL) {
		return 0;
	}

	cl_append(machine->states, state);
	return 1;
}

static int check_deps(TuringMachine machine, TuringState state) {
	if (state->zero_state != NULL) {
		if (machine_find_state(machine, state->zero_state->id) == NULL) return 0;
	}
	if (state->one_state != NULL) {
		if (machine_find_state(machine, state->one_state->id) == NULL) return 0;
	}
	return 1;
}
int machine_valid(TuringMachine machine) {
	if (machine->initial == NULL) return 0;
	if (machine->stop == NULL) return 0;

	if (!check_deps(machine, machine->initial)) return 0;
	if (!check_deps(machine, machine->stop)) return 0;

	ChainedCell cell = machine->states->entry;

	while (cell != NULL) {
		TuringState state = cell->value;

		if (!check_deps(machine, state)) return 0;

		cell = cell->next;
	}

	return 1;
}

static int include_deps(TuringMachine machine, TuringState state) {
	int i = 0;
	if (machine_find_state(machine, state->zero_state->id) == NULL) {
		i += machine_append_state(machine, state->zero_state);
	}
	if (machine_find_state(machine, state->one_state->id) == NULL) {
		i += machine_append_state(machine, state->one_state);
	}

	return i;
}
int machine_include_deps(TuringMachine machine) {
	int i = 0;
	ChainedCell cell = machine->states->entry;

	if (machine->initial != NULL) i+= include_deps(machine, machine->initial);
	if (machine->stop != NULL) i+= include_deps(machine, machine->stop);

	while (cell != NULL) {
		TuringState state = cell->value;

		i += include_deps(machine, state);

		cell = cell->next;
	}

	return i;
}

static int valid_input(char * input) {
	int i = 0;
	while (input[i] != 0) {
		if (input[i] != '0' && input[i] != '1') return 0;
		i++;
	}
	return 1;
}

long int machine_evaluate(TuringMachine machine, char * input, long int * lastp) {
	if (!machine_valid(machine)) return -1;
	if (!valid_input(input)) return -1;

	long int i = 0;
	long int c = 0;

	TuringState current = machine->initial;

	while (1) {
		int num = input[i] == '1';

		char * move = num ? current->one : current->zero;
		*(input + i) = 48 + (int)move[0];


		if (num) current = current->one_state;
		else current = current->zero_state;

		if (move[1] == 0) {
			if (i == 0) return -2;
			i--;
		} else {
			i++;
		}
		c++;

		if (current->id == machine->stop->id) {
			*lastp = i - 1;
			return c;
		}
	}	
}
