### To Replicate
1. Checkout the correct branch https://github.com/pytorch/pytorch/pull/62450
2. Build PyTorch
3. Clone this repo, and run `make init` in the same directory. Also make sure cmake is looking at the libs for the correct PyTorch (that you just built).
4. Run `sudo cpupower frequency-set --governor performance` to disable CPU scaling if necessary (I haven't observed a big difference from this)

Results:
```
----------- Baseline -----------
2021-08-11T19:11:36-04:00
Running ./example-app-baseline
Run on (32 X 4000.9 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 512 KiB (x16)
  L3 Unified 16384 KiB (x4)
Load Average: 1.89, 1.35, 3.84
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
op_call_no_requires_grad        804 ns          804 ns       843810
op_call                        1516 ns         1517 ns       460224
op_call_boxed                  1586 ns         1586 ns       437516
----------- Templated -----------
2021-08-11T19:11:39-04:00
Running ./example-app-templated
Run on (32 X 3592.73 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 512 KiB (x16)
  L3 Unified 16384 KiB (x4)
Load Average: 1.82, 1.34, 3.82
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
op_call_no_requires_grad        838 ns          838 ns       830875
op_call                        1639 ns         1639 ns       425610
op_call_boxed                  1691 ns         1691 ns       412156
----------- Boxed -----------
2021-08-11T19:11:41-04:00
Running ./example-app-boxed
Run on (32 X 3781.05 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 512 KiB (x16)
  L3 Unified 16384 KiB (x4)
Load Average: 1.82, 1.34, 3.82
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
op_call_no_requires_grad        909 ns          909 ns       783265
op_call                        1688 ns         1688 ns       413550
op_call_boxed                  1683 ns         1683 ns       421396
```

Note: this is probably not the best baseline since if it is still running as an AutogradCompositeImplicit op it is still building the graph and possibly dispatching into more autograd kernels, but still an interesting point of comparison.
