#include <linux/module.h>
#include <linux/sched/task.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/printk.h>


struct task_struct *root = &init_task;
//int cur_level0 = 0;

void printTask(struct task_struct *task, int cur_level){
	int num_spaces = 4*min(cur_level,20);
	printk(KERN_INFO "%*c\\_ [%d] %s \n", num_spaces, ' ', task->pid, task->comm);
}

void traverseAll(struct task_struct *task, int cur_level){
	struct list_head *child;
	struct list_head *list;
	struct task_struct *curr;
	child = &task->children;
	if(child==NULL){
	    cur_level = 0;
	    return;
	}
	cur_level = cur_level+1;
	list_for_each(list, child){
	    curr = list_entry(list, struct task_struct, sibling);
	    printTask(curr, cur_level);
	    //cur_level = cur_level+1;
	    traverseAll(curr, cur_level);
	}
}

int tasklist_init(void)
{
	printk(KERN_INFO "Loading Module\n");

	printk(KERN_INFO "-- [%d] %s \n", root->pid, root->comm); 
	traverseAll(root,0);

	return 0;
}

void tasklist_exit(void)
{
	printk(KERN_INFO "Removing Module\n");
}

module_init( tasklist_init );
module_exit( tasklist_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Tasklist Module");
MODULE_AUTHOR("SGG");
