#include <stdio.h>
#include <signal.h>
#include "shell.h"

int main() {
    signal(SIGCHLD, (void (*)(int))on_child_exit);
    setup_environment();
    shell();
    return 0;
}
