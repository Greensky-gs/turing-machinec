#ifndef __TURING_H__
#define __TURING_H__ 1
#include "cl.h"


struct sTuringState {
	int id;
	struct sTuringState * zero_state;
	struct sTuringState * one_state;
		
	char zero[2]; // First bit is for writing, second is for moving (0 for left and 1 for right)
	char one[2];
};
typedef struct sTuringState * TuringState;
struct sTuringMachine {
	int id_gen;
	ChainedList states;
	TuringState initial;
	TuringState stop;
};
typedef struct sTuringMachine * TuringMachine;

extern TuringState create_state(int id, TuringState zero, TuringState one, char zero_action[2], char one_action[2]);
extern void destroy_state(TuringState);

extern TuringMachine create_machine();
extern void destroy_machine(TuringMachine *);
extern TuringState machine_create_state(TuringMachine);

// Machine edits
extern int machine_append_state(TuringMachine, TuringState);
extern TuringState machine_set_initial(TuringMachine, TuringState);
extern TuringState machine_set_stop(TuringMachine, TuringState);
extern int machine_include_deps(TuringMachine);

// Machine utils
extern TuringState machine_find_state(TuringMachine, int);
extern int machine_valid(TuringMachine);

extern int save_machine(TuringMachine, char *);
extern void display_machine(TuringMachine);
extern TuringMachine load_machine(char *);


// State edition
extern void state_edit_reds(TuringState, TuringState zero, TuringState one);
extern void state_edit_oed(TuringState, char);
extern void state_edit_omv(TuringState, char);
extern void state_edit_zed(TuringState, char);
extern void state_edit_zmv(TuringState, char);

extern void state_edit_one(TuringState, char, char);
extern void state_edit_zer(TuringState, char, char);

// Machine start
//   Important: A zero in the input is evaluated as a '0' and a '1' as a '1', not the numerical values 
extern long int machine_evaluate(TuringMachine, char * input, long int *);
#endif
