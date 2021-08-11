#include <torch/extension.h>
#include <benchmark/benchmark.h>

using namespace torch;
using namespace aten;
using namespace torch::autograd;

template<class... Inputs>
inline std::vector<c10::IValue> makeStack(Inputs&&... inputs) {
  return {std::forward<Inputs>(inputs)...};
}

// Simple case: tensor inputs and outputs
torch::Tensor my_test_op(const torch::Tensor& self, const torch::Tensor& other) {
 return self + other;
}

Tensor my_test_op_dispatched(const Tensor& self, const Tensor& other) {
#ifdef _BASELINE
  at::AutoDispatchBelowADInplaceOrView guard;
#endif
  static auto op = torch::Dispatcher::singleton()
    .findSchemaOrThrow("my_ops::my_test_op", "")
    .typed<decltype(my_test_op)>();
  return op.call(self, other);
}

void my_test_op_dispatched_boxed(const Tensor& self, const Tensor& other) {
#ifdef _BASELINE
  at::AutoDispatchBelowADInplaceOrView guard;
#endif
  static auto op = torch::Dispatcher::singleton()
    .findSchemaOrThrow("my_ops::my_test_op", "");
  auto stack = makeStack(self, other);
  op.callBoxed(&stack);
}

TORCH_LIBRARY(my_ops, m) {
  m.def("my_test_op(Tensor self, Tensor other) -> Tensor ");
}

TORCH_LIBRARY_IMPL(my_ops, CPU, m) {
  m.impl("my_test_op", my_test_op);
}

// Don't register autograd for baseline
#ifndef _BASELINE
TORCH_LIBRARY_IMPL(my_ops, Autograd, m) {
#ifdef _USE_BOXED
  m.impl("my_test_op", torch::CppFunction::makeFromBoxedFunction<&autogradNotImplementedFallback>());
#else
  m.impl("my_test_op", torch::autograd::autogradNotImplementedFallback2<torch::Tensor, const torch::Tensor&, const torch::Tensor&>(
    &my_test_op, "my_test_op(Tensor self, Tensor other) -> Tensor"));
#endif
}
#endif

static void op_call_no_requires_grad(benchmark::State& state) {
  torch::Tensor a = torch::ones({1, 2, 3}).set_requires_grad(false);
  torch::Tensor b = torch::ones({1, 2, 3}).set_requires_grad(false);
  for (auto _ : state) {
    my_test_op_dispatched(a, b);
  }
}
BENCHMARK(op_call_no_requires_grad);

static void op_call(benchmark::State& state) {
  torch::Tensor a = torch::ones({1, 2, 3}).set_requires_grad(true);
  torch::Tensor b = torch::ones({1, 2, 3}).set_requires_grad(true);
  for (auto _ : state) {
    my_test_op_dispatched(a, b);
  }
}
BENCHMARK(op_call);

static void op_call_boxed(benchmark::State& state) {
  torch::Tensor a = torch::ones({1, 2, 3}).set_requires_grad(true);
  torch::Tensor b = torch::ones({1, 2, 3}).set_requires_grad(true);
  for (auto _ : state) {
    my_test_op_dispatched_boxed(a, b);
  }
}
BENCHMARK(op_call_boxed);

BENCHMARK_MAIN();
