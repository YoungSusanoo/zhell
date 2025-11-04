#include <commands.hpp>

#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>
#include <wait.h>

void zhell::exec_default(const CommandLine& line)
{
  if (line.args.empty())
  {
    return;
  }
  int pid = fork();
  if (pid == 0)
  {
    close_range(3, ~0U, 0);
    execlp(line.args.front().c_str(), line.args.front().c_str(), line.args[1].c_str(), (char*)NULL);
    std::cout << line.args.front() << ": " << strerror(errno);
  }
  waitpid(pid, nullptr, 0);
}