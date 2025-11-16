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

zhell::Parser::str_vec_t zhell::Parser::get_cmd()
{
  clean();
  std::getline(in_, str_line_, '\n');
  str_vec_t v;

  for (pos_ = 0; pos_ < str_line_.size(); pos_++)
  {
    if (escaped_)
    {
      temp_.append(str_line_.substr(token_start_, pos_ - token_start_ - 1));
      escaped_ = false;
      token_start_ = pos_;
    }
    else if (symbols_.contains(str_line_[pos_]))
    {
      (this->*symbols_[str_line_[pos_]])(v);
    }
  }
  if (token_start_ != pos_)
  {
    v.emplace_back(temp_ + str_line_.substr(token_start_, str_line_.size() - token_start_), false);
  }

  return v;
}

void zhell::Parser::clean()
{
  str_line_.clear();
  temp_.clear();
  token_start_ = 0;
  pos_ = 0;
  escaped_ = false;
  double_quoted_ = false;
}

void zhell::Parser::handle_ampersand(str_vec_t& v)
{
  if (!double_quoted_)
  {
    if (pos_ != token_start_)
    {
      v.emplace_back(temp_ + str_line_.substr(token_start_, pos_ - token_start_), true);
    }
    token_start_ = pos_;
    if (pos_ + 1 != str_line_.size() && str_line_[pos_ + 1] == '&')
    {
      pos_++;
    }
    v.emplace_back(temp_ + str_line_.substr(token_start_, pos_ + 1 - token_start_), true);
    token_start_ = pos_ + 1;
  }
}

void zhell::Parser::handle_slash(str_vec_t& v)
{
  if (!double_quoted_)
  {
    escaped_ = true;
  }
}

void zhell::Parser::handle_double_quote(str_vec_t& v)
{
  if (double_quoted_)
  {
    v.emplace_back(str_line_.substr(token_start_, pos_ - token_start_), false);
    double_quoted_ = false;
  }
  else
  {
    double_quoted_ = true;
  }
  token_start_ = pos_ + 1;
}

void zhell::Parser::handle_pipe(str_vec_t& v)
{
  if (!double_quoted_)
  {
    if (pos_ != token_start_)
    {
      v.emplace_back(temp_ + str_line_.substr(token_start_, pos_ - token_start_), true);
    }
    token_start_ = pos_;
    if (pos_ + 1 != str_line_.size() && str_line_[pos_ + 1] == '|')
    {
      pos_++;
    }
    v.emplace_back(temp_ + str_line_.substr(token_start_, pos_ + 1 - token_start_), true);
    token_start_ = pos_ + 1;
  }
}

void zhell::Parser::handle_space(str_vec_t& v)
{
  if (double_quoted_)
  {
    return;
  }
  if (pos_ != token_start_)
  {
    v.emplace_back(temp_ + str_line_.substr(token_start_, pos_ - token_start_), false);
    temp_.clear();
    token_start_ = pos_ + 1;
  }
  else
  {
    token_start_++;
  }
}