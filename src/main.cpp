#include <iostream>

#include <parser.hpp>
#include <commands.hpp>

int main()
{
  zhell::Parser parser(std::cin);
  while (std::cin)
  {
    zhell::Parser::lines_t lines = parser.get_cmd();
    zhell::exec_default(lines.front());
  }
}
