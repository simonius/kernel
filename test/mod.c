int main()
{
	int err;
	char hello[] = "Hello from elf";
	asm volatile( 	"mov $1, %%eax \n\t" // syscall number 1 (write)
			"mov $1, %%ebx \n\t" // fd for stdout
			"int $48 \n\t"
			: "=a"(err)
			: "c"(&hello[0]), "d"(sizeof(hello)) // adress of buf and size
			: "eax", "ebx", "ecx", "edx"
			);
	while(1);
}
