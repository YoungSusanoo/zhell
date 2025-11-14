#include <commands.hpp>

#include <iostream>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>
#include <wait.h>

namespace zhell
{
  int fork_and_exec(std::vector< std::string >& args, int in, int out);
}

void zhell::exec_default(std::vector< std::string >& args, int in, int out)
{
  if (args.empty())
  {
    return;
  }
  if (args.front() == "cd")
  {
    exec_cd(args);
  }

  int pid = fork_and_exec(args, in, out);
  waitpid(pid, nullptr, 0);
}

void zhell::exec_cd(std::vector< std::string >& args)
{
  if (args.empty())
  {
    return;
  }
  if (args.size() > 2)
  {
    std::cout << "cd: too many arguments\n";
    return;
  }
  if (args.front() != "cd")
  {
    std::cout << args.front() << ": No such file or directory\n";
    return;
  }
  if (chdir(args[1].c_str()) == -1)
  {
    std::cout << args.front() << ": " << args[1] << ": No such file or directory\n";
  }
}

int zhell::fork_and_exec(std::vector< std::string >& args, int in, int out)
{
  int pid = fork();
  if (pid)
  {
    return pid;
  }

  dup2(in, STDIN_FILENO);
  dup2(out, STDOUT_FILENO);
  close_range(3, ~0U, 0);

  char** args_c_str = new char*[args.size() + 1];
  std::transform(args.begin(), args.end(), args_c_str,
                 [](auto& a)
                 {
                   return a.data();
                 });
  args_c_str[args.size()] = nullptr;

  execvp(args.front().c_str(), args_c_str);
  std::cout << args.front() << ": " << strerror(errno);
  std::exit(-1);
}