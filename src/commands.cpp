#include <commands.hpp>

#include <iostream>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>
#include <wait.h>

void zhell::exec_default(CommandLine& line)
{
  if (line.args.empty())
  {
    return;
  }
  int pid = fork();
  if (pid == 0)
  {
    close_range(3, ~0U, 0);

    char** args = new char*[line.args.size() + 1];
    std::transform(line.args.begin(), line.args.end(), args,
                   [](auto& a)
                   {
                     return a.data();
                   });
    args[line.args.size()] = nullptr;

    execvp(line.args.front().c_str(), args);
    std::cout << line.args.front() << ": " << strerror(errno);
    std::exit(-1);
  }
  waitpid(pid, nullptr, 0);
}

void zhell::exec_cd(CommandLine& line)
{
  if (line.args.empty())
  {
    return;
  }
  if (line.args.size() > 2)
  {
    std::cout << "cd: too many arguments\n";
    return;
  }
  if (line.args.front() != "cd")
  {
    std::cout << line.args.front() << ": No such file or directory\n";
    return;
  }
  if (chdir(line.args[1].c_str()) == -1)
  {
    std::cout << line.args.front() << ": " << line.args[1] << ": No such file or directory\n";
  }
}