SUBDIRS	= kwq kwq_delay init_empty udpsocket \
	  varext wait

all: subdirs

subdirs:
	for n in $(SUBDIRS); do $(MAKE) -C $$n || exit1; done

clean:
	for n in $(SUBDIRS); do $(MAKE) -C $$n clean; done
