BUILD_DIR=bin
MODULE_DIR=modules

all:
	mkdir -p $(BUILD_DIR)/$(MODULE_DIR)
	
	$(MAKE) -C modexec
	cp ./modexec/bin/modexec $(BUILD_DIR)/modexec
	
	$(MAKE) -C $(MODULE_DIR)/sayhello
	cp $(MODULE_DIR)/sayhello/bin/sayhello.so $(BUILD_DIR)/$(MODULE_DIR)/sayhello.so

clean:
	rm -rf $(BUILD_DIR)/modexec
	rm -rf $(BUILD_DIR)/$(MODULE_DIR)/*.so
	
	$(MAKE) -C ./modexec clean
	$(MAKE) -C $(MODULE_DIR)/sayhello clean
