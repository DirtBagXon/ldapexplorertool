include GNUmakefile.config

all: src xrc

src:	
	cd src && make -f GNUmakefile 

xrc:

install: src xrc
	install src/$(TOOLNAME) $(TOOL_DIR)

uninstall:
	rm $(TOOL_DIR)/$(TOOLNAME)

clean:
	cd src && make -f GNUmakefile clean

.PHONY: src install
