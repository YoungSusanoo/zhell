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

int zhell::exec_default(std::vector< std::string >& args, int in, int out)
{
  if (args.empty())
  {
    throw std::invalid_argument("No args");
  }

  return fork_and_exec(args, in, out);
}

bool zhell::exec_cd(std::vector< std::string >& args)
{
  bool execution_completed = true;
  if (args.empty())
  {
    execution_completed = false;
  }
  else if (args.size() > 2)
  {
    std::cout << "cd: too many arguments\n";
    execution_completed = false;
  }
  else if (args.front() != "cd")
  {
    std::cout << args.front() << ": No such file or directory\n";
    execution_completed = false;
  }
  else if (chdir(args[1].c_str()) == -1)
  {
    std::cout << args.front() << ": " << args[1] << ": No such file or directory\n";
    execution_completed = false;
  }
  return execution_completed;
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
  std::cerr << args.front() << ": " << strerror(errno);
  std::exit(1);
}