#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "Queue.h"

// Povilo Carev (1-a grupė) darbas - Banko simuliacija
// Darbui pasiremta Augusto Budniko sukurta eilute

// Bankininko struktūra
typedef struct {
    int serviceTime; // Laikas, per kurį kasininkas aptarnauja klientą
    int remainingTime; // Likęs laikas, per kurį kasininkas baigs aptarnauti esamą klientą
} Teller;


// Inicializuojame bankininką su duotu aptarnavimo laiku
void initTeller(Teller *teller, int serviceTime) {
    teller->serviceTime = serviceTime;
    teller->remainingTime = 0;
}

// Patikriname, ar bankininkas yra laisvas
int isTellerFree(Teller *teller) {
    return (teller->remainingTime == 0);
}

// Aptarnaujamas naujas klientas, nustatomas likęs bankininko laikas į aptarnavimo laiką
void serveCustomer(Teller *teller) {
    teller->remainingTime = teller->serviceTime;
}

// Sumažinamas likusio darbo laikas per 1 laiko vnt.
void work(Teller *teller) {
    if (teller->remainingTime > 0) {
        teller->remainingTime--;
    }
}

// Banko struktūra
typedef struct {
    Teller *tellers; // Bankininkų masyvas
    int numTellers;  // Bankininkų skaičius
    Queue *queue;    // Klientų eilė
    double arrivalProb;  // Tikimybė, kad kiekvieną laiko žingsnį atvyks naujas klientas
    int totalCustomers;  // Klientų sk.
    int totalServedCustomers; // Aptarnautų klientų skaičius
    int totalWaitTime;  // Visų klientų laukimo laikas
    int maxQueueLength;  // Didžiausias eilės ilgis
    int *waitTimes; // Klientų laukimo laiko masyvas
    int waitTimesSize; // waitTimes masyvo dydis
} Bank;

// Inicializuojame banką su duotais parametrais
void initBank(Bank *bank, int numFastTellers, int fastServiceTime, int numSlowTellers, int slowServiceTime, double arrivalProb, int maxQueueSize) {
    bank->numTellers = numFastTellers + numSlowTellers;
    bank->tellers = (Teller*)malloc(bank->numTellers * sizeof(Teller));
    // Inicializuojami greiti kasininkai
    for (int i = 0; i < numFastTellers; ++i) {
        initTeller(&bank->tellers[i], fastServiceTime);
    }
    // Inicializuojami lėti kasininkai
    for (int i = 0; i < numSlowTellers; ++i) {
        initTeller(&bank->tellers[numFastTellers + i], slowServiceTime);
    }

    // Sukuriama eilutė
    bank->queue = createQueue();
    bank->arrivalProb = arrivalProb;
    bank->totalCustomers = 0;
    bank->totalServedCustomers = 0;
    bank->totalWaitTime = 0;
    bank->maxQueueLength = 0;
    bank->waitTimes = (int*)malloc(maxQueueSize * sizeof(int));
    bank->waitTimesSize = 0;
}

// Po simuliacijos, atlaisvinami resursai
void freeBank(Bank *bank) {
    free(bank->tellers);
    destroyQueue(bank->queue);
    free(bank->waitTimes);
}

// Simuliuojamas vienas banko veikimo žingsnis
void simulateStep(Bank *bank) {
     // Patikrinama, ar atvyksta naujas klientas
    if ((double)rand() / RAND_MAX < bank->arrivalProb) {
        enqueue(bank->queue, bank->queue->size);
        bank->totalCustomers++;
    }

    // Atnaujinamas didžiausias eilės ilgis.
    bank->maxQueueLength = (getSize(bank->queue) > bank->maxQueueLength) ? getSize(bank->queue) : bank->maxQueueLength;

    // Apdorojami bankininkai.
    for (int i = 0; i < bank->numTellers; ++i) {
        if (isTellerFree(&bank->tellers[i]) && !isEmpty(bank->queue)) {
            int waitTime = dequeue(bank->queue); // Kliento, esančio eilės pradžioje, laukimo laikas
            bank->waitTimes[bank->waitTimesSize++] = waitTime;
            bank->totalWaitTime += waitTime;
            serveCustomer(&bank->tellers[i]); // Kviečiama kliento aptarnavimo f-ja
            bank->totalServedCustomers++;
        }
        work(&bank->tellers[i]); // Sumažiname bankininko likusį aptarnavimo laiką 1 vnt.
    }
}

// Pagrindinė simuliacijos f-ja
void simulate(Bank *bank, int timeSteps) {
    // Padidinamas kiekvieno kliento laukimo laikas eilėje
    for (int t = 0; t < timeSteps; ++t) {
        Node *current = bank->queue->head;
        while (current != NULL) {
            current->value++;
            current = current->next;
        }

        simulateStep(bank); // Simuliuojamas vienas banko žingsnis
    }
}

// Spausdinami simuliacijos rezultatai
void printResults(Bank *bank) {
    double avgWaitTime = (bank->waitTimesSize == 0) ? 0 : (double)bank->totalWaitTime / bank->waitTimesSize;
    int maxWaitTime = (bank->waitTimesSize == 0) ? 0 : bank->waitTimes[0];

    // Randamas maksimalus laukimo laikas
    for (int i = 1; i < bank->waitTimesSize; ++i) {
        if (bank->waitTimes[i] > maxWaitTime) {
            maxWaitTime = bank->waitTimes[i];
        }
    }

    // Apskaičiuojamas vidutinis bankininkų panaudojimas.
    double avgUtilization = (bank->totalServedCustomers == 0) ? 0 : ((double)bank->totalServedCustomers / bank->totalCustomers) * 100;

    // Spausdinami rezultatai
    printf("Average Waiting Time: %.2f\n", avgWaitTime);
    printf("Max Waiting Time: %d\n", maxWaitTime);
    printf("Average Teller Utilization (%%): %.2f\n", avgUtilization);
    printf("Max Queue Length: %d\n", bank->maxQueueLength);
}

int main() {
    srand((unsigned int)time(0)); // Inicializuojamas atsitiktinių skaičių generatorius.

    // Simuliacijos parametrai
    int numFastTellers = 1;
    int fastServiceTime = 5;
    int numSlowTellers = 3;
    int slowServiceTime = 10;
    double arrivalProb = 0.3;
    int timeSteps = 1000;
    int maxQueueSize = 1000;

    // Inicializuojamas bankas
    Bank bank;
    initBank(&bank, numFastTellers, fastServiceTime, numSlowTellers, slowServiceTime, arrivalProb, maxQueueSize);

    // Paleidžiama simuliacija
    simulate(&bank, timeSteps);

    // Rezultatų spausdinimas
    printResults(&bank);

    // Resursų išlaisvinimas
    freeBank(&bank);

    return 0;
}
