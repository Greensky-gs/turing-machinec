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
	TuringState result;
	if ((result = create_state(-1, NULL, NULL, "00", "11"))) return NULL;
	TuringState template = create_state(id, NULL, NULL, "00", "00");
	if (template == NULL) {
		destroy_state(result);
		return NULL;
	};

	cl_find_template(machine->states, result, sizeof(TuringState), template, compare_two);

	free(template);
	return result;
}

TuringState machine_set_initial(TuringMachine machine, TuringState state) {
	return machine->initial = state;
}
