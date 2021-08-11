### To Replicate
1. Checkout the correct branch https://github.com/pytorch/pytorch/pull/62450
2. Build PyTorch
3. Clone this repo, and run `make init` in the same directory. Also make sure cmake is looking at the libs for the correct PyTorch (that you just built).
4. Run `sudo cpupower frequency-set --governor performance` to disable CPU scaling if necessary (I haven't observed a big difference from this)

Results:
```
----------- Baseline -----------
2021-08-11T19:56:42-04:00
Running ./example-app-baseline
Run on (32 X 3592.81 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 512 KiB (x16)
  L3 Unified 16384 KiB (x4)
Load Average: 0.71, 0.63, 0.63
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
op_call_no_requires_grad        699 ns          699 ns       962985
op_call                         679 ns          679 ns      1002686
op_call_boxed                   736 ns          736 ns       961718
----------- Templated -----------
2021-08-11T19:56:45-04:00
Running ./example-app-templated
Run on (32 X 3598.34 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 512 KiB (x16)
  L3 Unified 16384 KiB (x4)
Load Average: 0.71, 0.63, 0.63
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
op_call_no_requires_grad        847 ns          847 ns       820522
op_call                        1659 ns         1659 ns       421860
op_call_boxed                  1707 ns         1707 ns       404835
----------- Boxed -----------
2021-08-11T19:56:48-04:00
Running ./example-app-boxed
Run on (32 X 4155.37 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 512 KiB (x16)
  L3 Unified 16384 KiB (x4)
Load Average: 0.73, 0.64, 0.63
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
op_call_no_requires_grad        910 ns          910 ns       759083
op_call                        1725 ns         1725 ns       405478
op_call_boxed                  1715 ns         1715 ns       408859
```
