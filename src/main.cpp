#include <iostream>

#include <parser.hpp>
#include <commands.hpp>

int main()
{
  zhell::CommandLine line;
  while (std::cin)
  {
    line = zhell::get_cmd(std::cin);
    zhell::exec_default(line);
  }
}
