#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include <memory>

#include <command_line.hpp>

namespace zhell
{
  CommandLine get_cmd(std::istream& in);
}

#endif