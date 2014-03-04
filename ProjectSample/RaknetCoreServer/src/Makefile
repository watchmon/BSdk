SUBDIRS = Common NetworkModule RaknetSource Application

recursive:
	for i in $(SUBDIRS);\
	do\
		cd $$i;\
			if [ $$? -ne 0 ]; then\
				exit 1;\
			fi;\
		make;\
		if [ $$? -ne 0 ]; then\
			exit 1;\
		fi;\
		cd ..;\
	done;\

clean:
	-rm ./../bin/*
	for i in $(SUBDIRS);\
	do\
		cd $$i;\
		make clean;\
		if [ $$? -ne 0 ] ; then\
			exit 1;\
		fi; \
		cd ..;\
	done;\
