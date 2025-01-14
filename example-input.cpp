

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#if __has_attribute (signal_safe)
# define SIGNAL_SAFE __attribute__((signal_safe))
#else
# define SIGNAL_SAFE
#endif


template <typename T>
static SIGNAL_SAFE T templatefunc(T i) {
  return i + 1;
}

static SIGNAL_SAFE void handle_usr1(int sig) {
	write(STDOUT_FILENO, "X", 1); // write(2) is signal safe

	templatefunc(4);

	void *x = malloc(10); // expected-warning
	free(x); // expected-warning
}

static void handle_usr2(int sig) {
	void *x = malloc(10);
	free(x);
}

int main(int argc, const char **argv){
	signal(SIGUSR1, handle_usr1);
	signal(SIGUSR2, handle_usr2); // expected-warning

	struct sigaction sa;
	sa.sa_handler = handle_usr2;  // expected-warning

	/* todo */
	struct sigaction sa2 = {
		.sa_handler = handle_usr2,
	};
}
