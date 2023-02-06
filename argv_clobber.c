#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#define PATH_MAX    4096
extern char** environ;
int main(int argc, char* argv[]) {
    pid_t pid = getpid();
    char proc_pid_cmdline_path[PATH_MAX];
    char cmdline[_SC_PAGE_SIZE];
    sprintf(proc_pid_cmdline_path, "/proc/%d/cmdline", pid);

    FILE* proc_pid_cmdline = fopen(proc_pid_cmdline_path, "r");
    fgets(cmdline, _SC_PAGE_SIZE, proc_pid_cmdline);
    int env_len = -1;
    if (environ){
        while (environ[++env_len]);
    }
    unsigned int size;
    if (env_len > 0) {
        size = environ[env_len-1] + strlen(environ[env_len-1]) - argv[0];
    } else {
        size = argv[argc-1] + strlen(argv[argc-1]) - argv[0];
    }

    if (environ) {
        char **new_environ = malloc(env_len*sizeof(char *));
        unsigned int i = 3;
        
        new_environ[i] = strdup(environ[i]);
        
        environ = new_environ;
    }

    char *args = argv[0];
    memset(args, 0, size);
    snprintf(args, size -1, "\0"); 
    printf("PID: %d\n", getpid());
    pause();
}