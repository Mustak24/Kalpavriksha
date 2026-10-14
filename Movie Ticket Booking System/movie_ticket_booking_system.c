#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CUSTOMER_NAME_MAX_LENGTH 50

#define STANDARD_TICKET_INDEX 1
#define STANDARD_TICKET_PRICE 150.0

#define PREMIUM_TICKET_INDEX 2
#define PREMIUM_TICKET_PRICE 250.0

#define RECLINER_TICKET_INDEX 3
#define RECLINER_TICKET_PRICE 400.0

#define TICKET_COUNT_MAX_LIMIT 20
#define TICKET_COUNT_MIN_LIMIT 1

#define DISCOUNT_THRESHOLD 5
#define DISCOUNT_PERCENTAGE 10.0
#define PERCENTAGE_DIVISOR 100.0


typedef struct {
    char customerName[CUSTOMER_NAME_MAX_LENGTH];
    unsigned int ticketCount;
    unsigned int ticketCategory;

    float subTotal;
    float discount;
    float grandTotal;
} TicketInvoice;


void clearInputBuffer() {
    int bufferChar;
    while((bufferChar = getchar()) != '\n' && bufferChar != EOF);
}


int readStringInput(const char* prompt, char* inputString, size_t maxSize) {
    if(prompt != NULL) {
        printf("%s", prompt);
    }

    if(fgets(inputString, maxSize, stdin) == NULL) {
        printf("Error: Unable to read input string");
        return 1;
    }

    size_t inputLength = strlen(inputString);

    if(inputLength == maxSize-1 && inputString[inputLength-1] != '\n') {
        printf("Error: input string overflow");
        return 1;
    }

    if(inputString[inputLength-1] == '\n') {
        inputString[inputLength-1] = '\0';
        return 0;
    }

    clearInputBuffer();
    return 0;
}


bool isValidTicketCategory(unsigned int ticketCategory) {
    return (
        ticketCategory == STANDARD_TICKET_INDEX ||
        ticketCategory == PREMIUM_TICKET_INDEX ||
        ticketCategory == RECLINER_TICKET_INDEX
    );
}

bool isValidTicketCount(unsigned int ticketCount) {
    return (
        TICKET_COUNT_MIN_LIMIT <= ticketCount && 
        ticketCount <= TICKET_COUNT_MAX_LIMIT
    );
}


float getTicketPrice(unsigned int ticketCategory) {
    switch (ticketCategory) {
        case STANDARD_TICKET_INDEX:
            return STANDARD_TICKET_PRICE;
        case PREMIUM_TICKET_INDEX:
            return PREMIUM_TICKET_PRICE;
        case RECLINER_TICKET_INDEX:
            return RECLINER_TICKET_PRICE;
        default:
            return 0;
    }
}

float calculateSubTotal(unsigned int ticketCategory, unsigned int ticketCount) {
    float ticketPrice = getTicketPrice(ticketCategory);
    return ticketPrice * ticketCount;
}

float calculateDiscountAmount(unsigned int ticketCount, float subTotal) {
    if(ticketCount < DISCOUNT_THRESHOLD) return 0;
    return (subTotal * DISCOUNT_PERCENTAGE) / PERCENTAGE_DIVISOR;
}

float calculateGrandTotal(float subTotal, float discount) {
    return subTotal - discount;
}

const char* getTicketCategoryName(unsigned int ticketCategory) {
    switch (ticketCategory) {
        case STANDARD_TICKET_INDEX:
            return "Standard";
        case PREMIUM_TICKET_INDEX:
            return "Premium";
        case RECLINER_TICKET_INDEX:
            return "Recliner";
        default:
            printf("Error: Invalid ticket category");
            exit(1);
    }
}

int generateTicketInvoice(TicketInvoice* invoice) {
    int error = readStringInput(
        "Enter Customer Name: ", 
        invoice->customerName, 
        CUSTOMER_NAME_MAX_LENGTH
    );

    if(error != 0) {
        printf("Error: failed to read customer name\n");
        return error;
    }

    printf("Number of tickets: ");
    if(
        scanf("%u", &invoice->ticketCount) != 1 ||
        !isValidTicketCount(invoice->ticketCount)
    ) {
        printf("Error: Invalid ticket count, count must be form (1 - 20)\n");
        return 1;
    }
    clearInputBuffer();

    printf("Select ticket category (1 = Standard, 2 = Premium, 3 = Recliner): ");
    if(
        scanf("%u", &invoice->ticketCategory) != 1 || 
        !isValidTicketCategory(invoice->ticketCategory)
    ) {
        printf("Error: Invalid ticket category, category must be form (1, 2, 3)\n");
        return 1;
    } 
    clearInputBuffer();

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

    int error = generateTicketInvoice(&invoice);
    if(error != 0) {
        printf("Error: failed to generate movie ticket invoice\n");
        return error;
    }

    displayTicketInvoice(&invoice);
    return 0;
}