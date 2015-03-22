/**************************************************************************
 * Here we write daemon with double fork                                  *
 * About double fork daemon : http://goo.gl/l07eU0                        *
 * other: <<linux daemon writing HOWTO>> http://goo.gl/mNG0fb  (fork once)*
 **************************************************************************/
#include "daemon.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>

#define DEV_NULL "/dev/null"

void Daemon::SetSigHandler(int signal, sighandler_t handler)
{
    std::map<int, sighandler_t>::iterator it = sigmap_.find(signal);
    if(it == sigmap_.end())
    {
        sigmap_.insert(std::pair<int, sighandler_t>(signal, handler));
    }
    else
    {
        it->second = handler;
    }
}

bool Daemon::Run()
{
    /* Our process ID and Session ID */
    pid_t pid, sid;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
    {
        std::cerr << "First fock failed!" << std::endl;;
        return false;
    }
    /* If we got a good PID, then
     * we can exit the parent process. */
    if (pid > 0)
    {
        sleep(1);
        exit(EXIT_SUCCESS);
    }

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
    {
        std::cerr << "Setsid failed!" << std::endl;;
        return false;
    }

    /* Ignore signals */
    for(std::map<int, sighandler_t>::iterator it = sigmap_.begin(); it != sigmap_.end(); ++it)
    {
        signal(it->first, it->second);
    }

    /* Fork off the second time */
    pid = fork();
    if (pid < 0)
    {
        std::cerr << "Second fork failed!" << std::endl;;
        return false;
    }
    /* If we got a good PID, then
     * we can exit the parent process. */
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    /* record pid to lockfile */
    if(!pidfile_.empty())
    {
        fd_ = open(pidfile_.c_str(), O_RDWR|O_CREAT, 0640);
        if ( fd_ < 0)
        {
            std::cerr << "Can not open " << pidfile_ << std::endl;
            return false;
        }
        if (lockf(fd_, F_TLOCK, 0) < 0)
        {
            std::cerr << "Another instance is running!" << std::endl;;
            return false;
        }
        /*write and lock pid file*/
        std::stringstream stream;
        stream << getpid() << '\n';
        std::string pidstr = stream.str();
        ssize_t len = write(fd_, pidstr.c_str(), pidstr.length());
        if(len < 0)
        {
            std::cerr << "Write to " << pidfile_ << " failed!" << std::endl;;
            return false;
        }
    }

    /* Close out the standard file descriptors */
    if(close(STDIN_FILENO) == -1)
    {
        std::cerr << "Can not close STDIN_FILENO!" << std::endl;
        return false;
    }
    if(close(STDOUT_FILENO) == -1)
    {
        std::cerr << "Can not close STDOUT_FILENO!" << std::endl;
        return false;
    }
    if(close(STDERR_FILENO)  == -1)
    {
        std::cerr << "Can not close STDERR_FILENO!" << std::endl;
        return false;
    }
    /*redirect standard output*/
    int output_fd_ = open(DEV_NULL, O_RDWR, 0);
    if(output_fd_ == -1)
    {
      return false;
    }
    if(dup2(output_fd_, STDIN_FILENO) == -1)
    {
      return false;
    }
    if(dup2(output_fd_, STDOUT_FILENO) == -1)
    {
      return false;
    }
    if(dup2(output_fd_, STDERR_FILENO) == -1)
    {
      return false;
    }

    /* Change the file mode mask */
    umask(0);

    /* Change the current working directory */
    if ((chdir("/")) < 0) {
      return false;
    }

    return true;
}

