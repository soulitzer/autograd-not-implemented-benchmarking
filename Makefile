SHELL := /bin/bash
TORCH_PREFIX_PATH=`python -c 'import torch;print(torch.utils.cmake_prefix_path)'`

.PHONY: all clean build run

all: clean build run

init: clean-all clone build run

clone:
	git clone https://github.com/google/benchmark && \
	git clone https://github.com/google/googletest.git benchmark/googletest

build:
	mkdir -p build && \
		pushd build && \
		cmake -DCMAKE_PREFIX_PATH=$(TORCH_PREFIX_PATH)/Torch -DCMAKE_BUILD_TYPE=Release .. && \
		cmake --build . --config Release -j 32 && \
		popd; \
		echo "" && \
		echo "Note: " && \
		echo "1. If failed to build, make sure you are in the right conda environment " && \
		echo "2. Use 'sudo cpupower frequency-set --governor performance' to disable cpu scaling" && \
		echo ""

run:
	pushd build && \
		echo "----------- Baseline -----------" && \
		./example-app-baseline && \
		echo "----------- Templated -----------" && \
		./example-app-templated && \
		echo "----------- Boxed -----------" && \
		./example-app-boxed && \
		popd

clean:
	rm -rf build

clean-all:
	rm -rf build && rm -rf benchmark
