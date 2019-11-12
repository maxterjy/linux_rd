#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <syslog.h>   
#include <signal.h>

//terminal 1: tail -f /var/log/syslog
//terminal 2: sudo ./demo
//terminal 3: sudo ps -C demo -o "sid pgid ppid pid cmd tty"

int main() {
	signal(SIGHUP, SIG_IGN);
	umask(0);
	chdir("/");

	openlog("demo", LOG_PID, LOG_USER);
	syslog(LOG_INFO, "sid:%d pgid:%d ppid:%d pid:%d", getsid(0), getpgrp(), getppid(), getpid());

	pid_t pid = fork();
	
	if (pid > 0) exit(0);//parent exit	

	syslog(LOG_INFO, "sid:%d pgid:%d ppid:%d pid:%d", getsid(0), getpgrp(), getppid(), getpid());

	setsid(); //become session leader
	syslog(LOG_INFO, "become session leader");
	syslog(LOG_INFO, "sid:%d pgid:%d ppid:%d pid:%d", getsid(0), getpgrp(), getppid(), getpid());


	pid = fork();

	if (pid > 0) exit(0); //child 1 exit

	syslog(LOG_INFO, "fork again");
	syslog(LOG_INFO, "sid:%d pgid:%d ppid:%d pid:%d", getsid(0), getpgrp(), getppid(), getpid());


	long maxfd = sysconf(_SC_OPEN_MAX); //close all file descriptor
	for(int i = 0; i < maxfd; i++) {
		close(i);
	}

	
	//make stdin, stdout fd to become /dev/null
	int fd0 = open("/dev/null", O_RDWR);
	int fd1 = dup(fd0);
	int fd2 = dup(fd0);
	syslog(LOG_INFO, "fd0:%d fd1:%d fd2:%d", fd0, fd1, fd2);

	syslog(LOG_INFO, "sleep 10 seconds");
	sleep(10);
	syslog(LOG_INFO, "exit");

	return 0;
}