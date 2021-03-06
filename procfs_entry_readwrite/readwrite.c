#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/tty.h>		/* For the tty declarations */
#include <linux/console.h>
#include <linux/usb.h>

#define MY_MODULE_VERSION "1.0"
#define MODULE_NAME "procfs_readwrite"

char rbuf[80];
static struct proc_dir_entry *e = NULL;
static struct proc_dir_entry *entry = NULL;
static struct proc_dir_entry *eroot = NULL;
static struct proc_dir_entry *entry2= NULL;

static void *proc_seq_count_start (struct seq_file *s, loff_t * pos);
static void *proc_seq_count_next (struct seq_file *s, void *v, loff_t * pos);
static void proc_seq_count_stop (struct seq_file *s, void *v);
static int proc_seq_count_show (struct seq_file *s, void *v);
static int proc_seq_count_open (struct inode *inode, struct file *file);


static struct seq_operations proc_seq_count_ops = {
  .start = proc_seq_count_start,
  .next = proc_seq_count_next,
  .stop = proc_seq_count_stop,
  .show = proc_seq_count_show
};

static void *proc_seq_count_start (struct seq_file *s, loff_t * pos)
{
  loff_t *spos = kmalloc (sizeof (loff_t), GFP_KERNEL);
  if (!spos)
    return NULL;
  *spos = *pos;
  return spos;
}

static void *proc_seq_count_next (struct seq_file *s, void *v, loff_t * pos)
{
  loff_t *spos = (loff_t *) v;
  *pos = ++(*spos);
  return spos;
}


static void proc_seq_count_stop (struct seq_file *s, void *v)
{
  kfree (v);
}


/*  This will list the sequence
 *
 *  $cat /proc/procreadwrite/sequence
 *
*/

static int proc_seq_count_show (struct seq_file *s, void *v)
{
  loff_t *spos = (loff_t *) v;
  seq_printf (s, "%Ld\n", *spos);
  return 0;
}

static int proc_seq_count_open (struct inode *inode, struct file *file)
{
  return seq_open (file, &proc_seq_count_ops);
};

static int proc_procreadwrite_show (struct seq_file *m, void *v)
{
  static int count = 0;
  char buf[80];

  snprintf (buf, 80, " Test count %d  \n", count);

  seq_puts (m, buf);
  count++;
  return 0;
}


static int proc_procreadwrite_open (struct inode *inode, struct file *file)
{
  return single_open (file, proc_procreadwrite_show, NULL);
}

static int mtty_write (char *text)
{
  struct tty_struct *my_tty;
  static int null_tty = 0;

  
  my_tty = get_current_tty(); 

  if (my_tty != NULL)
    {
      ((my_tty->driver->ops)->write) (my_tty,		// The tty itself //
				      text,		// String //
				      strlen (text));	// Length //

    }
  else
    {
      null_tty++;		// debug for is calling with no tty...it does nothing at the moment //
    }

  return strlen (text);


}


/* write is for write to the kernel 
   echo -n "stuff" /proc/procreadwrite/count
*/

static ssize_t proc_procreadwrite_write (struct file *file,
			  const char * buf, size_t size, loff_t * ppos)
{

  size_t len = 20;
  char mbuf[21];


  if (len > size)
    len = size;

  if (copy_from_user (mbuf, buf, len))
    return -EFAULT;

  mbuf[len] = '\0';

  mtty_write ("in the WRITE mbuf=->");
  mtty_write (mbuf);
  mtty_write ("<-");
  mtty_write ("\015\012");	/* crlf */


  return len;
}

static struct file_operations proc_procreadwrite_operations = {
  .owner=THIS_MODULE,
  .open = proc_procreadwrite_open,
  .read = seq_read,
  .write = proc_procreadwrite_write,
  .llseek = seq_lseek,
  .release = single_release,
};


static struct file_operations proc_seq_count_file_ops = {
  .owner = THIS_MODULE,
  .open = proc_seq_count_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = seq_release
};


static int __init proc_procreadwrite_init (void)
{
  int merr = 0;

  eroot = proc_mkdir (MODULE_NAME, NULL);
  if (eroot == NULL)
    {
      merr = -ENOMEM;
      return merr;
    }

  e = proc_create("count", 0666,eroot, &proc_procreadwrite_operations);	

  entry = proc_create ("sequence", 0,eroot,&proc_seq_count_file_ops );
  
  return 0;
}

static void __exit cleanup_procfs_procreadwrite (void)
{
  remove_proc_entry ("count", eroot);
  remove_proc_entry ("sequence", eroot);

  printk (KERN_INFO "%s %s removed\n", MODULE_NAME, MY_MODULE_VERSION);
}


module_init (proc_procreadwrite_init);
module_exit (cleanup_procfs_procreadwrite);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION ("procfs examples procreadwrite");
