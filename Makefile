ROOT_DIR  = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BUILD_DIR = $(ROOT_DIR)/build
UNAME     = $(shell uname -s)

ifeq ($(UNAME),Darwin)
APP_NAME  = dots.app
else
APP_NAME  = dots
endif

_CMAKE    = $(BUILD_DIR)/Makefile


$(_CMAKE):
	@cmake -S $(ROOT_DIR) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release

build: $(_CMAKE)
	@export MACOSX_DEPLOYMENT_TARGET=10.9
	@cmake --build $(BUILD_DIR)
ifeq ($(UNAME),Darwin)
	@codesign --remove-signature "$(BUILD_DIR)/$(APP_NAME)"
	@codesign --force --deep --sign - "$(BUILD_DIR)/$(APP_NAME)"
endif
.PHONY: build


ifeq ($(UNAME),Darwin)

DMG_DIR = $(BUILD_DIR)/dmg
dmg: build
	@mkdir -p $(DMG_DIR)
	@cp -r $(BUILD_DIR)/$(APP_NAME) $(DMG_DIR)/Dots.app
	@hdiutil create $(BUILD_DIR)/Dots.dmg \
		-volname "Dots" \
		-fs HFS+ \
		-srcfolder $(DMG_DIR) \
		-ov -format UDZO
	@rm -rf $(DMG_DIR)
.PHONY: dmg
endif

ldd:
	@otool -L $(BUILD_DIR)/dots
.PHONY: ldd

info:
	@otool -l $(BUILD_DIR)/dots
.PHONY: info

clean:
	@rm -rf $(BUILD_DIR)
.PHONY: clean
