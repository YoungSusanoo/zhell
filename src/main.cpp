#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include <parser.hpp>

int main()
{
  zhell::CommandLine line = zhell::get_cmd(std::cin);
  std::cout << line.args.front();
  execlp(line.args.front().c_str(), line.args.front().c_str(), line.args[1].c_str(), (char*)NULL);
}
