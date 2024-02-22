CFLAGS += -Wall -ggdb -I/usr/include/tirpc

.PHONY: all
all: fs454-client fs454-server
	$(CC) -Wall $^ -lfuse3 -lpthread -o $@

fs454-client: src/fs454_fuse.o src/fs454_clnt.o src/fs454_xdr.o
	$(CC) -ggdb -Wall $^ -ltirpc -lfuse3 -lpthread -o $@

fs454-server: src/fs454_server.o src/fs454_svc.o src/fs454_xdr.o src/file_tree.o src/inode_tree.o src/fsio.o
	$(CC) -Wall $^ -ltirpc -o $@

.PHONY: clean
clean:
	rm -f src/*.o fs454-client fs454-server