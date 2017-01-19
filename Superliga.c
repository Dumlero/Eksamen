/*
 * Student: Lasse Ravn      Completed: Dec 1st, 2016
 * lrav16@student.aau.dk
 * B2-28a
 * Software
 * Instructor: Kurt Nymark  IMPR eksamen
 *
 * C-program that shows the stats from the danish football superliga.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define KAMPPRRUNDE 6  /* antal kampe per runde. Global kendt konstant */
#define ANTALHOLD 12
#define STORTNOK 30 /* til char array */

typedef struct PRINT{
  char plusfire[128];
  char udebanespillere[64];
  char bestemtekampe[64];
} PRINT;
typedef struct HOLD{
  char holdnavn[4];
  int hjemmevundet;
  int udevundet;
  int uafgjort;
  float hjemmetilskuere;
  int point;
  int maal;
  int maalimod;
} HOLD;
 typedef struct klokkeslaet{
   int timer;
   int minutter;
 } klokkeslaet;
 /* KAMP.tid.timer */
 typedef struct KAMP{
   int runde;
   char ugedag[4];
   int dag;
   int maaned;
   int aar;
   klokkeslaet tid;
   char hjemmehold[4];
   char udehold[4];
   int hjemmemaal;
   int udemaal;
   float tilskuertal;
 } KAMP;

/*void file_print(char *filename); */
int linietaeller(char *filename);
void gem_information(char *filename, KAMP *INFORMATION, int linietal);
void VIS_uafgjort(KAMP *INFORMATION, int linietal, PRINT *KAMPE);
int uafgjort_plus_fire(KAMP *INFORMATION, int linietal, PRINT *KAMPE);
int uafgjort_plus_fire_CHECKER(KAMP *INFORMATION, int i);
void VIS_mindre_end_10(KAMP *INFORMATION, int linietal);
void Mindre_end_10maal(KAMP *INFORMATION, int linietal, char *rundenr_og_maal);
void VIS_udebane_vinder(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER, PRINT *KAMPE);
int vinder_kampe(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER, PRINT *KAMPE);
void kamp_checker(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER);
void anden_struct_navnfyld(KAMP *INFORMATION, HOLD *OPLYSNINGER);
/*void ude_over_hjemme_print(HOLD *OPLYSNINGER, int h, PRINT *KAMPE); */
void faerrest_tilskuere_2015(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER,
                             char *daarlige_fans);
void vis_tidspunkt(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER, PRINT *KAMPE);
int tid_indlaes(KAMP *INFORMATION, int linietal, PRINT *KAMPE);
int tid_checker(KAMP *INFORMATION, char *tid_1, char *tid_2, char *kamp_tid, char *tidspunkt_dag, int g);
void vis_stilling(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER);
void optael_maal(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER);
void optael_point(HOLD *OPLYSNINGER);
int hold_sammenlign(const void *a, const void *b);




int main (int argc, char const *argv[]) {
   /* For at vælge hvilken option brugeren ønsker */
   int func_call;
      /* sender filnavn i istedet for fil til funktion */
   char filename[] = "superliga-2015-2016";
   char daarlige_fans[]="Faar inputtet, hvem der har faerrest tilskuere i 2015?";
   /* finder linietal for filen og gemmer det */
   int linietal = linietaeller(filename);
   /* Genererer array til fininformation */
   KAMP *INFORMATION;
      /* Til array af strings,når sådanne skal returneres. Massive memory use */
   PRINT *KAMPE;
   /* Array til at holde styr på de enkelte holds oplysninger */
   HOLD *OPLYSNINGER;
   /* allokerer plads til struct arrays */
   INFORMATION = malloc(linietal * sizeof(KAMP));
   KAMPE = malloc(STORTNOK * sizeof(PRINT));
   OPLYSNINGER = malloc(ANTALHOLD * sizeof(HOLD));

   /* Fylder filarray */
   gem_information(filename, INFORMATION, linietal);
   printf("\nScanned succesfully from the 2015-2016 data if %sdag == Mandag\n\n",
   INFORMATION[4].ugedag); /* Test if it works! */
   /* interface in progress*/

 if(argc > 1 && strcmp(argv[1], "--print") == 0){

   /* Opg 1 */
   VIS_uafgjort(INFORMATION, linietal, KAMPE);
   /* Opg 2 */
   VIS_mindre_end_10(INFORMATION, linietal);
   /* Opg 3 */
   VIS_udebane_vinder(INFORMATION, linietal, OPLYSNINGER, KAMPE);
   /* opg 4 */
   printf("\n****************** OPG 4 ******************\n");
   /* Crasher i min compiler, hvis længere end 64 tegn !? */
   /* crasher desuden også, hvis der laves underfunktion. */
   faerrest_tilskuere_2015(INFORMATION, linietal, OPLYSNINGER, daarlige_fans);
   printf("%s\n", daarlige_fans);
   /* opg 5 */
   vis_tidspunkt(INFORMATION, linietal, OPLYSNINGER, KAMPE);
   /* opg 6 */
   vis_stilling(INFORMATION, linietal, OPLYSNINGER);

 }else {  /* interface dialog*/
    printf("Hvilken funktion oensker De at benytte?\n"
           "Tryk 1 for at finde uafgjorte kampe med 4 maal eller mere.\n"
           "Tryk 2 for at finde en evt. runde, hvor der er scoret mindre end 10 maal.\n"
           "Tryk 3 for at se de hold, der vinder flere kampe ude end hjemme.\n"
           "Tryk 4 for at se det hold, der har faerrest tilskuere.\n"
           "Tryk 5 for at se de kampe, der finder sted paa et bestemt tidspunkt i ugen.\n"
           "Tryk 6 for at se saesonens stilling.\n");
    scanf("%d", &func_call);
    /* TIl styring af hvilken funktion, der kaldes */
    switch(func_call){
      case 1 :
       VIS_uafgjort(INFORMATION, linietal, KAMPE);
       break;
      case 2 :
        VIS_mindre_end_10(INFORMATION, linietal);
        break;
      case 3 :
        VIS_udebane_vinder(INFORMATION, linietal, OPLYSNINGER, KAMPE);
        break;
      case 4 :
        printf("\n****************** OPG 4 ******************\n");
        /* Crasher i min compiler, hvis længere end 64 tegn !? */
        /* Crasher desuden også, hvis der laves underfunktion lign. de andre */
        faerrest_tilskuere_2015(INFORMATION, linietal, OPLYSNINGER, daarlige_fans);
        printf("%s\n", daarlige_fans);
        break;
      case 5 :
        vis_tidspunkt(INFORMATION, linietal, OPLYSNINGER, KAMPE);
        break;
      case 6 :
       vis_stilling(INFORMATION, linietal, OPLYSNINGER);
       break;
    }
 }
   free (KAMPE);
   free(OPLYSNINGER);
   free(INFORMATION);
 return 0;
}


 /* optæl antal linjer i dokumentet, så der kan laves et tilsvarende størrelse array */
 int linietaeller(char *filename){
    int linie = 0;
   char cha;
   FILE *infilep = fopen(filename, "r");
   if (infilep == NULL){
     return printf("Cannot open inputfile");
   }
   /* tæller linie op en gang fordi den ellers ikke tæller for første linie */
   linie++;
   while (!feof(infilep)) {
     cha = fgetc(infilep);
     if (cha == '\n'){
       linie++;
     }
   }
   fclose(infilep);
   return linie;
 }

/* indlæser info fra fil og gemmer i struct array */
void gem_information(char *filename, KAMP *INFORMATION, int linietal){
  /* linietæller for at få passende størrelse array */
  int res=0;
  FILE *infilep = fopen(filename, "r");
  int i;
  for (i = 0; i < linietal;i++) {
    res = fscanf(infilep,
      "  R%d  %s     %d/%d/%d   %d.%d     %s - %s     %d - %d     %f ",   /* Whitespace lavet for forståelse */
      &INFORMATION[i].runde,
      INFORMATION[i].ugedag,
      &INFORMATION[i].dag,
      &INFORMATION[i].maaned,
      &INFORMATION[i].aar,
      &INFORMATION[i].tid.timer,
      &INFORMATION[i].tid.minutter,
      INFORMATION[i].hjemmehold,
      INFORMATION[i].udehold,
      &INFORMATION[i].hjemmemaal,
      &INFORMATION[i].udemaal,
      &INFORMATION[i].tilskuertal
    );
    if(res != 12){
      printf("Scan fejl - slukker\n");
      break;
    }else{
      /* fordi dotten ødelægger input bliver der ganget op med 1000 for at få korrekt tal */
      INFORMATION[i].tilskuertal = (INFORMATION[i].tilskuertal*1000);
      continue;
    }
  }
  fclose(infilep);
}

/* printer og kalder funktionen, der finder de kampe, hvor der var uafgjort og 4+ måL */
void VIS_uafgjort(KAMP *INFORMATION, int linietal, PRINT *KAMPE){
  int i, size;
  printf("\n****************** OPG 1 ******************\n" );
  size = uafgjort_plus_fire(INFORMATION, linietal, KAMPE);
  for (i = 0; i < size; i++) {
    printf("%s", KAMPE[i].plusfire);
  }
}

/* funktion der looper igennem struct og checker for uafgjorte kampe med +4 mål ved funktions
 * kald til checker, der kører et true/false statement, og derefter printer derefter */
int uafgjort_plus_fire(KAMP *INFORMATION, int linietal, PRINT *KAMPE){
  int i, g, a=0;
  printf("Runde\t Dag\t Dato\t\t Tid\t Hold\t\t Score\t Tilskuere\n" );
  for (i = 0; i < linietal; i++) {
    g = uafgjort_plus_fire_CHECKER(INFORMATION, i);
    /* Kunne laves til returnerbart multi-array ved at bruge sprintf til et
    array og så tælle en konstant op med en, der bruges til at styre arraypladsen,
     */
    if (g == i) {
      sprintf(KAMPE[a].plusfire, "%d\t %s\t %d/%d/%d\t %.2d:%.2d\t %s - %s\t %d - %d\t %.0f\n",
      INFORMATION[i].runde,
      INFORMATION[i].ugedag,
      INFORMATION[i].dag,
      INFORMATION[i].maaned,
      INFORMATION[i].aar,
      INFORMATION[i].tid.timer,
      INFORMATION[i].tid.minutter,
      INFORMATION[i].hjemmehold,
      INFORMATION[i].udehold,
      INFORMATION[i].hjemmemaal,
      INFORMATION[i].udemaal,
      INFORMATION[i].tilskuertal);
      a++;
    }
  }
 return a;
}

/* "false/true" checker for uafgjorte kampe med 4 og flere mål */
int uafgjort_plus_fire_CHECKER(KAMP *INFORMATION, int i){
  if ((INFORMATION[i].hjemmemaal == INFORMATION[i].udemaal) &&
     (INFORMATION[i].udemaal + INFORMATION[i].hjemmemaal) >= 4 ) {
       return i;
  }else
       return -1;
  }

/* Rammefunktion for at finde evt runde med mindre end 10 maal */
void VIS_mindre_end_10(KAMP *INFORMATION, int linietal){
  char rundenr_og_maal[]="Der er ingen runder med mindre end 10 maal";
  printf("\n****************** OPG 2 ******************\n" );
  Mindre_end_10maal(INFORMATION, linietal, rundenr_og_maal);
  printf("I hvilken runde er der scoret mindre end 10 maal?\n" );
  printf("%s\n\n", rundenr_og_maal);
}

/* funktion, der ved hjælp af struct og et nested for-loop finder evt. runde med <10maal */
void Mindre_end_10maal(KAMP *INFORMATION, int linietal, char *rundenr_og_maal){
  int i, maalakkumulator, h, j = 0;
  int g = INFORMATION[linietal-1].runde; /* for at få antal runder i alt */

  /* starter ved runde 1 og derfor i = 1 = rundetal */
  for (i = 1; i <= g; i++) {
    maalakkumulator = -1; /* Aner ikke hvorfor, men ellers returnes 9 og ikke 8, som er det rigtige.. */
    for (h = 1; h <= KAMPPRRUNDE; h++) {
      /* For at optælle en variabel korrekt til at køre igennem INFORMATION */
      if (j < linietal-1) {
        j++;
      }
      maalakkumulator = (maalakkumulator + (INFORMATION[j].udemaal + INFORMATION[j].hjemmemaal));
      /* Statement, der checker om runden har mindre end 10 mål */
        /*printf("Maal: %d i Runde: %d\n", maalakkumulator, i);  - TESTER*/
        if (maalakkumulator < 10 && h == 6) {
        memset(rundenr_og_maal, '\0', sizeof(rundenr_og_maal)/sizeof(char));
        sprintf(rundenr_og_maal, "Runde %d med kun %d maal", i, maalakkumulator);
        /* Kan evt. lave en variabel k og så et 2-dimensionelt array,
        så den er dynamisk med mere end 1 runde. */
      }
    }
  }
}

/* kalder funktion, der optæller kampe, og printer for de, der vinder flest kampe på udebane */
void VIS_udebane_vinder(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER, PRINT *KAMPE){
  int i, size;
  printf("\n****************** OPG 3 ******************\n");
  size = vinder_kampe(INFORMATION, linietal, OPLYSNINGER, KAMPE);
  for (i = 0; i < size; i++) {
    printf("%s", KAMPE[i].udebanespillere);
  }
}

/* fylder holdnavn i struct, samt får talt kampe op efter om de er vundet på ude ell hjemmebane,
 * eller om de er uafgjorte, og til sidst søger structen for et hold, der evt. har vundet flere
 * udebanekampe */
int vinder_kampe(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER, PRINT *KAMPE){
  int a = 0, h;
  /* indlæs holdnavne i struct fra det andet struct.
  2nd */
  anden_struct_navnfyld(INFORMATION, OPLYSNINGER);

  /* optæller om der er vundet eller uafgjort */
  kamp_checker(INFORMATION, linietal, OPLYSNINGER);
  for (h = 0; h < ANTALHOLD; h++) {
      if (OPLYSNINGER[h].udevundet > OPLYSNINGER[h].hjemmevundet) {
        sprintf(KAMPE[a].udebanespillere, "%s vandt %d kampe paa udebane men kun %d paa hjemmebane\n\n",
        OPLYSNINGER[h].holdnavn, OPLYSNINGER[h].udevundet, OPLYSNINGER[h].hjemmevundet);
        a++;
      }
    }
  return a;
}

/* flytter navnene over i sekundært struct, hvor der er givet plads til point og lign */
/* bruger 11, for at blive indenfor array */
void anden_struct_navnfyld(KAMP *INFORMATION, HOLD *OPLYSNINGER){
  int i;
  for (i = 0; i < KAMPPRRUNDE; i++) {
    strncpy(OPLYSNINGER[i].holdnavn, INFORMATION[i].hjemmehold, 4);
    strncpy(OPLYSNINGER[11-i].holdnavn, INFORMATION[i].udehold, 4);
  }
}

/* Optæller kampe for hvert hold. Hvor mange vundet på ude og hjemme, samt uafgjort */
void kamp_checker(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER){
  int h, g;
  /* Initialiserer alle til 0, således vi har kontrol over værdierne */
  for (h = 0; h < ANTALHOLD; h++) {
    OPLYSNINGER[h].uafgjort = 0;
    OPLYSNINGER[h].udevundet = 0;
    OPLYSNINGER[h].hjemmevundet = 0;
  }
  /* optæller om der er vundet eller uafgjort */
  for (g = 0; g < linietal; g++) {
    if (INFORMATION[g].hjemmemaal == INFORMATION[g].udemaal) {
      for (h = 0; h < ANTALHOLD; h++) {
        if (strcmp(INFORMATION[g].hjemmehold, OPLYSNINGER[h].holdnavn) == 0) {
          ++OPLYSNINGER[h].uafgjort;
        }else if (strcmp(INFORMATION[g].udehold, OPLYSNINGER[h].holdnavn) == 0){
          ++OPLYSNINGER[h].uafgjort;
        }
      }
    }else if (INFORMATION[g].hjemmemaal < INFORMATION[g].udemaal){
      for (h = 0; h < ANTALHOLD; h++) {
        if (strcmp(INFORMATION[g].udehold, OPLYSNINGER[h].holdnavn) == 0){
          ++OPLYSNINGER[h].udevundet;
        }
      }
    }else if (INFORMATION[g].hjemmemaal > INFORMATION[g].udemaal){
      for (h = 0; h < ANTALHOLD; h++) {
        if (strcmp(INFORMATION[g].hjemmehold, OPLYSNINGER[h].holdnavn) == 0){
          ++OPLYSNINGER[h].hjemmevundet;
        }
      }
    }
  }
}

/* finder det hold med færrest tilskuere i 2015 ved brug af strcmmp og struct,
 * hvori, der gemmes hvor mange tilskuere hver hold, har haft. Gemmes til sidst i
 * en string ved brug af sprintf */
void faerrest_tilskuere_2015(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER,
                             char *daarlige_fans){
  int g, h, i, hold;
  /* Initialiseres til nok */
  float minimum=1000000;

  /* sætter alle tilskuertal i mit struct til at være 0, da jeg ellers
    ikke kender til deres indhold */
  for (h = 0; h < ANTALHOLD; h++){
    OPLYSNINGER[h].hjemmetilskuere = 0;
  }
  /*Fylder navne i struct */
  anden_struct_navnfyld(INFORMATION, OPLYSNINGER);
  /* Manglende trinvis forfinelse */
  for (g = 0; g < linietal; g++){
    if (INFORMATION[g].aar == 2015) {

      for (i = 0; i < ANTALHOLD; i++) {
        if (strcmp(INFORMATION[g].hjemmehold, OPLYSNINGER[i].holdnavn) == 0) {
          OPLYSNINGER[i].hjemmetilskuere = OPLYSNINGER[i].hjemmetilskuere + INFORMATION[g].tilskuertal;
        }
      }
    }
  }
  for (h = 0; h < ANTALHOLD; h++) {
    if (OPLYSNINGER[h].hjemmetilskuere < minimum) {
      minimum = OPLYSNINGER[h].hjemmetilskuere;
      hold = h;
    }
  }
  /* string reset */
  memset(daarlige_fans, '\0', sizeof(daarlige_fans)/sizeof(char));
  sprintf(daarlige_fans,"De daarlige fans tilhoerer %s med kun %.0f tilskuere paa hjemmebane i 2015\n",
  OPLYSNINGER[hold].holdnavn, minimum);

}

/* Rammefunktion, der kalder underfunktioner til at finde kampe, der foregår
 * indenfor en bestemt tidsramme */
void vis_tidspunkt(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER, PRINT *KAMPE){
  int i, size;
  printf("\n****************** OPG 5 ******************\n");
  size = tid_indlaes(INFORMATION, linietal, KAMPE);
  for (i = 0; i < size; i++) {
    printf("%s", KAMPE[i].bestemtekampe);
  }
}

/* Funktion, der Initialiserer de inputs jeg skal bruge til at finde kampe på bestemte tidspunkter */
int tid_indlaes(KAMP *INFORMATION, int linietal, PRINT *KAMPE){
  int g, h=0;
  char tid_1[]="13:15";
  char tid_2[]="14:15";
  char kamp_tid[]="indlaeses fra struct";
  char tidspunkt_dag[]="Son";
  printf("Denne funktion finder kampe foregaaende paa en bestemt ugedag.\n"
        "Funktionen er defineret til at finde kampe foregaaende mellem %s og %s om %sdagen\n",
         tid_1, tid_2, tidspunkt_dag);
  /* String reset, så vi er sikret mod ukendte værdier */

  for (g = 0; g < linietal; g++){
    if(tid_checker(INFORMATION, tid_1, tid_2, kamp_tid, tidspunkt_dag, g) >= 0){
      sprintf(KAMPE[h].bestemtekampe, "%s - %s spiller %s d. %d/%d/%d, klokken %.2d:%.2d\n",
      INFORMATION[g].hjemmehold, INFORMATION[g].udehold, INFORMATION[g].ugedag,
      INFORMATION[g].dag, INFORMATION[g].maaned, INFORMATION[g].aar,
      INFORMATION[g].tid.timer, INFORMATION[g].tid.minutter);
      h++;
    }
  }
 return h;
}

/* Omdanner integers til string, for at kunne bruge strcmp og så kunne checke
 * kamp-arrayet igennem og finde, de kampe, der foregår indenfor denne.
 * som derpå bliver gemt i struct med sprintf. */
int tid_checker(KAMP *INFORMATION, char *tid_1, char *tid_2, char *kamp_tid, char *tidspunkt_dag, int g){
  sprintf(kamp_tid, "%.2d:%.2d", INFORMATION[g].tid.timer, INFORMATION[g].tid.minutter);
  if (strcmp(INFORMATION[g].ugedag, tidspunkt_dag) == 0) {
    if (strcmp(tid_1, kamp_tid) <= 0 && strcmp(tid_2, kamp_tid) >= 0){
      return g;
    }else{
      return -1;
    }
  }else{
    return -1;
  }
}

/* Kalder de nødvendige funktioner for at kunne vise holdstillingen til slut i sæsonen */
void vis_stilling(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER){
  int g;
  printf("\n****************** OPG 6 ******************\n" );
  anden_struct_navnfyld(INFORMATION, OPLYSNINGER);
  kamp_checker(INFORMATION, linietal, OPLYSNINGER);
  optael_maal(INFORMATION, linietal, OPLYSNINGER);
  optael_point(OPLYSNINGER);
  qsort(OPLYSNINGER, ANTALHOLD, sizeof(HOLD), hold_sammenlign);
  printf("Klub\t K\t V\t U\t T\t   Maal\t\t Diff\t Point\n");
  printf("_______________________________________________________________________\n");
  for (g = 0; g < ANTALHOLD; g++) {
    printf("%s\t 33\t %d\t %d\t %d\t %d - %d\t %+3d\t %d\n", OPLYSNINGER[g].holdnavn,
    OPLYSNINGER[g].hjemmevundet + OPLYSNINGER[g].udevundet, OPLYSNINGER[g].uafgjort,
    INFORMATION[linietal-1].runde - (OPLYSNINGER[g].hjemmevundet + OPLYSNINGER[g].udevundet +
    OPLYSNINGER[g].uafgjort), OPLYSNINGER[g].maal, OPLYSNINGER[g].maalimod,
    OPLYSNINGER[g].maal - OPLYSNINGER[g].maalimod, OPLYSNINGER[g].point);
  }
  printf("_______________________________________________________________________\n");
}

/* Optæller maal for de enkelte hold ved at gennemløbe kampene og gemme i struct */
void optael_maal(KAMP *INFORMATION, int linietal, HOLD *OPLYSNINGER){
  int g, i;
  for (g = 0; g < ANTALHOLD; g++){
    OPLYSNINGER[g].maal = 0;
    OPLYSNINGER[g].maalimod = 0;
  }
  for (g = 0; g < linietal; g++) {
    for (i = 0; i < ANTALHOLD; i++) {
      if (strcmp(INFORMATION[g].hjemmehold, OPLYSNINGER[i].holdnavn) == 0){
        OPLYSNINGER[i].maal = OPLYSNINGER[i].maal + INFORMATION[g].hjemmemaal;
        OPLYSNINGER[i].maalimod = OPLYSNINGER[i].maalimod + INFORMATION[g].udemaal;
      }else if (strcmp(INFORMATION[g].udehold, OPLYSNINGER[i].holdnavn) == 0) {
        OPLYSNINGER[i].maal = OPLYSNINGER[i].maal + INFORMATION[g].udemaal;
        OPLYSNINGER[i].maalimod = OPLYSNINGER[i].maalimod + INFORMATION[g].hjemmemaal;
      }
    }
  }
  /* QSORT DEBUGGING
  char holdfck[]="FCK";
  char holdsdr[]="SDR";
for (i = 0; i < ANTALHOLD; i++) {
  if (strcmp(OPLYSNINGER[i].holdnavn, holdfck) == 0) {
  OPLYSNINGER[i].maal = OPLYSNINGER[i].maal - 14;
  OPLYSNINGER[i].maalimod = OPLYSNINGER[i].maalimod + 8; /* HUSK !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
  if (strcmp(OPLYSNINGER[i].holdnavn, holdsdr) == 0) {
  OPLYSNINGER[i].maal = OPLYSNINGER[i].maal - 8; /* HUSK !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
}
 */
  /*for (g = 0; g < ANTALHOLD; g++){
    printf("Maal: %d, maalimod: %d\n",OPLYSNINGER[g].maal, OPLYSNINGER[g].maalimod );
  }*/
}

/* Optæller point for de enkelte hold ved at gennemløbe kampene og gemme i struct */
void optael_point(HOLD *OPLYSNINGER){
  int g;
  /* QSORT DEBUGGING
  char holdfck[]="FCK"; */
/* Obsolete pga det bliver tildelt i næste for-løkke
    for (g = 0; g < ANTALHOLD; g++){
    OPLYSNINGER[g].point = 0;
  } */
  for (g = 0; g < ANTALHOLD; g++) {
    OPLYSNINGER[g].point = ((OPLYSNINGER[g].udevundet + OPLYSNINGER[g].hjemmevundet) * 3);
    OPLYSNINGER[g].point = OPLYSNINGER[g].point + (OPLYSNINGER[g].uafgjort * 1);
/* QSORT DEBUGGING
    if (strcmp(OPLYSNINGER[g].holdnavn, holdfck) == 0) {
      OPLYSNINGER[g].point = OPLYSNINGER[g].point - 9; /* HUSK !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }  */
  } 
  /*for (g = 0; g < ANTALHOLD; g++){
    printf("%d\n", OPLYSNINGER[g].point);
  } */
}

/* qsort på arrayet på baggrund af forskellige parametre, der tages højde for
 * ved brug af if/else -statements, som er afgørende for, hvilken qsort den returnerer*/
int hold_sammenlign(const void *a, const void *b){
  HOLD *HOLDA = (HOLD *)a,
       *HOLDB = (HOLD *)b;

       /* Forståelse samt ide til if/else kæde til sortering fået fra samtale med gruppemedlem */
  if (HOLDB->point == HOLDA->point &&
     (HOLDB->maal - HOLDB->maalimod) == (HOLDA->maal - HOLDA->maalimod) &&
      HOLDB->maal == HOLDA->maal) {
    return strcmp(HOLDA->holdnavn, HOLDB->holdnavn);

  }else if(HOLDA->point == HOLDB->point &&
     (HOLDB->maal - HOLDB->maalimod) == (HOLDA->maal - HOLDA->maalimod)){
    return (HOLDB->maal - HOLDA->maal);

  }else if(HOLDA->point == HOLDB->point){
    return (HOLDB->maal - HOLDB->maalimod) - (HOLDA->maal - HOLDA->maalimod);

  }else{
    return (HOLDB->point - HOLDA->point);
  }
}
