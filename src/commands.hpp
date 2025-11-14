#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <string>

#include <command_line.hpp>

namespace zhell
{
  void exec_default(std::vector< std::string >& args, int in, int out);
  void exec_cd(std::vector< std::string >& args);
}

#endif