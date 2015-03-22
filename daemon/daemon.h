#ifndef _DAEMON_H_
#define _DAEMON_H_

#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <map>

class Daemon
{
public:
    Daemon() : fd_(), output_fd_(), pidfile_(), sigmap_()
    {
        sigmap_.insert(std::pair<int, sighandler_t>(SIGCHLD, SIG_IGN));
        sigmap_.insert(std::pair<int, sighandler_t>(SIGHUP, SIG_IGN));
    }
    ~Daemon()
    {
        if(!pidfile_.empty() && lockf(fd_, F_TEST, 0) == 0)
        {
            close(fd_);
            remove(pidfile_.c_str());
        }
        if(fcntl(output_fd_, F_GETFD) != -1)
        {
            close(output_fd_);
        }
    }
    void SetSigHandler(int signal, sighandler_t handler);
    void SingleInstance(std::string filename) { pidfile_ = std::string("/var/run/").append(filename); }
    bool Run();
private:
    int fd_;
    int output_fd_;
    std::string pidfile_;
    std::map<int, sighandler_t> sigmap_;
};

#endif
