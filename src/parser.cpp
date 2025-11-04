#include <parser.hpp>

#include <iostream>
#include <iterator>

zhell::Parser::Parser(std::istream& in):
  start_line_ {},
  curr_line_ {},
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

std::unique_ptr< zhell::CommandLine > zhell::Parser::get_cmd()
{
  std::getline(in_, str_line_, '\n');
  curr_line_ = std::make_unique< CommandLine >();

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
      (this->*symbols_[str_line_[pos_]])();
    }
  }
  curr_line_->args.emplace_back(temp_ + str_line_.substr(token_start_, str_line_.size() - token_start_));
  return std::move(curr_line_);
}

void zhell::Parser::handle_ampersand()
{}

void zhell::Parser::handle_slash()
{
  if (!double_quoted_)
  {
    escaped_ = true;
  }
}

void zhell::Parser::handle_double_quote()
{
  if (double_quoted_)
  {
    curr_line_->args.emplace_back(str_line_.substr(token_start_, pos_ - token_start_));
    double_quoted_ = false;
    token_start_ = pos_ + 1;
  }
  else
  {
    double_quoted_ = true;
  }
}

void zhell::Parser::handle_pipe()
{}

void zhell::Parser::handle_space()
{
  if (pos_ != token_start_)
  {
    curr_line_->args.emplace_back(temp_ + str_line_.substr(token_start_, pos_ - token_start_));
    temp_.clear();
    token_start_ = pos_ + 1;
  }
}
