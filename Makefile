PICO_SDK_PATH=../pico-sdk

debug:
	mkdir -p build && cd build && export PICO_SDK_PATH=$(PICO_SDK_PATH) && cmake .. && make

