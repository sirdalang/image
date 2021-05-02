TARGET=vir_target
JPEG_DIR=build_jpeg
EASYBMP_DIR=EasyBMP1.06

$(TARGET): $(JPEG_DIR) $(EASYBMP_DIR)
	make -C $(JPEG_DIR)
	make -C $(EASYBMP_DIR)

.PHONY:clean

clean:
	make -C $(JPEG_DIR) clean
	make -C $(EASYBMP_DIR) clean