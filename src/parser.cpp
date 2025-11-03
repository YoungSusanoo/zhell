#include <parser.hpp>

#include <iostream>
#include <iterator>
#include <algorithm>

zhell::CommandLine zhell::get_cmd(std::istream& in)
{
  std::vector< std::string > args;
  std::string temp;
  std::getline(in, temp, '\n');
  size_t last = 0;
  size_t curr = temp.find(' ');
  while (curr != std::string::npos)
  {
    args.emplace_back(temp.substr(last, curr - last));
    last = curr + 1;
    curr = temp.find(' ', last);
  }
  args.emplace_back(temp.substr(last, temp.size() - last));
  return CommandLine { std::move(args) };
}