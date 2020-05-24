#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/delay.h>
#include<linux/slab.h>

#define BUFFER_SIZE 5
static int mod_init(void);
static void mod_exit(void);

static struct task_struct *prod;
static struct task_struct *cons;

struct mutex mut;

int buffer[BUFFER_SIZE];
int i,j,count;


int producer(void* data)
{
	while(!kthread_should_stop()){
		mutex_lock(&mut);
		if(i==-1 && j==-1){
			i++;j+=2;
			printk(KERN_INFO "producer produced item: %d \n",0);
			count+=1;
		}
		if(count==BUFFER_SIZE) {
			printk(KERN_INFO "Buffer Full\n");
		}
		else{
//			if(i==-1)
//				i=0;
			printk(KERN_INFO "Producer produced item: %d \n",j);
			j=(j+1)%BUFFER_SIZE;
			count+=1;
		}
		mutex_unlock(&mut);
		msleep(1000);
	}
	return 0;
}

int consumer(void* data)
{
	while(!kthread_should_stop()){
		mutex_lock(&mut);
		if(count==0){
			printk(KERN_INFO "Buffer empty\n");
		}
		else{
			printk(KERN_INFO "Consumer consumed item: %d \n",i);
			i=(i+1)%BUFFER_SIZE;
			count-=1;
//			if(i<0){
//				i=(i+BUFFER_SIZE)%BUFFER_SIZE;
//			}
//			else{
//				i=i%BUFFER_SIZE;
//			}
		}
		mutex_unlock(&mut);
		msleep(5000);
	}
	return 0;
}

static int mod_init(void)
{
	printk(KERN_INFO "Loaded module\n");
	i=-1;
	j=-1;
	count=0;
	mutex_init(&mut);
	prod = kthread_run(producer,NULL, "producer_thread");
	if(prod){
		printk(KERN_INFO "Succesfully started producer thread\n");
	}
	else{
		printk(KERN_ERR "Error in starting producer thread\n");
	}

	cons = kthread_run(consumer,NULL, "consumer_thread");
	if(prod){
		printk(KERN_INFO "Succesfully started consumer thread\n");
	}
	else{
		printk(KERN_ERR "Error in starting consumer thread\n");
	}
	return 0;
}

static void mod_exit(void)
{
	kthread_stop(prod);
	kthread_stop(cons);	
	printk(KERN_INFO "Module unloaded\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akshit");
MODULE_DESCRIPTION("A simple producer consumer problem");
