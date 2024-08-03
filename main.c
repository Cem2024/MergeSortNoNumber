//
// Created by Cem Berk Arkar on 08.04.24.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define THREE 3

// Definition der verketteten Liste
typedef struct ListElement {
    int element;
    struct ListElement* next;
} ListElement;

// Listen Kopf
typedef struct ListHead {
    ListElement* head;
} ListHead;

// Erstellt neue Elemente für die Liste welche vorne eingefügt werden
ListElement* listElementConstructor(int number, ListElement* next) {
    ListElement* newElement = calloc(1, sizeof(ListElement));
    if (newElement == NULL) {
        fprintf(stderr, "Speicher konnte nicht allokiert werden.\n");
        exit(1);
    }
    newElement->element = number;
    newElement->next = next;
    return newElement;
}

// Für Tests ein Random Listen Füller (macht es einfacher und übersichtlicher)
void randomListGenerator(ListHead* list, int elementNumber, int lower_barrier, int upper_barrier) {
    for (int i = 0; i < elementNumber; i++) {
        int random_number = (rand() % (upper_barrier - lower_barrier + 1)) + lower_barrier;    ///ChatGPT für diese Zeile genutzt.
        list->head = listElementConstructor(random_number, list->head);
    }
}

// Print liste (test gründe)
void printList(ListHead* list) {
    ListElement* temp = list->head;
    while (temp) {
        printf("%d ", temp->element);
        temp = temp->next;
    }
}

/// Zählt wie viel Elemente in einer Liste sind
int listCounter(ListElement* listHead) {
    if (listHead == NULL) {
        fprintf(stderr, "Fehler: Die Liste ist leer");
        exit(1);
    }
    int counter = 0;
    ListElement* tmp = listHead;
    while (tmp) {
        counter++;
        tmp = tmp->next;
    }
    return counter;
}

///Teilt die Liste in Hälfte
/* Funktionalität: Läuft zum Mittelpunkt der Liste.
 * Gibt dem Ende der linken Liste ein NULL und Funktion gibt rechten Kopf der Liste zurück*/

ListElement* listDivider(ListElement* listHead, int length) {
    if (listHead == NULL || length <= 1) {
        return NULL;
    }

    int pivot = length / 2;
    ListElement* current = listHead;
    for (int i = 0; i < pivot - 1; i++) {
        current = current->next;
    }
    ListElement* rightHead = current->next;
    current->next = NULL;
    return rightHead;
}

/// Zusammenführen der sortierten Hälften
ListElement* merge(ListElement* leftHead, ListElement* rightHead) {

    //Falls eine Seite fertig durch ist schreibt er noch den sortierten Rest in die Liste
    if (leftHead == NULL) {
        return rightHead;
    }
    if (rightHead == NULL) {
        return leftHead;
    }

    /* Schaut sich an, ob das linke oder das rechte Element größer ist und ob Zahl 3 dabei ist
     * (falls ja rufe rekursiv merge neu auf(also skippe))*/

    ListElement* sortedList = NULL;

    if (leftHead->element == THREE) {
        sortedList = merge(leftHead->next, rightHead);
    }

    else if (rightHead->element == THREE) {

        sortedList = merge(leftHead, rightHead->next);
    }

    else if (leftHead->element <= rightHead->element) {
        sortedList = leftHead;
        sortedList->next = merge(leftHead->next, rightHead);
    }

    else {
        sortedList = rightHead;
        sortedList->next = merge(leftHead, rightHead->next);
    }

    return sortedList;
}

/// MergeSort Algorithmus
ListElement* mergeSort(ListElement* listHead) {

    if (listHead == NULL || listHead->next == NULL) {
        return listHead;
    }
    int length = listCounter(listHead);
    if (length <= 1) {
        return listHead;
    }
    //Zerteilen der Liste

    /// Wie MergeSort Aufrufen (durch Pseudocode aus Introprog Vorlesung)
    ListElement* rightHead = listDivider(listHead, length);
    listHead = mergeSort(listHead);
    rightHead = mergeSort(rightHead);

    //Setzt die zerteilten Listen zusammen und gibt bei merge (return sorted_list) den kopf der sortierten Liste
    return merge(listHead, rightHead);
}

/// Speicher freigeben
void freeList(ListElement* list) {

    ListElement* temp;
    while (list != NULL) {
        temp = list;
        list = list->next;
        free(temp);
    }
}

int main() {

    int elementNumber = 0;
    int lower_barrier = 0;
    int upper_barrier = 0;

    printf("Wie viele Elemente sollen sortiert werden? Bitte eine natürliche Zahl eingeben: \n");
    scanf("%u", &elementNumber);

    while(elementNumber < 0){
        printf("Keine natürliche Zahl geben Sie bitte nochmal ein: \n");
        scanf("%u", &elementNumber);
    }

    printf("Geben Sie bitte jetzt eine untere Schranke für den Wertebereich der Elemente: \n");
    scanf("%d", &lower_barrier);

    printf("Geben Sie jetzt die obere Schranke ein: \n");
    scanf("%d", &upper_barrier);

    while(lower_barrier > upper_barrier){
        printf("Obere Schranke muss größer als untere Schranke sein, geben Sie bitte eine Zahl ein die größer gleich  %d ist:  \n", lower_barrier);
        scanf("%d", &upper_barrier);
    }

    // Speicher für Listen Kopf
    ListHead* listOne = malloc(sizeof(ListHead));
    if (listOne == NULL) {
        fprintf(stderr, "Speicher konnte nicht allokiert werden.\n");
        exit(1);
    }

    listOne->head = NULL;


    // für unterschiedliche seeds der "random" zahlen
    srand(time(NULL));
    randomListGenerator(listOne, elementNumber, lower_barrier, upper_barrier);


    printf("Unsortierte Liste: ");
    printList(listOne);
    printf("\n");

    // MergeSort aufrufen
    listOne->head = mergeSort(listOne->head);

    printf("Sortierte Liste: ");
    printList(listOne);
    printf("\n");


    // Speicher freigeben
    freeList(listOne->head);
    free(listOne);
    return 0;
}
