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
  out_redired_ {},
  in_ { in }
{
  symbols_['&'] = &Parser::handle_ampersand;
  symbols_['\\'] = &Parser::handle_slash;
  symbols_['"'] = &Parser::handle_double_quote;
  symbols_['\''] = &Parser::handle_single_quote;
  symbols_['|'] = &Parser::handle_pipe;
  symbols_[' '] = &Parser::handle_space;
  symbols_['>'] = &Parser::handle_out_redir;
  symbols_['#'] = &Parser::handle_comment;
}

zhell::Parser::str_vec_t zhell::Parser::get_cmd()
{
  clean();

  str_vec_t v;
  v.emplace_back(CommandLine {});
  do
  {
    if (escaped_)
    {
      escaped_ = false;
    }
    std::getline(in_, str_line_, '\n');
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
    if (single_quoted_ || double_quoted_ || escaped_)
    {
      if (!escaped_)
      {
        temp_.append(str_line_.substr(token_start_, pos_ - token_start_) + "\n");
      }
      else
      {
        temp_.append(str_line_.substr(token_start_, pos_ - token_start_ - 1));
      }
      pos_ = 0;
      token_start_ = 0;
    }
  }
  while (single_quoted_ || double_quoted_ || escaped_);

  if (token_start_ != pos_)
  {
    emplace_str_or_filename(v);
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
  out_redired_ = false;
}

void zhell::Parser::handle_ampersand(str_vec_t& v)
{
  if (double_quoted_ || single_quoted_)
  {
    return;
  }

  if (pos_ != token_start_)
  {
    emplace_str_or_filename(v);
  }
  if (pos_ + 1 != str_line_.size() && str_line_[pos_ + 1] == '&')
  {
    pos_++;
    v.emplace_back(CommandLine {});
    v.back().connect_type = ConnectType::NO_EXEC_IF_FAIL;
  }
  else
  {
    v.back().background = true;
    v.emplace_back(CommandLine {});
  }
  token_start_ = pos_ + 1;
}

void zhell::Parser::handle_slash(str_vec_t& v)
{
  if (!single_quoted_)
  {
    escaped_ = true;
  }
}

void zhell::Parser::handle_double_quote(str_vec_t& v)
{
  if (!single_quoted_)
  {
    if (double_quoted_)
    {
      emplace_str_or_filename(v);
      double_quoted_ = false;
    }
    else
    {
      double_quoted_ = true;
    }
    token_start_ = pos_ + 1;
  }
}

void zhell::Parser::handle_single_quote(str_vec_t& v)
{
  if (!double_quoted_)
  {
    if (single_quoted_)
    {
      emplace_str_or_filename(v);
      single_quoted_ = false;
    }
    else
    {
      single_quoted_ = true;
    }
    token_start_ = pos_ + 1;
  }
}

void zhell::Parser::handle_pipe(str_vec_t& v)
{
  if (double_quoted_ || single_quoted_)
  {
    return;
  }

  if (pos_ != token_start_ || !temp_.empty())
  {
    emplace_str_or_filename(v);
  }
  if (pos_ + 1 != str_line_.size() && str_line_[pos_ + 1] == '|')
  {
    pos_++;
    v.emplace_back(CommandLine {});
    v.back().connect_type = ConnectType::EXEC_IF_FAIL;
  }
  else
  {
    v.back().output_type = OutputType::NEXT_LINE;
    v.emplace_back(CommandLine {});
  }
  token_start_ = pos_ + 1;
}

void zhell::Parser::handle_space(str_vec_t& v)
{
  if (double_quoted_ || single_quoted_)
  {
    return;
  }
  if (pos_ != token_start_)
  {
    emplace_str_or_filename(v);
    temp_.clear();
    token_start_ = pos_ + 1;
  }
  else
  {
    token_start_++;
  }
}

void zhell::Parser::handle_out_redir(str_vec_t& v)
{
  if (double_quoted_ || single_quoted_)
  {
    return;
  }
  if (pos_ != token_start_)
  {
    emplace_str_or_filename(v);
  }
  if (pos_ + 1 != str_line_.size() && str_line_[pos_ + 1] == '>')
  {
    pos_++;
    v.back().output_type = OutputType::FILE_APPEND;
  }
  else
  {
    v.back().output_type = OutputType::FILE_NEW;
  }
  out_redired_ = true;
  token_start_ = pos_ + 1;
}

void zhell::Parser::handle_comment(str_vec_t& v)
{
  if (double_quoted_ || single_quoted_)
  {
    return;
  }
  emplace_str_or_filename(v);
  pos_ = str_line_.size();
  token_start_ = pos_;
}

void zhell::Parser::emplace_str_or_filename(str_vec_t& v)
{
  if (out_redired_)
  {
    v.back().filename = temp_ + str_line_.substr(token_start_, pos_ - token_start_);
    temp_.clear();
    v.emplace_back(CommandLine {});
  }
  else
  {
    v.back().args.emplace_back(temp_ + str_line_.substr(token_start_, pos_ - token_start_));
    temp_.clear();
  }
}