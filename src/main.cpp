// Tests
#include "test_runner.hpp"

int main() {
  symmetric_inverse_bench();
  asymmetric_inverse_bench();

  symmetric_eigen_bench();
  asymmetric_eigen_bench();
  return 0;
}
