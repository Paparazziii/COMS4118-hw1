#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>

/*
 * Undefine commonly used macros -- DO NOT MODIFY
 * Please also do not use other macros and functions defined in <linux/list.h>,
 * such as INIT_LIST_HEAD, list_add, list_add_tail, list_del, list_empty,
 * or any of the internal helper functions beginning with __.
 */
#undef LIST_HEAD
#undef LIST_HEAD_INIT
#undef list_entry
#undef list_for_each_entry
#undef list_for_each_entry_safe
#undef container_of
#undef offsetof
#undef READ_ONCE
#undef WRITE_ONCE

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

struct list_head pokedex = {&pokedex, &pokedex};

void add_pokemon(char *name, int dex_no)
{
	/* TODO: write your code here */
	struct pokemon *new_pokemon;
	struct list_head *prev = pokedex.prev;
	new_pokemon = kmalloc(sizeof(*new_pokemon),GFP_KERNEL);
	strcpy(new_pokemon->name, name);
	new_pokemon->dex_no = dex_no;
	pokedex.prev = &new_pokemon->list;
	new_pokemon->list.next = &pokedex;
	new_pokemon->list.prev = prev;
	prev->next = &new_pokemon->list;

}

void print_pokedex(void)
{
	/* TODO: write your code here, using print_pokemon() */
	struct pokemon *curr;
	const typeof(((struct pokemon *)0)->list) *_mptr = (pokedex.next);
	for(curr = (struct pokemon *)((char *)_mptr - ((size_t)&((struct pokemon *)0)->list));&curr->list != (&pokedex);curr=(struct pokemon*)((char *)(curr->list.next)-((size_t)&((struct pokemon *)0)->list))){
	print_pokemon(curr);
	}
}

void delete_pokedex(void)
{
	struct pokemon *del;
	struct pokemon *next;
	const typeof(((struct pokemon *)0)->list) *_mptr = (pokedex.next);
	for(del=(struct pokemon *)((char *)_mptr - ((size_t)&((struct pokemon *)0)->list)), next=(struct pokemon *)((char*)(del->list.next)-((size_t)&((struct pokemon *)0)->list)); &del->list!=(&pokedex);del=next,next=(struct pokemon *)((char *)(next->list.next)-((size_t)&((struct pokemon *)0)->list))){
	struct pokemon *temp = del;
	del->list.prev->next = del->list.next;
	del->list.next->prev = temp->list.prev;	
	kfree(del);
	}
}

int pokedex_nom_init(void)
{
	printk(KERN_INFO "Loading Module\n");

	add_pokemon("Porygon-Z", 474);
	add_pokemon("Exeggutor", 103);
	add_pokemon("Poliwhirl", 61);
	add_pokemon("Infernape", 392);

	print_pokedex();

	return 0;
}

void pokedex_nom_exit(void)
{
	printk(KERN_INFO "Removing Module\n");

	print_pokedex();

	delete_pokedex();
}

module_init( pokedex_nom_init );
module_exit( pokedex_nom_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A handy Pokedex module, without list macros");
MODULE_AUTHOR("SGG");
