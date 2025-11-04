#include <iostream>

#include <parser.hpp>
#include <commands.hpp>

int main()
{
  zhell::Parser parser(std::cin);
  while (std::cin)
  {
    std::unique_ptr< zhell::CommandLine > line = parser.get_cmd();
    zhell::exec_default(*line);
  }
}
