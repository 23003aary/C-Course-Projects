#include "a1.h"

Restaurant* initialize_restaurant(char* name){
	struct Restaurant* init_restaurant = malloc(sizeof(struct Restaurant));
	init_restaurant->name = malloc(sizeof(char)*(strlen(name) + 1));
	strcpy(init_restaurant->name, name);

	// initialize menu here, after load_menu function is completed
	init_restaurant->menu = load_menu(MENU_FNAME);

	init_restaurant->num_completed_orders = 0;
	init_restaurant->num_pending_orders = 0;

	init_restaurant->pending_orders = malloc(sizeof(struct Queue));
	init_restaurant->pending_orders->head = NULL;
	init_restaurant->pending_orders->tail = NULL;
	// pending_queue is intitialized to a queue with its head and tail set to NULL
	// init_restaurant->pending_orders = init_pending_orders;

	return init_restaurant;
}

Menu* load_menu(char* fname){
	struct Menu* init_menu = malloc(sizeof(struct Menu));
	int init_num_items = 0;

	FILE* fp1 = fopen(fname, "r");
	char line1[1000];

	while (fgets(line1, 1000, fp1) != NULL) {
		init_num_items += 1;
	}
	fclose(fp1);

	// char* init_item_codes[init_num_items];
	// char* init_item_names[init_num_items];
	// double init_item_cost_per_unit[init_num_items];

	FILE* fp2 = fopen(fname, "r");
	char line2[1000];

	init_menu->item_codes = (char**)malloc(sizeof(char*)*init_num_items);
	init_menu->item_names = (char**)malloc(sizeof(char*)*init_num_items);
	init_menu->item_cost_per_unit = (double*)malloc(sizeof(double)*init_num_items);

	int counter = 0;
	while (fgets(line2, 1000, fp2) != NULL) {
		//char* token_1 = (char*)malloc(sizeof(char)*ITEM_CODE_LENGTH);
		//char* token_2 = (char*)malloc(sizeof(char)*MAX_ITEM_NAME_LENGTH);
		//char* token_3_char = (char*)malloc(sizeof(char)*8);
		init_menu->item_codes[counter] = (char*)malloc(sizeof(char)*ITEM_CODE_LENGTH);
		init_menu->item_names[counter] = (char*)malloc(sizeof(char)*MAX_ITEM_NAME_LENGTH);

		char* token_1 = strtok(line2, MENU_DELIM); // token_1 corresponds to the item code
		char* token_2 = strtok(NULL, MENU_DELIM); // token_2 corresponds to the item name
		char* token_3_char_$ = strtok(NULL, MENU_DELIM); // token_3 corresponds to the item cost per unit
		
		// remove white spaces before and after token
		// should you subtract 2 to account for null character???
		/*
		if (token_1[0] == ' ') {
			int count_1 = 0;
			char* nospace_token_1[ITEM_CODE_LENGTH];
			while (token_1[count_1] == ' ') {
				strcpy(nospace_token_1, token_1[count_1]);
			}
		}
		else {
			strcpy(init_menu->item_codes[counter], token_1); // init_item_codes[counter] = token_1;
		}
		*/

		// we have to get rid of the dollar sign and convert to double
		char* token_3_char = (char*)malloc(sizeof(char)*100);
		strcpy(token_3_char, (token_3_char_$ + 1));
		double token_3 = atof(token_3_char);

		strcpy(init_menu->item_codes[counter], token_1); // init_item_codes[counter] = token_1;
		strcpy(init_menu->item_names[counter], token_2); // init_item_names[counter] = token_2;
		init_menu->item_cost_per_unit[counter] = token_3;

		counter += 1;
    	free(token_3_char);
	}
	init_menu->num_items = init_num_items;
	fclose(fp2);

	return init_menu;
}

// return the floating point cost of the item <item_code> as given by <menu>
/*
	1. Search list for item_code and store idx
	2. Return value at idx in list menu->item_cost_per_unit 
*/

Order* build_order(char* items, char* quantities) {

	Order* init_order = (Order*)malloc(sizeof(Order));

	int init_num_items = strlen(items)/(ITEM_CODE_LENGTH - 1);
	init_order->num_items= init_num_items; 

	char quant_array[100];
	strcpy(quant_array, quantities);

	
	init_order->item_codes = (char**)malloc(sizeof(char*)*init_num_items);
	init_order->item_quantities = (int*)malloc(sizeof(int)*init_num_items);


	char* quant_token = strtok(quant_array, MENU_DELIM);

	int count = 0;


	while (quant_token != NULL) {
		int quant_token_int = atoi(quant_token);
		init_order->item_quantities[count] = quant_token_int;
		quant_token = strtok(NULL, MENU_DELIM);
		count++;
	}


	/*for(count=0; count<size; count++){
		printf("lol swag for loop\n");
		if(toktok != NULL){
			item_quantz[count] = strtol(toktok, NULL, 10);
			printf("wow intem qunts\n");
			//toktok = strtok(NULL, MENU_DELIM);
		}
	}
	while(quant_token != NULL) {
		item_quantz[count++] = strtol(toktok, NULL, 10);
		toktok = strtok(NULL, MENU_DELIM);
	}
	*/

	// extract separate strings from items and quantities
	for (int i = 0; i < init_num_items; i++) {
		init_order->item_codes[i] = (char*)malloc(sizeof(char)*ITEM_CODE_LENGTH);
		char init_item_code[ITEM_CODE_LENGTH];
		init_item_code[2] = '\0';
		init_item_code[ITEM_CODE_LENGTH -1];
		int idx = i*(ITEM_CODE_LENGTH - 1);


		for (int j = 0; j < (ITEM_CODE_LENGTH - 1); j++) {
			// init_order->item_codes[i][j] = items[idx + j];
			init_item_code[j] = items[idx+j];
		}
		strcpy(init_order->item_codes[i], init_item_code);
	}
	return init_order; 
}

void enqueue_order(Order* order, Restaurant* restaurant) {
	QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
	new_node->order = order;
	new_node->next = NULL;

	// the beginning of the list is the beginning of our queue
	if (restaurant->pending_orders->head == NULL) {
		restaurant->pending_orders->head = new_node;
		restaurant->pending_orders->tail = new_node;
	}

	restaurant->pending_orders->tail->next = new_node;
	restaurant->num_pending_orders += 1;

	free(new_node);
}

Order* dequeue_order(Restaurant* restaurant) {
	if(restaurant->pending_orders->head == NULL) {
		return NULL;
	}

	QueueNode* temp = restaurant->pending_orders->head;

	restaurant->pending_orders->head = restaurant->pending_orders->head->next;
	temp->next = NULL;

	Order* order_storage = temp->order;

	restaurant->num_completed_orders ++;
	restaurant->num_pending_orders --;
	free(temp);

	if(restaurant->num_pending_orders == 0) {
		restaurant->pending_orders->tail = NULL;
	}

	return order_storage;
}

double get_item_cost(char* item_code, Menu* menu) {
	int idx;
	for (int i = 0; i < menu->num_items; i++) {
		if (menu->item_codes[i] == item_code) {
			idx = i;
		}
	}

	float item_cost = menu->item_cost_per_unit[idx];

	return item_cost;
}


// return floating point cost of all the items in the <order>, with their respective quantities,
// with item costs given by menu

/*
	1. Get the item cost for each item specified by the item codes
	2. Multiply this cost by the number of each item
	3. Return total
*/
double get_order_subtotal(Order* order, Menu* menu) {
	double subtotal;
	int idx;
	for (int i = 0; i < order->num_items; i++) {
		for (int j = 0; j < menu->num_items; j++) {
			if (strcmp(menu->item_codes[j], order->item_codes[i]) == 0) {
				idx = j;
			}
		}
		subtotal += (menu->item_cost_per_unit[idx])*(order->item_quantities[i]); 
	}
	return subtotal;
}

double get_order_total(Order* order, Menu* menu) {
	double subtotal = get_order_subtotal(order, menu);
	double tax_rate = (TAX_RATE/100) + 1;
	double total = subtotal*tax_rate;

	return total;
}

// everytime dequeue is called, num_completed_orders should decrease by one
int get_num_completed_orders(Restaurant* restaurant) {
	return restaurant->num_completed_orders;
}

// everytime enqueue is called, num_pending_orders should increase by one
int get_num_pending_orders(Restaurant* restaurant) {
	/*
	QueueNode* cur_node = restaurant->pending_orders->head;
	int len_pending_orders = 1;
	while (cur_node->next != NULL) {
		len_pending_orders += 1;
		cur_node = cur_node->next;
	}
	restaurant->num_pending_orders = len_pending_orders;
	*/
	return restaurant->num_pending_orders;
}

void clear_menu(Menu** menu ) {
  for (int i = 0; i < (*menu)->num_items; i++) {
    free((*menu)->item_codes[i]);
    free((*menu)->item_names[i]);
  }
  free((*menu)->item_cost_per_unit);
  free((*menu)->item_codes);
  free((*menu)->item_names);
  free(*menu);
  *menu = NULL;
  // check assignment instructions
}

void clear_order(Order** order) {
	for(int i = 0; i < (*order)->num_items; i++){
		free((*order)->item_codes[i]);
	} 
	free((*order)->item_quantities);
	free((*order)->item_codes);
	free(*order);
	*order = NULL;
}

void close_restaurant(Restaurant** restaurant) {
	/*
	QueueNode* cur_node = (*restaurant)->pending_orders->head;
	QueueNode* temp;
	while (cur_node->next != NULL) {
		temp = cur_node;
		cur_node = cur_node->next;
		free(temp);
		clear_order(&order);
	}
	*/
	Order* cur_order = dequeue_order(*restaurant);
	while (cur_order != NULL) {
		clear_order(&cur_order);
		cur_order = dequeue_order(*restaurant);
	}

	free((*restaurant)->pending_orders);
	free((*restaurant)->name);
	clear_menu(&((*restaurant)->menu));
	free(*restaurant);
	*restaurant = NULL;
}

/*
for (int i = 0; i < (*restaurant)->num_pending_orders; i++) {
		QueueNode* cur_node = (*restaurant)->pending_orders->head;
		while ()
		//free((*restaurant)->pending_orders[i]);
	}
*/

void print_menu(Menu* menu){
	fprintf(stdout, "--- Menu ---\n");
	for (int i = 0; i < menu->num_items; i++){
		fprintf(stdout, "Test.");
		fprintf(stdout, "(%s) %s: %.2f\n", 
			menu->item_codes[i], 
			menu->item_names[i], 
			menu->item_cost_per_unit[i]	
		);
	}
}

void print_order(Order* order){
	for (int i = 0; i < order->num_items; i++){
		fprintf(
			stdout, 
			"%d x (%s)\n", 
			order->item_quantities[i], 
			order->item_codes[i]
		);
	}
}


void print_receipt(Order* order, Menu* menu){
	for (int i = 0; i < order->num_items; i++){
		double item_cost = get_item_cost(order->item_codes[i], menu);
		fprintf(
			stdout, 
			"%d x (%s)\n @$%.2f ea \t %.2f\n", 
			order->item_quantities[i],
			order->item_codes[i], 
			item_cost,
			item_cost * order->item_quantities[i]
		);
	}
	double order_subtotal = get_order_subtotal(order, menu);
	double order_total = get_order_total(order, menu);
	
	fprintf(stdout, "Subtotal: \t %.2f\n", order_subtotal);
	fprintf(stdout, "               -------\n");
	fprintf(stdout, "Tax %d%%: \t$%.2f\n", TAX_RATE, order_total);
	fprintf(stdout, "              ========\n");
}