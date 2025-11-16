#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

#include <command_line.hpp>

namespace zhell
{
  class Parser
  {
  public:
    using str_vec_t = std::vector< CommandLine >;

    Parser(std::istream& in);
    str_vec_t get_cmd();

  private:
    std::unordered_map< char, void (Parser::*)(str_vec_t&) > symbols_;
    std::string str_line_;
    std::string temp_;
    size_t token_start_;
    size_t pos_;
    bool escaped_;
    bool double_quoted_;
    bool out_redired_;
    std::istream& in_;

    void clean();

    void handle_ampersand(str_vec_t& v);
    void handle_slash(str_vec_t& v);
    void handle_double_quote(str_vec_t& v);
    void handle_pipe(str_vec_t& v);
    void handle_space(str_vec_t& v);
    void handle_out_redir(str_vec_t& v);

    void emplace_str_or_filename(str_vec_t& v);
  };
}

#endif