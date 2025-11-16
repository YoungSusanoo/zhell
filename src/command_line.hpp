#ifndef COMMAND_LINE_HPP
#define COMMAND_LINE_HPP

#include <memory>
#include <string>

namespace zhell
{
  enum class ConnectType
  {
    NONE,
    EXEC_IF_FAIL,
    NO_EXEC_IF_FAIL
  };

  enum class OutputType
  {
    STD_OUT,
    FILE_APPEND,
    FILE_NEW,
    NEXT_LINE
  };

  struct CommandLine
  {
    std::vector< std::string > args;
    std::string filename;
    ConnectType connect_type;
    OutputType output_type;
    bool background;
  };
}

#endif