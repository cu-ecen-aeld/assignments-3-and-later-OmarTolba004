CROSS_COMPILE = 

writer: writer.c
ifeq ($(CROSS_COMPILE),)
	@gcc -o writer writer.c
else
	@$(CROSS_COMPILE)gcc -o writer writer.c
endif

clean:
	@rm -f writer
	@rm -f *.o
