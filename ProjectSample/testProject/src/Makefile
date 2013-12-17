SUBDIRS = common module1 module2 application

recursive:
	for i in $(SUBDIRS);\
	do\
		cd $$i;\
			if [ $$? -ne 0 ]; then\
				exit 1;\
			fi;\
		gmake;\
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
		gmake clean;\
		if [ $$? -ne 0 ] ; then\
			exit 1;\
		fi; \
		cd ..;\
	done;\
