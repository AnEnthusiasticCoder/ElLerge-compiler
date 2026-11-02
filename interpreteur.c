#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definir les mnemoniques
typedef enum
{
    ADD,
    SUB,
    MUL,
    DIV,
    EQL,
    NEQ,
    GTR,
    LSS,
    GEQ,
    LEQ,
    PRN,
    INN,
    INT,
    LDI,
    LDA,
    LDV,
    STO,
    BRN,
    BZE,
    HLT,
    LEN,
    FLDA,
    SLDA,
    BLDA,
    FLDI,
    SLDI,
    BLDI,
    TLDA,
    ILDA,
    ILDI,
    EFOR,
    BFOR,
    TABI,
    TABA,
    CASET,
    CASEE
} MNEMONIQUES;

// Changement
int bfor_value = 0; // Pour stocker la valeur de BFOR
int efor_value = 0; // Pour stocker la valeur de EFOR
int loop_counter = 0; // Pour compter le nombre d'iterations

// Definir la structure d'instruction
typedef struct
{
    MNEMONIQUES MNE;
    int SUITE;
} INSTRUCTION;

// Definir la pile et la memoire
#define STACK_SIZE 1000
#define MEMORY_SIZE 1000

//.......................2..................................

#define LOOP_STACK_SIZE 100

int loop_stack[LOOP_STACK_SIZE]; // Pile pour les adresses de retour des boucles
int loop_sp = -1;                // Pointeur de pile pour les boucles
//..............................................................

double memory[MEMORY_SIZE]; // Memoire pour les entiers et les flottants
double stack[STACK_SIZE];   // Pile pour les operations (maintenant double)
int sp = -1;                // Pointeur de pile
int pc = 0;                 // Compteur de programme

// Programme P-code
INSTRUCTION pcode[MEMORY_SIZE];
int pcode_size = 0;

// Fonction pour empiler une valeur sur la pile
void push(double value)
{
    if (sp >= STACK_SIZE - 1)
    {
        printf("Stack overflow!\n");
        exit(EXIT_FAILURE);
    }
    stack[++sp] = value;
}

// Fonction pour depiler une valeur de la pile
double pop()
{
    if (sp < 0)
    {
        printf("Stack underflow!\n");
        exit(EXIT_FAILURE);
    }
    return stack[sp--];
}

//...........................................ajout2 loopfor

// Fonction pour empiler une adresse de retour
void push_loop(int address) {
    if (loop_sp >= LOOP_STACK_SIZE - 1) {
        printf("Loop stack overflow!\n");
        exit(EXIT_FAILURE);
    }
    loop_stack[++loop_sp] = address;
}

// Fonction pour depiler une adresse de retour
int pop_loop() {
    if (loop_sp < 0) {
        printf("Loop stack underflow!\n");
        exit(EXIT_FAILURE);
    }
    return loop_stack[loop_sp--];
}

//..................................................fin ajout loopfor................

// Fonction pour executer le P-code
void execute_pcode()
{
//.................................................................

    int loop_start = 0; // Pour stocker l'adresse de debut de la boucle
    int loop_counter = 0; // Pour compter le nombre d'iterations
//..............................................................
    while (pc < pcode_size)
    {
        INSTRUCTION instr = pcode[pc];
        printf("Execution de l'instruction %d: mnemonic=%d, input=%d\n", pc, instr.MNE, instr.SUITE); // Debugging
        printf("Etat de la pile: ");
        for (int i = 0; i <= sp; i++)
        {
            printf("%.2f ", stack[i]); // Afficher les valeurs de la pile en double
        }
        printf("\n");

        MNEMONIQUES op = instr.MNE;
        int arg = instr.SUITE;

        switch (op)
        {
        case ADD:
        {
            double b = pop();
            double a = pop();
            push(a + b);
            pc++;
            break;
        }
        case SUB:
        {
            double b = pop();
            double a = pop();
            push(a - b);
            pc++;
            break;
        }
        case MUL:
        {
            double b = pop();
            double a = pop();
            push(a * b);
            pc++;
            break;
        }
        case DIV:
        {
            double b = pop();
            double a = pop();
            push(a / b);
            pc++;
            break;
        }
        case EQL:
        {
            double b = pop();
            double a = pop();
            push(a == b);
            pc++;
            break;
        }
        case NEQ:
        {
            double b = pop();
            double a = pop();
            push(a != b);
            pc++;
            break;
        }
        case GTR:
        {
            double b = pop();
            double a = pop();
            push(a > b);
            pc++;
            break;
        }
        case LSS:
        {
            double b = pop();
            double a = pop();
            push(a < b);
            pc++;
            break;
        }
        case GEQ:
        {
            double b = pop();
            double a = pop();
            push(a >= b);
            pc++;
            break;
        }
        case LEQ:
        {
            double b = pop();
            double a = pop();
            push(a <= b);
            pc++;
            break;
        }
        case PRN:
        {
            double value = pop();
            if (value == (int)value)
            {
                printf("%d\n", (int)value); // Afficher en entier si c'est un nombre entier
            }
            else
            {
                printf("%.2f\n", value); // Afficher en flottant sinon
            }
            pc++;
            break;
        }
        case INN:
        {
            int address = (int)pop();
            printf("Entrez un nombre: ");
            scanf("%lf", &memory[address]); // Lire un double
            pc++;
            break;
        }
        case INT:
        {
            sp += arg; // Allouer de l'espace pour les variables
            pc++;
            break;
        }
        case LDI:
        {
            push((double)arg); // Charger une valeur entiere immediate en double
            pc++;
            break;
        }
        case LDA:
        {
            push((double)arg); // Charger une adresse en double
            pc++;
            break;
        }
        case LDV:
        {
            int address = (int)pop();
            push(memory[address]); // Charger une valeur depuis la memoire
            pc++;
            break;
        }
        case STO:
        {
            double value = pop();     // Valeur a stocker
            int address = (int)pop(); // Adresse ou stocker
            memory[address] = value;
            pc++;
            break;
        }
        case BRN:
        {
            pc = arg; // Branchement inconditionnel
            break;
        }
        case BZE:
        {
            double condition = pop();
            if (condition == 0)
                pc = arg; // Branchement si zero
            else
                pc++;
            break;
        }
        case HLT:
        {
            printf("Programme arrete.\n");
            return; // Terminer le programme
        }
        case LEN:
        {
            // push(arg); // Empiler la longueur sur la pile
            pc++;
            break;
        }
        case FLDA:
        {
            push((double)arg); // Charger l'adresse d'un flottant
            pc++;
            break;
        }
        case FLDI:
        {
            // Collecter les valeurs ASCII pour le flottant
            char buffer[32];
            int len = 0;
            do
            {
                buffer[len++] = (char)arg; // Stocker la valeur ASCII
                pc++;
                if (pc >= pcode_size)
                    break;             // Eviter l'acces hors limites
                arg = pcode[pc].SUITE; // Obtenir la prochaine valeur ASCII
            } while (pcode[pc].MNE == FLDI); // Continuer jusqu'a ce que toutes les valeurs FLDI soient traitees
            buffer[len] = '\0';          // Terminer la chaine par un caractere nul
            double value = atof(buffer); // Convertir en double
            push(value);                 // Empiler la valeur flottante sur la pile
            break;
        }
        case ILDA:
        {
            push((double)arg); // Charger l'adresse d'un entier
            pc++;
            break;
        }
        case ILDI:
        {
            push((double)arg); // Charger une valeur entiere immediate en double
            pc++;
            break;
        }

        //......................................

        case TLDA:
        {
            // Charger l'adresse d'un tableau
            push((double)arg); // Empiler l'adresse du tableau
            pc++;
            break;
        }

        case TABA:
        {
            // Empiler l'adresse de base du tableau
            push((double)arg); // arg contient l'adresse de base du tableau
            pc++;
            break;
        }
        case TABI:
        {
            // Depiler l'adresse de base du tableau
            double base_address = pop();

            // Calculer l'adresse effective : base_address + arg (indice)
            double effective_address = base_address + (double)arg;

            // Empiler l'adresse effective
            push(effective_address);
            pc++;
            break;
        }

        //..................................BFOR ET EFOR ..............................***************

        // case BFOR:
        // {
        //     // Sauvegarder l'adresse de retour (debut de la boucle)
        //     push_loop(pc + 1);

        //     // Charger la valeur de depart sur la pile
        //     push((double)arg);

        //     // Stocker la valeur de depart dans la variable de boucle
        //     int address = (int)pop(); // Adresse de la variable de boucle
        //     memory[address] = (double)arg;

        //     pc++;
        //     break;
        // }
        // case EFOR:
        // {
        //     // Recuperer l'adresse de retour (debut de la boucle)
        //     int loop_start = pop_loop();

        //     // Charger la valeur actuelle de la variable de boucle
        //     int address = (int)pop(); // Adresse de la variable de boucle
        //     double current_value = memory[address];

        //     // Verifier si la condition de fin est atteinte
        //     if (current_value <= (double)arg) {
        //         // Incrementer la variable de boucle
        //         memory[address] += 1;

        //         // Re-empiler l'adresse de la variable de boucle pour la prochaine iteration
        //         push((double)address);

        //         // Revenir au debut de la boucle
        //         pc = loop_start - 1; // -1 car pc est incremente apres chaque instruction
        //     } else {
        //         // Sortir de la boucle
        //         pc++;
        //     }
        //     break;
        // }

        // case BRN:
        // {
        //     // Calculer la difference entre efor_value et bfor_value
        //     int difference = efor_value - bfor_value;

        //     if (difference== 0){
        //         pc=arg;
                
        //     }
        //     else{
        //         // Repeter la boucle autant de fois que la difference
        //     if (loop_counter < difference)
        //     {
        //         pc = loop_start; // Revenir au debut de la boucle
        //         loop_counter++; // Incrementer le compteur de boucle
        //     }
        //     else
        //     {
            
        //         pc++; // Sortir de la boucle
        //         loop_counter = 0; // Reinitialiser le compteur de boucle
        //         efor_value =0;
        //         bfor_value=0;

        //     }
        //     }
        //     break;
        // }
        //......................................******************

        default:
        {
            printf("Instruction inconnue: %d\n", op);
            exit(EXIT_FAILURE);
        }
        }
    }
}

// Fonction pour charger le P-code depuis un fichier
void load_pcode(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char mnemonic[10];
    int input;
    while (fscanf(file, "%s %d", mnemonic, &input) != EOF)
    {
        MNEMONIQUES op;
        if (strcmp(mnemonic, "ADD") == 0)
            op = ADD;
        else if (strcmp(mnemonic, "SUB") == 0)
            op = SUB;
        else if (strcmp(mnemonic, "MUL") == 0)
            op = MUL;
        else if (strcmp(mnemonic, "DIV") == 0)
            op = DIV;
        else if (strcmp(mnemonic, "EQL") == 0)
            op = EQL;
        else if (strcmp(mnemonic, "NEQ") == 0)
            op = NEQ;
        else if (strcmp(mnemonic, "GTR") == 0)
            op = GTR;
        else if (strcmp(mnemonic, "LSS") == 0)
            op = LSS;
        else if (strcmp(mnemonic, "GEQ") == 0)
            op = GEQ;
        else if (strcmp(mnemonic, "LEQ") == 0)
            op = LEQ;
        else if (strcmp(mnemonic, "PRN") == 0)
            op = PRN;
        else if (strcmp(mnemonic, "INN") == 0)
            op = INN;
        else if (strcmp(mnemonic, "INT") == 0)
            op = INT;
        else if (strcmp(mnemonic, "LDI") == 0)
            op = LDI;
        else if (strcmp(mnemonic, "LDA") == 0)
            op = LDA;
        else if (strcmp(mnemonic, "LDV") == 0)
            op = LDV;
        else if (strcmp(mnemonic, "STO") == 0)
            op = STO;
        else if (strcmp(mnemonic, "BRN") == 0)
            op = BRN;
        else if (strcmp(mnemonic, "BZE") == 0)
            op = BZE;
        else if (strcmp(mnemonic, "HLT") == 0)
            op = HLT;
        else if (strcmp(mnemonic, "LEN") == 0)
            op = LEN;
        else if (strcmp(mnemonic, "FLDA") == 0)
            op = FLDA;
        else if (strcmp(mnemonic, "FLDI") == 0)
            op = FLDI;
        else if (strcmp(mnemonic, "ILDA") == 0)
            op = ILDA;
        else if (strcmp(mnemonic, "ILDI") == 0)
            op = ILDI;
        else if (strcmp(mnemonic, "BFOR") == 0) // Ajout de BFOR
            op = BFOR;
        else if (strcmp(mnemonic, "EFOR") == 0) // Ajout de EFOR
            op = EFOR;
        else if (strcmp(mnemonic, "TABA") == 0) // Ajout de TABA
            op = TABA;
        else if (strcmp(mnemonic, "TABI") == 0) // Ajout de TABI
            op = TABI;
        else if (strcmp(mnemonic, "TLDA") == 0) // Ajout de TLDA
            op = TLDA;
        else
        {
            printf("Mnemonic inconnu: %s\n", mnemonic);
            exit(EXIT_FAILURE);
        }

        pcode[pcode_size].MNE = op;
        pcode[pcode_size].SUITE = input;
        pcode_size++;
    }

    fclose(file);
}

// Fonction principale
int main()
{
    // Charger le P-code depuis un fichier
    load_pcode("fichierSortie.po");

    // Executer le P-code
    execute_pcode();

    return 0;
}