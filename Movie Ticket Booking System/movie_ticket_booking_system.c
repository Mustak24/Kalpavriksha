#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CUSTOMER_NAME_MAX_LENGTH 50

#define STANDARD_TICKET_PRICE 150.0
#define PREMIUM_TICKET_PRICE 250.0
#define RECLINER_TICKET_PRICE 400.0

#define TICKET_COUNT_MIN_LIMIT 1
#define TICKET_COUNT_MAX_LIMIT 20

#define DISCOUNT_THRESHOLD 5
#define DISCOUNT_PERCENTAGE 10.0
#define PERCENTAGE_DIVISOR 100.0


typedef enum {
    TICKET_CATEGORY_STANDARD = 1,
    TICKET_CATEGORY_PREMIUM,
    TICKET_CATEGORY_RECLINER
} TicketCategory ;

typedef struct {
    char customerName[CUSTOMER_NAME_MAX_LENGTH];
    unsigned int ticketCount;
    TicketCategory ticketCategory;

    float subTotal;
    float discount;
    float grandTotal;
} TicketInvoice;


void clearInputBuffer() {
    int bufferChar;
    while((bufferChar = getchar()) != '\n' && bufferChar != EOF);
}

bool isValidTicketCategory(unsigned int ticketCategory) {
    return (
        ticketCategory == TICKET_CATEGORY_STANDARD ||
        ticketCategory == TICKET_CATEGORY_PREMIUM ||
        ticketCategory == TICKET_CATEGORY_RECLINER
    );
}

bool isValidTicketCount(unsigned int ticketCount) {
    return (
        TICKET_COUNT_MIN_LIMIT <= ticketCount && 
        ticketCount <= TICKET_COUNT_MAX_LIMIT
    );
}

float getTicketPrice(TicketCategory category) {
    switch (category) {
        case TICKET_CATEGORY_STANDARD:
            return STANDARD_TICKET_PRICE;
        case TICKET_CATEGORY_PREMIUM:
            return PREMIUM_TICKET_PRICE;
        case TICKET_CATEGORY_RECLINER:
            return RECLINER_TICKET_PRICE;
        default:
            printf("Error: Invalid ticket category\n");
            exit(1);
    }
}

float calculateSubTotal(TicketCategory category, unsigned int ticketCount) {
    float ticketPrice = getTicketPrice(category);
    return ticketPrice * ticketCount;
}

float calculateDiscountAmount(unsigned int ticketCount, float subTotal) {
    if(ticketCount < DISCOUNT_THRESHOLD) return 0;
    return (subTotal * DISCOUNT_PERCENTAGE) / PERCENTAGE_DIVISOR;
}

float calculateGrandTotal(float subTotal, float discount) {
    return subTotal - discount;
}

const char* getTicketCategoryName(TicketCategory category) {
    switch (category) {
        case TICKET_CATEGORY_STANDARD:
            return "Standard";
        case TICKET_CATEGORY_PREMIUM:
            return "Premium";
        case TICKET_CATEGORY_RECLINER:
            return "Recliner";
        default:
            printf("Error: Invalid ticket category\n");
            exit(1);
    }
}


int readStringInput(const char* prompt, char* inputString, size_t maxSize) {
    if(prompt != NULL) {
        printf("%s", prompt);
    }

    if(fgets(inputString, maxSize, stdin) == NULL) {
        printf("Error: Unable to read input string\n");
        return 1;
    }

    size_t inputLength = strlen(inputString);

    if(inputLength == maxSize-1 && inputString[inputLength-1] != '\n') {
        printf("Error: input string overflow\n");
        return 1;
    }

    if(inputString[inputLength-1] == '\n') {
        inputString[inputLength-1] = '\0';
        return 0;
    }

    clearInputBuffer();
    return 0;
}

int readTicketCount(unsigned int* ticketCount) {
    printf("Enter Number of tickets: ");

    int result;
    while(
        (result = scanf("%u", ticketCount)) != 1 ||
        !isValidTicketCount(*ticketCount)
    ) {
        if(result == EOF) {
            printf("Error: Failed to read ticket count\n");
            return 1;
        }

        printf(
            "Warning, please enter valid ticket count from (%d - %d): ",
            TICKET_COUNT_MIN_LIMIT,
            TICKET_COUNT_MAX_LIMIT
        );

        clearInputBuffer();
    }

    clearInputBuffer();
    return 0;
}

int readTicketCategory(TicketCategory* category) {
    printf(
        "Select ticket category (%d = %s, %d = %s, %d = %s): ",
        TICKET_CATEGORY_STANDARD, getTicketCategoryName(TICKET_CATEGORY_STANDARD),
        TICKET_CATEGORY_PREMIUM, getTicketCategoryName(TICKET_CATEGORY_PREMIUM),
        TICKET_CATEGORY_RECLINER, getTicketCategoryName(TICKET_CATEGORY_RECLINER)
    );

    int result;
    unsigned int tempCategory;

    while(
        (result = scanf("%u", &tempCategory)) != 1 ||
        !isValidTicketCategory(tempCategory)
    ) {
        if(result == EOF) {
            printf("Error: failed to read ticket category\n");
            return 1;
        }

        printf("Warning, please select valid category from above: ");
        clearInputBuffer();
    }

    *category = tempCategory;
    clearInputBuffer();
    return 0;
}


int generateTicketInvoice(TicketInvoice* invoice) {
    int error = readStringInput(
        "Enter you name: ", 
        invoice->customerName, 
        CUSTOMER_NAME_MAX_LENGTH
    );

    if(error != 0) {
        printf("Error: failed to read customer name\n");
        return error;
    }

    error = readTicketCount(&invoice->ticketCount);
    if(error != 0) return error;

    error = readTicketCategory(&invoice->ticketCategory);
    if(error != 0) return error;

    invoice->subTotal = calculateSubTotal(invoice->ticketCategory, invoice->ticketCount);
    invoice->discount = calculateDiscountAmount(invoice->ticketCount, invoice->subTotal);
    invoice->grandTotal = calculateGrandTotal(invoice->subTotal, invoice->discount);

    return 0;
}


void displayTicketInvoice(TicketInvoice* invoice){
    printf("\n\n------INVOICE DETAILED------\n");
    printf("Customer        : %s\n",   invoice->customerName);
    printf("Ticket Category : %s\n",   getTicketCategoryName(invoice->ticketCategory));
    printf("Tickets         : %u\n",   invoice->ticketCount);
    printf("Subtotal        : %.2f\n", invoice->subTotal);
    printf("Discount        : %.2f\n", invoice->discount);
    printf("Final Bill      : %.2f\n", invoice->grandTotal);
}


int main() {
    TicketInvoice invoice;

    printf("\n-----MOVIE TICKET BOOKING SYSTEM----\n");
    int error = generateTicketInvoice(&invoice);
    if(error != 0) {
        printf("Error: failed to generate movie ticket invoice\n");
        return error;
    }

    displayTicketInvoice(&invoice);
    return 0;
}