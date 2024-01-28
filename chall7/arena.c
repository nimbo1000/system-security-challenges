#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_PLAYERS 1250
#define MAX_WEAPONS 50

#define PUNCH_SIZE  8192

/* Store weapon objects */
typedef struct weapon_s {
    char name[24];                  /* Name of the weapon */
    char description[PUNCH_SIZE];   /* TODO: future improvement */
    unsigned int minDamage;         /* Minimum damage of the weapon */
    unsigned int maxDamage;         /* Maximum damage of the weapon */
} *weapon_t;

/* Store player objects */
typedef struct player_s {
    unsigned int (*attack) (
            struct player_s **,
            unsigned int,
            struct player_s *);     /* Attack function (depending on strategy) */
    char name[24];                  /* Name of the warrior */
    char punchline[PUNCH_SIZE];     /* Battle cry */
    weapon_t weapon;                /* Used weapon */
    unsigned int hp;                /* Current health points */
} *player_t;

/* This is one of the possible strategies: target the weakest enemy */
unsigned int attackWeakest (player_t *players, unsigned int nbPlayers, player_t me) {
    unsigned int target;
    int i;

    if (nbPlayers < 2)
        return 0;

    for (target=0; (players[target]->hp == 0 || players[target] == me) && target < nbPlayers; target++);

    for (i=target+1; i<nbPlayers; i++)
        if (players[i]->hp > 0 && players[i] != me && players[i]->hp < players[target]->hp)
            target = i;

    return target;
}

/* This is one of the possible strategies: target the strongest enemy */
unsigned int attackStrongest (player_t *players, unsigned int nbPlayers, player_t me) {
    unsigned int target;
    int i;

    if (nbPlayers < 2)
        return 0;

    for (target=0; (players[target]->hp == 0 || players[target] == me) && target < nbPlayers; target++);

    for (i=target+1; i<nbPlayers; i++)
        if (players[i] != me && players[i]->hp > players[target]->hp)
            target = i;

    return target;
}

/* This is one of the possible strategies: target a random player (including the one attacking) */
unsigned int attackRandom (player_t *players, unsigned int nbPlayers, player_t me) {
    unsigned int target;

    for (target=0; players[target]->hp == 0 && target < nbPlayers; target++);
    if (target == nbPlayers)
        return nbPlayers;

    do {
        target = rand () % nbPlayers;
    } while (players[target]->hp == 0);

    return target;
}

/* Start a battle royale! */
unsigned int fight (player_t *players, unsigned int nbPlayers, weapon_t *weaponry, unsigned int nbWeapons) {
    int i;
    unsigned int remainingPlayers;
    unsigned int winner;

    if (nbPlayers < 2) {
        fprintf (stderr, "You should have at least two players for a proper fight.\n");
        return nbPlayers;
    }

    if (nbWeapons < 1) {
        fprintf (stderr, "Please load a set of weapons before fighting.\n");
        return nbPlayers;
    }

    /* Fill hp of every player and draw random weapons */
    for (i=0; i<nbPlayers; i++) {
        players[i]->hp = 100;
        players[i]->weapon = weaponry[rand () % nbWeapons];
    }

    /* Make them fight until only one remains */
    for (remainingPlayers = nbPlayers; remainingPlayers > 1;) {

        /* Draw the attacking player */
        unsigned int player, target, damage;
        do {
           player = rand () % nbPlayers;
        } while (players[player]->hp == 0);

        /* Choose the target according to the strategy of the player */
        target = (players[player]->attack) (players, nbPlayers, players[player]);

        /* Compute damage according to player's weapon */
        damage = players[player]->weapon->minDamage + (rand () % (players[player]->weapon->maxDamage - players[player]->weapon->minDamage + 1));
        if (damage > players[target]->hp)
            damage = players[target]->hp;

        /* Deal the damage */
        players[target]->hp -= damage;

        if (target == player)
            printf ("%s falls on his %s, losing %dhp...\n",
                    players[player]->name,
                    players[player]->weapon->name,
                    damage);
        else
            printf ("%s fiercely attacks %s with a %s, dealing %d damage !\n",
                    players[player]->name,
                    players[target]->name,
                    players[player]->weapon->name,
                    damage);

        if (players[target]->hp == 0) {
            /* Target is dead */
            printf ("  %s's soul reached his ancestors.\n", players[target]->name);
            remainingPlayers --;
        }
    }

    /* There should be only one player still alive */
    for (winner=0; players[winner]->hp == 0 && winner < nbPlayers; winner++);

    printf ("\n%s, last standing warrior, steps over the dead bodies of his opponents and, raising his %s towards the sky, shouts:\n",
                players[winner]->name, players[winner]->weapon->name);
    printf ("    -- \"%s\"\n", players[winner]->punchline);

    return winner;
}

/* Add a new player before the fight */
unsigned int addPlayer (player_t *players, unsigned int nbPlayers) {
    unsigned int player;
    char buffer[PUNCH_SIZE];
    char *pos;
    int str = 0;
    player_t p;

    if (nbPlayers < MAX_PLAYERS) {
        /* There is still place left for another fighter */
        player = nbPlayers;
        nbPlayers ++;
    } else {
        /* No slot left: get rid of a previous player */
        player = rand () % nbPlayers;
        printf ("Too many players... Kicking player '%s' (%p)\n", players[player]->name, (void *) players[player]);
        free (players[player]);
    }

    p = (player_t) malloc (sizeof (struct player_s));

    /* Get the name of the player */
    printf ("Give me a name (23 chars max): ");
    fflush (stdout);
    if (fgets (buffer, sizeof buffer, stdin) == NULL) exit (EXIT_FAILURE);
    strncpy (p->name, buffer, 23);
    p->name[23] = '\0';
    if ((pos=strchr(p->name, '\n')) != NULL) *pos = '\0';

    /* Get the punchline of the player */
    printf ("Give me a punchline (%d chars max): ", PUNCH_SIZE-1);
    fflush (stdout);
    if (fgets (buffer, sizeof buffer, stdin) == NULL) exit (EXIT_FAILURE);
    strncpy (p->punchline, buffer, PUNCH_SIZE-1);
    p->punchline[PUNCH_SIZE-1] = '\0';
    if ((pos=strchr(p->punchline, '\n')) != NULL) *pos = '\0';

    /* Get the strategy of the player */
    printf ("Choose a strategy (1: cunning, 2: dumb, 3: blind): ");
    fflush (stdout);
    if (fgets (buffer, sizeof buffer, stdin) == NULL) exit (EXIT_FAILURE);
    sscanf (buffer, "%d\n", &str);

    if (str == 1)
        p->attack = attackWeakest;
    else if (str == 2)
        p->attack = attackStrongest;
    else if (str == 3)
        p->attack = attackRandom;
    else {
        fprintf (stderr, "Unknown strategy. Aborting...\n");
        free (p);
        return nbPlayers;
    }

    /* Player has been successfully created so add it to players' array */
    players[player] = p;

    return nbPlayers;
}

/* Read the weaponry file to load the weapon set */
unsigned int readWeaponry (weapon_t *weaponry, FILE *fp) {
    char buffer[30];
    unsigned int nbWeapons = 0;

    /* Go the beginning of the file */
    fseek (fp, 0, SEEK_SET);

    while (fgets (buffer, sizeof buffer, fp) != NULL && nbWeapons < MAX_WEAPONS) {
        char *pos;
        int minDamage = 0;
        int maxDamage = 0;

        /* Each line is of the form "aa bb long name..." */
        /* "aa": min damage */
        /* "bb": max damage */
        /* "long name...": name of the weapon */
        buffer[29] = '\0';
        if ((pos=strchr(buffer, '\n')) != NULL) *pos = '\0';

        buffer[5] = '\0';
        if (sscanf (buffer, "%d %d", &minDamage, &maxDamage) != 2)
            continue;

        if (minDamage > 0 && maxDamage > minDamage) {
            weaponry[nbWeapons] = (weapon_t) malloc (sizeof (struct weapon_s));
            strncpy(weaponry[nbWeapons]->name, buffer+6, 23);
            weaponry[nbWeapons]->name[23] = '\0';
            weaponry[nbWeapons]->minDamage = minDamage;
            weaponry[nbWeapons]->maxDamage = maxDamage;
            nbWeapons ++;
        }
    }

    printf ("%d weapons loaded!\n", nbWeapons);

    return nbWeapons;
}

int main (int argc, char **argv) {
    FILE * fp;

    unsigned int nbPlayers = 0;
    unsigned int nbWeapons = 0;

    weapon_t weaponry[MAX_WEAPONS];
    player_t players[MAX_PLAYERS];

    if (argc < 2) {
        fprintf (stderr, "Usage: %s <weaponry>\n", argv[0]);
        exit (EXIT_FAILURE);
    }

    fp = fopen (argv[1], "r");
    if (fp == NULL) {
        fprintf (stderr, "Failed to open the weaponry file.\n");
        exit (EXIT_FAILURE);
    }

    /* Clear the environment, just to be sure... */
    clearenv ();

    srand(time(NULL));

    for (;;) {
        char buffer [256];
        int cmd = 0;

        printf ("\nWhat do you want to do? (1: load weaponry, 2: add new player, 3: fight, 4: quit) ");
        fflush (stdout);

        /* Read a command */
        if (fgets (buffer, sizeof buffer, stdin) == NULL) exit (EXIT_FAILURE);

        sscanf (buffer, "%d\n", &cmd);

        if (cmd == 1)
            nbWeapons = readWeaponry (weaponry, fp);
        else if (cmd == 2)
            nbPlayers = addPlayer (players, nbPlayers);
        else if (cmd == 3)
            fight (players, nbPlayers, weaponry, nbWeapons);
        else if (cmd == 4)
            break;
    }

    fclose (fp);

    return 0;
}
