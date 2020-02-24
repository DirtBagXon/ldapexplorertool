include GNUmakefile.config


all: src xrc

src:	
	cd src && make -f GNUmakefile 

xrc:

install: src xrc
	install -d $(XRC_DIR)
	install src/$(TOOLNAME) $(TOOL_DIR)
	install -m644 xrc/$(TOOLNAME).xrc $(XRC_DIR)

uninstall:
	rm $(TOOL_DIR)/$(TOOLNAME)
	rm $(XRC_DIR)/$(TOOLNAME).xrc
	rmdir $(XRC_DIR)/$(TOOLNAME)
clean:
	cd src && make -f GNUmakefile clean

.PHONY: src install
