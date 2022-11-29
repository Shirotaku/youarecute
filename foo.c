#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define S 1024UL*1024UL*100UL

void
sig_handler(int signum) {
    char buf[128];

    snprintf(buf, 128,
             "Harharhar, I'm ticklish. Don't throw signal %d at me.",
             signum);
    puts(buf);
}


int
main(__attribute((__unused__))int argc,
     __attribute((__unused__))char **argv) {
    char *c;
    struct sigaction new_action;

    new_action.sa_handler = sig_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction(SIGINT, &new_action, NULL);    
    sigaction(SIGHUP, &new_action, NULL);
    sigaction(SIGTERM, &new_action, NULL);
    sigaction(SIGTSTP, &new_action, NULL);
    sigaction(SIGQUIT, &new_action, NULL);
    
    /* Let's consume some delicious memory */
    if (NULL == (c = malloc(S))) {
        fprintf(stderr, "Oops, ate too much memory.\n");
        return -1;
    }

    memset(c, 42, S);
    
    /* Nothing burns cpu cycles like busy waiting */
    for(;;) {
        struct timeval tv = {0, 1};

         if (-1 == select(0, NULL, NULL, NULL, &tv)) {
             fprintf(stderr, "Oops, time's going fast here.\n");
         }
    }
}
