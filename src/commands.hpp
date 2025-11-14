#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <string>

#include <command_line.hpp>

namespace zhell
{
  void exec_default(CommandLine& line, int in, int out);
  void exec_cd(CommandLine& line);
}

#endif