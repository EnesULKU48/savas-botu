#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //acces , getcw icin
#include <curl/curl.h>
#include <SDL2/SDL.h>

struct SavasanOzellikler
{
    float hasar_orani;
    int birimSayi;
    float saldiri;
    float savunma;
    float saglik;
    float kritik_sans;
    float hasar_dagilimi;
};

struct KahramanOzellikler
{
    char senaryoKahrAd[4][25];
    char bonus_turu[15];
    char etkiledigi_birim[20];
    float bonus_degeri;
};

struct CanavarOzellikler
{
    char senaryoCanvAd[4][25];
    float etki_degeri;
    char etki_turu[15];
    char etkiledigi_birim[20];
};

struct ArastirmaOzellikler
{
    int senaryoSeviyesi;
    float deger;
    char etki_turu[15];;
    char etkiledigi_birim[20];
};

struct BirimlerInsan
{
    struct SavasanOzellikler piyadeler;
    struct SavasanOzellikler okcular;
    struct SavasanOzellikler suvariler;
    struct SavasanOzellikler kusatma_makineleri;
};

struct BirimlerOrg
{
    struct SavasanOzellikler ork_dovusculeri;
    struct SavasanOzellikler mizrakcilar;
    struct SavasanOzellikler varg_binicileri;
    struct SavasanOzellikler troller;
};

struct KahramanlarInsan
{
    struct KahramanOzellikler Alparslan;
    struct KahramanOzellikler Fatih_Sultan_Mehmet;
    struct KahramanOzellikler Yavuz_Sultan_Selim;
    struct KahramanOzellikler Tugrul_Bey;
    struct KahramanOzellikler Mete_Han;
};

struct KahramanlarOrg
{
    struct KahramanOzellikler Goruk_Vahsi;
    struct KahramanOzellikler Thruk_Kemikkiran;
    struct KahramanOzellikler Vrog_Kafakiran;
    struct KahramanOzellikler Ugar_Zalim;
};

struct CanavarlarInsan
{
    struct CanavarOzellikler Ejderha;
    struct CanavarOzellikler Agri_Dagi_Devleri;
    struct CanavarOzellikler Tepegoz;
    struct CanavarOzellikler Karakurt;
    struct CanavarOzellikler Samur;
};

struct CanavarlarOrg
{
    struct CanavarOzellikler Kara_Troll;
    struct CanavarOzellikler Golge_Kurtlari;
    struct CanavarOzellikler Camur_Devleri;
    struct CanavarOzellikler Ates_Iblisi;
    struct CanavarOzellikler Buz_Devleri;
    struct CanavarOzellikler Makrog_Savas_Beyi;
};

struct ArastirmaSeviyesi
{
    struct ArastirmaOzellikler savunma_ustaligi;
    struct ArastirmaOzellikler saldiri_gelistirmesi;
    struct ArastirmaOzellikler elit_egitim;
    struct ArastirmaOzellikler kusatma_ustaligi;
};

struct Arastirma
{
    struct ArastirmaSeviyesi seviye_1;
    struct ArastirmaSeviyesi seviye_2;
    struct ArastirmaSeviyesi seviye_3;
};

struct InsanImparatorlugu
{
    struct BirimlerInsan birimler;
    struct KahramanlarInsan kahramanlar;
    struct CanavarlarInsan canavarlar;
    struct Arastirma arastirma_seviyesi;
};

struct OrkLegi
{
    struct BirimlerOrg birimler;
    struct KahramanlarOrg kahramanlar;
    struct CanavarlarOrg canavarlar;
    struct Arastirma arastirma_seviyesi;
};

struct Savasanlar
{
    struct InsanImparatorlugu insanImparatorlugu;
    struct OrkLegi orkLegi;
};

void canavarEtkiledigiBirimAta(struct CanavarOzellikler *canavar, char *okunan_satir);
void canavarAtama(struct Savasanlar *s, char *okunan_satir, char *birimAd, int i);
void KahramanEtkiledigiBirimAta(struct KahramanOzellikler *kahraman, char *okunan_satir);
void kahramanAtama(struct Savasanlar *s, char *okunan_satir, char *birimAdi, int i);
void birimAtama(struct Savasanlar *s, char *okunan_satir, char *birimAdi, int i);
void dosyaOku(struct Savasanlar *s, char *ordu, char *birimAdi);
void birimAyristir(struct Savasanlar *s, char *dosyaAdi, char *birimAd);
void kahramanAyristir(struct Savasanlar *s, char *dosyaAdi, char *birimAd);
void canavarAyristir(struct Savasanlar *s, char *dosyaAdi, char *birimAd);
void arastirmaAtama(struct Savasanlar *s, char *okunan_satir, char *birimAd, char *grup, int seviye, int i);
void arastirmaEtkiledigiBirimAta(struct ArastirmaOzellikler *arastir, char *okunan_satir);
void arastirmaAyristir(struct Savasanlar *s, char *birimAd, char *grup, int seviye);
void senaryoAyristir(struct Savasanlar *s);

void savas(struct Savasanlar *s);
float insanSaldiriHesapla(struct BirimlerInsan *birim, int saldiri_sayisi, FILE *savas_sim);
float insanSavunmaHesapla(struct BirimlerInsan *birim);
float orkSaldiriHesapla(struct BirimlerOrg *birim, int saldiri_sayisi, FILE *savas_sim);
float orkSavunmaHesapla(struct BirimlerOrg *birim);
void birim_guncelle_ork(struct BirimlerOrg *birim, float netHasar, float toplamSavunmaGucu, FILE *savas_sim);
void hasar_dagilimi_ork(struct BirimlerOrg *birim, float netHasar, float toplamSavunmaGucu);
void birim_kayip_ork(struct BirimlerOrg *birim, FILE *savas_sim);
void olen_hasardagit_ork(struct BirimlerOrg *birim, float birimHasarDagilimi, float kalanSavunmaGucu);
void birim_guncelle_insan(struct BirimlerInsan *birim, float netHasar, float toplamSavunmaGucu, FILE *savas_sim);
void hasar_dagilimi_insan(struct BirimlerInsan *birim, float netHasar, float toplamSavunmaGucu);
void olen_hasardagit_insan(struct BirimlerInsan *birim, float birimHasarDagilimi, float kalanSavunmaGucu);
void birim_kayip_insan(struct BirimlerInsan *birim, FILE *savas_sim);
void saglik_durumu_ork(struct BirimlerOrg *birim, FILE *savas_sim);
void saglik_durumu_insan(struct BirimlerInsan *birim, FILE *savas_sim);
void insan_kahraman_etkisi(struct KahramanOzellikler *kahraman, struct BirimlerInsan *birimlerInsan, int kahraman_bil, FILE *savas_sim);
void insan_kahraman_canavar_etkisi(struct KahramanlarInsan *kahraman, struct CanavarlarInsan *canavar, struct BirimlerInsan *birimler, FILE *savas_sim);
void canavar_kahraman_bonusu_uygula(char *bonus_turu, float bonus_degeri, float *savunma, float *saldiri, float *kritik_sans);
void insan_canavar_etkisi(struct CanavarOzellikler *canavar, struct BirimlerInsan *birimlerInsan, int canavar_bil, FILE *savas_sim);
void ork_kahraman_canavar_etkisi(struct KahramanlarOrg *kahraman, struct CanavarlarOrg *canavar, struct BirimlerOrg *birimler, FILE *savas_sim);
void ork_kahraman_etkisi(struct KahramanOzellikler *kahraman, struct BirimlerOrg *birimlerOrg, int kahraman_bil, FILE *savas_sim);
void ork_canavar_etkisi(struct CanavarOzellikler *canavar, struct BirimlerOrg *birimlerOrg, int canavar_bil, FILE *savas_sim);
void arastirma_etkisi(struct InsanImparatorlugu *insan_imp, struct OrkLegi *ork_imp, FILE *savas_sim);
void insan_arastirma_etki_uygula(struct ArastirmaOzellikler *arastirma, struct InsanImparatorlugu *insan_imp, float etki, FILE *savas_sim);
void ork_arastirma_etki_uygula(struct ArastirmaOzellikler *arastirma, struct OrkLegi *ork_imp, float etki, FILE *savas_sim);
void yorgunluk_uygula(struct BirimlerInsan *birimInsan, struct BirimlerOrg *birimOrk);
float kritik_hasar_hesapla(float normal_saldiri, float kritik_sans, int saldiri_sayisi, int birim_bil, FILE *savas_sim);
int ork_ordu_oldu_mu(struct BirimlerOrg *birim);
int insan_ordu_oldu_mu(struct BirimlerInsan *birim);


int main(int argc, char *argv[])
{
    struct Savasanlar *s = (struct Savasanlar *)malloc(sizeof(struct Savasanlar));
    memset(s, 0, sizeof(struct Savasanlar));

    if (!senaryoIndirme())
        return 1;

    senaryoAyristir(s);

    savas(s);

    free(s);
    return 0;
}

void savas(struct Savasanlar *s)
{
    struct BirimlerInsan *birimlerInsan = &s->insanImparatorlugu.birimler;
    struct BirimlerOrg *birimlerOrk = &s->orkLegi.birimler;

    struct KahramanlarInsan *kahramanInsan = &s->insanImparatorlugu.kahramanlar;
    struct KahramanlarOrg *kahramanOrg = &s->orkLegi.kahramanlar;

    struct CanavarlarInsan *canavarInsan = &s->insanImparatorlugu.canavarlar;
    struct CanavarlarOrg *canavarOrg = &s->orkLegi.canavarlar;

    int adim_say = 1;
    int insan_saldiri_say = 0;
    int ork_saldiri_say = 0;


    FILE *savas_sim;
    savas_sim = fopen("Files\\savas_sim.txt", "a");
    if (savas_sim == NULL)
    {
        printf("\nsavas_sim dosyasi acilamadi");
        return;
    }

    while (1)
    {
        fprintf(savas_sim,"\n\n\n----------adim sayisi : %d----------\n\n",adim_say);
        if (adim_say % 5 == 0)
        {
            yorgunluk_uygula(birimlerInsan, birimlerOrk);
            fprintf(savas_sim,"\n%d. adimda yorgunluk uygulandi\n\n",adim_say);
        }

        fprintf(savas_sim,"\n\n***InsanImp icin Kaharaman ve Canavar etkileri***\n");
        insan_kahraman_canavar_etkisi(kahramanInsan, canavarInsan, birimlerInsan, savas_sim);
        fprintf(savas_sim,"\n\n\n***OrkImp icin Kaharaman ve Canavar etkileri***\n");
        ork_kahraman_canavar_etkisi(kahramanOrg, canavarOrg, birimlerOrk, savas_sim);
        fprintf(savas_sim,"\n\n\n***Arastirma etkileri***\n");
        arastirma_etkisi(&s->insanImparatorlugu, &s->orkLegi, savas_sim);

        if (adim_say % 2 == 1)
        {
            insan_saldiri_say++;
            fprintf(savas_sim,"\n\n\n####Insan Imparatorlugu saldiriyor.####\n\n");

            float insan_saldiri_gucu = insanSaldiriHesapla(birimlerInsan, insan_saldiri_say, savas_sim);
            float ork_savunma_gucu = orkSavunmaHesapla(birimlerOrk);

            fprintf(savas_sim,"\n\nInsan saldiri gucu : %f\n", insan_saldiri_gucu);
            fprintf(savas_sim,"\nOrk savunma gucu : %f\n", ork_savunma_gucu);


            float orkimp_nethasar = insan_saldiri_gucu - ork_savunma_gucu;
            fprintf(savas_sim,"\nInsan Imparatorlugunun net hasari : %f\n", orkimp_nethasar);

            if(orkimp_nethasar > 0)
            {
                fprintf(savas_sim,"\nInsan Imparatorlugunun saldirisinda net hasar pozitif. Ork Legine hasar verdi.\n\n");
                birim_guncelle_ork(birimlerOrk, orkimp_nethasar, ork_savunma_gucu, savas_sim);
                if(ork_ordu_oldu_mu(birimlerOrk))
                {
                    fprintf(savas_sim,"\n\n\nInsan Imparatorlugu kazandi!!!\n\n");
                    break;
                }
            }

        }
        else
        {
            ork_saldiri_say++;
            fprintf(savas_sim,"\n\n\n####Ork Legi saldiriyor.####\n\n");

            float ork_saldiri_gucu = orkSaldiriHesapla(birimlerOrk, ork_saldiri_say, savas_sim);
            float insan_savunma_gucu = insanSavunmaHesapla(birimlerInsan);

            fprintf(savas_sim,"\n\nOrk saldiri gucu : %f\n", ork_saldiri_gucu);
            fprintf(savas_sim,"\nInsan savunma gucu : %f\n", insan_savunma_gucu);


            float insanimp_nethasar = ork_saldiri_gucu - insan_savunma_gucu;
            fprintf(savas_sim,"\nOrk Leginin net hasari : %f\n", insanimp_nethasar);

            if(insanimp_nethasar > 0)
            {
                fprintf(savas_sim,"\nOrk Leginin saldirisinda net hasar pozitif. Insan Imparatorluguna hasar verdi.\n\n");
                birim_guncelle_insan(birimlerInsan, insanimp_nethasar, insan_savunma_gucu,savas_sim);
                if(insan_ordu_oldu_mu(birimlerInsan))
                {
                    fprintf(savas_sim,"\n\n\nOrk Legi kazandi!!!\n\n");
                    break;
                }
            }

        }
        adim_say++;
    }
    fclose(savas_sim);
}

int ork_ordu_oldu_mu(struct BirimlerOrg *birim)
{
    int kontrol = 0;
    if(birim->mizrakcilar.birimSayi == 0 && birim->ork_dovusculeri.birimSayi == 0 && birim->troller.birimSayi == 0 && birim->varg_binicileri.birimSayi == 0)
        kontrol = 1;
    return kontrol;
}

int insan_ordu_oldu_mu(struct BirimlerInsan *birim)
{
    int kontrol = 0;
    if(birim->kusatma_makineleri.birimSayi == 0 && birim->okcular.birimSayi == 0 && birim->piyadeler.birimSayi == 0 && birim->suvariler.birimSayi == 0)
        kontrol = 1;
    return kontrol;
}

void insan_arastirma_etki_uygula(struct ArastirmaOzellikler *arastirma, struct InsanImparatorlugu *insan_imp, float etki, FILE *savas_sim)
{
    if(!strcmp(arastirma->etkiledigi_birim, "tum_birimler"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin eski savunma gucleri = %f - %f - %f - %f dir. ",insan_imp->birimler.suvariler.savunma,insan_imp->birimler.piyadeler.savunma,insan_imp->birimler.okcular.savunma,insan_imp->birimler.kusatma_makineleri.savunma);
            if(insan_imp->birimler.kusatma_makineleri.birimSayi > 0)
                insan_imp->birimler.kusatma_makineleri.savunma *= etki;
            if(insan_imp->birimler.okcular.birimSayi > 0)
                insan_imp->birimler.okcular.savunma *= etki;
            if(insan_imp->birimler.piyadeler.birimSayi > 0)
                insan_imp->birimler.piyadeler.savunma *= etki;
            if(insan_imp->birimler.suvariler.birimSayi > 0)
                insan_imp->birimler.suvariler.savunma *= etki;
            fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin yeni savunma gucleri = %f - %f - %f - %f dir. ",insan_imp->birimler.suvariler.savunma,insan_imp->birimler.piyadeler.savunma,insan_imp->birimler.okcular.savunma,insan_imp->birimler.kusatma_makineleri.savunma);
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin eski saldiri gucleri = %f - %f - %f - %f dir. ",insan_imp->birimler.suvariler.saldiri,insan_imp->birimler.piyadeler.saldiri,insan_imp->birimler.okcular.saldiri,insan_imp->birimler.kusatma_makineleri.saldiri);
            if(insan_imp->birimler.kusatma_makineleri.birimSayi > 0)
                insan_imp->birimler.kusatma_makineleri.saldiri *= etki;
            if(insan_imp->birimler.okcular.birimSayi > 0)
                insan_imp->birimler.okcular.saldiri *= etki;
            if(insan_imp->birimler.piyadeler.birimSayi > 0)
                insan_imp->birimler.piyadeler.saldiri *= etki;
            if(insan_imp->birimler.suvariler.birimSayi > 0)
                insan_imp->birimler.suvariler.saldiri *= etki;
            fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin yeni saldiri gucleri = %f - %f - %f - %f dir. ",insan_imp->birimler.suvariler.saldiri,insan_imp->birimler.piyadeler.saldiri,insan_imp->birimler.okcular.saldiri,insan_imp->birimler.kusatma_makineleri.saldiri);
        }
        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin eski kritik sans gucleri = %f - %f - %f - %f dir. ",insan_imp->birimler.suvariler.kritik_sans,insan_imp->birimler.piyadeler.kritik_sans,insan_imp->birimler.okcular.kritik_sans,insan_imp->birimler.kusatma_makineleri.kritik_sans);
            if(insan_imp->birimler.kusatma_makineleri.birimSayi > 0)
                insan_imp->birimler.kusatma_makineleri.kritik_sans *= etki;
            if(insan_imp->birimler.okcular.birimSayi > 0)
                insan_imp->birimler.okcular.kritik_sans *= etki;
            if(insan_imp->birimler.piyadeler.birimSayi > 0)
                insan_imp->birimler.piyadeler.kritik_sans *= etki;
            if(insan_imp->birimler.suvariler.birimSayi > 0)
                insan_imp->birimler.suvariler.kritik_sans *= etki;
            fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin yeni kritik sans gucleri = %f - %f - %f - %f dir. ",insan_imp->birimler.suvariler.kritik_sans,insan_imp->birimler.piyadeler.kritik_sans,insan_imp->birimler.okcular.kritik_sans,insan_imp->birimler.kusatma_makineleri.kritik_sans);
        }
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "kusatma_makineleri") && insan_imp->birimler.kusatma_makineleri.birimSayi > 0)
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            fprintf(savas_sim,"\nKusatma Makinelerinin eski savunma gucu : %f dir. ",insan_imp->birimler.kusatma_makineleri.savunma);
            insan_imp->birimler.kusatma_makineleri.savunma *= etki;
            fprintf(savas_sim,"Yeni savunma gucu : %f dir. ",insan_imp->birimler.kusatma_makineleri.savunma);
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            fprintf(savas_sim,"\nKusatma Makinelerinin eski saldiri gucu : %f dir. ",insan_imp->birimler.kusatma_makineleri.saldiri);
            insan_imp->birimler.kusatma_makineleri.saldiri *= etki;
            fprintf(savas_sim,"Yeni saldiri gucu : %f dir. ",insan_imp->birimler.kusatma_makineleri.saldiri);
        }

        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            fprintf(savas_sim,"\nKusatma Makinelerinin eski kritik sans gucu : %f dir. ",insan_imp->birimler.kusatma_makineleri.kritik_sans);
            insan_imp->birimler.kusatma_makineleri.kritik_sans *= etki;
            fprintf(savas_sim,"Yeni kritik sans gucu : %f dir. ",insan_imp->birimler.kusatma_makineleri.kritik_sans);
        }
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "okcu") && insan_imp->birimler.okcular.birimSayi > 0)
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            fprintf(savas_sim,"\nOkcularin eski savunma gucu : %f dir. ",insan_imp->birimler.okcular.savunma);
            insan_imp->birimler.okcular.savunma *= etki;
            fprintf(savas_sim,"Yeni savunma gucu : %f dir. ",insan_imp->birimler.okcular.savunma);
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            fprintf(savas_sim,"\nOkcularin eski saldiri gucu : %f dir. ",insan_imp->birimler.okcular.saldiri);
            insan_imp->birimler.okcular.saldiri *= etki;
            fprintf(savas_sim,"Yeni saldiri gucu : %f dir. ",insan_imp->birimler.okcular.saldiri);
        }
        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            fprintf(savas_sim,"\nOkcularin eski kritik sans gucu : %f dir. ",insan_imp->birimler.okcular.kritik_sans);
            insan_imp->birimler.okcular.kritik_sans *= etki;
            fprintf(savas_sim,"Yeni kritik sans gucu : %f dir. ",insan_imp->birimler.okcular.kritik_sans);
        }
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "piyade") && insan_imp->birimler.piyadeler.birimSayi > 0)
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            fprintf(savas_sim,"\nPiyadelerin eski savunma gucu : %f dir. ",insan_imp->birimler.piyadeler.savunma);
            insan_imp->birimler.piyadeler.savunma *= etki;
            fprintf(savas_sim,"Yeni savunma gucu : %f dir. ",insan_imp->birimler.piyadeler.savunma);
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            fprintf(savas_sim,"\nPiyadelerin eski saldiri gucu : %f dir. ",insan_imp->birimler.piyadeler.saldiri);
            insan_imp->birimler.piyadeler.saldiri *= etki;
            fprintf(savas_sim,"Yeni saldiri gucu : %f dir. ",insan_imp->birimler.piyadeler.saldiri);
        }
        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            fprintf(savas_sim,"\nPiyadelerin eski kritik sans gucu : %f dir. ",insan_imp->birimler.piyadeler.kritik_sans);
            insan_imp->birimler.piyadeler.kritik_sans *= etki;
            fprintf(savas_sim,"Yeni kritik sans gucu : %f dir. ",insan_imp->birimler.piyadeler.kritik_sans);
        }
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "suvari") && insan_imp->birimler.suvariler.birimSayi > 0)
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            fprintf(savas_sim,"\nSuvarilerin eski savunma gucu : %f dir. ",insan_imp->birimler.suvariler.savunma);
            insan_imp->birimler.suvariler.savunma *= etki;
            fprintf(savas_sim,"Yeni savunma gucu : %f dir. ",insan_imp->birimler.suvariler.savunma);
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            fprintf(savas_sim,"\nSuvarilerin eski saldiri gucu : %f dir. ",insan_imp->birimler.suvariler.saldiri);
            insan_imp->birimler.suvariler.saldiri *= etki;
            fprintf(savas_sim,"Yeni saldiri gucu : %f dir. ",insan_imp->birimler.suvariler.saldiri);
        }
        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            fprintf(savas_sim,"\nSuvarilerin eski kritik sans gucu : %f dir. ",insan_imp->birimler.suvariler.kritik_sans);
            insan_imp->birimler.suvariler.kritik_sans *= etki;
            fprintf(savas_sim,"Yeni kritik sans gucu : %f dir. ",insan_imp->birimler.suvariler.kritik_sans);
        }
    }
}

void ork_arastirma_etki_uygula(struct ArastirmaOzellikler *arastirma, struct OrkLegi *ork_imp, float etki, FILE *savas_sim)
{
    if(!strcmp(arastirma->etkiledigi_birim, "tum_birimler"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            fprintf(savas_sim,"\nOrk Dovusculeri - Troller - Varg Binicileri - Mizrakci birimlerinin eski savunma gucleri = %f - %f - %f - %f dir. ",ork_imp->birimler.ork_dovusculeri.savunma,ork_imp->birimler.troller.savunma,ork_imp->birimler.varg_binicileri.savunma,ork_imp->birimler.mizrakcilar.savunma);
            if(ork_imp->birimler.ork_dovusculeri.birimSayi > 0)
                ork_imp->birimler.ork_dovusculeri.savunma *= etki;
            if(ork_imp->birimler.troller.birimSayi > 0)
                ork_imp->birimler.troller.savunma *= etki;
            if(ork_imp->birimler.varg_binicileri.birimSayi > 0)
                ork_imp->birimler.varg_binicileri.savunma *= etki;
            if(ork_imp->birimler.mizrakcilar.birimSayi > 0)
                ork_imp->birimler.mizrakcilar.savunma *= etki;
            fprintf(savas_sim,"\nOrk Dovusculeri - Troller - Varg Binicileri - Mizrakci birimlerinin yeni savunma gucleri = %f - %f - %f - %f dir. ",ork_imp->birimler.ork_dovusculeri.savunma,ork_imp->birimler.troller.savunma,ork_imp->birimler.varg_binicileri.savunma,ork_imp->birimler.mizrakcilar.savunma);
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            fprintf(savas_sim,"\nOrk Dovusculeri - Troller - Varg Binicileri - Mizrakci birimlerinin eski saldiri gucleri = %f - %f - %f - %f dir. ",ork_imp->birimler.ork_dovusculeri.saldiri,ork_imp->birimler.troller.saldiri,ork_imp->birimler.varg_binicileri.saldiri,ork_imp->birimler.mizrakcilar.saldiri);
            if(ork_imp->birimler.ork_dovusculeri.birimSayi > 0)
                ork_imp->birimler.ork_dovusculeri.saldiri *= etki;
            if(ork_imp->birimler.troller.birimSayi > 0)
                ork_imp->birimler.troller.saldiri *= etki;
            if(ork_imp->birimler.varg_binicileri.birimSayi > 0)
                ork_imp->birimler.varg_binicileri.saldiri *= etki;
            if(ork_imp->birimler.mizrakcilar.birimSayi > 0)
                ork_imp->birimler.mizrakcilar.saldiri *= etki;
            fprintf(savas_sim,"\nOrk Dovusculeri - Troller - Varg Binicileri - Mizrakci birimlerinin yeni saldiri gucleri = %f - %f - %f - %f dir. ",ork_imp->birimler.ork_dovusculeri.saldiri,ork_imp->birimler.troller.saldiri,ork_imp->birimler.varg_binicileri.saldiri,ork_imp->birimler.mizrakcilar.saldiri);
        }
        else if(!strcmp(arastirma->etki_turu, "kritik_sans"))
        {
            fprintf(savas_sim,"\nOrk Dovusculeri - Troller - Varg Binicileri - Mizrakci birimlerinin eski kritik sans gucleri = %f - %f - %f - %f dir. ",ork_imp->birimler.ork_dovusculeri.kritik_sans,ork_imp->birimler.troller.kritik_sans,ork_imp->birimler.varg_binicileri.kritik_sans,ork_imp->birimler.mizrakcilar.kritik_sans);
            if(ork_imp->birimler.ork_dovusculeri.birimSayi > 0)
                ork_imp->birimler.ork_dovusculeri.kritik_sans *= etki;
            if(ork_imp->birimler.troller.birimSayi > 0)
                ork_imp->birimler.troller.kritik_sans *= etki;
            if(ork_imp->birimler.varg_binicileri.birimSayi > 0)
                ork_imp->birimler.varg_binicileri.kritik_sans *= etki;
            if(ork_imp->birimler.mizrakcilar.birimSayi > 0)
                ork_imp->birimler.mizrakcilar.kritik_sans *= etki;
            fprintf(savas_sim,"\nOrk Dovusculeri - Troller - Varg Binicileri - Mizrakci birimlerinin yeni kritik sans gucleri = %f - %f - %f - %f dir. ",ork_imp->birimler.ork_dovusculeri.kritik_sans,ork_imp->birimler.troller.kritik_sans,ork_imp->birimler.varg_binicileri.kritik_sans,ork_imp->birimler.mizrakcilar.kritik_sans);
        }
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "ork_dovusculeri") && ork_imp->birimler.ork_dovusculeri.birimSayi > 0)
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            fprintf(savas_sim,"\nOrk Dovusculerinin eski savunma gucu : %f dir. ",ork_imp->birimler.ork_dovusculeri.savunma);
            ork_imp->birimler.ork_dovusculeri.savunma *= etki;
            fprintf(savas_sim,"Yeni savunma gucu : %f dir. ",ork_imp->birimler.ork_dovusculeri.savunma);
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            fprintf(savas_sim,"\nOrk Dovusculerinin eski saldiri gucu : %f dir. ",ork_imp->birimler.ork_dovusculeri.saldiri);
            ork_imp->birimler.ork_dovusculeri.saldiri *= etki;
            fprintf(savas_sim,"Yeni saldiri gucu : %f dir. ",ork_imp->birimler.ork_dovusculeri.saldiri);
        }
        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            fprintf(savas_sim,"\nOrk Dovusculerinin eski kritik sans gucu : %f dir. ",ork_imp->birimler.ork_dovusculeri.kritik_sans);
            ork_imp->birimler.ork_dovusculeri.kritik_sans *= etki;
            fprintf(savas_sim,"Yeni kritik sans gucu : %f dir. ",ork_imp->birimler.ork_dovusculeri.kritik_sans);
        }
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "troller") && ork_imp->birimler.troller.birimSayi > 0)
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            fprintf(savas_sim,"\nTrollerin eski savunma gucu : %f dir. ",ork_imp->birimler.troller.savunma);
            ork_imp->birimler.troller.savunma *= etki;
            fprintf(savas_sim,"Yeni savunma gucu : %f dir. ",ork_imp->birimler.troller.savunma);
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            fprintf(savas_sim,"\nTrollerin eski saldiri gucu : %f dir. ",ork_imp->birimler.troller.saldiri);
            ork_imp->birimler.troller.saldiri *= etki;
            fprintf(savas_sim,"Yeni saldiri gucu : %f dir. ",ork_imp->birimler.troller.saldiri);
        }
        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            fprintf(savas_sim,"\nTrollerin eski kritik sans gucu : %f dir. ",ork_imp->birimler.troller.kritik_sans);
            ork_imp->birimler.troller.kritik_sans *= etki;
            fprintf(savas_sim,"Yeni kritik sans gucu : %f dir. ",ork_imp->birimler.troller.kritik_sans);
        }
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "varg_binicileri") && ork_imp->birimler.varg_binicileri.birimSayi > 0)
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            fprintf(savas_sim,"\nVarg Binicilerinin eski savunma gucu : %f dir. ",ork_imp->birimler.varg_binicileri.savunma);
            ork_imp->birimler.varg_binicileri.savunma *= etki;
            fprintf(savas_sim,"Yeni savunma gucu : %f dir. ",ork_imp->birimler.varg_binicileri.savunma);
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            fprintf(savas_sim,"\nVarg Binicilerinin eski saldiri gucu : %f dir. ",ork_imp->birimler.varg_binicileri.saldiri);
            ork_imp->birimler.varg_binicileri.saldiri *= etki;
            fprintf(savas_sim,"Yeni saldiri gucu : %f dir. ",ork_imp->birimler.varg_binicileri.saldiri);
        }
        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            fprintf(savas_sim,"\nVarg Binicilerinin eski kritik sans gucu : %f dir. ",ork_imp->birimler.varg_binicileri.kritik_sans);
            ork_imp->birimler.varg_binicileri.kritik_sans *= etki;
            fprintf(savas_sim,"Yeni kritik sans gucu : %f dir. ",ork_imp->birimler.varg_binicileri.kritik_sans);
        }
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "mizrakci") && ork_imp->birimler.mizrakcilar.birimSayi > 0)
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            fprintf(savas_sim,"\nMizrakcinin eski savunma gucu : %f dir. ",ork_imp->birimler.mizrakcilar.savunma);
            ork_imp->birimler.mizrakcilar.savunma *= etki;
            fprintf(savas_sim,"Yeni savunma gucu : %f dir. ",ork_imp->birimler.mizrakcilar.savunma);
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            fprintf(savas_sim,"\nMizrakcinin eski saldiri gucu : %f dir. ",ork_imp->birimler.mizrakcilar.saldiri);
            ork_imp->birimler.mizrakcilar.saldiri *= etki;
            fprintf(savas_sim,"Yeni saldiri gucu : %f dir. ",ork_imp->birimler.mizrakcilar.saldiri);
        }
        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            fprintf(savas_sim,"\nMizrakcinin eski kritik sans gucu : %f dir. ",ork_imp->birimler.mizrakcilar.kritik_sans);
            ork_imp->birimler.mizrakcilar.kritik_sans *= etki;
            fprintf(savas_sim,"Yeni kritik sans gucu : %f dir. ",ork_imp->birimler.mizrakcilar.kritik_sans);
        }
    }
}

void arastirma_etkisi(struct InsanImparatorlugu *insan_imp, struct OrkLegi *ork_imp, FILE *savas_sim)
{
    struct ArastirmaOzellikler *arastirma;
    if(insan_imp->arastirma_seviyesi.seviye_1.elit_egitim.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_1.elit_egitim;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Elit Egitim arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }
    if(insan_imp->arastirma_seviyesi.seviye_2.elit_egitim.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_2.elit_egitim;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Elit Egitim arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }
    if(insan_imp->arastirma_seviyesi.seviye_3.elit_egitim.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_3.elit_egitim;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Elit Egitim arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }

    if(ork_imp->arastirma_seviyesi.seviye_1.elit_egitim.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_1.elit_egitim;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Elit Egitim arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }
    if(ork_imp->arastirma_seviyesi.seviye_2.elit_egitim.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_2.elit_egitim;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Elit Egitim arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }
    if(ork_imp->arastirma_seviyesi.seviye_3.elit_egitim.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_3.elit_egitim;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Elit Egitim arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }



    if(insan_imp->arastirma_seviyesi.seviye_1.kusatma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_1.kusatma_ustaligi;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Kusatma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }
    if(insan_imp->arastirma_seviyesi.seviye_2.kusatma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_2.kusatma_ustaligi;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Kusatma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }
    if(insan_imp->arastirma_seviyesi.seviye_3.kusatma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_3.kusatma_ustaligi;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Kusatma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }

    if(ork_imp->arastirma_seviyesi.seviye_1.kusatma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_1.kusatma_ustaligi;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Kusatma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }
    if(ork_imp->arastirma_seviyesi.seviye_2.kusatma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_2.kusatma_ustaligi;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Kusatma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }
    if(ork_imp->arastirma_seviyesi.seviye_3.kusatma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_3.kusatma_ustaligi;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Kusatma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }



    if(insan_imp->arastirma_seviyesi.seviye_1.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_1.saldiri_gelistirmesi;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Saldiri Gelistirmesi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }
    if(insan_imp->arastirma_seviyesi.seviye_2.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_2.saldiri_gelistirmesi;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Saldiri Gelistirmesi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }
    if(insan_imp->arastirma_seviyesi.seviye_3.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_3.saldiri_gelistirmesi;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Saldiri Gelistirmesi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }

    if(ork_imp->arastirma_seviyesi.seviye_1.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_1.saldiri_gelistirmesi;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Saldiri Gelistirmesi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }
    if(ork_imp->arastirma_seviyesi.seviye_2.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_2.saldiri_gelistirmesi;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Saldiri Gelistirmesi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }
    if(ork_imp->arastirma_seviyesi.seviye_3.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_3.saldiri_gelistirmesi;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Saldiri Gelistirmesi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }



    if(insan_imp->arastirma_seviyesi.seviye_1.savunma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_1.savunma_ustaligi;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Savunma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }
    if(insan_imp->arastirma_seviyesi.seviye_2.savunma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_2.savunma_ustaligi;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Savunma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }
    if(insan_imp->arastirma_seviyesi.seviye_3.savunma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_3.savunma_ustaligi;
        fprintf(savas_sim,"\n\nInsan Imp %d. seviye Savunma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki, savas_sim);
    }

    if(ork_imp->arastirma_seviyesi.seviye_1.savunma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_1.savunma_ustaligi;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Savunma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }
    if(ork_imp->arastirma_seviyesi.seviye_2.savunma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_2.savunma_ustaligi;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Savunma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }
    if(ork_imp->arastirma_seviyesi.seviye_3.savunma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_3.savunma_ustaligi;
        fprintf(savas_sim,"\n\nOrk Legi %d. seviye Savunma Ustaligi arastirmasi %s e %%%.0f %s uyguladi.",arastirma->senaryoSeviyesi, arastirma->etkiledigi_birim, arastirma->deger, arastirma->etki_turu);
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki, savas_sim);
    }
}

void canavar_kahraman_bonusu_uygula(char *bonus_turu, float bonus_degeri, float *savunma, float *saldiri, float *kritik_sans)
{
    if (!strcmp(bonus_turu, "savunma"))
        *savunma *= (100 + bonus_degeri) / 100;

    else if (!strcmp(bonus_turu, "saldiri"))
        *saldiri *= (100 + bonus_degeri) / 100;

    else if(!strcmp(bonus_turu, "kritik_sans"))
    {
        *kritik_sans *= (100 + bonus_degeri) / 100;
        if (*kritik_sans > 100)
            *kritik_sans = 100;
    }

}

void ork_kahraman_etkisi(struct KahramanOzellikler *kahraman, struct BirimlerOrg *birimlerOrg, int kahraman_bil, FILE *savas_sim)
{
    if ((int)kahraman->bonus_degeri != 0)
    {
        float bonus = kahraman->bonus_degeri;

        if(kahraman_bil == 1)
            fprintf(savas_sim,"\n\nGoruk Vahsi kahramani ");
        else if(kahraman_bil == 2)
            fprintf(savas_sim,"\n\nThruk Kemikkiran kahramani ");
        else if(kahraman_bil == 3)
            fprintf(savas_sim,"\n\nUgar Zalim kahramani ");
        else if(kahraman_bil == 4)
            fprintf(savas_sim,"\n\nVrog Kafakiran kahramani ");

        if (!strcmp(kahraman->etkiledigi_birim, "ork_dovusculeri") && birimlerOrg->ork_dovusculeri.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",kahraman->etkiledigi_birim, bonus, kahraman->bonus_turu);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",kahraman->etkiledigi_birim, birimlerOrg->ork_dovusculeri.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerOrg->ork_dovusculeri.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerOrg->ork_dovusculeri.kritik_sans);

            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->ork_dovusculeri.savunma, &birimlerOrg->ork_dovusculeri.saldiri, &birimlerOrg->ork_dovusculeri.kritik_sans);

            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir.",birimlerOrg->ork_dovusculeri.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir.",birimlerOrg->ork_dovusculeri.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir.",birimlerOrg->ork_dovusculeri.kritik_sans);
        }
        else if (!strcmp(kahraman->etkiledigi_birim, "mizrakcilar") && birimlerOrg->mizrakcilar.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",kahraman->etkiledigi_birim, bonus, kahraman->bonus_turu);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",kahraman->etkiledigi_birim, birimlerOrg->mizrakcilar.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerOrg->mizrakcilar.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerOrg->mizrakcilar.kritik_sans);

            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->mizrakcilar.savunma, &birimlerOrg->mizrakcilar.saldiri, &birimlerOrg->mizrakcilar.kritik_sans);

            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir.",birimlerOrg->mizrakcilar.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir.",birimlerOrg->mizrakcilar.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir.",birimlerOrg->mizrakcilar.kritik_sans);
        }
        else if (!strcmp(kahraman->etkiledigi_birim, "varg_binicileri") && birimlerOrg->varg_binicileri.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",kahraman->etkiledigi_birim, bonus, kahraman->bonus_turu);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",kahraman->etkiledigi_birim, birimlerOrg->varg_binicileri.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerOrg->varg_binicileri.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerOrg->varg_binicileri.kritik_sans);

            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->varg_binicileri.savunma, &birimlerOrg->varg_binicileri.saldiri, &birimlerOrg->varg_binicileri.kritik_sans);

            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir.",birimlerOrg->varg_binicileri.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir.",birimlerOrg->varg_binicileri.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir.",birimlerOrg->varg_binicileri.kritik_sans);
        }
        else if (!strcmp(kahraman->etkiledigi_birim, "troller") && birimlerOrg->troller.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",kahraman->etkiledigi_birim, bonus, kahraman->bonus_turu);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",kahraman->etkiledigi_birim, birimlerOrg->troller.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerOrg->troller.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerOrg->troller.kritik_sans);

            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->troller.savunma, &birimlerOrg->troller.saldiri, &birimlerOrg->troller.kritik_sans);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir.",birimlerOrg->troller.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir.",birimlerOrg->troller.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir.",birimlerOrg->troller.kritik_sans);
        }
        else if (!strcmp(kahraman->etkiledigi_birim, "tum_birimlere"))
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",kahraman->etkiledigi_birim, bonus, kahraman->bonus_turu);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin eski savunma gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.savunma,birimlerOrg->mizrakcilar.savunma,birimlerOrg->varg_binicileri.savunma,birimlerOrg->troller.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin eski saldiri gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.saldiri,birimlerOrg->mizrakcilar.saldiri,birimlerOrg->varg_binicileri.saldiri,birimlerOrg->troller.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin eski kritik sans gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.kritik_sans,birimlerOrg->mizrakcilar.kritik_sans,birimlerOrg->varg_binicileri.kritik_sans,birimlerOrg->troller.kritik_sans);

            if(birimlerOrg->ork_dovusculeri.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->ork_dovusculeri.savunma, &birimlerOrg->ork_dovusculeri.saldiri, &birimlerOrg->ork_dovusculeri.kritik_sans);
            if(birimlerOrg->mizrakcilar.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->mizrakcilar.savunma, &birimlerOrg->mizrakcilar.saldiri, &birimlerOrg->mizrakcilar.kritik_sans);
            if(birimlerOrg->varg_binicileri.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->varg_binicileri.savunma, &birimlerOrg->varg_binicileri.saldiri, &birimlerOrg->varg_binicileri.kritik_sans);
            if(birimlerOrg->troller.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->troller.savunma, &birimlerOrg->troller.saldiri, &birimlerOrg->troller.kritik_sans);

            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin yeni savunma gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.savunma,birimlerOrg->mizrakcilar.savunma,birimlerOrg->varg_binicileri.savunma,birimlerOrg->troller.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin yeni saldiri gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.saldiri,birimlerOrg->mizrakcilar.saldiri,birimlerOrg->varg_binicileri.saldiri,birimlerOrg->troller.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin yeni kritik sans gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.kritik_sans,birimlerOrg->mizrakcilar.kritik_sans,birimlerOrg->varg_binicileri.kritik_sans,birimlerOrg->troller.kritik_sans);

        }
    }
}

void ork_canavar_etkisi(struct CanavarOzellikler *canavar, struct BirimlerOrg *birimlerOrg, int canavar_bil, FILE *savas_sim)
{
    if ((int)canavar->etki_degeri != 0)
    {
        float bonus = canavar->etki_degeri;

        if(canavar_bil == 1)
            fprintf(savas_sim,"\n\nAtes Iblisi canavari ");
        else if(canavar_bil == 2)
            fprintf(savas_sim,"\n\nBuz Devleri canavari ");
        else if(canavar_bil == 3)
            fprintf(savas_sim,"\n\nCamur Devleri canavari ");
        else if(canavar_bil == 4)
            fprintf(savas_sim,"\n\nGolge Kurtlari canavari ");
        else if(canavar_bil == 5)
            fprintf(savas_sim,"\n\nKara Troll canavari ");
        else if(canavar_bil == 6)
            fprintf(savas_sim,"\n\nMakrog Savas Beyi canavari ");

        if (!strcmp(canavar->etkiledigi_birim, "ork_dovusculeri") && birimlerOrg->ork_dovusculeri.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",canavar->etkiledigi_birim, bonus, canavar->etki_turu);
            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",canavar->etkiledigi_birim, birimlerOrg->ork_dovusculeri.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",canavar->etkiledigi_birim, birimlerOrg->ork_dovusculeri.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",canavar->etkiledigi_birim, birimlerOrg->ork_dovusculeri.kritik_sans);

            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->ork_dovusculeri.savunma, &birimlerOrg->ork_dovusculeri.saldiri, &birimlerOrg->ork_dovusculeri.kritik_sans);

            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerOrg->ork_dovusculeri.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerOrg->ork_dovusculeri.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerOrg->ork_dovusculeri.kritik_sans);
        }

        else if (!strcmp(canavar->etkiledigi_birim, "mizrakcilar") && birimlerOrg->mizrakcilar.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",canavar->etkiledigi_birim, bonus, canavar->etki_turu);
            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",canavar->etkiledigi_birim, birimlerOrg->mizrakcilar.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",canavar->etkiledigi_birim, birimlerOrg->mizrakcilar.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",canavar->etkiledigi_birim, birimlerOrg->mizrakcilar.kritik_sans);

            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->mizrakcilar.savunma, &birimlerOrg->mizrakcilar.saldiri, &birimlerOrg->mizrakcilar.kritik_sans);

            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerOrg->mizrakcilar.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerOrg->mizrakcilar.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerOrg->mizrakcilar.kritik_sans);
        }

        else if (!strcmp(canavar->etkiledigi_birim, "varg_binicileri") && birimlerOrg->varg_binicileri.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",canavar->etkiledigi_birim, bonus, canavar->etki_turu);
            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",canavar->etkiledigi_birim, birimlerOrg->varg_binicileri.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",canavar->etkiledigi_birim, birimlerOrg->varg_binicileri.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",canavar->etkiledigi_birim, birimlerOrg->varg_binicileri.kritik_sans);

            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->varg_binicileri.savunma, &birimlerOrg->varg_binicileri.saldiri, &birimlerOrg->varg_binicileri.kritik_sans);

            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerOrg->varg_binicileri.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerOrg->varg_binicileri.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerOrg->varg_binicileri.kritik_sans);
        }

        else if (!strcmp(canavar->etkiledigi_birim, "troller") && birimlerOrg->troller.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",canavar->etkiledigi_birim, bonus, canavar->etki_turu);
            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",canavar->etkiledigi_birim, birimlerOrg->troller.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",canavar->etkiledigi_birim, birimlerOrg->troller.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",canavar->etkiledigi_birim, birimlerOrg->troller.kritik_sans);

            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->troller.savunma, &birimlerOrg->troller.saldiri, &birimlerOrg->troller.kritik_sans);

            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerOrg->troller.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerOrg->troller.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerOrg->troller.kritik_sans);
        }

        else if (!strcmp(canavar->etkiledigi_birim, "tum_birimlere"))
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",canavar->etkiledigi_birim, bonus, canavar->etki_turu);
            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin eski savunma gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.savunma,birimlerOrg->mizrakcilar.savunma,birimlerOrg->varg_binicileri.savunma,birimlerOrg->troller.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin eski saldiri gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.saldiri,birimlerOrg->mizrakcilar.saldiri,birimlerOrg->varg_binicileri.saldiri,birimlerOrg->troller.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin eski kritik sans gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.kritik_sans,birimlerOrg->mizrakcilar.kritik_sans,birimlerOrg->varg_binicileri.kritik_sans,birimlerOrg->troller.kritik_sans);

            if(birimlerOrg->ork_dovusculeri.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->ork_dovusculeri.savunma, &birimlerOrg->ork_dovusculeri.saldiri, &birimlerOrg->ork_dovusculeri.kritik_sans);
            if(birimlerOrg->mizrakcilar.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->mizrakcilar.savunma, &birimlerOrg->mizrakcilar.saldiri, &birimlerOrg->mizrakcilar.kritik_sans);
            if(birimlerOrg->varg_binicileri.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->varg_binicileri.savunma, &birimlerOrg->varg_binicileri.saldiri, &birimlerOrg->varg_binicileri.kritik_sans);
            if(birimlerOrg->troller.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->troller.savunma, &birimlerOrg->troller.saldiri, &birimlerOrg->troller.kritik_sans);

            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin yeni savunma gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.savunma,birimlerOrg->mizrakcilar.savunma,birimlerOrg->varg_binicileri.savunma,birimlerOrg->troller.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin yeni saldiri gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.saldiri,birimlerOrg->mizrakcilar.saldiri,birimlerOrg->varg_binicileri.saldiri,birimlerOrg->troller.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Ork Dovusculeri - Mizrakcilar - Varg Binicileri - Troller birimlerinin yeni kritik sans gucleri = %f - %f - %f - %f dir. ",birimlerOrg->ork_dovusculeri.kritik_sans,birimlerOrg->mizrakcilar.kritik_sans,birimlerOrg->varg_binicileri.kritik_sans,birimlerOrg->troller.kritik_sans);

        }
    }
}

void ork_kahraman_canavar_etkisi(struct KahramanlarOrg *kahraman, struct CanavarlarOrg *canavar, struct BirimlerOrg *birimler, FILE *savas_sim)
{
    ork_kahraman_etkisi(&kahraman->Goruk_Vahsi, birimler, 1, savas_sim);
    ork_kahraman_etkisi(&kahraman->Thruk_Kemikkiran, birimler, 2, savas_sim);
    ork_kahraman_etkisi(&kahraman->Ugar_Zalim, birimler, 3, savas_sim);
    ork_kahraman_etkisi(&kahraman->Vrog_Kafakiran, birimler, 4, savas_sim);

    ork_canavar_etkisi(&canavar->Ates_Iblisi, birimler, 1, savas_sim);
    ork_canavar_etkisi(&canavar->Buz_Devleri, birimler, 2, savas_sim);
    ork_canavar_etkisi(&canavar->Camur_Devleri, birimler, 3, savas_sim);
    ork_canavar_etkisi(&canavar->Golge_Kurtlari, birimler, 4, savas_sim);
    ork_canavar_etkisi(&canavar->Kara_Troll, birimler, 5, savas_sim);
    ork_canavar_etkisi(&canavar->Makrog_Savas_Beyi, birimler, 6, savas_sim);
}

void insan_kahraman_etkisi(struct KahramanOzellikler *kahraman, struct BirimlerInsan *birimlerInsan, int kahraman_bil, FILE *savas_sim)
{
    if ((int)kahraman->bonus_degeri != 0)
    {
        float bonus = kahraman->bonus_degeri;

        if(kahraman_bil == 1)
            fprintf(savas_sim,"\n\nAlparslan kahramani ");
        else if(kahraman_bil == 2)
            fprintf(savas_sim,"\n\nMete Han kahramani ");
        else if(kahraman_bil == 3)
            fprintf(savas_sim,"\n\nFatih Sultan Mehmet kahramani ");
        else if(kahraman_bil == 4)
            fprintf(savas_sim,"\n\nTugrul Bey kahramani ");
        else if(kahraman_bil == 5)
            fprintf(savas_sim,"\n\nYavuz Sultan Selim kahramani ");

        if (!strcmp(kahraman->etkiledigi_birim, "piyade") && birimlerInsan->piyadeler.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. ",kahraman->etkiledigi_birim, bonus, kahraman->bonus_turu);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",kahraman->etkiledigi_birim, birimlerInsan->piyadeler.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerInsan->piyadeler.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerInsan->piyadeler.kritik_sans);

            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->piyadeler.savunma, &birimlerInsan->piyadeler.saldiri, &birimlerInsan->piyadeler.kritik_sans);

            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerInsan->piyadeler.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerInsan->piyadeler.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerInsan->piyadeler.kritik_sans);
        }
        else if (!strcmp(kahraman->etkiledigi_birim, "okcu") && birimlerInsan->okcular.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",kahraman->etkiledigi_birim, bonus, kahraman->bonus_turu);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",kahraman->etkiledigi_birim, birimlerInsan->okcular.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerInsan->okcular.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerInsan->okcular.kritik_sans);

            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->okcular.savunma, &birimlerInsan->okcular.saldiri, &birimlerInsan->okcular.kritik_sans);

            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerInsan->okcular.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerInsan->okcular.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerInsan->okcular.kritik_sans);
        }
        else if (!strcmp(kahraman->etkiledigi_birim, "kusatma_makinesi") && birimlerInsan->kusatma_makineleri.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",kahraman->etkiledigi_birim, bonus, kahraman->bonus_turu);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",kahraman->etkiledigi_birim, birimlerInsan->kusatma_makineleri.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerInsan->kusatma_makineleri.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerInsan->kusatma_makineleri.kritik_sans);

            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->kusatma_makineleri.savunma, &birimlerInsan->kusatma_makineleri.saldiri, &birimlerInsan->kusatma_makineleri.kritik_sans);


            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerInsan->kusatma_makineleri.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerInsan->kusatma_makineleri.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerInsan->kusatma_makineleri.kritik_sans);
        }
        else if (!strcmp(kahraman->etkiledigi_birim, "suvari") && birimlerInsan->suvariler.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",kahraman->etkiledigi_birim, bonus, kahraman->bonus_turu);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerInsan->suvariler.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerInsan->suvariler.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",kahraman->etkiledigi_birim, birimlerInsan->suvariler.kritik_sans);

            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->suvariler.savunma, &birimlerInsan->suvariler.saldiri, &birimlerInsan->suvariler.kritik_sans);

            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerInsan->suvariler.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerInsan->suvariler.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerInsan->suvariler.kritik_sans);
        }
        else if (!strcmp(kahraman->etkiledigi_birim, "tum_birimlere"))
        {
            fprintf(savas_sim," %s %%%.0f %s bonus uyguladi. \n",kahraman->etkiledigi_birim, bonus, kahraman->bonus_turu);
            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"Suvari - Piyade - Okcu - Kusatma mak. birimlerinin eski savunma gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.savunma,birimlerInsan->piyadeler.savunma,birimlerInsan->okcular.savunma,birimlerInsan->kusatma_makineleri.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"Suvari - Piyade - Okcu - Kusatma mak. birimlerinin eski saldiri gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.saldiri,birimlerInsan->piyadeler.saldiri,birimlerInsan->okcular.saldiri,birimlerInsan->kusatma_makineleri.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"Suvari - Piyade - Okcu - Kusatma mak. birimlerinin eski kritik sans gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.kritik_sans,birimlerInsan->piyadeler.kritik_sans,birimlerInsan->okcular.kritik_sans,birimlerInsan->kusatma_makineleri.kritik_sans);

            if(birimlerInsan->piyadeler.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->piyadeler.savunma, &birimlerInsan->piyadeler.saldiri, &birimlerInsan->piyadeler.kritik_sans);
            if(birimlerInsan->okcular.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->okcular.savunma, &birimlerInsan->okcular.saldiri, &birimlerInsan->okcular.kritik_sans);
            if(birimlerInsan->kusatma_makineleri.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->kusatma_makineleri.savunma, &birimlerInsan->kusatma_makineleri.saldiri, &birimlerInsan->kusatma_makineleri.kritik_sans);
            if(birimlerInsan->suvariler.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->suvariler.savunma, &birimlerInsan->suvariler.saldiri, &birimlerInsan->suvariler.kritik_sans);

            if (!strcmp(kahraman->bonus_turu, "savunma"))
                fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin yeni savunma gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.savunma,birimlerInsan->piyadeler.savunma,birimlerInsan->okcular.savunma,birimlerInsan->kusatma_makineleri.savunma);
            else if (!strcmp(kahraman->bonus_turu, "saldiri"))
                fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin yeni saldiri gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.saldiri,birimlerInsan->piyadeler.saldiri,birimlerInsan->okcular.saldiri,birimlerInsan->kusatma_makineleri.saldiri);
            else if(!strcmp(kahraman->bonus_turu, "kritik_sans"))
                fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin yeni kritik sans gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.kritik_sans,birimlerInsan->piyadeler.kritik_sans,birimlerInsan->okcular.kritik_sans,birimlerInsan->kusatma_makineleri.kritik_sans);

        }
    }
}

void insan_canavar_etkisi(struct CanavarOzellikler *canavar, struct BirimlerInsan *birimlerInsan, int canavar_bil, FILE *savas_sim)
{
    if ((int)canavar->etki_degeri != 0)
    {
        float bonus = canavar->etki_degeri;

        if(canavar_bil == 1)
            fprintf(savas_sim,"\n\nAgri Dagi Devleri canavari ");
        else if(canavar_bil == 2)
            fprintf(savas_sim,"\n\Ejderha canavari ");
        else if(canavar_bil == 3)
            fprintf(savas_sim,"\n\nKarakurt canavari ");
        else if(canavar_bil == 4)
            fprintf(savas_sim,"\n\nSamur canavari ");
        else if(canavar_bil == 5)
            fprintf(savas_sim,"\n\nTepegoz canavari ");

        if (!strcmp(canavar->etkiledigi_birim, "piyade") && birimlerInsan->piyadeler.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",canavar->etkiledigi_birim, bonus, canavar->etki_turu);
            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",canavar->etkiledigi_birim, birimlerInsan->piyadeler.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",canavar->etkiledigi_birim, birimlerInsan->piyadeler.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",canavar->etkiledigi_birim, birimlerInsan->piyadeler.kritik_sans);

            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->piyadeler.savunma, &birimlerInsan->piyadeler.saldiri, &birimlerInsan->piyadeler.kritik_sans);

            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerInsan->piyadeler.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerInsan->piyadeler.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerInsan->piyadeler.kritik_sans);
        }
        else if (!strcmp(canavar->etkiledigi_birim, "okcu") && birimlerInsan->okcular.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",canavar->etkiledigi_birim, bonus, canavar->etki_turu);
            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",canavar->etkiledigi_birim, birimlerInsan->okcular.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",canavar->etkiledigi_birim, birimlerInsan->okcular.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",canavar->etkiledigi_birim, birimlerInsan->okcular.kritik_sans);

            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->okcular.savunma, &birimlerInsan->okcular.saldiri, &birimlerInsan->okcular.kritik_sans);

            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerInsan->okcular.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerInsan->okcular.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerInsan->okcular.kritik_sans);
        }
        else if (!strcmp(canavar->etkiledigi_birim, "kusatma_makinesi") && birimlerInsan->kusatma_makineleri.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",canavar->etkiledigi_birim, bonus, canavar->etki_turu);
            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",canavar->etkiledigi_birim, birimlerInsan->kusatma_makineleri.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",canavar->etkiledigi_birim, birimlerInsan->kusatma_makineleri.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",canavar->etkiledigi_birim, birimlerInsan->kusatma_makineleri.kritik_sans);

            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->kusatma_makineleri.savunma, &birimlerInsan->kusatma_makineleri.saldiri, &birimlerInsan->kusatma_makineleri.kritik_sans);

            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerInsan->kusatma_makineleri.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerInsan->kusatma_makineleri.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerInsan->kusatma_makineleri.kritik_sans);
        }
        else if (!strcmp(canavar->etkiledigi_birim, "suvari") && birimlerInsan->suvariler.birimSayi > 0)
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",canavar->etkiledigi_birim, bonus, canavar->etki_turu);
            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"%s biriminin eski savunma gucu = %fdir. ",canavar->etkiledigi_birim, birimlerInsan->suvariler.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"%s biriminin eski saldiri gucu = %f dir. ",canavar->etkiledigi_birim, birimlerInsan->suvariler.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"%s biriminin eski kritik sans gucu = %f dir. ",canavar->etkiledigi_birim, birimlerInsan->suvariler.kritik_sans);

            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->suvariler.savunma, &birimlerInsan->suvariler.saldiri, &birimlerInsan->suvariler.kritik_sans);

            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Yeni savunma gucu = %fdir. ",birimlerInsan->suvariler.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Yeni saldiri gucu = %f dir. ",birimlerInsan->suvariler.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Yeni kritik sans gucu = %f dir. ",birimlerInsan->suvariler.kritik_sans);
        }
        else if (!strcmp(canavar->etkiledigi_birim, "tum_birimlere"))
        {
            fprintf(savas_sim," %s birimine %%%.0f %s bonusu uyguladi. \n",canavar->etkiledigi_birim, bonus, canavar->etki_turu);
            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"Suvari - Piyade - Okcu - Kusatma mak. birimlerinin eski savunma gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.savunma,birimlerInsan->piyadeler.savunma,birimlerInsan->okcular.savunma,birimlerInsan->kusatma_makineleri.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"Suvari - Piyade - Okcu - Kusatma mak. birimlerinin eski saldiri gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.saldiri,birimlerInsan->piyadeler.saldiri,birimlerInsan->okcular.saldiri,birimlerInsan->kusatma_makineleri.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"Suvari - Piyade - Okcu - Kusatma mak. birimlerinin eski kritik sans gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.kritik_sans,birimlerInsan->piyadeler.kritik_sans,birimlerInsan->okcular.kritik_sans,birimlerInsan->kusatma_makineleri.kritik_sans);

            if(birimlerInsan->piyadeler.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->piyadeler.savunma, &birimlerInsan->piyadeler.saldiri, &birimlerInsan->piyadeler.kritik_sans);
            if(birimlerInsan->okcular.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->okcular.savunma, &birimlerInsan->okcular.saldiri, &birimlerInsan->okcular.kritik_sans);
            if(birimlerInsan->kusatma_makineleri.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->kusatma_makineleri.savunma, &birimlerInsan->kusatma_makineleri.saldiri, &birimlerInsan->kusatma_makineleri.kritik_sans);
            if(birimlerInsan->suvariler.birimSayi > 0)
                canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->suvariler.savunma, &birimlerInsan->suvariler.saldiri, &birimlerInsan->suvariler.kritik_sans);

            if (!strcmp(canavar->etki_turu, "savunma"))
                fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin yeni savunma gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.savunma,birimlerInsan->piyadeler.savunma,birimlerInsan->okcular.savunma,birimlerInsan->kusatma_makineleri.savunma);
            else if (!strcmp(canavar->etki_turu, "saldiri"))
                fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin yeni saldiri gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.saldiri,birimlerInsan->piyadeler.saldiri,birimlerInsan->okcular.saldiri,birimlerInsan->kusatma_makineleri.saldiri);
            else if(!strcmp(canavar->etki_turu, "kritik_sans"))
                fprintf(savas_sim,"\nSuvari - Piyade - Okcu - Kusatma mak. birimlerinin yeni kritik sans gucleri = %f - %f - %f - %f dir. ",birimlerInsan->suvariler.kritik_sans,birimlerInsan->piyadeler.kritik_sans,birimlerInsan->okcular.kritik_sans,birimlerInsan->kusatma_makineleri.kritik_sans);

        }
    }
}

void insan_kahraman_canavar_etkisi(struct KahramanlarInsan *kahraman, struct CanavarlarInsan *canavar, struct BirimlerInsan *birimler, FILE *savas_sim)
{
    insan_kahraman_etkisi(&kahraman->Alparslan, birimler, 1, savas_sim);
    insan_kahraman_etkisi(&kahraman->Mete_Han, birimler, 2, savas_sim);
    insan_kahraman_etkisi(&kahraman->Fatih_Sultan_Mehmet, birimler, 3, savas_sim);
    insan_kahraman_etkisi(&kahraman->Tugrul_Bey, birimler, 4, savas_sim);
    insan_kahraman_etkisi(&kahraman->Yavuz_Sultan_Selim, birimler, 5, savas_sim);

    insan_canavar_etkisi(&canavar->Agri_Dagi_Devleri, birimler, 1, savas_sim);
    insan_canavar_etkisi(&canavar->Ejderha, birimler, 2, savas_sim);
    insan_canavar_etkisi(&canavar->Karakurt, birimler, 3, savas_sim);
    insan_canavar_etkisi(&canavar->Samur, birimler, 4, savas_sim);
    insan_canavar_etkisi(&canavar->Tepegoz, birimler, 5, savas_sim);
}


void hasar_dagilimi_ork(struct BirimlerOrg *birim, float netHasar, float toplamSavunmaGucu)
{
    if (birim->mizrakcilar.birimSayi > 0)
        birim->mizrakcilar.hasar_dagilimi = netHasar * (((float)birim->mizrakcilar.birimSayi * birim->mizrakcilar.savunma) / toplamSavunmaGucu);

    if (birim->ork_dovusculeri.birimSayi > 0)
        birim->ork_dovusculeri.hasar_dagilimi = netHasar * (((float)birim->ork_dovusculeri.birimSayi * birim->ork_dovusculeri.savunma) / toplamSavunmaGucu);

    if (birim->troller.birimSayi > 0)
        birim->troller.hasar_dagilimi = netHasar * (((float)birim->troller.birimSayi * birim->troller.savunma) / toplamSavunmaGucu);

    if (birim->varg_binicileri.birimSayi > 0)
        birim->varg_binicileri.hasar_dagilimi = netHasar * (((float)birim->varg_binicileri.birimSayi * birim->varg_binicileri.savunma) / toplamSavunmaGucu);

}

void olen_hasardagit_ork(struct BirimlerOrg *birim, float birimHasarDagilimi, float kalanSavunmaGucu)
{
    if (birim->mizrakcilar.birimSayi > 0)
        birim->mizrakcilar.hasar_dagilimi += birimHasarDagilimi * (((float)birim->mizrakcilar.birimSayi * birim->mizrakcilar.savunma) / kalanSavunmaGucu);

    if (birim->ork_dovusculeri.birimSayi > 0)
        birim->ork_dovusculeri.hasar_dagilimi += birimHasarDagilimi * (((float)birim->ork_dovusculeri.birimSayi * birim->ork_dovusculeri.savunma) / kalanSavunmaGucu);

    if (birim->troller.birimSayi > 0)
        birim->troller.hasar_dagilimi += birimHasarDagilimi * (((float)birim->troller.birimSayi * birim->troller.savunma) / kalanSavunmaGucu);

    if (birim->varg_binicileri.birimSayi > 0)
        birim->varg_binicileri.hasar_dagilimi += birimHasarDagilimi * (((float)birim->varg_binicileri.birimSayi * birim->varg_binicileri.savunma) / kalanSavunmaGucu);
}

void birim_kayip_ork(struct BirimlerOrg *birim, FILE *savas_sim)
{
    int Msavasta = 0;
    int Vsavasta = 0;
    int Osavasta = 0;
    int Tsavasta = 0;

    fprintf(savas_sim,"\n\n\n*****Ork Birim Kayiplari*****\n\n");
    if (birim->mizrakcilar.birimSayi > 0)
    {
        Msavasta = 1;
        fprintf(savas_sim,"\nMizrakcilar hasar dagilimi : %f\n",birim->mizrakcilar.hasar_dagilimi);
        fprintf(savas_sim,"Mizrakcilar onceki sayisi : %d , ",birim->mizrakcilar.birimSayi);
        birim->mizrakcilar.birimSayi -= birim->mizrakcilar.hasar_dagilimi / birim->mizrakcilar.saglik;
        fprintf(savas_sim,"sonraki sayisi : %d , olen birim sayisi : %f\n",birim->mizrakcilar.birimSayi, birim->mizrakcilar.hasar_dagilimi / birim->mizrakcilar.saglik);
    }
    if (birim->varg_binicileri.birimSayi > 0)
    {
        Vsavasta = 1;
        fprintf(savas_sim,"\nVarg Binicileri hasar dagilimi : %f\n",birim->varg_binicileri.hasar_dagilimi);
        fprintf(savas_sim,"Varg Binicileri onceki sayisi : %d , ",birim->varg_binicileri.birimSayi);
        birim->varg_binicileri.birimSayi -= birim->varg_binicileri.hasar_dagilimi / birim->varg_binicileri.saglik;
        fprintf(savas_sim,"sonraki sayisi : %d , olen birim sayisi : %f\n",birim->varg_binicileri.birimSayi, birim->varg_binicileri.hasar_dagilimi / birim->varg_binicileri.saglik);
    }
    if (birim->ork_dovusculeri.birimSayi > 0)
    {
        Osavasta = 1;
        fprintf(savas_sim,"\nOrk Dovusculeri hasar dagilimi : %f\n",birim->ork_dovusculeri.hasar_dagilimi);
        fprintf(savas_sim,"Ork Dovusculeri onceki sayisi : %d , ",birim->ork_dovusculeri.birimSayi);
        birim->ork_dovusculeri.birimSayi -= birim->ork_dovusculeri.hasar_dagilimi / birim->ork_dovusculeri.saglik;
        fprintf(savas_sim,"sonraki sayisi : %d , olen birim sayisi : %f\n",birim->ork_dovusculeri.birimSayi, birim->ork_dovusculeri.hasar_dagilimi / birim->ork_dovusculeri.saglik);
    }
    if (birim->troller.birimSayi > 0)
    {
        Tsavasta = 1;
        fprintf(savas_sim,"\nTroller hasar dagilimi : %f\n",birim->troller.hasar_dagilimi);
        fprintf(savas_sim,"Troller onceki sayisi : %d , ",birim->troller.birimSayi);
        birim->troller.birimSayi -= birim->troller.hasar_dagilimi / birim->troller.saglik;
        fprintf(savas_sim,"sonraki sayisi : %d , olen birim sayisi : %f\n",birim->troller.birimSayi, birim->troller.hasar_dagilimi / birim->troller.saglik);
    }
    int Mkontrol = 0;
    int Okontrol = 0;
    int Tkontrol = 0;
    int Vkontrol = 0;

    if (birim->mizrakcilar.birimSayi <= 0 && Mkontrol == 0 && Msavasta == 1)
    {
        Mkontrol = 1;
        fprintf(savas_sim,"\nMizrakci oldu.\n");
        birim->mizrakcilar.birimSayi = 0;
        olen_hasardagit_ork(birim, birim->mizrakcilar.hasar_dagilimi, orkSavunmaHesapla(birim));
        fprintf(savas_sim,"Ork yeni hasar dagilimi : %f\n", birim->ork_dovusculeri.hasar_dagilimi);
        fprintf(savas_sim,"Trol yeni hasar dagilimi : %f\n", birim->troller.hasar_dagilimi);
        fprintf(savas_sim,"Varg Binicileri yeni hasar dagilimi : %f\n", birim->varg_binicileri.hasar_dagilimi);
    }
    if (birim->ork_dovusculeri.birimSayi <= 0 && Okontrol == 0 && Osavasta == 1)
    {
        Okontrol = 1;
        fprintf(savas_sim,"\nOrk Dovusculeri oldu\n");
        birim->ork_dovusculeri.birimSayi = 0;
        olen_hasardagit_ork(birim, birim->ork_dovusculeri.hasar_dagilimi, orkSavunmaHesapla(birim));
        fprintf(savas_sim,"Mizrakci yeni hasar dagilimi : %f\n", birim->mizrakcilar.hasar_dagilimi);
        fprintf(savas_sim,"Trol yeni hasar dagilimi : %f\n", birim->troller.hasar_dagilimi);
        fprintf(savas_sim,"Varg Binicileri yeni hasar dagilimi : %f\n", birim->varg_binicileri.hasar_dagilimi);
    }
    if (birim->troller.birimSayi <= 0 && Tkontrol == 0 && Tsavasta == 1)
    {
        Tkontrol = 1;
        fprintf(savas_sim,"\nTroller oldu\n");
        birim->troller.birimSayi = 0;
        olen_hasardagit_ork(birim, birim->troller.hasar_dagilimi, orkSavunmaHesapla(birim));
        fprintf(savas_sim,"Mizrakci yeni hasar dagilimi : %f\n", birim->mizrakcilar.hasar_dagilimi);
        fprintf(savas_sim,"Ork yeni hasar dagilimi : %f\n", birim->ork_dovusculeri.hasar_dagilimi);
        fprintf(savas_sim,"Varg Binicileri yeni hasar dagilimi : %f\n", birim->varg_binicileri.hasar_dagilimi);
    }
    if (birim->varg_binicileri.birimSayi <= 0 && Vkontrol == 0 && Vsavasta == 1)
    {
        Vkontrol = 1;
        fprintf(savas_sim,"\nVSarg Binicileri oldu\n");
        birim->varg_binicileri.birimSayi = 0;
        olen_hasardagit_ork(birim, birim->varg_binicileri.hasar_dagilimi, orkSavunmaHesapla(birim));
        fprintf(savas_sim,"Mizrakci yeni hasar dagilimi : %f\n", birim->mizrakcilar.hasar_dagilimi);
        fprintf(savas_sim,"Ork yeni hasar dagilimi : %f\n", birim->ork_dovusculeri.hasar_dagilimi);
        fprintf(savas_sim,"Trol yeni hasar dagilimi : %f\n", birim->troller.hasar_dagilimi);
    }
}

void saglik_durumu_ork(struct BirimlerOrg *birim, FILE *savas_sim)
{
    fprintf(savas_sim,"\n\n++++Ork Legi Saglik Durumlari++++\n\n");
    if (birim->mizrakcilar.birimSayi > 0)
    {
        fprintf(savas_sim,"\nMizrakcinin onceki sagligi : %f , ",birim->mizrakcilar.saglik);
        birim->mizrakcilar.saglik -= birim->mizrakcilar.hasar_dagilimi / (float)birim->mizrakcilar.birimSayi;
        fprintf(savas_sim,"sonraki sagligi : %f\n",birim->mizrakcilar.saglik);
        if(birim->mizrakcilar.saglik <= 0)
        {
            fprintf(savas_sim,"\nMizrakci sagligi 0 yada altina dustugu icin oldu!!!\n");
            birim->mizrakcilar.birimSayi = 0;
            birim->mizrakcilar.saglik = 0;
        }
    }
    if (birim->varg_binicileri.birimSayi > 0)
    {
        fprintf(savas_sim,"\nVarg Binicilerinin onceki sagligi : %f , ",birim->varg_binicileri.saglik);
        birim->varg_binicileri.saglik -= birim->varg_binicileri.hasar_dagilimi / (float)birim->varg_binicileri.birimSayi;
        fprintf(savas_sim,"sonraki sagligi : %f\n",birim->varg_binicileri.saglik);
        if(birim->varg_binicileri.saglik <= 0)
        {
            fprintf(savas_sim,"\nVarg Binicileri sagligi 0 yada altina dustugu icin oldu!!!\n");
            birim->varg_binicileri.birimSayi = 0;
            birim->varg_binicileri.saglik = 0;
        }
    }
    if (birim->ork_dovusculeri.birimSayi > 0)
    {
        fprintf(savas_sim,"\nOrk Dovusculerinin onceki sagligi : %f , ",birim->ork_dovusculeri.saglik);
        birim->ork_dovusculeri.saglik -= birim->ork_dovusculeri.hasar_dagilimi / (float)birim->ork_dovusculeri.birimSayi;
        fprintf(savas_sim,"sonraki sagligi : %f\n",birim->ork_dovusculeri.saglik);
        if(birim->ork_dovusculeri.saglik <= 0)
        {
            fprintf(savas_sim,"\nOrk Dovusculeri sagligi 0 yada altina dustugu icin oldu!!!\n");
            birim->ork_dovusculeri.birimSayi = 0;
            birim->ork_dovusculeri.saglik = 0;
        }
    }
    if (birim->troller.birimSayi > 0)
    {
        fprintf(savas_sim,"\nTrollerin onceki sagligi : %f , ",birim->troller.saglik);
        birim->troller.saglik -= birim->troller.hasar_dagilimi / (float)birim->troller.birimSayi;
        fprintf(savas_sim,"sonraki sagligi : %f\n",birim->troller.saglik);
        if(birim->troller.saglik <= 0)
        {
            fprintf(savas_sim,"\nTrollerin sagligi 0 yada altina dustugu icin oldu!!!\n");
            birim->troller.birimSayi = 0;
            birim->troller.saglik = 0;
        }
    }
    if(birim->mizrakcilar.saglik == 0 && birim->ork_dovusculeri.saglik == 0 && birim->troller.saglik == 0 &&birim->varg_binicileri.saglik == 0)
        fprintf(savas_sim,"\nButun birimlerin sagligi 0 oldu, hepsi oldu.");
}

void birim_guncelle_ork(struct BirimlerOrg *birim, float netHasar, float toplamSavunmaGucu, FILE *savas_sim)
{
    hasar_dagilimi_ork(birim, netHasar, toplamSavunmaGucu);
    birim_kayip_ork(birim, savas_sim);
    saglik_durumu_ork(birim, savas_sim);
}

void hasar_dagilimi_insan(struct BirimlerInsan *birim, float netHasar, float toplamSavunmaGucu)
{
    if (birim->kusatma_makineleri.birimSayi > 0)
        birim->kusatma_makineleri.hasar_dagilimi = netHasar * (((float)birim->kusatma_makineleri.birimSayi * birim->kusatma_makineleri.savunma) / toplamSavunmaGucu);

    if (birim->okcular.birimSayi > 0)
        birim->okcular.hasar_dagilimi = netHasar * (((float)birim->okcular.birimSayi * birim->okcular.savunma) / toplamSavunmaGucu);

    if (birim->piyadeler.birimSayi > 0)
        birim->piyadeler.hasar_dagilimi = netHasar * (((float)birim->piyadeler.birimSayi * birim->piyadeler.savunma) / toplamSavunmaGucu);

    if (birim->suvariler.birimSayi > 0)
        birim->suvariler.hasar_dagilimi = netHasar * (((float)birim->suvariler.birimSayi * birim->suvariler.savunma) / toplamSavunmaGucu);
}

void olen_hasardagit_insan(struct BirimlerInsan *birim, float birimHasarDagilimi, float kalanSavunmaGucu)
{
    if (birim->kusatma_makineleri.birimSayi > 0)
        birim->kusatma_makineleri.hasar_dagilimi += birimHasarDagilimi * (((float)birim->kusatma_makineleri.birimSayi * birim->kusatma_makineleri.savunma) / kalanSavunmaGucu);

    if (birim->okcular.birimSayi > 0)
        birim->okcular.hasar_dagilimi += birimHasarDagilimi * (((float)birim->okcular.birimSayi * birim->okcular.savunma) / kalanSavunmaGucu);

    if (birim->piyadeler.birimSayi > 0)
        birim->piyadeler.hasar_dagilimi += birimHasarDagilimi * (((float)birim->piyadeler.birimSayi * birim->piyadeler.savunma) / kalanSavunmaGucu);

    if (birim->suvariler.birimSayi > 0)
        birim->suvariler.hasar_dagilimi += birimHasarDagilimi * (((float)birim->suvariler.birimSayi * birim->suvariler.savunma) / kalanSavunmaGucu);
}

void birim_kayip_insan(struct BirimlerInsan *birim, FILE *savas_sim)
{
    int Ksavasta = 0;
    int Osavasta = 0;
    int Psavasta = 0;
    int Ssavasta = 0;

    fprintf(savas_sim,"\n\n\n*****Insan Birim Kayiplari*****\n\n");
    if (birim->kusatma_makineleri.birimSayi > 0)
    {
        Ksavasta = 1;
        fprintf(savas_sim,"\nKusatma Makineleri hasar dagilimi : %f\n",birim->kusatma_makineleri.hasar_dagilimi);
        fprintf(savas_sim,"Kusatma Makineleri onceki sayisi : %d , ",birim->kusatma_makineleri.birimSayi);
        birim->kusatma_makineleri.birimSayi -= birim->kusatma_makineleri.hasar_dagilimi / birim->kusatma_makineleri.saglik;
        fprintf(savas_sim,"sonraki sayisi : %d , olen birim sayisi : %f\n",birim->kusatma_makineleri.birimSayi, birim->kusatma_makineleri.hasar_dagilimi / birim->kusatma_makineleri.saglik);
    }
    if (birim->okcular.birimSayi > 0)
    {
        Osavasta = 1;
        fprintf(savas_sim,"\nOkcular hasar dagilimi : %f\n",birim->okcular.hasar_dagilimi);
        fprintf(savas_sim,"Okcular onceki sayisi : %d , ",birim->okcular.birimSayi);
        birim->okcular.birimSayi -= birim->okcular.hasar_dagilimi / birim->okcular.saglik;
        fprintf(savas_sim,"sonraki sayisi : %d , olen birim sayisi : %f\n",birim->okcular.birimSayi, birim->okcular.hasar_dagilimi / birim->okcular.saglik);
    }
    if (birim->piyadeler.birimSayi > 0)
    {
        Psavasta = 1;
        fprintf(savas_sim,"\nPiyadeler hasar dagilimi : %f\n",birim->piyadeler.hasar_dagilimi);
        fprintf(savas_sim,"Piyadeler onceki sayisi : %d , ",birim->piyadeler.birimSayi);
        birim->piyadeler.birimSayi -= birim->piyadeler.hasar_dagilimi / birim->piyadeler.saglik;
        fprintf(savas_sim,"sonraki sayisi : %d , olen birim sayisi : %f\n",birim->piyadeler.birimSayi, birim->piyadeler.hasar_dagilimi / birim->piyadeler.saglik);
    }
    if (birim->suvariler.birimSayi > 0)
    {
        Ssavasta = 1;
        fprintf(savas_sim,"\nSuvariler hasar dagilimi : %f\n",birim->suvariler.hasar_dagilimi);
        fprintf(savas_sim,"Suvariler onceki sayisi : %d , ",birim->suvariler.birimSayi);
        birim->suvariler.birimSayi -= birim->suvariler.hasar_dagilimi / birim->suvariler.saglik;
        fprintf(savas_sim,"sonraki sayisi : %d , olen birim sayisi : %f\n",birim->suvariler.birimSayi, birim->suvariler.hasar_dagilimi / birim->suvariler.saglik);
    }

    int Kkontrol = 0;
    int Okontrol = 0;
    int Pkontrol = 0;
    int Skontrol = 0;

    if (birim->kusatma_makineleri.birimSayi <= 0 && Kkontrol == 0 && Ksavasta == 1)
    {
        Kkontrol = 1;
        fprintf(savas_sim,"\nKusatma Makineleri oldu.\n");
        birim->kusatma_makineleri.birimSayi = 0;
        olen_hasardagit_insan(birim, birim->kusatma_makineleri.hasar_dagilimi, insanSavunmaHesapla(birim));
        fprintf(savas_sim,"\nOkcu yeni hasar dagilimi : %f\n", birim->okcular.hasar_dagilimi);
        fprintf(savas_sim,"\nPiyade yeni hasar dagilimi : %f\n", birim->piyadeler.hasar_dagilimi);
        fprintf(savas_sim,"\nSuvari yeni hasar dagilimi : %f\n", birim->suvariler.hasar_dagilimi);
    }
    if (birim->okcular.birimSayi <= 0 && Okontrol == 0 && Osavasta == 1)
    {
        Okontrol = 1;
        fprintf(savas_sim,"\nOkcular oldu.\n");
        birim->okcular.birimSayi = 0;
        olen_hasardagit_insan(birim, birim->okcular.hasar_dagilimi, insanSavunmaHesapla(birim));
        fprintf(savas_sim,"\nPiyade yeni hasar dagilimi : %f\n", birim->piyadeler.hasar_dagilimi);
        fprintf(savas_sim,"\nSuvari yeni hasar dagilimi : %f\n", birim->suvariler.hasar_dagilimi);
        fprintf(savas_sim,"\nKusatma mak. yeni hasar dagilimi : %f\n", birim->kusatma_makineleri.hasar_dagilimi);
    }
    if (birim->piyadeler.birimSayi <= 0 && Pkontrol == 0 && Psavasta == 1)
    {
        Pkontrol = 1;
        fprintf(savas_sim,"\nPiyadeler oldu.\n");
        birim->piyadeler.birimSayi = 0;
        olen_hasardagit_insan(birim, birim->piyadeler.hasar_dagilimi, insanSavunmaHesapla(birim));
        fprintf(savas_sim,"\nSuvari yeni hasar dagilimi : %f\n", birim->suvariler.hasar_dagilimi);
        fprintf(savas_sim,"\nKusatma mak. yeni hasar dagilimi : %f\n", birim->kusatma_makineleri.hasar_dagilimi);
        fprintf(savas_sim,"\nOkcu yeni hasar dagilimi : %f\n", birim->okcular.hasar_dagilimi);
    }
    if (birim->suvariler.birimSayi <= 0 && Skontrol == 0 && Ssavasta == 1)
    {
        Skontrol = 1;
        fprintf(savas_sim,"\nSuvariler oldu.\n");
        birim->suvariler.birimSayi = 0;
        olen_hasardagit_insan(birim, birim->suvariler.hasar_dagilimi, insanSavunmaHesapla(birim));
        fprintf(savas_sim,"\nKusatma mak. yeni hasar dagilimi : %f\n", birim->kusatma_makineleri.hasar_dagilimi);
        fprintf(savas_sim,"\nOkcu yeni hasar dagilimi : %f\n", birim->okcular.hasar_dagilimi);
        fprintf(savas_sim,"\nPiyade yeni hasar dagilimi : %f\n", birim->piyadeler.hasar_dagilimi);
    }
}

void saglik_durumu_insan(struct BirimlerInsan *birim, FILE *savas_sim)
{
    fprintf(savas_sim,"\n\n++++Insan Imparatorlugu Saglik Durumlari++++\n\n");
    if (birim->kusatma_makineleri.birimSayi > 0)
    {
        fprintf(savas_sim,"\nKusatma Makineleri onceki sagligi : %f , ",birim->kusatma_makineleri.saglik);
        birim->kusatma_makineleri.saglik -= birim->kusatma_makineleri.hasar_dagilimi / (float)birim->kusatma_makineleri.birimSayi;
        fprintf(savas_sim,"sonraki sagligi : %f\n",birim->kusatma_makineleri.saglik);
        if(birim->kusatma_makineleri.saglik <= 0)
        {
            fprintf(savas_sim,"\nKusatma Makineleri sagligi 0 yada altina dustugu icin oldu!!!\n");
            birim->kusatma_makineleri.birimSayi = 0;
            birim->kusatma_makineleri.saglik = 0;
        }
    }
    if (birim->okcular.birimSayi > 0)
    {
        fprintf(savas_sim,"\nOkcularin onceki sagligi : %f , ",birim->okcular.saglik);
        birim->okcular.saglik -= birim->okcular.hasar_dagilimi / (float)birim->okcular.birimSayi;
        fprintf(savas_sim,"sonraki sagligi : %f\n",birim->okcular.saglik);
        if(birim->okcular.saglik <= 0)
        {
            fprintf(savas_sim,"\nOkcu sagligi 0 yada altina dustugu icin oldu!!!\n");
            birim->okcular.birimSayi = 0;
            birim->okcular.saglik = 0;
        }
    }
    if (birim->piyadeler.birimSayi > 0)
    {
        fprintf(savas_sim,"\nPiyadelerin onceki sagligi : %f , ",birim->piyadeler.saglik);
        birim->piyadeler.saglik -= birim->piyadeler.hasar_dagilimi / (float)birim->piyadeler.birimSayi;
        fprintf(savas_sim,"sonraki sagligi : %f\n",birim->piyadeler.saglik);
        if(birim->piyadeler.saglik <= 0)
        {
            fprintf(savas_sim,"\nPiyade sagligi 0 yada altina dustugu icin oldu!!!\n");
            birim->piyadeler.birimSayi = 0;
            birim->piyadeler.saglik = 0;
        }
    }
    if (birim->suvariler.birimSayi > 0)
    {
        fprintf(savas_sim,"\nSuvarin onceki sagligi : %f , ",birim->suvariler.saglik);
        birim->suvariler.saglik -= birim->suvariler.hasar_dagilimi / (float)birim->suvariler.birimSayi;
        fprintf(savas_sim,"sonraki sagligi : %f\n",birim->suvariler.saglik);
        if(birim->suvariler.saglik <= 0)
        {
            fprintf(savas_sim,"\nSuvari sagligi 0 yada altina dustugu icin oldu!!!\n");
            birim->suvariler.birimSayi = 0;
            birim->suvariler.saglik = 0;
        }
    }
    if(birim->suvariler.saglik == 0 && birim->piyadeler.saglik == 0 && birim->okcular.saglik == 0 &&birim->kusatma_makineleri.saglik == 0)
        fprintf(savas_sim,"\nButun birimlerin sagligi 0 oldu, hepsi oldu.");

}

void birim_guncelle_insan(struct BirimlerInsan *birim, float netHasar, float toplamSavunmaGucu, FILE *savas_sim)
{
    hasar_dagilimi_insan(birim, netHasar, toplamSavunmaGucu);
    birim_kayip_insan(birim, savas_sim);
    saglik_durumu_insan(birim, savas_sim);
}


void yorgunluk_uygula(struct BirimlerInsan *birimInsan, struct BirimlerOrg *birimOrk)
{
    birimInsan->kusatma_makineleri.saldiri *= 0.9;
    birimInsan->okcular.saldiri *= 0.9;
    birimInsan->piyadeler.saldiri *= 0.9;
    birimInsan->suvariler.saldiri *= 0.9;
    birimInsan->kusatma_makineleri.savunma *= 0.9;
    birimInsan->okcular.savunma *= 0.9;
    birimInsan->piyadeler.savunma *= 0.9;
    birimInsan->suvariler.savunma *= 0.9;

    birimOrk->mizrakcilar.saldiri *= 0.9;
    birimOrk->ork_dovusculeri.saldiri *= 0.9;
    birimOrk->troller.saldiri *= 0.9;
    birimOrk->varg_binicileri.saldiri *= 0.9;
    birimOrk->mizrakcilar.savunma *= 0.9;
    birimOrk->ork_dovusculeri.savunma *= 0.9;
    birimOrk->troller.savunma *= 0.9;
    birimOrk->varg_binicileri.savunma *= 0.9;
}

float kritik_hasar_hesapla(float normal_saldiri, float kritik_sans, int saldiri_sayisi, int birim_bil, FILE *savas_sim)
{
    if ((kritik_sans > 0  && kritik_sans <= 100) && saldiri_sayisi % (int)(100 / kritik_sans) == 0)
    {
        if(birim_bil == 1)
            fprintf(savas_sim,"\n\n\n%%%.0f kritik sansli Kusatma Makinesinin %.0f saldiri gucu %%50 artmistir.", kritik_sans, normal_saldiri);
        else if(birim_bil == 2)
            fprintf(savas_sim,"\n\n\n%%%.0f kritik sansli Okcunun %.0f saldiri gucu %%50 artmistir.", kritik_sans, normal_saldiri);
        else if(birim_bil == 3)
            fprintf(savas_sim,"\n\n\n%%%.0f kritik sansli Piyadenin %.0f saldiri gucu %%50 artmistir.", kritik_sans, normal_saldiri);
        else if(birim_bil == 4)
            fprintf(savas_sim,"\n\n\n%%%.0f kritik sansli Suvarinin %.0f saldiri gucu %%50 artmistir.", kritik_sans, normal_saldiri);
        else if(birim_bil == 5)
            fprintf(savas_sim,"\n\n\n%%%.0f kritik sansli Mizrakcinin %.0f saldiri gucu %%50 artmistir.", kritik_sans, normal_saldiri);
        else if(birim_bil == 6)
            fprintf(savas_sim,"\n\n\n%%%.0f kritik sansli Ork Dovusculerinin %.0f saldiri gucu %%50 artmistir.", kritik_sans, normal_saldiri);
        else if(birim_bil == 7)
            fprintf(savas_sim,"\n\n\n%%%.0f kritik sansli Trollerin %.0f saldiri gucu %%50 artmistir.", kritik_sans, normal_saldiri);
        else if(birim_bil == 8)
            fprintf(savas_sim,"\n\n\n%%%.0f kritik sansli Varg Binicilerinin %.0f saldiri gucu %%50 artmistir.", kritik_sans, normal_saldiri);

        return normal_saldiri * 1.5;
    }
    return normal_saldiri;
}

float insanSaldiriHesapla(struct BirimlerInsan *birim, int saldiri_sayisi, FILE *savas_sim)
{
    float kusatma_makineleri_saldiri = kritik_hasar_hesapla(birim->kusatma_makineleri.saldiri, birim->kusatma_makineleri.kritik_sans, saldiri_sayisi, 1, savas_sim);
    float okcular_saldiri = kritik_hasar_hesapla(birim->okcular.saldiri, birim->okcular.kritik_sans, saldiri_sayisi, 2, savas_sim);
    float piyadeler_saldiri = kritik_hasar_hesapla(birim->piyadeler.saldiri, birim->piyadeler.kritik_sans, saldiri_sayisi, 3, savas_sim);
    float suvariler_saldiri = kritik_hasar_hesapla(birim->suvariler.saldiri, birim->suvariler.kritik_sans, saldiri_sayisi, 4, savas_sim);

    float insan_saldiri_gucu =
        (float)birim->kusatma_makineleri.birimSayi * kusatma_makineleri_saldiri +
        (float)birim->okcular.birimSayi * okcular_saldiri +
        (float)birim->piyadeler.birimSayi * piyadeler_saldiri +
        (float)birim->suvariler.birimSayi * suvariler_saldiri;

    return insan_saldiri_gucu;
}

float insanSavunmaHesapla(struct BirimlerInsan *birim)
{

    float insan_savunma_gucu =  (float)birim->kusatma_makineleri.birimSayi * birim->kusatma_makineleri.savunma +
                                (float)birim->okcular.birimSayi * birim->okcular.savunma +
                                (float)birim->piyadeler.birimSayi * birim->piyadeler.savunma +
                                (float)birim->suvariler.birimSayi * birim->suvariler.savunma;

    return insan_savunma_gucu;
}

float orkSaldiriHesapla(struct BirimlerOrg *birim, int saldiri_sayisi, FILE *savas_sim)
{

    float mizrakcilar_saldiri = kritik_hasar_hesapla(birim->mizrakcilar.saldiri, birim->mizrakcilar.kritik_sans, saldiri_sayisi, 5, savas_sim);
    float ork_dovusculeri_saldiri = kritik_hasar_hesapla(birim->ork_dovusculeri.saldiri, birim->ork_dovusculeri.kritik_sans, saldiri_sayisi, 6, savas_sim);
    float troller_saldiri = kritik_hasar_hesapla(birim->troller.saldiri, birim->troller.kritik_sans, saldiri_sayisi, 7, savas_sim);
    float varg_binicileri_saldiri = kritik_hasar_hesapla(birim->varg_binicileri.saldiri, birim->varg_binicileri.kritik_sans, saldiri_sayisi, 8, savas_sim);

    float ork_saldiri_gucu =
        (float)birim->ork_dovusculeri.birimSayi * ork_dovusculeri_saldiri +
        (float)birim->mizrakcilar.birimSayi * mizrakcilar_saldiri +
        (float)birim->varg_binicileri.birimSayi * varg_binicileri_saldiri +
        (float)birim->troller.birimSayi * troller_saldiri;

    return ork_saldiri_gucu;
}

float orkSavunmaHesapla(struct BirimlerOrg *birim)
{

    float ork_savunma_gucu =  (float)birim->mizrakcilar.birimSayi * birim->mizrakcilar.savunma +
                              (float)birim->ork_dovusculeri.birimSayi * birim->ork_dovusculeri.savunma +
                              (float)birim->troller.birimSayi * birim->troller.savunma +
                              (float)birim->varg_binicileri.birimSayi * birim->varg_binicileri.savunma;

    return ork_savunma_gucu;
}


size_t dosyaYaz(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

int senaryoIndirme()
{
    CURL *curl;
    FILE *dosya;
    CURLcode sonuc;
    int i = 0;

    const char url[10][80] = {"https://yapbenzet.org.tr/1.json", "https://yapbenzet.org.tr/2.json", "https://yapbenzet.org.tr/3.json",
                              "https://yapbenzet.org.tr/4.json", "https://yapbenzet.org.tr/5.json", "https://yapbenzet.org.tr/6.json", "https://yapbenzet.org.tr/7.json",
                              "https://yapbenzet.org.tr/8.json", "https://yapbenzet.org.tr/9.json", "https://yapbenzet.org.tr/10.json"
                             };
    char *outfilename;

    do
    {
        printf("Kacinci linki indirmek istiyorsunuz : ");
        scanf("%d", &i);
        if(i > 10 || i < 1)
            printf("Lutfen 1-10 arasindaki sayiyi giriniz!!!\n\n");
    }
    while(i > 10 || i < 1);

    outfilename = "./Files/senaryo.json";

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        dosya = fopen(outfilename, "wb");
        if (!dosya)
        {
            printf("Dosya acilamadi!");
            return 0;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url[i - 1]);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dosyaYaz);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, dosya);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        sonuc = curl_easy_perform(curl);

        if (sonuc != CURLE_OK)
        {
            printf("Hata link indirilemedi!\n");
            return 0;
        }

        fclose(dosya);
        curl_easy_cleanup(curl);
    }
    else
    {
        printf("cURL baslatilamadi.\n");
        return 0;
    }

    curl_global_cleanup();
    return 1;
}

void dosyaOku(struct Savasanlar *s, char *ordu, char *birimAdi)
{
    char calismaDizini[200];
    char *dosyaYolu;
    char *dosyaAdi;


    if(!strcmp(ordu,"birimler"))
    {
        getcwd(calismaDizini, sizeof(calismaDizini));
        dosyaYolu = strcat(calismaDizini, "\\Files\\unit_types.json");
        if (access(dosyaYolu, F_OK) != 0)
        {
            printf("Dosya '%s' bulunamadi. Lutfen dosyanin dogru isimde ve dogru dizinde oldugundan emin olun.", dosyaYolu);
            return;
        }

        dosyaAdi = "Files\\unit_types.json";
        birimAyristir(s, dosyaAdi, birimAdi);
    }


    else if(!strcmp(ordu,"kahramanlar"))
    {
        getcwd(calismaDizini, sizeof(calismaDizini));
        dosyaYolu = strcat(calismaDizini, "\\Files\\heroes.json");
        if (access(dosyaYolu, F_OK) != 0)
        {
            printf("Dosya '%s' bulunamadi. Lutfen dosyanin dogru isimde ve dogru dizinde oldugundan emin olun.", dosyaYolu);
            return;
        }
        dosyaAdi = "Files\\heroes.json";
        kahramanAyristir(s, dosyaAdi, birimAdi);
    }


    else if(!strcmp(ordu,"canavarlar"))
    {
        getcwd(calismaDizini, sizeof(calismaDizini));
        dosyaYolu = strcat(calismaDizini, "\\Files\\creatures.json");
        if (access(dosyaYolu, F_OK) != 0)
        {
            printf("Dosya '%s' bulunamadi. Lutfen dosyanin dogru isimde ve dogru dizinde oldugundan emin olun.", dosyaYolu);
            return;
        }
        dosyaAdi = "Files\\creatures.json";
        canavarAyristir(s, dosyaAdi, birimAdi);
    }

}

void birimAtama(struct Savasanlar *s, char *okunan_satir, char *birimAdi, int i)
{
    float deger;
    sscanf(okunan_satir, "%*s %f", &deger);

    if (!strcmp(birimAdi, "piyadeler"))
    {
        if (i == 1)
            s->insanImparatorlugu.birimler.piyadeler.saldiri = deger;
        else if (i == 2)
            s->insanImparatorlugu.birimler.piyadeler.savunma = deger;
        else if (i == 3)
            s->insanImparatorlugu.birimler.piyadeler.saglik = deger;
        else if (i == 4)
            s->insanImparatorlugu.birimler.piyadeler.kritik_sans = deger;
    }
    else if (!strcmp(birimAdi, "okcular"))
    {
        if (i == 1)
            s->insanImparatorlugu.birimler.okcular.saldiri = deger;
        else if (i == 2)
            s->insanImparatorlugu.birimler.okcular.savunma = deger;
        else if (i == 3)
            s->insanImparatorlugu.birimler.okcular.saglik = deger;
        else if (i == 4)
            s->insanImparatorlugu.birimler.okcular.kritik_sans = deger;
    }
    else if (!strcmp(birimAdi, "suvariler"))
    {
        if (i == 1)
            s->insanImparatorlugu.birimler.suvariler.saldiri = deger;
        else if (i == 2)
            s->insanImparatorlugu.birimler.suvariler.savunma = deger;
        else if (i == 3)
            s->insanImparatorlugu.birimler.suvariler.saglik = deger;
        else if (i == 4)
            s->insanImparatorlugu.birimler.suvariler.kritik_sans = deger;
    }
    else if (!strcmp(birimAdi, "kusatma_makineleri"))
    {
        if (i == 1)
            s->insanImparatorlugu.birimler.kusatma_makineleri.saldiri = deger;
        else if (i == 2)
            s->insanImparatorlugu.birimler.kusatma_makineleri.savunma = deger;
        else if (i == 3)
            s->insanImparatorlugu.birimler.kusatma_makineleri.saglik = deger;
        else if (i == 4)
            s->insanImparatorlugu.birimler.kusatma_makineleri.kritik_sans = deger;
    }
    else if (!strcmp(birimAdi, "ork_dovusculeri"))
    {
        if (i == 1)
            s->orkLegi.birimler.ork_dovusculeri.saldiri = deger;
        else if (i == 2)
            s->orkLegi.birimler.ork_dovusculeri.savunma = deger;
        else if (i == 3)
            s->orkLegi.birimler.ork_dovusculeri.saglik = deger;
        else if (i == 4)
            s->orkLegi.birimler.ork_dovusculeri.kritik_sans = deger;
    }
    else if (!strcmp(birimAdi, "troller"))
    {
        if (i == 1)
            s->orkLegi.birimler.troller.saldiri = deger;
        else if (i == 2)
            s->orkLegi.birimler.troller.savunma = deger;
        else if (i == 3)
            s->orkLegi.birimler.troller.saglik = deger;
        else if (i == 4)
            s->orkLegi.birimler.troller.kritik_sans = deger;
    }
    else if (!strcmp(birimAdi, "mizrakcilar"))
    {
        if (i == 1)
            s->orkLegi.birimler.mizrakcilar.saldiri = deger;
        else if (i == 2)
            s->orkLegi.birimler.mizrakcilar.savunma = deger;
        else if (i == 3)
            s->orkLegi.birimler.mizrakcilar.saglik = deger;
        else if (i == 4)
            s->orkLegi.birimler.mizrakcilar.kritik_sans = deger;
    }
    else if (!strcmp(birimAdi, "varg_binicileri"))
    {
        if (i == 1)
            s->orkLegi.birimler.varg_binicileri.saldiri = deger;
        else if (i == 2)
            s->orkLegi.birimler.varg_binicileri.savunma = deger;
        else if (i == 3)
            s->orkLegi.birimler.varg_binicileri.saglik = deger;
        else if (i == 4)
            s->orkLegi.birimler.varg_binicileri.kritik_sans = deger;
    }
}

void KahramanEtkiledigiBirimAta(struct KahramanOzellikler *kahraman, char *okunan_satir)
{
    if (strstr(okunan_satir, "piyade"))
        strcpy(kahraman->etkiledigi_birim, "piyade");
    else if (strstr(okunan_satir, "okcu"))
        strcpy(kahraman->etkiledigi_birim, "okcu");
    else if (strstr(okunan_satir, "kusatma_makineleri"))
        strcpy(kahraman->etkiledigi_birim, "kusatma_makinesi");
    else if (strstr(okunan_satir, "suvari"))
        strcpy(kahraman->etkiledigi_birim, "suvari");
    else if (strstr(okunan_satir, "ork_dovusculeri"))
        strcpy(kahraman->etkiledigi_birim, "ork_dovusculeri");
    else if (strstr(okunan_satir, "troller"))
        strcpy(kahraman->etkiledigi_birim, "troller");
    else if (strstr(okunan_satir, "varg_binicileri"))
        strcpy(kahraman->etkiledigi_birim, "varg_binicileri");
    else if (strstr(okunan_satir, "mizrakci"))
        strcpy(kahraman->etkiledigi_birim, "mizrakcilar");
    else if (strstr(okunan_satir, "tum_birimlere"))
        strcpy(kahraman->etkiledigi_birim, "tum_birimlere");
}

void kahramanAtama(struct Savasanlar *s, char *okunan_satir, char *birimAdi, int i)
{
    char bos[200];
    char bonus_tur[100];
    float deger;

    struct Savasanlar *kahraman;

    if (i == 1)
    {
        sscanf(okunan_satir, " \"%[^\"]\" : \"%[^\"]\" ", bos, bonus_tur);
        if (!strcmp(birimAdi, "Alparslan"))
            strcpy(s->insanImparatorlugu.kahramanlar.Alparslan.bonus_turu, bonus_tur);
        else if (!strcmp(birimAdi, "Fatih_Sultan_Mehmet"))
            strcpy(s->insanImparatorlugu.kahramanlar.Fatih_Sultan_Mehmet.bonus_turu, bonus_tur);
        else if (!strcmp(birimAdi, "Mete_Han"))
            strcpy(s->insanImparatorlugu.kahramanlar.Mete_Han.bonus_turu, bonus_tur);
        else if (!strcmp(birimAdi, "Yavuz_Sultan_Selim"))
            strcpy(s->insanImparatorlugu.kahramanlar.Yavuz_Sultan_Selim.bonus_turu, bonus_tur);
        else if (!strcmp(birimAdi, "Tugrul_Bey"))
            strcpy(s->insanImparatorlugu.kahramanlar.Tugrul_Bey.bonus_turu, bonus_tur);
        else if (!strcmp(birimAdi, "Goruk_Vahsi"))
            strcpy(s->orkLegi.kahramanlar.Goruk_Vahsi.bonus_turu, bonus_tur);
        else if (!strcmp(birimAdi, "Thruk_Kemikkiran"))
            strcpy(s->orkLegi.kahramanlar.Thruk_Kemikkiran.bonus_turu, bonus_tur);
        else if (!strcmp(birimAdi, "Vrog_Kafakiran"))
            strcpy(s->orkLegi.kahramanlar.Vrog_Kafakiran.bonus_turu, bonus_tur);
        else if (!strcmp(birimAdi, "Ugar_Zalim"))
            strcpy(s->orkLegi.kahramanlar.Ugar_Zalim.bonus_turu, bonus_tur);
    }
    else if (i == 2)
    {
        sscanf(okunan_satir, " \"%*[^\"]\": \"%f\"", &deger);
        if (!strcmp(birimAdi, "Alparslan"))
            s->insanImparatorlugu.kahramanlar.Alparslan.bonus_degeri = deger;
        else if (!strcmp(birimAdi, "Fatih_Sultan_Mehmet"))
            s->insanImparatorlugu.kahramanlar.Fatih_Sultan_Mehmet.bonus_degeri = deger;
        else if (!strcmp(birimAdi, "Mete_Han"))
            s->insanImparatorlugu.kahramanlar.Mete_Han.bonus_degeri = deger;
        else if (!strcmp(birimAdi, "Yavuz_Sultan_Selim"))
            s->insanImparatorlugu.kahramanlar.Yavuz_Sultan_Selim.bonus_degeri = deger;
        else if (!strcmp(birimAdi, "Tugrul_Bey"))
            s->insanImparatorlugu.kahramanlar.Tugrul_Bey.bonus_degeri = deger;
        else if (!strcmp(birimAdi, "Goruk_Vahsi"))
            s->orkLegi.kahramanlar.Goruk_Vahsi.bonus_degeri = deger;
        else if (!strcmp(birimAdi, "Thruk_Kemikkiran"))
            s->orkLegi.kahramanlar.Thruk_Kemikkiran.bonus_degeri = deger;
        else if (!strcmp(birimAdi, "Vrog_Kafakiran"))
            s->orkLegi.kahramanlar.Vrog_Kafakiran.bonus_degeri = deger;
        else if (!strcmp(birimAdi, "Ugar_Zalim"))
            s->orkLegi.kahramanlar.Ugar_Zalim.bonus_degeri = deger;
    }
    else if (i == 3)
    {
        if (!strcmp(birimAdi, "Alparslan"))
        {
            kahraman = &s->insanImparatorlugu.kahramanlar.Alparslan;
            KahramanEtkiledigiBirimAta(kahraman, okunan_satir);
        }
        else if (!strcmp(birimAdi, "Fatih_Sultan_Mehmet"))
        {
            kahraman = &s->insanImparatorlugu.kahramanlar.Fatih_Sultan_Mehmet;
            KahramanEtkiledigiBirimAta(kahraman, okunan_satir);
        }
        else if (!strcmp(birimAdi, "Mete_Han"))
        {
            kahraman = &s->insanImparatorlugu.kahramanlar.Mete_Han;
            KahramanEtkiledigiBirimAta(kahraman, okunan_satir);
        }
        else if (!strcmp(birimAdi, "Yavuz_Sultan_Selim"))
        {
            kahraman = &s->insanImparatorlugu.kahramanlar.Yavuz_Sultan_Selim;
            KahramanEtkiledigiBirimAta(kahraman, okunan_satir);
        }
        else if (!strcmp(birimAdi, "Tugrul_Bey"))
        {
            kahraman = &s->insanImparatorlugu.kahramanlar.Tugrul_Bey;
            KahramanEtkiledigiBirimAta(kahraman, okunan_satir);
        }
        else if (!strcmp(birimAdi, "Goruk_Vahsi"))
        {
            kahraman = &s->orkLegi.kahramanlar.Goruk_Vahsi;
            KahramanEtkiledigiBirimAta(kahraman, okunan_satir);
        }
        else if (!strcmp(birimAdi, "Thruk_Kemikkiran"))
        {
            kahraman = &s->orkLegi.kahramanlar.Thruk_Kemikkiran;
            KahramanEtkiledigiBirimAta(kahraman, okunan_satir);
        }
        else if (!strcmp(birimAdi, "Vrog_Kafakiran"))
        {
            kahraman = &s->orkLegi.kahramanlar.Vrog_Kafakiran;
            KahramanEtkiledigiBirimAta(kahraman, okunan_satir);
        }
        else if (!strcmp(birimAdi, "Ugar_Zalim"))
        {
            kahraman = &s->orkLegi.kahramanlar.Ugar_Zalim;
            KahramanEtkiledigiBirimAta(kahraman, okunan_satir);
        }
    }
}

void canavarEtkiledigiBirimAta(struct CanavarOzellikler *canavar, char *okunan_satir)
{
    if (strstr(okunan_satir, "piyade"))
        strcpy(canavar->etkiledigi_birim, "piyade");
    else if (strstr(okunan_satir, "okcu"))
        strcpy(canavar->etkiledigi_birim, "okcu");
    else if (strstr(okunan_satir, "kusatma_makineleri"))
        strcpy(canavar->etkiledigi_birim, "kusatma_makinesi");
    else if (strstr(okunan_satir, "suvari"))
        strcpy(canavar->etkiledigi_birim, "suvari");
    else if (strstr(okunan_satir, "ork_dovusculeri"))
        strcpy(canavar->etkiledigi_birim, "ork_dovusculeri");
    else if (strstr(okunan_satir, "troller"))
        strcpy(canavar->etkiledigi_birim, "troller");
    else if (strstr(okunan_satir, "varg_binicileri"))
        strcpy(canavar->etkiledigi_birim, "varg_binicileri");
    else if (strstr(okunan_satir, "mizrakci"))
        strcpy(canavar->etkiledigi_birim, "mizrakcilar");
    else if (strstr(okunan_satir, "tum_birimlere"))
        strcpy(canavar->etkiledigi_birim, "tum_birimlere");
}

void canavarAtama(struct Savasanlar *s, char *okunan_satir, char *birimAd, int i)
{
    char bos[200];
    char etki_tur[100];
    float deger;

    struct Savasanlar *canavar;

    if (i == 1)
    {
        sscanf(okunan_satir, "%*[^0-9]%f", &deger);
        if (!strcmp(birimAd, "Ejderha"))
            s->insanImparatorlugu.canavarlar.Ejderha.etki_degeri = deger;

        else if (!strcmp(birimAd, "Agri_Dagi_Devleri"))
            s->insanImparatorlugu.canavarlar.Agri_Dagi_Devleri.etki_degeri = deger;

        else if (!strcmp(birimAd, "Tepegoz"))
            s->insanImparatorlugu.canavarlar.Tepegoz.etki_degeri = deger;

        else if (!strcmp(birimAd, "Karakurt"))
            s->insanImparatorlugu.canavarlar.Karakurt.etki_degeri = deger;

        else if (!strcmp(birimAd, "Samur"))
            s->insanImparatorlugu.canavarlar.Samur.etki_degeri = deger;

        else if (!strcmp(birimAd, "Kara_Troll"))
            s->orkLegi.canavarlar.Kara_Troll.etki_degeri = deger;

        else if (!strcmp(birimAd, "Golge_Kurtlari"))
            s->orkLegi.canavarlar.Golge_Kurtlari.etki_degeri = deger;

        else if (!strcmp(birimAd, "Camur_Devleri"))
            s->orkLegi.canavarlar.Camur_Devleri.etki_degeri = deger;

        else if (!strcmp(birimAd, "Ates_Iblisi"))
            s->orkLegi.canavarlar.Ates_Iblisi.etki_degeri = deger;

        else if (!strcmp(birimAd, "Makrog_Savas_Beyi"))
            s->orkLegi.canavarlar.Makrog_Savas_Beyi.etki_degeri = deger;

        else if (!strcmp(birimAd, "Buz_Devleri"))
            s->orkLegi.canavarlar.Buz_Devleri.etki_degeri = deger;
    }
    else if (i == 2)
    {
        sscanf(okunan_satir, " \"%[^\"]\" : \"%[^\"]\" ", bos, etki_tur);
        if (!strcmp(birimAd, "Ejderha"))
            strcpy(s->insanImparatorlugu.canavarlar.Ejderha.etki_turu, etki_tur);
        else if (!strcmp(birimAd, "Agri_Dagi_Devleri"))
            strcpy(s->insanImparatorlugu.canavarlar.Agri_Dagi_Devleri.etki_turu, etki_tur);
        else if (!strcmp(birimAd, "Tepegoz"))
            strcpy(s->insanImparatorlugu.canavarlar.Tepegoz.etki_turu, etki_tur);
        else if (!strcmp(birimAd, "Karakurt"))
            strcpy(s->insanImparatorlugu.canavarlar.Karakurt.etki_turu, etki_tur);
        else if (!strcmp(birimAd, "Samur"))
            strcpy(s->insanImparatorlugu.canavarlar.Samur.etki_turu, etki_tur);
        else if (!strcmp(birimAd, "Kara_Troll"))
            strcpy(s->orkLegi.canavarlar.Kara_Troll.etki_turu, etki_tur);
        else if (!strcmp(birimAd, "Golge_Kurtlari"))
            strcpy(s->orkLegi.canavarlar.Golge_Kurtlari.etki_turu, etki_tur);
        else if (!strcmp(birimAd, "Camur_Devleri"))
            strcpy(s->orkLegi.canavarlar.Camur_Devleri.etki_turu, etki_tur);
        else if (!strcmp(birimAd, "Ates_Iblisi"))
            strcpy(s->orkLegi.canavarlar.Ates_Iblisi.etki_turu, etki_tur);
        else if (!strcmp(birimAd, "Makrog_Savas_Beyi"))
            strcpy(s->orkLegi.canavarlar.Makrog_Savas_Beyi.etki_turu, etki_tur);
        else if (!strcmp(birimAd, "Buz_Devleri"))
            strcpy(s->orkLegi.canavarlar.Buz_Devleri.etki_turu, etki_tur);
    }
    else if (i == 3)
    {
        if (!strcmp(birimAd, "Ejderha"))
        {
            canavar = &s->insanImparatorlugu.canavarlar.Ejderha;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Agri_Dagi_Devleri"))
        {
            canavar = &s->insanImparatorlugu.canavarlar.Agri_Dagi_Devleri;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Tepegoz"))
        {
            canavar = &s->insanImparatorlugu.canavarlar.Tepegoz;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Karakurt"))
        {
            canavar = &s->insanImparatorlugu.canavarlar.Karakurt;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Samur"))
        {
            canavar = &s->insanImparatorlugu.canavarlar.Samur;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Kara_Troll"))
        {
            canavar = &s->orkLegi.canavarlar.Kara_Troll;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Golge_Kurtlari"))
        {
            canavar = &s->orkLegi.canavarlar.Golge_Kurtlari;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Camur_Devleri"))
        {
            canavar = &s->orkLegi.canavarlar.Camur_Devleri;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Ates_Iblisi"))
        {
            canavar = &s->orkLegi.canavarlar.Ates_Iblisi;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Makrog_Savas_Beyi"))
        {
            canavar = &s->orkLegi.canavarlar.Makrog_Savas_Beyi;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Buz_Devleri"))
        {
            canavar = &s->orkLegi.canavarlar.Buz_Devleri;
            canavarEtkiledigiBirimAta(canavar, okunan_satir);
        }
    }

}

void birimAyristir(struct Savasanlar *s, char *dosyaAdi, char *birimAd)
{
    char ayrisan[200];
    char okunan_satir[100];
    FILE *dosya;
    dosya = fopen(dosyaAdi, "r");

    if (dosya == NULL)
    {
        printf("\ndosya acilamadi");
        return;
    }

    while (fgets(ayrisan, 200, dosya))
    {
        if (strstr(ayrisan, birimAd))
        {
            while (fgets(okunan_satir, 100, dosya) && !strchr(okunan_satir, '}'))
            {
                if (strstr(okunan_satir, "saldiri"))
                    birimAtama(s, okunan_satir, birimAd, 1);
                else if (strstr(okunan_satir, "savunma"))
                    birimAtama(s, okunan_satir, birimAd, 2);
                else if (strstr(okunan_satir, "saglik"))
                    birimAtama(s, okunan_satir, birimAd, 3);
                else if (strstr(okunan_satir, "kritik_sans"))
                    birimAtama(s, okunan_satir, birimAd, 4);
            }
        }
    }

    fclose(dosya);
}

void kahramanAyristir(struct Savasanlar *s, char *dosyaAdi, char *birimAd)
{
    char ayrisan[200];
    char okunan_satir[100];

    FILE *dosya;
    dosya = fopen(dosyaAdi, "r");

    if (dosya == NULL)
    {
        printf("\ndosya acilamadi");
        return;
    }

    while (fgets(ayrisan, 200, dosya))
    {
        if (strstr(ayrisan, birimAd))
        {
            while (fgets(okunan_satir, 100, dosya) && !strchr(okunan_satir, '}'))
            {
                if (strstr(okunan_satir, "bonus_turu"))
                    kahramanAtama(s, okunan_satir, birimAd, 1);
                else if (strstr(okunan_satir, "bonus_degeri"))
                    kahramanAtama(s, okunan_satir, birimAd, 2);
                else if (strstr(okunan_satir, "aciklama"))
                    kahramanAtama(s, okunan_satir, birimAd, 3);
            }
        }
    }

    fclose(dosya);
}

void canavarAyristir(struct Savasanlar *s, char *dosyaAdi, char *birimAd)
{
    char ayrisan[200];
    char okunan_satir[100];

    FILE *dosya;
    dosya = fopen(dosyaAdi, "r");

    if (dosya == NULL)
    {
        printf("\ndosya acilamadi");
        return;
    }

    while (fgets(ayrisan, 200, dosya))
    {
        if (strstr(ayrisan, birimAd))
        {
            while (fgets(okunan_satir, 100, dosya) && !strchr(okunan_satir, '}'))
            {
                if (strstr(okunan_satir, "etki_degeri"))
                    canavarAtama(s, okunan_satir, birimAd, 1);
                else if (strstr(okunan_satir, "etki_turu"))
                    canavarAtama(s, okunan_satir, birimAd, 2);
                else if (strstr(okunan_satir, "aciklama"))
                    canavarAtama(s, okunan_satir, birimAd, 3);
            }
        }
    }

    fclose(dosya);
}

void arastirmaEtkiledigiBirimAta(struct ArastirmaOzellikler *arastir, char *okunan_satir)
{
    if (strstr(okunan_satir, "piyade"))
        strcpy(arastir->etkiledigi_birim, "piyade");
    else if (strstr(okunan_satir, "okcu"))
        strcpy(arastir->etkiledigi_birim, "okcu");
    else if (strstr(okunan_satir, "kusatma_makineleri"))
        strcpy(arastir->etkiledigi_birim, "kusatma_makineleri");
    else if (strstr(okunan_satir, "suvari"))
        strcpy(arastir->etkiledigi_birim, "suvari");
    else if (strstr(okunan_satir, "ork_dovusculeri"))
        strcpy(arastir->etkiledigi_birim, "ork_dovusculeri");
    else if (strstr(okunan_satir, "troller"))
        strcpy(arastir->etkiledigi_birim, "troller");
    else if (strstr(okunan_satir, "varg_binicileri"))
        strcpy(arastir->etkiledigi_birim, "varg_binicileri");
    else if (strstr(okunan_satir, "mizrakci"))
        strcpy(arastir->etkiledigi_birim, "mizrakci");
    else if (strstr(okunan_satir, "tum_birimler"))
        strcpy(arastir->etkiledigi_birim, "tum_birimler");

    if(strstr(okunan_satir, "savunma"))
        strcpy(arastir->etki_turu, "savunma");
    else if(strstr(okunan_satir, "saldiri"))
        strcpy(arastir->etki_turu, "saldiri");
    else if(strstr(okunan_satir, "kritik"))
        strcpy(arastir->etki_turu, "kritik_sans");
}

void arastirmaAtama(struct Savasanlar *s, char *okunan_satir, char *birimAd, char *grup, int seviye, int i)
{
    float deger;

    struct ArastirmaOzellikler *arastir;

    if(i == 5)
        sscanf(okunan_satir, "%*[^0-9]%f", &deger);
    if (!strcmp(birimAd, "savunma_ustaligi"))
    {
        if (seviye == 1)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_1.savunma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_1.savunma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
        else if (seviye == 2)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_2.savunma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_2.savunma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
        else if (seviye == 3)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_3.savunma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_3.savunma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
    }
    else if (!strcmp(birimAd, "saldiri_gelistirmesi"))
    {
        if (seviye == 1)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_1.saldiri_gelistirmesi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_1.saldiri_gelistirmesi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
        else if (seviye == 2)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_2.saldiri_gelistirmesi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_2.saldiri_gelistirmesi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
        else if (seviye == 3)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_3.saldiri_gelistirmesi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_3.saldiri_gelistirmesi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
    }
    else if (!strcmp(birimAd, "elit_egitim"))
    {
        if (seviye == 1)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_1.elit_egitim;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_1.elit_egitim;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
        else if (seviye == 2)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_2.elit_egitim;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_2.elit_egitim;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
        else if (seviye == 3)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_3.elit_egitim;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_3.elit_egitim;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
    }
    else if (!strcmp(birimAd, "kusatma_ustaligi"))
    {
        if (seviye == 1)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_1.kusatma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_1.kusatma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
        else if (seviye == 2)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_2.kusatma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_2.kusatma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
        else if (seviye == 3)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
            {
                arastir = &s->insanImparatorlugu.arastirma_seviyesi.seviye_3.kusatma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
            else if(!strcmp(grup, "ork_legi"))
            {
                arastir = &s->orkLegi.arastirma_seviyesi.seviye_3.kusatma_ustaligi;
                if(i == 5)
                    arastir->deger = deger;
                else
                    arastirmaEtkiledigiBirimAta(arastir, okunan_satir);
            }
        }
    }
}

void arastirmaAyristir(struct Savasanlar *s, char *birimAd, char *grup, int seviye)
{
    char calismaDizini[200];
    char *dosyaYolu;
    char *dosyaAdi;

    getcwd(calismaDizini, sizeof(calismaDizini));
    dosyaYolu = strcat(calismaDizini, "\\Files\\research.json");
    if (access(dosyaYolu, F_OK) != 0)
    {
        printf("Dosya '%s' bulunamadi. Lutfen dosyanin dogru isimde ve dogru dizinde oldugundan emin olun.", dosyaYolu);
        return;
    }
    dosyaAdi = "Files\\research.json";

    char ayrisan[200];
    char okunan_satir[100];
    char okunan_satir2[100];
    int kontrol = 0;

    FILE *dosya;
    dosya = fopen(dosyaAdi, "r");

    if (dosya == NULL)
    {
        printf("\ndosya acilamadi");
        return;
    }

    while (fgets(ayrisan, 200, dosya))
    {
        if (strstr(ayrisan, birimAd))
        {
            while (fgets(okunan_satir, 100, dosya))
            {
                if(seviye == 1)
                {
                    if (strstr(okunan_satir, "seviye_1") && kontrol == 0)
                    {
                        kontrol = 1;
                        while (fgets(okunan_satir2, 100, dosya) && !strchr(okunan_satir2, '}'))
                        {
                            if (strstr(okunan_satir2, "deger"))
                                arastirmaAtama(s, okunan_satir2, birimAd, grup, 1, 5);
                            else if(strstr(okunan_satir2, "aciklama"))
                                arastirmaAtama(s, okunan_satir2, birimAd, grup, 1, 0);
                        }
                    }
                }
                else if(seviye == 2)
                {
                    if (strstr(okunan_satir, "seviye_2") && kontrol == 0)
                    {
                        kontrol = 1;
                        while (fgets(okunan_satir2, 100, dosya) && !strchr(okunan_satir2, '}'))
                        {
                            if (strstr(okunan_satir2, "deger"))
                                arastirmaAtama(s, okunan_satir2, birimAd, grup, 2, 5);
                            else if(strstr(okunan_satir2, "aciklama"))
                                arastirmaAtama(s, okunan_satir2, birimAd, grup, 2, 0);
                        }
                    }
                }
                else if(seviye == 3)
                {
                    if (strstr(okunan_satir, "seviye_3") && kontrol == 0)
                    {
                        kontrol = 1;
                        while (fgets(okunan_satir2, 100, dosya) && !strchr(okunan_satir2, '}'))
                        {
                            if (strstr(okunan_satir2, "deger"))
                                arastirmaAtama(s, okunan_satir2, birimAd, grup, 3, 5);
                            else if(strstr(okunan_satir2, "aciklama"))
                                arastirmaAtama(s, okunan_satir2, birimAd, grup, 3, 0);
                        }
                    }
                }

            }
        }
    }

    fclose(dosya);
}

void senaryoAyristir(struct Savasanlar *s)
{
    char calismaDizini[200];
    char *dosyaYolu;
    char *dosyaAdi;

    getcwd(calismaDizini, sizeof(calismaDizini));
    dosyaYolu = strcat(calismaDizini, "\\Files\\senaryo.json");
    if (access(dosyaYolu, F_OK) != 0)
    {
        printf("Dosya '%s' bulunamadi. Lutfen dosyanin dogru isimde ve dogru dizinde oldugundan emin olun.", dosyaYolu);
        return;
    }

    int yazildi = 0;
    int kontrol = 0;
    int deger;
    int birimSay;
    char okunan_satir[200];
    char okunan_satir2[200];
    char *ayirici = "[,\"] ";
    char *token;
    char *kahraman;
    char *canavar;


    FILE *savas_sim;
    savas_sim = fopen("Files\\savas_sim.txt", "w");
    if (savas_sim == NULL)
    {
        printf("\nsavas_sim dosyasi acilamadi");
        return;
    }


    dosyaAdi = "Files\\senaryo.json";
    FILE *dosya;
    dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL)
    {
        printf("\nSenaryo dosyasi acilamadi");
        return;
    }


    while (fgets(okunan_satir2, 200, dosya))
    {
        if (strstr(okunan_satir2, "birimler"))
        {
            yazildi = 0;
            if(!yazildi++)
                fprintf(savas_sim,"\n\nImparatorluklarin Birim Sayilari\n\n");
            while (fgets(okunan_satir, 200, dosya) && !strchr(okunan_satir, '}'))
            {
                sscanf(okunan_satir, "%*s %d", &birimSay);
                if (strstr(okunan_satir, "piyadeler"))
                {
                    fprintf(savas_sim,"InsanImp birimi  ==  Piyade sayisi : %d\n", birimSay);
                    s->insanImparatorlugu.birimler.piyadeler.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "piyadeler");
                }
                else if (strstr(okunan_satir, "okcular"))
                {
                    fprintf(savas_sim,"InsanImp birimi  ==  Okcu sayisi : %d\n", birimSay);
                    s->insanImparatorlugu.birimler.okcular.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "okcular");
                }
                else if (strstr(okunan_satir, "suvariler"))
                {
                    fprintf(savas_sim,"InsanImp birimi  ==  Suvari sayisi : %d\n", birimSay);
                    s->insanImparatorlugu.birimler.suvariler.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "suvariler");
                }
                else if (strstr(okunan_satir, "kusatma_makineleri"))
                {
                    fprintf(savas_sim,"InsanImp birimi  ==  Kusatma makineleri sayisi : %d\n", birimSay);
                    s->insanImparatorlugu.birimler.kusatma_makineleri.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "kusatma_makineleri");
                }
                else if (strstr(okunan_satir, "ork_dovusculeri"))
                {
                    fprintf(savas_sim,"OrkImp birimi  ==  Ork dovusculeri sayisi : %d\n", birimSay);
                    s->orkLegi.birimler.ork_dovusculeri.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "ork_dovusculeri");
                }
                else if (strstr(okunan_satir, "mizrakcilar"))
                {
                    fprintf(savas_sim,"OrkImp birimi  ==  Mizrakci sayisi : %d\n", birimSay);
                    s->orkLegi.birimler.mizrakcilar.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "mizrakcilar");
                }
                else if (strstr(okunan_satir, "varg_binicileri"))
                {
                    fprintf(savas_sim,"OrkImp birimi  ==  Varg binicileri sayisi : %d\n", birimSay);
                    s->orkLegi.birimler.varg_binicileri.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "varg_binicileri");
                }
                else if (strstr(okunan_satir, "troller"))
                {
                    fprintf(savas_sim,"OrkImp birimi  ==  Trol sayisi : %d\n", birimSay);
                    s->orkLegi.birimler.troller.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "troller");
                }
            }
        }
        else if(strstr(okunan_satir2, "kahramanlar"))
        {
            yazildi = 0;
            if(!yazildi++)
                fprintf(savas_sim,"\n\nImparatorluklarin Kahramanlari\n\n");

            token = strtok(okunan_satir2, "[");
            token = strtok(NULL, "]");

            kahraman = strtok(token, ayirici);
            while (kahraman)
            {
                if(!strcmp(kahraman,"Alparslan"))
                {
                    fprintf(savas_sim,"InsanImp kahramani : Alparslan\n");
                    strcpy(s->insanImparatorlugu.kahramanlar.Alparslan.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Alparslan");
                }
                else if(!strcmp(kahraman,"Fatih_Sultan_Mehmet"))
                {
                    fprintf(savas_sim,"InsanImp kahramani : Fatih Sultan Mehmet\n");
                    strcpy(s->insanImparatorlugu.kahramanlar.Fatih_Sultan_Mehmet.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Fatih_Sultan_Mehmet");
                }
                else if(!strcmp(kahraman,"Mete_Han"))
                {
                    fprintf(savas_sim,"InsanImp kahramani : Mete Han\n");
                    strcpy(s->insanImparatorlugu.kahramanlar.Mete_Han.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Mete_Han");
                }
                else if(!strcmp(kahraman,"Yavuz_Sultan_Selim"))
                {
                    fprintf(savas_sim,"InsanImp kahramani : Yavuz Sultan Selim\n");
                    strcpy(s->insanImparatorlugu.kahramanlar.Yavuz_Sultan_Selim.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Yavuz_Sultan_Selim");
                }
                else if(!strcmp(kahraman,"Tugrul_Bey"))
                {
                    fprintf(savas_sim,"InsanImp kahramani : Tugrul Bey\n");
                    strcpy(s->insanImparatorlugu.kahramanlar.Tugrul_Bey.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Tugrul_Bey");
                }
                else if(!strcmp(kahraman,"Goruk_Vahsi"))
                {
                    fprintf(savas_sim,"OrkImp kahramani : Goruk Vahsi\n");
                    strcpy(s->orkLegi.kahramanlar.Goruk_Vahsi.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Goruk_Vahsi");
                }
                else if(!strcmp(kahraman,"Thruk_Kemikkiran"))
                {
                    fprintf(savas_sim,"OrkImp kahramani : Thruk Kemikkiran\n");
                    strcpy(s->orkLegi.kahramanlar.Thruk_Kemikkiran.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Thruk_Kemikkiran");
                }
                else if(!strcmp(kahraman,"Vrog_Kafakiran"))
                {
                    fprintf(savas_sim,"OrkImp kahramani : Vrog Kafakiran\n");
                    strcpy(s->orkLegi.kahramanlar.Vrog_Kafakiran.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Vrog_Kafakiran");
                }
                else if(!strcmp(kahraman,"Ugar_Zalim"))
                {
                    fprintf(savas_sim,"OrkImp kahramani : Ugar Zalim\n");
                    strcpy(s->orkLegi.kahramanlar.Ugar_Zalim.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Ugar_Zalim");
                }
                kahraman = strtok(NULL, ayirici);
            }
        }
        else if(strstr(okunan_satir2, "canavarlar"))
        {
            yazildi = 0;
            if(!yazildi++)
                fprintf(savas_sim,"\n\nImparatorluklarin Canavarlari\n\n");

            token = strtok(okunan_satir2, "[");
            token = strtok(NULL, "]");

            canavar = strtok(token, ayirici);
            while (canavar)
            {
                if(!strcmp(canavar,"Ejderha"))
                {
                    fprintf(savas_sim,"InsanImp canavari : Ejderha\n");
                    strcpy(s->insanImparatorlugu.canavarlar.Ejderha.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Ejderha");
                }
                else if(!strcmp(canavar,"Agri_Dagi_Devleri"))
                {
                    fprintf(savas_sim,"InsanImp canavari : Agri Dagi Devleri\n");
                    strcpy(s->insanImparatorlugu.canavarlar.Agri_Dagi_Devleri.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Agri_Dagi_Devleri");
                }
                else if(!strcmp(canavar,"Tepegoz"))
                {
                    fprintf(savas_sim,"InsanImp canavari : Tepegoz\n");
                    strcpy(s->insanImparatorlugu.canavarlar.Tepegoz.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Tepegoz");
                }
                else if(!strcmp(canavar,"Karakurt"))
                {
                    fprintf(savas_sim,"InsanImp canavari : Karakurt\n");
                    strcpy(s->insanImparatorlugu.canavarlar.Karakurt.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Karakurt");
                }
                else if(!strcmp(canavar,"Samur"))
                {
                    fprintf(savas_sim,"InsanImp canavari : Samur\n");
                    strcpy(s->insanImparatorlugu.canavarlar.Samur.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Samur");
                }
                else if(!strcmp(canavar,"Kara_Troll"))
                {
                    fprintf(savas_sim,"OrkImp canavari : Kara Troll\n");
                    strcpy(s->orkLegi.canavarlar.Kara_Troll.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Kara_Troll");
                }
                else if(!strcmp(canavar,"Golge_Kurtlari"))
                {
                    fprintf(savas_sim,"OrkImp canavari : Golge Kurtlari\n");
                    strcpy(s->orkLegi.canavarlar.Golge_Kurtlari.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Golge_Kurtlari");
                }
                else if(!strcmp(canavar,"Camur_Devleri"))
                {
                    fprintf(savas_sim,"OrkImp canavari : Camur Devleri\n");
                    strcpy(s->orkLegi.canavarlar.Camur_Devleri.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Camur_Devleri");
                }
                else if(!strcmp(canavar,"Ates_Iblisi"))
                {
                    fprintf(savas_sim,"OrkImp canavari : Ates Iblisi\n");
                    strcpy(s->orkLegi.canavarlar.Ates_Iblisi.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Ates_Iblisi");
                }
                else if(!strcmp(canavar,"Makrog_Savas_Beyi"))
                {
                    fprintf(savas_sim,"OrkImp canavari : Makrog Savas Beyi\n");
                    strcpy(s->orkLegi.canavarlar.Makrog_Savas_Beyi.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Makrog_Savas_Beyi");
                }
                else if(!strcmp(canavar,"Buz_Devleri"))
                {
                    fprintf(savas_sim,"OrkImp canavari :Buz Devleri\n");
                    strcpy(s->orkLegi.canavarlar.Buz_Devleri.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Buz_Devleri");
                }
                canavar = strtok(NULL, ayirici);
            }
        }

    }

    fprintf(savas_sim,"\n\nImparatorluklarin Arastirma ve Seviyeleri\n\n");
    rewind(dosya);
    while (fgets(okunan_satir, 200, dosya))
    {
        if (strstr(okunan_satir, "insan_imparatorlugu"))
        {
            while (fgets(okunan_satir, 200, dosya))
            {
                if (strstr(okunan_satir, "arastirma_seviyesi") && kontrol == 0)
                {
                    kontrol = 1;
                    while (fgets(okunan_satir, 200, dosya) && !strchr(okunan_satir, '}'))
                    {
                        sscanf(okunan_satir, "%*s %d", &deger);
                        if(deger == 1)
                        {
                            if (strstr(okunan_satir, "savunma_ustaligi"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Savunma Ustaligi : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Saldiri Gelistirmesi : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Elit Egitim : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Kusatma Ustaligi : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "insan_imparatorlugu", deger);
                            }
                        }
                        else if(deger == 2)
                        {
                            if (strstr(okunan_satir, "savunma_ustaligi"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Savunma Ustaligi : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Saldiri Gelistirmesi : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Elit Egitim : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Kusatma Ustaligi : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "insan_imparatorlugu", deger);
                            }
                        }
                        else if(deger == 3)
                        {
                            if (strstr(okunan_satir, "savunma_ustaligi"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Savunma Ustaligi : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Saldiri Gelistirmesi : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Elit Egitim : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                fprintf(savas_sim,"InsanImp arastirmasi  Kusatma Ustaligi : %d\n",deger);
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "insan_imparatorlugu", deger);
                            }
                        }

                    }
                }
            }
        }
    }

    rewind(dosya);
    kontrol = 0;
    while (fgets(okunan_satir, 200, dosya))
    {
        if (strstr(okunan_satir, "ork_legi"))
        {
            while (fgets(okunan_satir, 200, dosya))
            {
                if (strstr(okunan_satir, "arastirma_seviyesi") && kontrol == 0)
                {
                    kontrol = 1;
                    while (fgets(okunan_satir, 200, dosya) && !strchr(okunan_satir, '}'))
                    {
                        sscanf(okunan_satir, "%*s %d", &deger);
                        if(deger == 1)
                        {
                            if (strstr(okunan_satir, "savunma_ustaligi"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Savunma Ustaligi : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_1.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Saldiri Gelistirmesi : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_1.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Elit Egitim : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_1.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Kusatma Ustaligi : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_1.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "ork_legi", deger);
                            }
                        }
                        else if(deger == 2)
                        {
                            if (strstr(okunan_satir, "savunma_ustaligi"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Savunma Ustaligi : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_2.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Saldiri Gelistirmesi : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_2.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Elit Egitim : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_2.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Kusatma Ustaligi : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_2.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "ork_legi", deger);
                            }
                        }
                        else if(deger == 3)
                        {
                            if (strstr(okunan_satir, "savunma_ustaligi"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Savunma Ustaligi : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_3.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Saldiri Gelistirmesi : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_3.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Elit Egitim : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_3.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                fprintf(savas_sim,"OrkImp arastirmasi  Kusatma Ustaligi : %d\n",deger);
                                s->orkLegi.arastirma_seviyesi.seviye_3.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "ork_legi", deger);
                            }
                        }

                    }
                }
            }
        }
    }

    fclose(savas_sim);
    fclose(dosya);
}

