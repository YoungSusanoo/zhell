#include <iostream>

#include <parser.hpp>
#include <commands.hpp>

int main()
{
  zhell::Parser parser(std::cin);
  while (std::cin)
  {
    zhell::Parser::str_vec_t lines = parser.get_cmd();
    int curr_input = STDIN_FILENO;
    int next_input = STDIN_FILENO;
    int curr_output = STDOUT_FILENO;
    bool last_exec_success = true;
    for (auto& i : lines)
    {
      if (i.output_type == zhell::OutputType::NEXT_LINE)
      {
        int pipes[2];
        pipe(pipes);
        next_input = pipes[0];
        curr_output = pipes[1];
      }

      bool cond_to_execute = last_exec_success && i.connect_type != zhell::ConnectType::EXEC_IF_FAIL;
      cond_to_execute = cond_to_execute || (!last_exec_success && i.connect_type != zhell::ConnectType::NO_EXEC_IF_FAIL);
      if (cond_to_execute)
      {
        last_exec_success = zhell::exec_default(i.args, curr_input, curr_output);
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
  }
}
