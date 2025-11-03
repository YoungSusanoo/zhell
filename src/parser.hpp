#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include <memory>

namespace zhell
{
  struct CommandLine
  {
    std::vector< std::string > args;
  };

  CommandLine get_cmd(std::istream& in);
}

#endif