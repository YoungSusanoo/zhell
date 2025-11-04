#include <iostream>

#include <parser.hpp>
#include <commands.hpp>

int main()
{
  zhell::Parser parser(std::cin);
  while (std::cin)
  {
    zhell::Parser::lines_t lines = parser.get_cmd();
    if (!lines.front().args.empty())
    {
      if (lines.front().args.front() == "cd")
      {
        zhell::exec_cd(lines.front());
      }
      else
      {
        zhell::exec_default(lines.front());
      }
    }
  }
}
