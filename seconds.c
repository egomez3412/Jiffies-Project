/**
 * hello.c
 *
 * Kernel module that communicates with /proc file system.
 * 
 * The makefile must be modified to compile this program.
 * Change the line "simple.o" to "hello.o"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <asm/param.h>

#define BUFFER_SIZE 128

#define PROC_NAME "seconds"
//unsigned long start_time, end_time;
long jiffies_on_load = 0;
long jiffies_delta = 0;
/**
 * Function prototypes
 */

ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
};


/* This function is called when the module is loaded. */
int proc_init(void)
{

        // creates the /proc/hello entry
        // the following function call is a wrapper for
        // proc_create_data() passing NULL as the last argument
        proc_create(PROC_NAME, 0, NULL, &proc_ops);
        
        //Create start time for elapsed time.
        //start_time = jiffies/HZ;
        jiffies_on_load = jiffies;
        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
        printk(KERN_INFO "I proc_init(), Jiffies is: %lu and HZ is: %d\n", jiffies, HZ);
        printk(KERN_INFO "I proc_init(), seconds is: %lu s\n", (jiffies - jiffies_on_load)/HZ);

	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {

        // removes the /proc/hello entry
        remove_proc_entry(PROC_NAME, NULL);

        printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/hello is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;

        if (completed) {
                completed = 0;
                return 0;
        }

        completed = 1;
        
        //Create end time for elapsed time.
        //end_time = jiffies/HZ;

        rv = sprintf(buffer, "\nKernal is Running: %lu sec\n", (jiffies - jiffies_on_load)/HZ);
        printk(KERN_INFO "%s", buffer);
        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");

//==========================================================
// shell script for seconds.ko
//==========================================================
// if [ "$#" -ne 1 ]; then
//     echo "Illegal number of parameters -- Usage: ./insrem.sh modulename"
//     exit
// fi

// sudo -p "osc"
// sudo dmesg -c 
// clear 

// printf "beginning script...\n"
// printf "Module name is: %s\n\n" $1".ko"

// sudo insmod $1".ko"

// for time in {1..4}
// do
//   printf "\tsleeping for %s second" $time
//   if [ "$time" -gt "1" ]; then 
//     echo "s"
//   fi
//   echo "..."

//   sleep $time
//   cat "/proc/"$1
// done
// printf "\n"

// sudo rmmod $1 
// sudo dmesg 

// printf "\n...end of script\n\n"
//==========================================================
// OUTPUT from seconds.ko
//==========================================================
// beginning script...
// Module name is: seconds.ko

//         sleeping for 1 second...

// Kernal is Running: 1 sec
//         sleeping for 2 seconds
// ...

// Kernal is Running: 3 sec
//         sleeping for 3 seconds
// ...

// Kernal is Running: 6 sec
//         sleeping for 4 seconds
// ...

// Kernal is Running: 10 sec

// [ 3363.373879] /proc/seconds created
// [ 3363.373881] I proc_init(), Jiffies is: 4295732997 and HZ is: 250
// [ 3363.373882] I proc_init(), seconds is: 0 s
// [ 3364.378418] 
//                Kernal is Running: 1 sec
// [ 3366.380234] 
//                Kernal is Running: 3 sec
// [ 3369.385525] 
//                Kernal is Running: 6 sec
// [ 3373.391179] 
//                Kernal is Running: 10 sec
// [ 3373.395207] /proc/seconds removed

// ...end of script
//==========================================================
// end of OUTPUT from seconds.ko
//==========================================================