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
    using lines_t = std::vector< CommandLine >;

    Parser(std::istream& in);
    lines_t get_cmd();

  private:
    std::unordered_map< char, void (Parser::*)(lines_t&) > symbols_;
    std::string str_line_;
    std::string temp_;
    size_t token_start_;
    size_t pos_;
    bool escaped_;
    bool double_quoted_;
    std::istream& in_;

    void clean();

    void handle_ampersand(lines_t& v);
    void handle_slash(lines_t& v);
    void handle_double_quote(lines_t& v);
    void handle_pipe(lines_t& v);
    void handle_space(lines_t& v);
  };
}

#endif