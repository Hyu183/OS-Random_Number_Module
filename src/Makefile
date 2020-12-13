KDIR = /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=`pwd`
	$(CC) user_test.c -o user_test

clean:
	make -C $(KDIR) M=`pwd` clean
	rm user_test
	
