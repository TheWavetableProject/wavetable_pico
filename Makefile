PICO_SDK_PATH=../pico-sdk

debug:
	mkdir -p build && sed -e "s/<COMMIT\]/$(shell git log -1 --pretty=oneline | cut -c -8)/" main.c > build/out.c && cd build && export PICO_SDK_PATH=$(PICO_SDK_PATH) && cmake .. && make

