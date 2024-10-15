ROOT_DIR  = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BUILD_DIR = $(ROOT_DIR)/build

_CMAKE    = $(BUILD_DIR)/Makefile

$(_CMAKE):
	@cmake -S $(ROOT_DIR) -B $(BUILD_DIR)

build: $(_CMAKE)
	@cmake --build $(BUILD_DIR)
.PHONY: build

ldd:
	@otool -L $(BUILD_DIR)/dots
.PHONY: ldd

info:
	@otool -l $(BUILD_DIR)/dots
.PHONY: info

clean:
	@rm -rf $(BUILD_DIR)
.PHONY: clean
