#include<stdio.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>

int main(){
	long int sys = syscall(548);
	printf("Syscall return %ld\n",sys);
	return 0;
}
