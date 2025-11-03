#include <parser.hpp>

#include <iostream>
#include <iterator>
#include <algorithm>

namespace zhell
{
  std::string read_quotes(std::istream& in);
}

zhell::CommandLine zhell::get_cmd(std::istream& in)
{
  std::vector< std::string > args;
  std::string temp;
  std::getline(in, temp, '\n');
  size_t last_ws = 0;
  for (size_t it = 0; it < temp.size(); it++)
  {
    if (temp[it] == '\"')
    {
      size_t last_quote = temp.find('"', it + 1);
      if (last_quote != std::string::npos)
      {
        args.emplace_back(temp.substr(it + 1, last_quote - it - 1));
        it = last_quote + 1;
        last_ws = last_quote + 1;
      }
    }
    else if (temp[it] == ' ')
    {
      args.emplace_back(temp.substr(last_ws, it - last_ws));
      last_ws = it + 1;
    }
    else if (it + 1 == temp.size())
    {
      args.emplace_back(temp.substr(last_ws, temp.size() - last_ws));
    }
  }
  return CommandLine { std::move(args) };
}

std::string zhell::read_quotes(std::istream& in)
{
  std::string temp;
  std::getline(in, temp, '"');
  return temp;
}