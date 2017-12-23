//
//  main.c
//  kill_prog
//
//  Created by Carsten Hood on 9/21/17.
//  Copyright © 2017 Carsten Hood. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>

int main(int argc, const char * argv[]) {
    int pid = atoi(argv[0]);
    printf("killing: %d", pid);
    kill(pid, SIGKILL);
    return 0;
}



//int main()
//{
//    FILE *getPIDS;
//    char line[130];
//    pid_t killpid;
//    // setuid to that of root//
//    pid_t myppid  = getppid();
//    getPIDS = popen("pidof -x yes","r");
//    while (fgets(line,sizeof line,getPIDS)) {
//        printf("KILL PID: %s",line);
//        kill(line,SIGKILL);
//    }
//}

