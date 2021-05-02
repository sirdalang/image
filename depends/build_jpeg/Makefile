BUILD_DIR=./build
OUTPUT_DIR=./output
CONFIGURE=../../jpeg-9d/configure

TARGET=virtual_target

MKDIR=mkdir -p
RM=rm -rfv
CP=cp -v

$(TARGET):$(BUILD_DIR) $(OUTPUT_DIR) $(CONFIGURE)
	make -C $(BUILD_DIR) -j4
	make -C $(BUILD_DIR) install
	$(CP) output/include/* ../../include/jpeglib/
	$(CP) output/lib/libjpeg.a ../../lib/

$(BUILD_DIR):
	$(MKDIR) $@

$(OUTPUT_DIR):
	$(MKDIR) $@

$(CONFIGURE): $(BUILD_DIR)
	cd $(BUILD_DIR) && $(CONFIGURE) --prefix=$(shell pwd)/$(OUTPUT_DIR)

.PHONY: clean

clean:
	$(RM) $(BUILD_DIR)
	$(RM) $(OUTPUT_DIR)