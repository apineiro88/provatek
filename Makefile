obj-m += netkm.o

netkm:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

udp_client: udp_client.c
	gcc -o udp_client udp_client.c

udp_server: udp_server.c
	gcc -o udp_server udp_server.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
