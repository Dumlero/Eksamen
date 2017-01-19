/**
 * Dette er programmet som er afleveret i forbindelse med IMPR-eksamen
 * på AAU, d. 1 december 2016 klokken 12:00.
 *
 * * Programmet er skrevet af:
 * * Malte Zoëga Andreasen
 * * mzan16@student.aau.dk
 * * B2-28a
 * * Software
 *
 * Selve programmet indlæser en test-fil, med en masse oplysninger om
 * de 198 kampe der har været i Superligaen 2015/-16. Disse data behandles
 * og udskrives, ud fra en opgavebeskrivelse.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N_RUNDER 33
#define KAMPE_PR_N 6
#define N_KAMPE N_RUNDER * KAMPE_PR_N
#define N_HOLD 12
typedef enum hold_navn {AGF, FCM, HOB, RFC, EFB, SDR, FCN,
                        BIF, FCK, OB,  AAB, VFF, NaN} hold_navn;
typedef struct k_dat {
  int  runde;
  char ugedag[4];
  int  dato,
       maaned,
       aar,
       timer,
       minutter;
  char hold_hjemme[4];
  char hold_ude[4];
  int  maal_hjemme,
       maal_ude;
  double tilskuere;
} k_dat;
typedef struct hold_statstik {
  hold_navn navn;
  char navn_str[4];
  int point,
      sejre_hjemme,
      sejre_ude,
      uafgjorte,
      maal_scoret,
      maal_ikke_reddet,
      n_antal_kampe;
  double aku_tilskuere_hjemme_2015,
         aku_tilskuere_hjemme_2016;
} hold_statstik;
void GetKampe(k_dat *kamp);
int comp_hold(const void *a, const void *b);
void PrintKampe(k_dat *kamp, int N);
int TestForUafgjort(k_dat *uafgjorte_kampe, k_dat *kamp);
int MindreEnd10Maal(int output[33][2], k_dat *kamp);
void GetHoldStatistik(hold_statstik *hold, k_dat *kamp);
void ResetHold(hold_statstik *hold);
hold_navn HoldTilEnum(char *navn);
int GetUdeSejrende(hold_statstik *udesejrende, hold_statstik *hold);
void PrintUdeSejrende(hold_statstik *input, int n);
int KampKlokken(k_dat *output, k_dat *kamp);
int KampKlokken_std(k_dat *output, k_dat *kamp);
void EnumTilStr(char *str, hold_navn navn);
void LavVinderTabel(hold_statstik *hold);
void PrintVinderTabel(hold_statstik *hold);
void PrintMindreEnd10Maal(int input[33][2], int n);
void PrintLavesteTilskuere2015(hold_statstik *hold);
void PrintInstruktioner(void);

/**
 * Dette er selve main-funktionen, hvori der interageres med brugeren.
 */
int main(int argc, char const *argv[]) {
  k_dat *kamp = (k_dat*) malloc(N_KAMPE * sizeof(k_dat));
  k_dat *uafgjorte_kampe = (k_dat*) malloc(N_KAMPE * sizeof(k_dat));
  k_dat *klokke_kampe = (k_dat*) malloc(N_KAMPE * sizeof(k_dat));
  hold_statstik hold[N_HOLD+1];
  hold_statstik udesejrende[N_HOLD];
  int runder_med_under_10_maal[33][2];
  int n_uafgjorte, n_under_10_maal, n_udesejrende, n_kampklokken;
  char user_input_chr;
  int user_input_int;
  /**
   * Data indlæses, og første behandling sker allerede her, inden der er
   * snakket med brugeren overhovedet.
   */
  GetKampe(kamp);
  GetHoldStatistik(hold, kamp);

  /**
   * Hvis, og kun hvis, brugeren starter programmet med argumentet "--print"
   * vil programmet løbe igennem alle 6 opgaver, og printe resultatet på
   * skærmen, uden at brugeren behøves at lave noget ekstra input.
   * De 6 opgaver bliver beskrevet for brugeren, efterfulgt at opgavens
   * udførelse.
   */
  if ((argc > 1) && (strcmp(argv[1],"--print") == 0)) {
    printf("\n\t\t##### OPGAVE 1 #####\n"
           "I denne opgave vil programmet finde de alle kampe som ender uafgjort, hvor der\n"
           "er scoret fire maal eller flere.\n\n");
    n_uafgjorte = TestForUafgjort(uafgjorte_kampe, kamp);
    PrintKampe(uafgjorte_kampe, n_uafgjorte);

    printf("\n\n\t\t##### OPGAVE 2 #####\n"
           "I denne opgave vil programmet finde alle de runder hvori der er scoret\n"
           "mindre end 10 maal.\n\n");
    n_under_10_maal = MindreEnd10Maal(runder_med_under_10_maal, kamp);
    PrintMindreEnd10Maal(runder_med_under_10_maal, n_under_10_maal);

    printf("\n\n\t\t##### OPGAVE 3 #####\n"
           "I denne opgave vil programmet finde alle de hold som vinder flere\n"
           "kampe paa udebane end paa hjemmebane\n\n");
    n_udesejrende = GetUdeSejrende(udesejrende, hold);
    PrintUdeSejrende(udesejrende, n_udesejrende);

    printf("\n\n\t\t##### OPGAVE 4 #####\n"
           "I denne opgave vil programmet finde det hold som har haft det mindste\n"
           "antal samlede tilskuere paa hjemmebane i 2015.\n\n");
    PrintLavesteTilskuere2015(hold);

    printf("\n\n\t\t##### OPGAVE 5 #####\n"
           "I denne opgave vil programmet finde alle de kampe som spilles en\n"
           "bestemt ugedag, inden for et bestemt tidsinterval.\n"
           "I dette gennemloeb er dagen fastsat til soendag (Son), og tidsintervallet\n"
           "er sat til 13:15 - 14:15.\n\n");
    n_kampklokken = KampKlokken_std(klokke_kampe, kamp);
    PrintKampe(klokke_kampe, n_kampklokken);

    printf("\n\n\t\t##### OPGAVE 6 #####\n"
           "I denne opgave vil programmet finde alle deltagende holds placering i\n"
           "superligaen 2015/-16, stillet op i en meget flot tabel.\n\n");
    LavVinderTabel(hold);
    PrintVinderTabel(hold);
  } else {
    do {
      /**
       * Hvis brugeren har startet programmet uden "--print", vil der først
       * blive printet nogle instruktioner, så brugeren kan interagere med
       * programmet. Brugeren vil blive ved med at kunne bede om en ny
       * opgave, indtil brugeren selv slutter programmet.
       */
      PrintInstruktioner();
      printf("Indtast venligst den opgave du vil have lavet: ");
      scanf(" %d", &user_input_int);
      switch (user_input_int) {
        case 1: /* Opgave 1 vælges, og udføres. */
          n_uafgjorte = TestForUafgjort(uafgjorte_kampe, kamp);
          PrintKampe(uafgjorte_kampe, n_uafgjorte);
          break;
        case 2: /* Opgave 2 vælges, og udføres. */
          n_under_10_maal = MindreEnd10Maal(runder_med_under_10_maal, kamp);
          PrintMindreEnd10Maal(runder_med_under_10_maal, n_under_10_maal);
          break;
        case 3: /* Opgave 3 vælges, og udføres. */
          n_udesejrende = GetUdeSejrende(udesejrende, hold);
          PrintUdeSejrende(udesejrende, n_udesejrende);
          break;
        case 4: /* Opgave 4 vælges, og udføres. */
          PrintLavesteTilskuere2015(hold);
          break;
        case 5: /* Opgave 5 vælges, og udføres. */
          n_kampklokken = KampKlokken(klokke_kampe, kamp);
          PrintKampe(klokke_kampe, n_kampklokken);
          break;
        case 6: /* Opgave 6 vælges, og udføres. */
          LavVinderTabel(hold);
          PrintVinderTabel(hold);
          break;
        case 0: /* Tast på '0' afslutter programmet. */
          printf("Tak for nu. Hav en god dag!\n");
          return 0;
        default: /* Standard svar, hvis ikke svaret opfattes gyldigt */
          printf("Jeg fangede desvaerre ikke hvad du ville.\n");
          break;
      }
      do {
        /**
         * Her vil programmet bede brugeren om at tage stilling til, om
         * der skal fortsættes. Brugeren kommer ikke videre med programmet
         * før der er indtastet et gyldigt svar.
         */
        printf("Vil du proeve igen? (y/n): ");
        scanf(" %c", &user_input_chr);
      } while((user_input_chr != 'Y') && (user_input_chr != 'y') &&
              (user_input_chr != 'N') && (user_input_chr != 'n'));
    } while((user_input_chr == 'y') || (user_input_chr == 'Y'));
    /**
     * En afsluttende hilsen til brugeren, inden programmet lukkes.
     * Samme som i linje 166.
     */
    printf("Tak for nu. Hav en god dag!\n");
  }
  /**
   * Hukommelse frigøres.
   */
  free(kamp);
  free(uafgjorte_kampe);
  free(klokke_kampe);
  return 0;
}

/**
 * Dette er funktionen som sørger for at selve den 'rå' data fra tekstfilen
 * bliver læst ind i selve programmet.
 * @param kamp - er selve pladserne, hvori hver alle kampe kan lagres til
 *               senere analysering. Dette skal være et array, når funktionen
 *               kaldes i main.
 */
void GetKampe(k_dat *kamp) {
  FILE *fp = fopen("superliga-2015-2016", "r");
  if (fp == NULL) {
    /* I fald selve filen ikke kan åbnes, skrives en fejlbesked. */
    printf("Error reading input-file\n");
  } else {
    int i = 0, scanres = 0;
    do {
      /**
       * Oplysnigerne fra filen scannes linje for linje. Det er her der
       * er størst chance for at tingene kan gå galt senere, da det er
       * her al information, som skal analyseres senere, opstår.
       */
      scanres = fscanf(fp,
             " R%d %s %d / %d / %d %d . %d %s - %s %d - %d %lf ",
             &kamp[i].runde,
              kamp[i].ugedag,
             &kamp[i].dato,
             &kamp[i].maaned,
             &kamp[i].aar,
             &kamp[i].timer,
             &kamp[i].minutter,
              kamp[i].hold_hjemme,
              kamp[i].hold_ude,
             &kamp[i].maal_hjemme,
             &kamp[i].maal_ude,
             &kamp[i].tilskuere);
      i++;
    }
    /**
     * Der bliver scannet linjer, så længe alle 12 pladser i scan-settet
     * får overført en værdi med success.
     */
    while (scanres == 12);
  }
  fclose(fp);
}

/**
 * Den første analysering af de oplysninger som indlæses af GetKampe.
 * Her tildeles de enklte hold, som er samlet i et array, et hold_navn,
 * deres point, mål, udesejre, osv. Der beyttes Enum-datatyper til at holde
 * styr på, hvilket hold der skal have hvilke data.
 * @param hold - et array af hold, som hver især skal have deres data
 *               fra kampene indtastet.
 * @param kamp - array'et af kampe, som er output fra funktionen GetKampe.
 */
void GetHoldStatistik(hold_statstik *hold, k_dat *kamp) {
  hold_navn hjemme, ude;
  int i;
  /* Først bliver hele array'et af hold nulstillet. */
  ResetHold(hold);
  /* Derefter får array'et af hold indtastet ny data. */
  for (i = 0; i < N_KAMPE; i++) {
    /* Ved hjælp af Enums, kan der, for hver kamp, bestemmes hjemme- og udehold. */
    hjemme = HoldTilEnum(kamp[i].hold_hjemme);
    ude = HoldTilEnum(kamp[i].hold_ude);
    /* Begge hold får talt deres totale antal kampe op. */
    hold[hjemme].n_antal_kampe++;
    hold[ude].n_antal_kampe++;
    /* Hjemmeholdet får indtastet antal mål de har scoret, og fået scoret mod sig. */
    hold[hjemme].maal_scoret += kamp[i].maal_hjemme;
    hold[hjemme].maal_ikke_reddet += kamp[i].maal_ude;
    /* Udeholdet får indtastet antal mål de har scoret, og fået scoret mod sig. */
    hold[ude].maal_scoret += kamp[i].maal_ude;
    hold[ude].maal_ikke_reddet += kamp[i].maal_hjemme;
    /* Herunder tildeles selve pointene for kampene. */
    if (kamp[i].maal_hjemme > kamp[i].maal_ude) {
      /* En sejr til hjemmeholdet giver 3 point til hjemme, og intet til ude. */
      hold[hjemme].point += 3;
      hold[hjemme].sejre_hjemme++;
    } else if (kamp[i].maal_hjemme < kamp[i].maal_ude) {
      /* En sejr til udeholdet giver 3 point til ude, og intet til hjemme. */
      hold[ude].point += 3;
      hold[ude].sejre_ude++;
    } else {
      /* Uafgjort givet begge hold 1 point. */
      hold[hjemme].point += 1;
      hold[hjemme].uafgjorte++;
      hold[ude].point += 1;
      hold[ude].uafgjorte++;
    }

    /*
     * Antallet af tilskuere i hhv. 2015 og -16 tælles op for
     * hjemmeholdet i denne kamp.
     */
    if (kamp[i].aar == 2015) {
      hold[hjemme].aku_tilskuere_hjemme_2015 += kamp[i].tilskuere;
    } else if (kamp[i].aar == 2016) {
      hold[hjemme].aku_tilskuere_hjemme_2016 += kamp[i].tilskuere;
    }
  }
}

/**
 * Denne funktion oversætter et char aray, altså en tekst-streng, til
 * den tilsvarende Enum datatype. Denne funktion kan se lettere koldset
 * ud, men den sørge for en mere overskuelig process i
 * funktionen GetHoldStatistik.
 * @param  navn - Her skal funktionen bruge en tekst-streng som input.
 * @return      - Funktionen returnerer en passende Enum værdi.
 */
hold_navn HoldTilEnum(char *navn) {
  if(strcmp(navn, "AGF") == 0) {
    return AGF;
  } else if(strcmp(navn, "FCM") == 0) {
    return FCM;
  } else if(strcmp(navn, "HOB") == 0) {
    return HOB;
  } else if(strcmp(navn, "RFC") == 0) {
    return RFC;
  } else if(strcmp(navn, "EFB") == 0) {
    return EFB;
  } else if(strcmp(navn, "SDR") == 0) {
    return SDR;
  } else if(strcmp(navn, "FCN") == 0) {
    return FCN;
  } else if(strcmp(navn, "BIF") == 0) {
    return BIF;
  } else if(strcmp(navn, "FCK") == 0) {
    return FCK;
  } else if(strcmp(navn, "OB")  == 0) {
    return OB;
  } else if(strcmp(navn, "AAB") == 0) {
    return AAB;
  } else if(strcmp(navn, "VFF") == 0) {
    return VFF;
  } else {
    /**
     * I fald der skulle opstå en fejl i oversættelsen fra tekst-streng til
     * Enum, vil der bliver returneret et Not a Name, og en fejlmeddelelse
     * printes i terminalen.
     */
    printf("Error getting name\n");
    return NaN;
  }
}

/**
 * Her vil array'et af hold bliver nulstillet.
 * @param hold  - et array af hold, som endnu ikke er klar til at få
 *                indtastet data i sig.
 */
void ResetHold(hold_statstik *hold) {
  int i;
  for (i = 0; i < N_HOLD+1; i++) {
    hold[i].navn = (hold_navn) i;
    EnumTilStr(hold[i].navn_str, (hold_navn) i);
    hold[i].point = 0;
    hold[i].sejre_hjemme = 0;
    hold[i].sejre_ude = 0;
    hold[i].uafgjorte = 0;
    hold[i].maal_scoret = 0;
    hold[i].maal_ikke_reddet = 0;
    hold[i].n_antal_kampe = 0;
    hold[i].aku_tilskuere_hjemme_2015 = 0.0;
    hold[i].aku_tilskuere_hjemme_2016 = 0.0;
  }
}

/**
 * Oversættelse fra Enum til tekst-streng, som udføres med en switch-case.
 * @param str  - funktionens output, som vil være en tekst-streng med et
 *               holnavn.
 * @param navn - funktionens input, som er en Enum.
 */
void EnumTilStr(char *str, hold_navn navn) {
  switch (navn) {
    case AGF: strcpy(str, "AGF"); break;
    case FCM: strcpy(str, "FCM"); break;
    case HOB: strcpy(str, "HOB"); break;
    case RFC: strcpy(str, "RFC"); break;
    case EFB: strcpy(str, "EFB"); break;
    case SDR: strcpy(str, "SDR"); break;
    case FCN: strcpy(str, "FCN"); break;
    case BIF: strcpy(str, "BIF"); break;
    case FCK: strcpy(str, "FCK"); break;
    case OB:  strcpy(str, "OB");  break;
    case AAB: strcpy(str, "AAB"); break;
    case VFF: strcpy(str, "VFF"); break;
    case NaN: strcpy(str, "NaN"); break;
  }
}

/**
 * Til opgave 1 skal der bruges en funktion som kan finde kampe der er endt
 * uafgjort, og hvor der er score fire mål eller flere.
 * @param  uafgjorte_kampe - funktionens primære output, i form af et array af kampe.
 * @param  kamp            - funktionens input, i form af alle kampe indlæst
 *                           fra input-filen.
 * @return                 - funktionens sekundære output, som er antallet af
 *                           kampe som opfylder de ting vi har ledt efter.
 */
int TestForUafgjort(k_dat *uafgjorte_kampe, k_dat *kamp) {
  int i, j = 0;
  for (i = 0; i < N_KAMPE; i++) {
    if ((kamp[i].maal_hjemme == kamp[i].maal_ude) &&
        ((kamp[i].maal_hjemme + kamp[i].maal_ude) >= 4)) {
      uafgjorte_kampe[j] = kamp[i];
      j++;
    }
  }
  return j;
}

/**
 * Et array af kampe bliver udprintet på terminalen i denne funktion.
 * Hvor kampene skal findes, og hvor mange der skal printes, er styret af
 * parametre. Der printes i samme stil/formatering, som der indlæses i starten
 * af programmet.
 * @param kamp - Hvor kampene skal findes.
 * @param N    - Hvor mange kampe der skal printes.
 */
void PrintKampe(k_dat *kamp, int N) {
  int i;
  if (N == 0) {
    printf("Der er ingen kampe at printe.\n");
  } else {
    for (i = 0; i < N; i++) {
      printf("R%-3d %-6s %.2d/%.2d/%-6d %.2d.%-6.2d %-3s - %-7s %d - %-5d %6.3f\n",
            kamp[i].runde,
            kamp[i].ugedag,
            kamp[i].dato,
            kamp[i].maaned,
            kamp[i].aar,
            kamp[i].timer,
            kamp[i].minutter,
            kamp[i].hold_hjemme,
            kamp[i].hold_ude,
            kamp[i].maal_hjemme,
            kamp[i].maal_ude,
            kamp[i].tilskuere);
    }
  }
}

/**
 * Til opgave 2, skal der undersøges om der findes runder hvori der er blevet
 * scoret færre end 10 mål alt i alt.
 * @param  output - første parameter er et 2-dimensionelt array. Første
 *                  dimension er 33 pladser lang, i tilfælde af at alle 33
 *                  runder opfylder det vi leder efter. Anden dimension
 *                  er 2 pladser lang, for at kunne indholde rundenummer
 *                  og antal scorede mål.
 * @param  kamp   - funktionens input, som er alle kampene som tidligere er
 *                  blevet indlæst.
 * @return        - antallet af runder, som funktionen har fundet opfylder
 *                  de som vi leder efter i opgaven.
 */
int MindreEnd10Maal(int output[33][2], k_dat *kamp) {
  int i, j, maal, k = 0;
  for (i = 0; i < N_RUNDER; i++) {
    maal = 0;
    for (j = 0; j < KAMPE_PR_N; j++) {
      maal += (kamp[i*KAMPE_PR_N + j].maal_hjemme + kamp[i*KAMPE_PR_N + j].maal_ude);
    }
    if (maal < 10) {
      output[k][0] = i+1;
      output[k][1] = maal;
      k++;
    }
  }
  return k;
}

/**
 * De fundne runder fra funktionen MindreEnd10Maal printes i terminalen.
 * @param input - Dette er det samlede 2-dimensionelle array, som blev
 *                behandlet i funktionen MindreEnd10Maal.
 * @param n     - Dette er antallet af runder som skal printes.
 */
void PrintMindreEnd10Maal(int input[33][2], int n) {
  int i;
  if (n == 0) {
    printf("Ingen runder opfylder disse krav.\n");
  } else {
    for (i = 0; i < n; i++) {
      printf("Runde %2d med %d maal.\n", input[i][0], input[i][1]);
    }
  }
}

/**
 * I opgave 3 skal det undersøges, hvilke hold der vinder mere på udebane
 * end på hjemmebane.
 * @param  udesejrende - funktionens primære output, som et array af hold
 *                       der vinder flere kampe på udebane end på hjemmebane.
 * @param  hold        - funktionens input, som et array med alle hold og deres
 *                       samlede statistikker.
 * @return             - funktionens sekundære output, som er antallet af
 *                       hold som opfylder det vi leder efter i opgaven.
 */
int GetUdeSejrende(hold_statstik *udesejrende, hold_statstik *hold) {
  int i, j = 0;
  for(i = 0; i <= N_HOLD; i++) {
    if (hold[i].sejre_ude > hold[i].sejre_hjemme) {
      udesejrende[j] = hold[i];
      j++;
    }
  }
  return j;
}

/**
 * Alle fundne hold fra funktionen GetUdeSejrende printes her.
 * @param input - et array med hold som kunne opfylde det vi leder efter
 *                i opgaven.
 * @param n     - antallet af hold som skal printes.
 */
void PrintUdeSejrende(hold_statstik *input, int n) {
  int i;
  if (n == 0) {
    printf("Der er ingen hold at printe.\n");
  } else {
    for (i = 0; i < n; i++) {
      printf("%s har %d ude-sejre og %d hjemme-sejre.\n",
      input[i].navn_str,
      input[i].sejre_ude,
      input[i].sejre_hjemme);
    }
  }
}

/**
 * I opgave 4 ønskes det at finde det hold med det laveste antal samlede
 * tilskuere for kampene i år 2015. Dette udskrives til brugeren med det
 * samme i denne funktion.
 * @param hold - Et array indholdende alle hold, og deres statistikker.
 */
void PrintLavesteTilskuere2015(hold_statstik *hold) {
  int i, j;
  double some_large_number = 100000.0;
  for (i = 0; i < N_HOLD; i++) {
    /* en hurtig omgang sammenligning, finder frem til det rette hold. */
    if (hold[i].aku_tilskuere_hjemme_2015 < some_large_number) {
      some_large_number = hold[i].aku_tilskuere_hjemme_2015;
      j = i;
    }
  }
  printf("%s havde kun %.3f tilskuere tilsammen i 2015\n",
        hold[j].navn_str,
        hold[j].aku_tilskuere_hjemme_2015);
}

/**
 * I opgave 5 ønskes det at finde nogle bestemte kampe, inden for en bestemt
 * tidsramme, som brugeren selv kan få lov til at indtaste.
 * @param  output - funktionens primære output, med et array af de kampe som
 *                  som opfylder det vi leder efter.
 * @param  kamp   - inputtet, med alle kampene som vi så kan undersøge.
 * @return        - funktionens sekundære output, som er antallet af
 *                  fundne kampe som opfylder det vi leder efter.
 */
int KampKlokken(k_dat *output, k_dat *kamp) {
  int i, j = 0;
  char dag[4], tidspunkt_1[6], tidspunkt_2[6], tidspunkt_3[6];

  /* Brugeren instrueres i hvad man kan indtaste, og der promptes for input. */
  printf("Tidsintervallet skal indtastet som eksempel: 13:15 - 14:15\n");
  printf("Indtast oensket tidsinterval: ");
  scanf(" %[0123456789:] %*c %[0123456789:]", tidspunkt_1, tidspunkt_2);
  /* Brugeren instrueres i hvad man kan indtaste, og der promptes for input, igen. */
  printf("Der kan skrives foelgende dage: Man, Tir, Ons, Tor, Fre, lor eller Son.\n");
  printf("Ugedag skal indtastes som eksempel: Son\n");
  printf("Indtast oensket ugedag: ");
  scanf(" %s", dag);

  for (i = 0; i < N_KAMPE; i++) {
    /**
     * For hver kamp er findes, laves den tidspunkts-data om til en tekst-streng
     * som senere kan sammenlignes med en strcmp.
     */
    sprintf(tidspunkt_3, "%.2d:%.2d", kamp[i].timer, kamp[i].minutter);
    if (strcmp(kamp[i].ugedag, dag) == 0) {
      if (strcmp(tidspunkt_1, tidspunkt_3) <=0 &&
          strcmp(tidspunkt_2, tidspunkt_3) >=0) {
        output[j] = kamp[i];
        j++;
      }
    }
  }
  return j;
}

/**
 * Dette er næsten en kopi af funktionen KampKlokken. I denne funktion
 * vil brugeren dog ikke have indflydelse på det tidsinterval som skal
 * undersøges.
 * @param  output - funktionens primære output, med et array af de kampe som
 *                  som opfylder det vi leder efter.
 * @param  kamp   - inputtet, med alle kampene som vi så kan undersøge.
 * @return        - funktionens sekundære output, som er antallet af
 *                  fundne kampe som opfylder det vi leder efter.
 */
int KampKlokken_std(k_dat *output, k_dat *kamp) {
  int i, j = 0;
  char dag[4], tidspunkt_1[6], tidspunkt_2[6], tidspunkt_3[6];
  strcpy(dag, "Son");
  strcpy(tidspunkt_1, "13:15");
  strcpy(tidspunkt_2, "14:15");

  for (i = 0; i < N_KAMPE; i++) {
    sprintf(tidspunkt_3, "%.2d:%.2d", kamp[i].timer, kamp[i].minutter);
    if (strcmp(kamp[i].ugedag, dag) == 0) {
      if (strcmp(tidspunkt_1, tidspunkt_3) <=0 &&
          strcmp(tidspunkt_2, tidspunkt_3) >=0) {
        output[j] = kamp[i];
        j++;
      }
    }
  }
  return j;
}

/**
 * Denne funktion gør ikke meget, ud over at kalde en qsort. Funktionens
 * primære formål er at få main til at give mere mening.
 * @param hold - Et array med alle hold og deres data, som kan sorteres.
 */
void LavVinderTabel(hold_statstik *hold) {
  qsort(hold, N_HOLD+1, sizeof(hold_statstik), *comp_hold);
}

/**
 * Selve sammenlignings-funktionen, som qsort skal bruge til at kunne sortere
 * alle hold efter de givne spilleregler.
 */
int comp_hold(const void *a, const void *b) {
  hold_statstik *holdA = (hold_statstik *)a;
  hold_statstik *holdB = (hold_statstik *)b;
  int comp;

  /* Først sammenlignes holdenes point. */
  comp = holdA->point - holdB->point;
  if (comp < 0) {
    return 1;
  } else if (comp > 0) {
    return -1;
  } else {
    /* Hvis pointene er lige, kigges der på målforskel. */
    comp = (holdA->maal_scoret - holdA->maal_ikke_reddet) - (holdB->maal_scoret - holdB->maal_ikke_reddet);
    if (comp < 0) {
      return 1;
    } else if (comp > 0) {
      return -1;
    } else {
      /* Hvis målforskellen er ens, kigges der på antal scorede mål. */
      comp = holdA->maal_scoret - holdB->maal_scoret;
      if (comp < 0) {
        return 1;
      } else if (comp > 0) {
        return -1;
      } else {
        /* Hvis alt ovenstående står lige, sorteres der alfabetisk. */
        return strcmp(holdA->navn_str, holdB->navn_str);
      }
    }
  }
}

/**
 * I opgave 6 ønskes det er få udprintet en tabel med alle hold og deres
 * indbyrdes position i Superligaen. Der er taget indspiration efter en
 * tabel fra en wiki-artikel, som blev linket til i opgavebeskrivelsen.
 * @param hold - Et array med alle hold, og deres statistikker.
 *               Array'et skal være sorteret, for at tabellen bliver printet
 *               korrekt, med holdene i rigtig rækkefølge.
 */
void PrintVinderTabel(hold_statstik *hold) {
  int i;
  /* Der printes først et "sidehoved", som skal forklare de enkelte tal i tabellen. */
  printf("______________________________________________________________________________\n"
         "|| Pos |||  Hold |||  K |||  V |||  U |||  T |||  M+ |||  M- |||  MF |||  P ||\n"
         "|/-----\\|/-------\\|/----\\|/----\\|/----\\|/----\\|/-----\\|/-----\\|/-----\\|/----\\|\n");
  /* Holdene, og deres data, printes i terminalen. */
  for (i = 0; i < N_HOLD; i++) {
    printf("|   %2d  |   %3s   |  %2d  |  %2d  |  %2d  |  %2d  |   %2d  |   %2d  |  %+3d  |  %2d  |\n",
          i+1,
          hold[i].navn_str,
          hold[i].n_antal_kampe,
          hold[i].sejre_ude + hold[i].sejre_hjemme,
          hold[i].uafgjorte,
          hold[i].n_antal_kampe - (hold[i].sejre_ude + hold[i].sejre_hjemme + hold[i].uafgjorte),
          hold[i].maal_scoret,
          hold[i].maal_ikke_reddet,
          hold[i].maal_scoret - hold[i].maal_ikke_reddet,
          hold[i].point);
    /* Hvis ikke sidste hold er printet, så printes en linje til at adskille holdene. */
    if(i < N_HOLD-1)
      printf("|=======|=========|======|======|======|======|=======|=======|=======|======|\n");
  }
  /* Til slut printes en "sidefod". */
  printf("|\\~~~~~/|\\~~~~~~~/|\\~~~~/|\\~~~~/|\\~~~~/|\\~~~~/|\\~~~~~/|\\~~~~~/|\\~~~~~/|\\~~~~/|\n");
}

/**
 * Denne funktion printer instruktionerne til brugeren, hvis denne vælger ikke
 * at skrive "--print". Dette er altså den interaktive menu som brugeren mødes
 * af, og som skal bruges til at kunne navigere rundt i, hvilke opgaver brugeren
 * ønsker vist i terminalen.
 */
void PrintInstruktioner(void) {
  printf("\n");
  printf("Disse opgaver kan blive loest i dette program:\n\n"
         " 1\t- I denne opgave vil programmet finde de alle kampe som ender uafgjort, hvor der\n"
         "\t  er scoret fire maal eller flere.\n\n"
         " 2\t- I denne opgave vil programmet finde alle de runder hvor der er scoret\n"
         "\t  mindre end 10 maal.\n\n"
         " 3\t- I denne opgave vil programmet finde alle de hold som vinder flere\n"
         "\t  kampe paa udebane end paa hjemmebane\n\n"
         " 4\t- I denne opgave vil programmet finde de kampe som ender uafgjort, hvor der\n"
         "\t  antal samlede tilskuere paa hjemmebane i 2015.\n");
  /* En ny printf var nødvendig, for at overholde ISO C90 standard på maks 509 tegn. */
  printf("\n 5\t- I denne opgave vil programmet finde alle de kampe som spilles en\n"
         "\t  brugerbestemt ugedag, inden for et brugerbestemt tidsinterval.\n\n"
         " 6\t- I denne opgave vil programmet finde alle deltagende holds placering i\n"
         "\t  superligaen 2015/-16, stillet op i en meget flot tabel.\n\n"
         " 0\t- Hvis du taster 0, stoppes programmet.\n\n");
}
