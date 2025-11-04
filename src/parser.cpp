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
  std::string str_line;
  std::getline(in, str_line, '\n');

  size_t token_start = 0;
  bool escaped = false;
  bool double_quoted = false;
  std::string temp;
  for (size_t it = 0; it < str_line.size(); it++)
  {
    if (double_quoted)
    {
      if (str_line[it] == '"')
      {
        args.emplace_back(str_line.substr(token_start, it - token_start));
        double_quoted = false;
        token_start = it + 1;
      }
    }
    else if (escaped)
    {
      temp.append(str_line.substr(token_start, it - token_start - 1));
      if (str_line[it] == ' ')
      {
        temp.append(" ");
      }
      if (str_line[it] == '\\')
      {
        temp.append("\\");
      }
      escaped = false;
      token_start = it + 1;
    }
    else if (it != token_start && str_line[it] == ' ')
    {
      args.emplace_back(temp + str_line.substr(token_start, it - token_start));
      token_start = it + 1;
    }
    else if (!double_quoted && str_line[it] == '\\')
    {
      escaped = true;
    }
    else if (str_line[it] == '"')
    {
      double_quoted = true;
    }
  }
  args.emplace_back(temp + str_line.substr(token_start, str_line.size() - token_start));
  return CommandLine { std::move(args) };
}

std::string zhell::read_quotes(std::istream& in)
{
  std::string temp;
  std::getline(in, temp, '"');
  return temp;
}