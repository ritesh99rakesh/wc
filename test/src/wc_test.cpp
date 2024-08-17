#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "wc" ? 0 : 1;
}
