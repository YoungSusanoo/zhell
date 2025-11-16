#include <iostream>
#include <queue>

#include <fcntl.h>
#include <wait.h>

#include <parser.hpp>
#include <commands.hpp>

int main()
{
  zhell::Parser parser(std::cin);
  std::queue< int > children;
  while (std::cin)
  {
    zhell::Parser::str_vec_t lines = parser.get_cmd();
    int curr_input = STDIN_FILENO;
    int next_input = STDIN_FILENO;
    int curr_output = STDOUT_FILENO;
    for (auto& i : lines)
    {
      if (i.output_type == zhell::OutputType::NEXT_LINE)
      {
        int pipes[2];
        pipe(pipes);
        next_input = pipes[0];
        curr_output = pipes[1];
      }
      else if (i.output_type == zhell::OutputType::FILE_APPEND)
      {
        curr_output = open(i.filename.c_str(), O_WRONLY | O_APPEND | O_CREAT);
      }
      else if (i.output_type == zhell::OutputType::FILE_NEW)
      {
        curr_output = open(i.filename.c_str(), O_WRONLY | O_TRUNC | O_CREAT);
      }

      if (i.connect_type == zhell::ConnectType::NONE)
      {
        children.emplace(zhell::exec_default(i.args, curr_input, curr_output));
      }
      else
      {
        int status = 0;
        while (!children.empty())
        {
          waitpid(children.front(), &status, 0);
          children.pop();
        }
        if (i.connect_type == zhell::ConnectType::EXEC_IF_FAIL && status)
        {
          children.emplace(zhell::exec_default(i.args, curr_input, curr_output));
        }
        else if (i.connect_type == zhell::ConnectType::NO_EXEC_IF_FAIL && !status)
        {
          children.emplace(zhell::exec_default(i.args, curr_input, curr_output));
        }
      }

      if (curr_input != STDIN_FILENO)
      {
        close(curr_input);
      }
      if (curr_output != STDOUT_FILENO)
      {
        close(curr_output);
      }

      curr_input = next_input;
      next_input = STDIN_FILENO;
      curr_output = STDOUT_FILENO;
    }

    while (!children.empty())
    {
      waitpid(children.front(), nullptr, 0);
      children.pop();
    }
  }
}
