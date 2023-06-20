// Douglas Nascimento
// André Henrique Fiatkoski Lustosa
// Pablo Celestino da Rocha Lobo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

const int MAX = 50; // Limiting the array size
int sortData = 0;   // Variable to sort the data

// Type of a special data (Resgistry)
typedef struct Tproduct{
    long int productCode;
    char name[41];
    int productQuantity;
    float productBuyPrice;
    int productGroup;
    char productDescription[41];
    char productUnit[3];
    char productSupplier[41];
    float productSellPrice;
    float productMinimalProfit ;
    int inventoryMin;
    struct tm expiredDate;
} Tproduct;

// Function for validating the expiring date
int dateExp(struct tm dateE);

// Substitute for fflush(stdin)
void cleanStdin(void);

// Struct for adding a expiration date for a product
struct tm getExpiredDate(){
    struct tm dateE;
    do {
        printf("Expiring date of a product.\n");
        printf("Day: ");
        scanf("%d", &dateE.tm_mday);
        printf("Mounth: ");
        scanf("%d", &dateE.tm_mon);
        printf("Year: ");
        scanf("%d", &dateE.tm_year);
        cleanStdin();
    } while (!dateExp(dateE));
    return dateE;
}

// Function for the reading of the inventory.dat file
void readFile(Tproduct inventory[], int *size);

// Function for writing to the inventory.dat file
void writeFile(Tproduct inventory[], int size);

// Function for binary search
int binarySearch(Tproduct inventory[], int key, int size);

// Auxiliary function to verifie if the array is empty
int null(int size);

// Function to sort the products array per product code
void sortProduct(Tproduct inventory[], int size);

// Function to add a new product
void addProdutct(Tproduct inventory[], int *size);

// Function to exhibit the general report of registered products
void generalReport(Tproduct inventory[], int size);

// Function to make a change in a alredy existing product
void changeProduct(Tproduct inventory[], int size);

// Function to delete a product from the inventary
void deleteProduct(Tproduct inventory[], int *size);

// Function to search for a specific product
void consultProduct(Tproduct inventory[], int size);

// Function for menu of special reports
void espMenu();

// Funtion to show a report of expired products
void expired(Tproduct inventory[], int size);

// Funtion to increase the buy price of products from a specific supplier
void increaseSupplierPrices(Tproduct inventory[], int size);

// Function to show a report of products that are bellow the minimal\
 quantity in the inventory
void reportMinimalInventory(Tproduct inventory[], int size);

void listReport(Tproduct inventory[], int index);

// Function to change the expiration dates of a specific product
void changeExpiredMenu(Tproduct inventory[], int size);

// Function to change all values of a specific product
void changeFunction(Tproduct inventory[], int size);


// Main function
int main(){
	
	Tproduct inventory[MAX];
    struct tm *date;
    time_t dayToday;
    time(&dayToday);
    date = localtime(&dayToday);
    int size=0, opt;
    // Open the file of the data base
    readFile(inventory, &size);
    do{
        printf("Today is: %d/%d/%d\n", date->tm_mday, date->tm_mon + 1, \
        date->tm_year + 1900);
        printf("================================\n");
        printf(" 1 - REGISTER PRODUCT\n");
        printf(" 2 - CHANGE PRODUCT DETAILS\n");
        printf(" 3 - DELETE PRODUCT\n");
        printf(" 4 - SEARCH PRODUCT\n");
    	printf(" 5 - GENERAL REPORT\n");
    	printf(" 6 - SPECIAL REPORTS\n");
        printf(" 0 - EXIT\n");
        printf("\nCHOOSE THE DESIRED OPTION:\n");
        scanf("%d",&opt);
        system("clear");
        switch(opt){
            case 1:
            	addProdutct(inventory, &size);
            	writeFile(inventory, size);
				break;
			case 2 :
				changeProduct(inventory, size);
                writeFile(inventory, size);
				break;
			case 3 :
				deleteProduct(inventory, &size);
                writeFile(inventory, size);
				break;
			case 4 :
				consultProduct(inventory, size);
				break;
            case 5:
            	generalReport(inventory,  size);
				break;
			case 6:
				espMenu();		
		    	break;
            case 0:
                printf("Thanks for using our software!!\n");
                exit(0);
            default:
                printf("INVALID OPTION!!\n\n");
                system("clear");
			}
				
    }while(opt!=0);
        // Writing to the file after finishing the \
                opperation
		writeFile(inventory, size);
	return 0;
				
}

void readFile(Tproduct inventory[], int *size){
    FILE *file;
  	file = fopen("inventory.dat", "a+b");
    // Open to attach to the file
  	if (!file){
  		printf("Error to open file!");
  		return;
  	}
    // Read the data of the file
    while(!feof(file)){
       fread (&inventory[*size], sizeof (Tproduct) , 1, file);
       (*size)++;
    }
	(*size)--;
    // Close the file
	fclose(file);
	return;
}

void writeFile(Tproduct inventory[], int size){
    FILE *file;
    int i;
    file = fopen("inventory.dat", "w+b");	   
    // Open and erase the contents of the file
  	if (!file){
  		printf("Error while opening the file!\n");
  		return;
  	}
    // Write the stucture to the file
    for(i = 0;i < size;i++){
        fwrite(&inventory[i], sizeof(Tproduct), 1, file);   
    }
    // Close the data file
	fclose(file);
	return;
}

int binarySearch(Tproduct inventory[], int key, int size){
    // Null array
    if(null(size))
       return -1;                                                     
    if (!sortData){
        // Sort products
        sortProduct(inventory,size);   
        // Global variable                             
        sortData = 1;                                            
    }
    // Found the product
    int start = 0,end = size, middle;
    while (start <= end){
        middle = (int)(start+end) / 2;
        if(inventory[middle].productCode == key)
           return middle;                                      
        if(inventory[middle].productCode < key)
           start = middle+1;
        else
           end = middle-1;
    }
    // Did not find the product
    return -1;                                                  
}

int null(int size){
    // Null
     if(size==0){
        printf("\nEMPTY REGISTRY!\n");
        return 1;
     }
     // Filled in
     return 0;
}

void sortProduct(Tproduct inventory[], int size){
	int i,j;
    Tproduct aux;
    for(i=0;i<size-1;i++)
        for(j=i+1;j<size;j++)
          	if (inventory[i].productCode > inventory[j].productCode){
              	aux=inventory[i];
              	inventory[i]=inventory[j];
              	inventory[j]=aux;
            }
}

void addProdutct(Tproduct inventory[], int* size){
    if(*size == MAX){
        printf("\nERROR!\nFILE IS FULL.\n");
        return;
    }

    Tproduct aux;
    char correct = 'n';

    do{
        printf("Code: ");
        scanf("%ld", &aux.productCode);
        cleanStdin();
    } while (aux.productCode <= 0);

    if(binarySearch(inventory, aux.productCode, *size) >= 0){
        printf("\nTHE PRODUCT CODE IS ALREDY REGISTERED !!\n");
        printf("\nTYPE ENTER TO GO BACK TO THE MENU\n");
        getchar();
        system("clear");
        return;
    }

    do{
        printf("\nProduct name: ");
        fgets(aux.name, sizeof(aux.name), stdin);
        cleanStdin();
        aux.name[strcspn(aux.name, "\n")] = '\0';
        if (aux.name[0] == '\0') {
            printf("THE FIELD CANT BE EMPTY\n");
        }
    } while(aux.name[0] == '\0');

    do{
        printf("\nDescription of the product: ");
        fgets(aux.productDescription, sizeof(aux.productDescription), stdin);
        cleanStdin();
        aux.productDescription[strcspn(aux.productDescription, "\n")] = '\0';
        if (aux.productDescription[0] == '\0') {
            printf("THE FIELD CANT BE EMPTY\n");
        }
    } while (aux.productDescription[0] == '\0');

    do{
        printf("\nSupplier of the product: ");
        fgets(aux.productSupplier, sizeof(aux.productSupplier), stdin);
        cleanStdin();
        aux.productSupplier[strcspn(aux.productSupplier, "\n")] = '\0';
        if (aux.productSupplier[0] == '\0') {
            printf("THE FIELD CANT BE EMPTY\n");
        }
    } while (aux.productSupplier[0] == '\0');

    do{
        printf("\nMinimal quantity of the product in the inventory: ");
        scanf("%d", &aux.inventoryMin);
        cleanStdin();
    } while (aux.inventoryMin < 0);

    do{
        printf("\nGroup: \n");
        printf("1 - HAIR : \n");
        printf("2 - NAIL : \n");
        printf("3 - SKIN : \n");
        printf("CHOOSE ONE OF THE GROUPS ABOVE: ");
        scanf("%d", &aux.productGroup);
        cleanStdin();

        if (aux.productGroup != 1 && aux.productGroup != 2 && aux.productGroup != 3){
            printf("\nInvalid product group. Type again.\n");
        }
    } while (aux.productGroup != 1 && aux.productGroup != 2 && aux.productGroup != 3);

    do{
        printf("\nProduct quantity in inventory: ");
        scanf("%d", &aux.productQuantity);
        cleanStdin();
    } while (aux.productQuantity < 0);

    do{
        printf("\nProduct Unit (UN or LT): ");
        fgets(aux.productUnit, sizeof(aux.productUnit), stdin);
        cleanStdin();
        aux.productUnit[strcspn(aux.productUnit, "\n")] = '\0';
        if (strcmp(aux.productUnit, "UN") != 0 && strcmp(aux.productUnit, "LT") != 0) {
            printf("Invalid product unit. Type again.\n");
        }
    } while (strcmp(aux.productUnit, "UN") != 0 && strcmp(aux.productUnit, "LT") != 0);

    do{
        printf("\nBuy price of the product from the supplier: R$ ");
        scanf("%f", &aux.productBuyPrice);
        cleanStdin();
    } while (aux.productBuyPrice < 0);

    do{
        printf("\nMinimal profit margin for the product: R$ ");
        scanf("%f", &aux.productMinimalProfit);
        cleanStdin();
    } while (aux.productMinimalProfit < 0);

    do{
        printf("\nSell price of the product: R$ ");
        scanf("%f", &aux.productSellPrice);
        cleanStdin();
    } while (aux.productSellPrice < 0);

    aux.expiredDate = getExpiredDate();

    printf("\nThe data is correct? (Y/N)");
    correct = getchar();
    cleanStdin();
    system("clear");

    if(correct == 'y' || correct == 'Y'){
        // Makes the transfer to the array
        inventory[*size] = aux;
        (*size)++;
        sortData = 0;
        printf("\tTHE PRODUCT WAS ADDED!!\n");
    }

    printf("\nTYPE ENTER TO GO BACK TO THE MENU\n");
    cleanStdin();
    getchar();
    system("clear");
    return;
}

void generalReport(Tproduct inventory[], int size){

    struct tm *dateToday;
    time_t dayT;
    time(&dayT);
    dateToday = localtime(&dayT);

    int i, counter = 0;
    // Define the limit of pages being shown per page
    int limit = 2;
    // Number of the current
    int page = 1;
    // Calculates the number of pages
    int totalPages = (size + limit - 1) / limit;

    cleanStdin();
    printf("\tInventory control - General report - %d/%d/%d\n", dateToday->tm_mday, \
    dateToday->tm_mon + 1, dateToday->tm_year + 1900);
    printf("=======================================================================\n");
    sortProduct(inventory, size);
    printf("Product Code\t\t\t\t\tPrice\n");
    while (counter < size) {
        // Exhibit the current products of the page
        for(i = counter; i < size && i < counter + limit; i++){
            printf("%ld\t\t\t\t\tR$ %.2f\n", inventory[i].productCode, inventory[i].productBuyPrice);
            printf("Name: %s\t\t\t\tDescription %s\n", inventory[i].name, inventory[i].productDescription);
            printf("Unit: %s\t\tSupplier: %s\n", inventory[i].productUnit, inventory[i].productSupplier);
            printf("Buy price: R$ %.2f\tSell price: R$ %.2f\tMinimal profit margin: R$ %.2f\n", \
            inventory[i].productBuyPrice, \
            inventory[i].productSellPrice, inventory[i].productMinimalProfit);
            printf("Product quantity in inventory: %d\tMinimal product quantity for the inventory: %d\n", inventory[i].productQuantity, \
            inventory[i].inventoryMin);
            printf("Expiration date: %d/%d/%d\n", inventory[i].expiredDate.tm_mday, \
            inventory[i].expiredDate.tm_mon, inventory[i].expiredDate.tm_year);
            printf("--------------------------------------------------------------------------\n");
        }

        printf("Page %d of %d\n", page, totalPages);
        
        if (page < totalPages) {
            printf("TYPE 'N' TO GO TO THE NEXT PAGE ");
        }
        
        if (page > 1) {
            printf("TYPE 'P' TO GO TO PREVIOUS PAGE ");
        }
        
        // Clean the character of the pending line
        printf("OR TYPE 'Q' TO QUIT THE REPORT\n");
        char opt = getchar();
        getchar();

        // Exits the loop if the user decides to go back to the menu
        if (opt == 'q' || opt == 'Q'){
            system("clear");
            break; 
        // Go to the next page
        } else if (opt == 'n' || opt == 'N') {
            if (page < totalPages) {
                counter += limit; 
                // Updates the number of the page
                page++;
            }
        // Go back a page
        } else if (opt == 'p' || opt == 'P'){
            if (page > 1) {
                counter -= limit; 
                if (counter < 0) {
                    // Garantees that the counter cant be negative
                    counter = 0;
                }
                // Updates the page number
                page--; 
            }
        }
        
        system("clear");
    }
}

void changeProduct(Tproduct inventory[], int size){

    if (null(size)) {
    	printf("\nTYPE ENTER TO GO BACK TO THE MENU");
	    cleanStdin();
	    getchar();
	    system("clear");
        return;
    }

    long int productCode;
    int position, opt;

    printf("Type the code of the product were is gonna be changed: ");
    scanf("%ld", &productCode);

    position = binarySearch(inventory, productCode, size);

    if (position == -1) {
        printf("PRODUCT NOT FOUND!!\n");
        printf("\nTYPE ENTER TO GO BACK TO THE MENU");
	    cleanStdin();
	    getchar();
	    system("clear");
        return;
    }

    printf("======================================================\n");
    printf(" 1 - NAME\n");
    printf(" 2 - GROUP\n");
    printf(" 3 - DESCRIPTION\n");
    printf(" 4 - UNIT\n");
    printf(" 5 - SUPPLIER\n");
    printf(" 6 - PRODUCT QUANTITY\n");
    printf(" 7 - BUY PRICE\n");
    printf(" 8 - SELL PRICE\n");
    printf(" 9 - MINIMAL PROFIT MARGIN\n");
    printf(" 10 - MINIMAL PRODUCT QUANTITY FOR THE INVENTORY\n");
    printf(" 11 - EXPIRATION DATE\n");
    printf(" 12 - ALL THE OPTIONS ABOVE\n");
    printf(" 0 - EXIT\n");
    printf("\nCHOOSE THE DESIRED OPTION TO EDIT:\n");
    scanf("%d",&opt);
    system("clear");

    switch(opt){
    case 1:
        printf("Type the new name of the product: ");
        cleanStdin();
        fgets(inventory[position].name, 41, stdin);
        inventory[position].name[strcspn(inventory[position].name, "\n")] = '\0';
        break;
    case 2:
        printf("Type the new group that the product belongs: ");
        scanf("%d", &inventory[position].productGroup);
        break;
    case 3:
        printf("Type the new description of the product: ");
        cleanStdin();
        fgets(inventory[position].productDescription, 41, stdin);
        inventory[position].productDescription[strcspn(inventory[position].productDescription, "\n")] = '\0';
        break;
    case 4:
        printf("Type the new unit: ");
        cleanStdin();
        fgets(inventory[position].productUnit, 3, stdin);
        inventory[position].productUnit[strcspn(inventory[position].productUnit, "\n")] = '\0';
        break;
    case 5:
        printf("Type the new name of the supplier of the product: ");
        cleanStdin();
        fgets(inventory[position].productSupplier, 41, stdin);
        inventory[position].productSupplier[strcspn(inventory[position].productSupplier, "\n")] = '\0';
        break;
    case 6:
        printf("Type the new quantity of the product in the inventory: ");
        scanf("%d", &inventory[position].productQuantity);
        break;
    case 7:
        printf("Type the new buy price of the product: ");
        scanf("%f", &inventory[position].productBuyPrice);
        break;
    case 8:
        printf("Type the new sell price of the product: ");
        scanf("%f", &inventory[position].productSellPrice);
        break;
    case 9:
        printf("Type the new minimal profit margin of the product: ");
        scanf("%f", &inventory[position].productMinimalProfit);
        break;
    case 10:
        printf("Type the new minimal quantity of the product necessary to be in the inventory: ");
        scanf("%d", &inventory[position].inventoryMin);
    break;
    case 11:
        changeExpiredMenu(inventory, size);
        break;
    case 12:
        changeFunction(inventory, size);
        break;
    case 0:
        return;
        break;
    default:
        printf("INVALID OPTION!!\n\n");
        system("clear");
    }

    printf("\nPRODUCT MODIFIED WITH SUCCESS!!\n");
    printf("\nTYPE ENTER TO GO BACK TO THE MENU");
    cleanStdin();
    getchar();
    system("clear");
    return;
}

void deleteProduct(Tproduct inventory[], int* size) {
    if (null(*size)) {
    	printf("\nTYPE ENTER TO GO BACK TO THE MENU");
	    cleanStdin();
	    getchar();
	    system("clear");
        return;
        
    }

    long int productCode;
    int position, i;

    printf("Type the code of the product desired to be deleted: ");
    scanf("%ld", &productCode);

    position = binarySearch(inventory, productCode, *size);

    if (position == -1) {
        printf("PRODUCT NOT FOUND!!\n");
        printf("\nTYPE ENTER TO GO BACK TO THE MENU");
	    cleanStdin();
	    getchar();
	    system("clear");
        return;
    }

    for (i = position; i < (*size) - 1; i++) {
        inventory[i] = inventory[i + 1];
    }
    

    (*size)--;
    sortData = 0;
    printf("\n PRODUCT DELETED WITH SUCCESS!!... \n ");
    system("\n pause \n");
    system("clear");
}

void consultProduct(Tproduct inventory[], int size){
    if (null(size)) {
    	printf("\nTYPE ENTER TO GO BACK TO THE MENU");
	    cleanStdin();
	    getchar();
	    system("clear");
        return;
    }

    long int productCode;
    int position;

    printf("Type the code of the product desired to be consulted: ");
    scanf("%ld", &productCode);

    position = binarySearch(inventory, productCode, size);

    if(position == -1){
        printf("PRODUCT NOT FOUND!!\n");
        printf("\nTYPE ENTER TO GO BACK TO THE MENU");
	    cleanStdin();
	    getchar();
	    system("clear");
        return;
    }

    printf("\n--------------------------------------------------------------------------\n");
    printf("Code: %ld\n", inventory[position].productCode);
    printf("Name: %s", inventory[position].name);
    printf("Group: %d\n", inventory[position].productGroup);
    printf("Description: %s\n", inventory[position].productDescription);
    printf("Unit: %s\n", inventory[position].productUnit);
    printf("Supplier: %s\n", inventory[position].productSupplier);
    printf("Quantity in inventory: %.d\n", inventory[position].productQuantity);
    printf("Buy Price: %.2f\n", inventory[position].productBuyPrice);
    printf("Sell Price: %.2f\n", inventory[position].productSellPrice);
    printf("Minimal profit margin: %.2f\n", inventory[position].productMinimalProfit);
    printf("Minimal product quantity in inventory: %d\n", inventory[position].inventoryMin);
    printf("Expiration date: %d/%d/%d\n", inventory[position].expiredDate.tm_mday, \
    inventory[position].expiredDate.tm_mon, inventory[position].expiredDate.tm_year);
    printf("--------------------------------------------------------------------------\n");

    printf("\nTYPE ENTER TO GO BACK TO THE MENU");
    cleanStdin();
    getchar();
    system("clear");
    return;
}

void reportMinimalInventory(Tproduct inventory[], int size){
    if (size == 0) {
        printf("\tEMPTY INVENTORY!!\n");
        printf("\nTYPE ENTER TO GO BACK TO THE MENU");
        cleanStdin();
        getchar();
        system("clear");
        return;
    }

    int i;
    int productFound = 0;

    for (i = 0; i < size; i++) {
        if (inventory[i].productQuantity < inventory[i].inventoryMin) {
            if (productFound == 0) {
                cleanStdin();
                printf("There are products bellow the minimal quantity!\n\n");
            }

            printf("Products bellow the minimal quantity in the inventory:\n");
            cleanStdin();
            printf("Code: %ld \n", inventory[i].productCode);
            printf("Group: %d\n", inventory[i].productGroup);
            printf("Description: %s\n", inventory[i].productDescription);
            printf("Unit: %s\n", inventory[i].productUnit);
            printf("Supplier: %s\n", inventory[i].productSupplier);
            printf("Quantity in inventory: %d\n", inventory[i].productQuantity);
            printf("Buy price: %.2f\n", inventory[i].productBuyPrice);
            printf("Sell price: %.2f\n", inventory[i].productSellPrice);
            printf("Minimal margin profit: %.2f\n", inventory[i].productMinimalProfit);
            printf("Minimal quantity in inventory: %d\n", inventory[i].inventoryMin);
            printf("\n");
            productFound++;
        }
    }

    if (productFound == 0) {
        cleanStdin();
        printf("No product is bellow the minimal quantity in the inventory!\n\n");
    }

    printf("TYPE ENTER TO GO BACK TO THE MENU");
    getchar();
    system("clear");
    return;
}

void increaseSupplierPrices(Tproduct inventory[], int size){

    char productSupplier[50];
    float percentageAdjustment;

    printf("Inform supplier name: ");
    cleanStdin();
    fgets(productSupplier, 50, stdin);
    // Remove the charactere of the break line
    cleanStdin();
    productSupplier[strcspn(productSupplier, "\n")] = '\0';

    printf("Inform the percentage adjustment in the buy price: ");
    scanf("%f", &percentageAdjustment);

    int i;
    int alteredProducts = 0;

    for(i = 0; i < size; i++){
        if(strcmp(inventory[i].productSupplier, productSupplier) == 0){
            printf("\nProduct found:\n");
            printf("Code: %ld\n", inventory[i].productCode);
            printf("Name: %s", inventory[i].name);
            printf("Description: %s\n", inventory[i].productDescription);
            printf("Current product buy price: R$ %.2f\n", inventory[i].productBuyPrice);
            
            float newProductBuyPrice = inventory[i].productBuyPrice * (1 + percentageAdjustment / 100);
            
            printf("New product buy price: R$ %.2f\n", newProductBuyPrice);
            
            printf("Type 'Y' to confirm the change: ");
            char opt;
            cleanStdin();
            scanf("%c", &opt);
            
            if(opt == 'y' || opt == 'Y'){
                inventory[i].productBuyPrice = newProductBuyPrice;
                alteredProducts++;
                printf("Increase in the buy price of the product with the code %ld.\n", \
                inventory[i].productCode);
            }
        }
    }

    if(alteredProducts > 0){
        printf("\nPrice increase of the buy price of the supplier products completed. \
        Total of products were prices were increased: %d\n", 
        alteredProducts);
    } else{
        printf("\nNo product was found for the informed supplier.\n");
    }

    printf("\nTYPE ENTER TO GO BACK TO THE MENU");
    cleanStdin();
    getchar();
    system("clear");
    return;
}

void expired(Tproduct inventory[], int size){

    struct tm *date;
    time_t dayT;
    time(&dayT);
    date = localtime(&dayT);
    
    if (size == 0){
        printf("EMPTY INVENTORY!!\n");
        printf("\nTYPE ENTER TO GO BACK TO THE MENU");
	    cleanStdin();
	    getchar();
	    system("clear");
        return;
    }

    int i;
    int productsFound = 0;

    printf("Products that expired:\n");

    for (i = 0; i < size; i++){
        if(inventory[i].expiredDate.tm_mday < date->tm_mday \
        && inventory[i].expiredDate.tm_mon < date->tm_mon + 1\
        || inventory[i].expiredDate.tm_year < date->tm_year + 1900){
            printf("\n--------------------------------------------------------------------------\n");
            printf("Code: %ld \n", inventory[i].productCode);
            printf("Name: %s \n", inventory[i].name);
            printf("Group: %d\n", inventory[i].productGroup);
            printf("Description: %s\n", inventory[i].productDescription);
            printf("Unit: %s\n", inventory[i].productUnit);
            printf("Supplier: %s\n", inventory[i].productSupplier);
            printf("Quantity in inventory: %d\n", inventory[i].productQuantity);
            printf("Buy price : %.2f\n", inventory[i].productBuyPrice);
            printf("Sell price: %.2f\n", inventory[i].productSellPrice);
            printf("Minimal profit margin: %.2f\n", inventory[i].productMinimalProfit);
            printf("Minimal product quantity in inventory: %d\n", inventory[i].inventoryMin);
            printf("Expiration date: %d/%d/%d\n", inventory[i].expiredDate.tm_mday, \
            inventory[i].expiredDate.tm_mon, inventory[i].expiredDate.tm_year);
            printf("--------------------------------------------------------------------------\n");
            printf("\n");
            productsFound++;
        }
    }

    printf("TYPE ENTER TO GO BACK TO THE MENU\n");
    cleanStdin();
    getchar();
    system("clear");
    return;
}

void espMenu(){
    
    Tproduct inventory[MAX];
    struct tm *date;
    time_t dayT;
    time(&dayT);
    date = localtime(&dayT);
    int size = 0, opt;

    do{
        printf("Today is: %d/%d/%d\n", date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
		printf("\n===== SPECIAL REPORTS =====\n");
		printf("1 - REPORTS OF PRODUCTS BELLOW THE MINIMAL QUANTITY : \n");
        printf("2 - INCREASE THE BUY PRICE OF PRODUCTS FROM A SUPPLIER\n");
        printf("3 - REPORT WITH EXPIRED PRODUCTS\n");
		printf("0 - GO BACK TO MAIN MENU\n");
		printf("Choose an options: ");
		scanf("%d", &opt);
		system("clear");
			
	    switch (opt){
		    case 1:
                reportMinimalInventory(inventory, size);
                break;
            case 2:
                increaseSupplierPrices(inventory, size);
				writeFile(inventory, size);
                break;
            case 3:
                expired(inventory, size);
                break;
			case 0:
                return;
			    break;
			default:
			    printf("INVALID OPTION!!\n\n");
                system("clear");
			        }
		} while (opt != 0);
}


void changeExpiredMenu(Tproduct inventory[], int size){

    int opt;

    printf("Type the option to edit the expired date:\n");
    printf("1 - Day ");
    printf("2 - Mounth");
    printf("3 - Year");
    printf("4 - The 3 choises");
    scanf("%d", & opt);
    switch (opt){
        case 1:
            
            break;
        case 2:

            break;
        case 3:

            break;
        case 4:
            getExpiredDate();
            break;
        default:
            printf("INVALID OPTION!!\n\n");
    }
}

void listReport(Tproduct inventory[], int index){
    printf("%ld\t\t\t\t%.2f\n", inventory[index].productCode, \
    inventory[index].productSellPrice);
}

void changeFunction(Tproduct inventory[], int size){
    
    int position;

    printf("Type the new group that the product belongs: ");
    scanf("%d", &inventory[position].productGroup);
    
    printf("Type the new name of the product: ");
    cleanStdin();
    fgets(inventory[position].name, 41, stdin);
    inventory[position].name[strcspn(inventory[position].name, "\n")] = '\0';

    printf("Type the new description of the product: ");
    cleanStdin();
    fgets(inventory[position].productDescription, 41, stdin);
    inventory[position].productDescription[strcspn(inventory[position].productDescription, "\n")] = '\0';

    printf("Type the new unit: ");
    cleanStdin();
    fgets(inventory[position].productUnit, 3, stdin);
    inventory[position].productUnit[strcspn(inventory[position].productUnit, "\n")] = '\0';

    printf("Type the new name of the supplier of the product: ");
    cleanStdin();
    fgets(inventory[position].productSupplier, 41, stdin);
    inventory[position].productSupplier[strcspn(inventory[position].productSupplier, "\n")] = '\0';

    printf("Type the new quantity of the product in the inventory: ");
    scanf("%d", &inventory[position].productQuantity);

    printf("Type the new buy price of the product: ");
    scanf("%f", &inventory[position].productBuyPrice);

    printf("Type the new sell price of the product: ");
    scanf("%f", &inventory[position].productSellPrice);

    printf("Type the new minimal profit margin of the product: ");
    scanf("%f", &inventory[position].productMinimalProfit);

    printf("Type the new minimal quantity of the product necessary to be in the inventory: ");
    scanf("%d", &inventory[position].inventoryMin);

    changeExpiredMenu(inventory, size);
}

void cleanStdin(void){
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}


int dateExp(struct tm dateE){
    // Checks if is a valid date
    if (dateE.tm_year < 1900){
        printf("INVALID DATE\n");
        return 0;
    }
    if (dateE.tm_mon < 1 || dateE.tm_mon > 12){
        printf("INVALID DATE\n!!");
        return 0;
    }
    if (dateE.tm_mday < 1 || dateE.tm_mday > 31){
        printf("INVALID DATE!!\n");
        return 0;
    }
    return 1;
}