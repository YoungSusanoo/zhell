#include <parser.hpp>

#include <iostream>
#include <iterator>

zhell::Parser::Parser(std::istream& in):
  str_line_ {},
  temp_ {},
  token_start_ {},
  pos_ {},
  escaped_ {},
  double_quoted_ {},
  in_ { in }
{
  symbols_['&'] = &Parser::handle_ampersand;
  symbols_['\\'] = &Parser::handle_slash;
  symbols_['"'] = &Parser::handle_double_quote;
  symbols_['|'] = &Parser::handle_pipe;
  symbols_[' '] = &Parser::handle_space;
}

zhell::Parser::lines_t zhell::Parser::get_cmd()
{
  std::getline(in_, str_line_, '\n');
  lines_t v;
  v.emplace_back(CommandLine {});

  for (pos_ = 0; pos_ < str_line_.size(); pos_++)
  {
    if (escaped_)
    {
      temp_.append(str_line_.substr(token_start_, pos_ - token_start_ - 1));
      escaped_ = false;
      token_start_ = pos_;
    }
    if (symbols_.contains(str_line_[pos_]))
    {
      (this->*symbols_[str_line_[pos_]])(v);
    }
  }
  v.back().args.emplace_back(temp_ + str_line_.substr(token_start_, str_line_.size() - token_start_));
  return v;
}

void zhell::Parser::handle_ampersand(lines_t& v)
{}

void zhell::Parser::handle_slash(lines_t& v)
{
  if (!double_quoted_)
  {
    escaped_ = true;
  }
}

void zhell::Parser::handle_double_quote(lines_t& v)
{
  if (double_quoted_)
  {
    v.back().args.emplace_back(str_line_.substr(token_start_, pos_ - token_start_));
    double_quoted_ = false;
    token_start_ = pos_ + 1;
  }
  else
  {
    double_quoted_ = true;
  }
}

void zhell::Parser::handle_pipe(lines_t& v)
{}

void zhell::Parser::handle_space(lines_t& v)
{
  if (pos_ != token_start_)
  {
    v.back().args.emplace_back(temp_ + str_line_.substr(token_start_, pos_ - token_start_));
    temp_.clear();
    token_start_ = pos_ + 1;
  }
}
