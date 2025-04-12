#include "syscall-wrappers.h"

static char msg[] = "hello static\n";

int main() {
// 	e_break();
	run(4);
	return 0;
}
