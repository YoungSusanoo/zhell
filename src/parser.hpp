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
    Parser(std::istream& in);
    std::unique_ptr< CommandLine > get_cmd();

  private:
    std::unordered_map< char, void (Parser::*)() > symbols_;
    std::unique_ptr< CommandLine > start_line_;
    std::unique_ptr< CommandLine > curr_line_;
    std::string str_line_;
    std::string temp_;
    size_t token_start_;
    size_t pos_;
    bool escaped_;
    bool double_quoted_;
    std::istream& in_;

    void handle_ampersand();
    void handle_slash();
    void handle_double_quote();
    void handle_pipe();
    void handle_space();
  };
}

#endif