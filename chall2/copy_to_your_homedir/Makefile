# -*- mode: Makefile -*-
all: cleanall prepare_demo

#clear_cache:clear_cache.c
#	gcc -o clear_cache clear_cache.c
#	@sudo chown root:root clear_cache
#	@sudo chmod +s clear_cache
#	@sudo chmod ugo+x clear_cache

#scat:suid_cat.c
#	@echo "[-] create suid prog"
#	@gcc -o scat suid_cat.c
#	@sudo chown root:root scat
#	@sudo chmod +s scat
#	@sudo chmod ugo+x scat

#private:
#	@echo "[-] create private file"
#	@echo "Congrats ! You access this file and its very secret information: " > private
#	@echo "SUPERSECRET" >> private
#	@sudo chown root:root private	
#	@sudo chmod 600 private

#public:
#	@echo "[-] create public file"
#	@echo "PUBLIC FILE" > public
#	@echo "NOBODY CARES WHAT IS IN THIS FILE" >> public
#	@sudo chmod 644 public

clean:
	@rm -f scat 

#cleanall: clean
#	@echo "[-] clean all"
#	@rm -rf goodmaze badmaze chain* entry exit 
#	@rm -f public private file clear_cache

prepare_demo: #cleanall scat public private clear_cache
	@echo "[-] setup maze"
	@echo " ***TODO***  insert here commands to create the maze"

	@echo "[-] setup original link to maze"
	@ln -s entry/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk/lnk file
	@echo 
	@echo "for the demo run ./cmd in one terminal and ./attack in another terminal before cmd completes"
	@echo "running ./clear_cache may help flushing the cache"
