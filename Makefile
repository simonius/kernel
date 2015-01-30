

kernel:
	(cd kernel ; make)
	mv kernel/kernel simux

clean:
	(cd kernel ; make clean)


.PHONY: clean kernel
