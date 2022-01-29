#include <linux/module.h>
#include <linux/list.h>
#include <linux/printk.h>
#include <linux/slab.h>

static LIST_HEAD(pokedex);

struct pokemon {
	char name[32];
	int dex_no;
	struct list_head list;
};

void print_pokemon(struct pokemon *p)
{
	printk(KERN_INFO "%s: National Dex No. #%d\n", p->name, p->dex_no);
}

/* TODO: declare a single static struct list_head, named pokedex */

void add_pokemon(char *name, int dex_no)
{
	/* TODO: write your code here */
	struct pokemon *new_pokemon;
	new_pokemon = kmalloc(sizeof(*new_pokemon),GFP_KERNEL);
	strcpy(new_pokemon->name, name);
	new_pokemon->dex_no = dex_no;
	INIT_LIST_HEAD(&new_pokemon->list);
	list_add_tail(&new_pokemon->list, &pokedex);
}

void print_pokedex(void)
{
	/* TODO: write your code here, using print_pokemon() */
	struct pokemon *curr;

	list_for_each_entry(curr, &pokedex,list){
	    print_pokemon(curr);
	}
}

void delete_pokedex(void)
{
	/* TODO: write your code here */
	struct pokemon *del;
	struct pokemon *next;
	list_for_each_entry_safe(del, next, &pokedex, list){
	    list_del(&del->list);
	    kfree(del);
	}
}

int pokedex_init(void)
{
	printk(KERN_INFO "Loading Module\n");

	add_pokemon("Porygon-Z", 474);
	add_pokemon("Exeggutor", 103);
	add_pokemon("Poliwhirl", 61);
	add_pokemon("Infernape", 392);

	print_pokedex();

	return 0;
}

void pokedex_exit(void)
{
	printk(KERN_INFO "Removing Module\n");

	print_pokedex();

	delete_pokedex();
}

module_init( pokedex_init );
module_exit( pokedex_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A handy Pokedex module");
MODULE_AUTHOR("SGG");
