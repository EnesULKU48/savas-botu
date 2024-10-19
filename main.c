#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //acces , getcw i�in
#include <curl/curl.h>

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
float insanSaldiriHesapla(struct BirimlerInsan *birim, int saldiri_sayisi);
float insanSavunmaHesapla(struct BirimlerInsan *birim);
float orkSaldiriHesapla(struct BirimlerOrg *birim, int saldiri_sayisi);
float orkSavunmaHesapla(struct BirimlerOrg *birim);
void birim_guncelle_ork(struct BirimlerOrg *birim, float netHasar, float toplamSavunmaGucu);
void hasar_dagilimi_ork(struct BirimlerOrg *birim, float netHasar, float toplamSavunmaGucu);
void birim_kayip_ork(struct BirimlerOrg *birim);
void olen_hasardagit_ork(struct BirimlerOrg *birim, float birimHasarDagilimi, float kalanSavunmaGucu);
void birim_guncelle_insan(struct BirimlerInsan *birim, float netHasar, float toplamSavunmaGucu);
void hasar_dagilimi_insan(struct BirimlerInsan *birim, float netHasar, float toplamSavunmaGucu);
void olen_hasardagit_insan(struct BirimlerInsan *birim, float birimHasarDagilimi, float kalanSavunmaGucu);
void birim_kayip_insan(struct BirimlerInsan *birim);
void saglik_durumu_ork(struct BirimlerOrg *birim);
void saglik_durumu_insan(struct BirimlerInsan *birim);
void insan_kahraman_etkisi(struct KahramanOzellikler *kahraman, struct BirimlerInsan *birimlerInsan);
void insan_kahraman_canavar_etkisi(struct KahramanlarInsan *kahraman, struct CanavarlarInsan *canavar, struct BirimlerInsan *birimler);
void canavar_kahraman_bonusu_uygula(char *bonus_turu, float bonus_degeri, float *savunma, float *saldiri, float *kritik_sans);
void insan_canavar_etkisi(struct CanavarOzellikler *canavar, struct BirimlerInsan *birimlerInsan);
void ork_kahraman_canavar_etkisi(struct KahramanlarOrg *kahraman, struct CanavarlarOrg *canavar, struct BirimlerOrg *birimler);
void ork_kahraman_etkisi(struct KahramanOzellikler *kahraman, struct BirimlerOrg *birimlerOrg);
void ork_canavar_etkisi(struct CanavarOzellikler *canavar, struct BirimlerOrg *birimlerOrg);
void arastirma_etkisi(struct InsanImparatorlugu *insan_imp, struct OrkLegi *ork_imp);
void insan_arastirma_etki_uygula(struct ArastirmaOzellikler *arastirma, struct InsanImparatorlugu *insan_imp, float etki);
void ork_arastirma_etki_uygula(struct ArastirmaOzellikler *arastirma, struct OrkLegi *ork_imp, float etki);
void yorgunluk_uygula(struct BirimlerInsan *birimInsan, struct BirimlerOrg *birimOrk);
float kritik_hasar_hesapla(float normal_saldiri, float kritik_sans, int saldiri_sayisi);


int main()
{
    struct Savasanlar *s = (struct Savasanlar *)malloc(sizeof(struct Savasanlar));
    memset(s, 0, sizeof(struct Savasanlar));

    if (!senaryoIndirme())
        printf("Senaryo indirmede sorun olustu!!!");

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

    while (1)
    {
        if (adim_say % 5 == 0)
            yorgunluk_uygula(birimlerInsan, birimlerOrk);

        insan_kahraman_canavar_etkisi(kahramanInsan, canavarInsan, birimlerInsan);
        ork_kahraman_canavar_etkisi(kahramanOrg, canavarOrg, birimlerOrk);
        arastirma_etkisi(&s->insanImparatorlugu, &s->orkLegi);

        if (adim_say % 2 == 1)
        {
            insan_saldiri_say++;

            printf("\n\n\n----adim sayisi : %d----\n",adim_say);

            float insan_saldiri_gucu = insanSaldiriHesapla(birimlerInsan, insan_saldiri_say);
            float ork_savunma_gucu = orkSavunmaHesapla(birimlerOrk);

            if(insan_saldiri_gucu == 0)
            {
                printf("\n\nork kazandi\n\n");
                break;
            }
            else if(ork_savunma_gucu == 0)
            {
                printf("\n\ninsankazandi\n\n");
                break;
            }

            printf("insan saldiri gucu : %f\n", insan_saldiri_gucu);
            printf("ork savunma gucu : %f\n", ork_savunma_gucu);

            float orkimp_nethasar = insan_saldiri_gucu - ork_savunma_gucu;

            if(orkimp_nethasar > 0)
                birim_guncelle_ork(birimlerOrk, orkimp_nethasar, ork_savunma_gucu);

            printf("net hasar : %f\n", orkimp_nethasar);


        }
        else
        {
            ork_saldiri_say++;

            printf("\n\n\nadim sayisi : %d\n",adim_say);

            float ork_saldiri_gucu = orkSaldiriHesapla(birimlerOrk, ork_saldiri_say);
            float insan_savunma_gucu = insanSavunmaHesapla(birimlerInsan);

            if(ork_saldiri_gucu <= 0)
            {
                printf("\n\ninsan kazandi\n\n");
                break;
            }
            else if(insan_savunma_gucu <= 0)
            {
                printf("\n\nork kazandi\n\n");
                break;
            }

            printf("ork saldiri gucu : %f\n", ork_saldiri_gucu);
            printf("insan savunma gucu : %f\n", insan_savunma_gucu);

            float insanimp_nethasar = ork_saldiri_gucu - insan_savunma_gucu;

            if(insanimp_nethasar > 0)
                birim_guncelle_insan(birimlerInsan, insanimp_nethasar, insan_savunma_gucu);

            printf("net hasar : %f\n", insanimp_nethasar);

        }
        adim_say++;
    }

}

void insan_arastirma_etki_uygula(struct ArastirmaOzellikler *arastirma, struct InsanImparatorlugu *insan_imp, float etki)
{
    if(!strcmp(arastirma->etkiledigi_birim, "tum_birimler"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            insan_imp->birimler.kusatma_makineleri.savunma *= etki;
            insan_imp->birimler.okcular.savunma *= etki;
            insan_imp->birimler.piyadeler.savunma *= etki;
            insan_imp->birimler.suvariler.savunma *= etki;
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            insan_imp->birimler.kusatma_makineleri.saldiri *= etki;
            insan_imp->birimler.okcular.saldiri *= etki;
            insan_imp->birimler.piyadeler.saldiri *= etki;
            insan_imp->birimler.suvariler.saldiri *= etki;
        }
        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            insan_imp->birimler.kusatma_makineleri.kritik_sans *= etki;
            insan_imp->birimler.okcular.kritik_sans *= etki;
            insan_imp->birimler.piyadeler.kritik_sans *= etki;
            insan_imp->birimler.suvariler.kritik_sans *= etki;
        }
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "kusatma_makineleri"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
            insan_imp->birimler.kusatma_makineleri.savunma *= etki;
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
            insan_imp->birimler.kusatma_makineleri.saldiri *= etki;
        else if(!strcmp(arastirma->etki_turu, "kritik"))
            insan_imp->birimler.kusatma_makineleri.kritik_sans *= etki;
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "okcu"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
            insan_imp->birimler.okcular.savunma *= etki;
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
            insan_imp->birimler.okcular.saldiri *= etki;
        else if(!strcmp(arastirma->etki_turu, "kritik"))
            insan_imp->birimler.okcular.kritik_sans *= etki;
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "piyade"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
            insan_imp->birimler.piyadeler.savunma *= etki;
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
            insan_imp->birimler.piyadeler.saldiri *= etki;
        else if(!strcmp(arastirma->etki_turu, "kritik"))
            insan_imp->birimler.piyadeler.kritik_sans *= etki;
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "suvari"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
            insan_imp->birimler.suvariler.savunma *= etki;
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
            insan_imp->birimler.suvariler.saldiri *= etki;
        else if(!strcmp(arastirma->etki_turu, "kritik"))
            insan_imp->birimler.suvariler.kritik_sans *= etki;
    }
}

void ork_arastirma_etki_uygula(struct ArastirmaOzellikler *arastirma, struct OrkLegi *ork_imp, float etki)
{
    if(!strcmp(arastirma->etkiledigi_birim, "tum_birimler"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
        {
            ork_imp->birimler.ork_dovusculeri.savunma *= etki;
            ork_imp->birimler.troller.savunma *= etki;
            ork_imp->birimler.varg_binicileri.savunma *= etki;
            ork_imp->birimler.mizrakcilar.savunma *= etki;
        }
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
        {
            ork_imp->birimler.ork_dovusculeri.saldiri *= etki;
            ork_imp->birimler.troller.saldiri *= etki;
            ork_imp->birimler.varg_binicileri.saldiri *= etki;
            ork_imp->birimler.mizrakcilar.saldiri *= etki;
        }
        else if(!strcmp(arastirma->etki_turu, "kritik"))
        {
            ork_imp->birimler.ork_dovusculeri.kritik_sans *= etki;
            ork_imp->birimler.troller.kritik_sans *= etki;
            ork_imp->birimler.varg_binicileri.kritik_sans *= etki;
            ork_imp->birimler.mizrakcilar.kritik_sans *= etki;
        }
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "ork_dovusculeri"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
            ork_imp->birimler.ork_dovusculeri.savunma *= etki;
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
            ork_imp->birimler.ork_dovusculeri.saldiri *= etki;
        else if(!strcmp(arastirma->etki_turu, "kritik"))
            ork_imp->birimler.ork_dovusculeri.kritik_sans *= etki;
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "troller"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
            ork_imp->birimler.troller.savunma *= etki;
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
            ork_imp->birimler.troller.saldiri *= etki;
        else if(!strcmp(arastirma->etki_turu, "kritik"))
            ork_imp->birimler.troller.kritik_sans *= etki;
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "varg_binicileri"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
            ork_imp->birimler.varg_binicileri.savunma *= etki;
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
            ork_imp->birimler.varg_binicileri.saldiri *= etki;
        else if(!strcmp(arastirma->etki_turu, "kritik"))
            ork_imp->birimler.varg_binicileri.kritik_sans *= etki;
    }
    else if(!strcmp(arastirma->etkiledigi_birim, "mizrakci"))
    {
        if(!strcmp(arastirma->etki_turu, "savunma"))
            ork_imp->birimler.mizrakcilar.savunma *= etki;
        else if(!strcmp(arastirma->etki_turu, "saldiri"))
            ork_imp->birimler.mizrakcilar.saldiri *= etki;
        else if(!strcmp(arastirma->etki_turu, "kritik"))
            ork_imp->birimler.mizrakcilar.kritik_sans *= etki;
    }
}

void arastirma_etkisi(struct InsanImparatorlugu *insan_imp, struct OrkLegi *ork_imp)
{
    struct ArastirmaOzellikler *arastirma;
    if(insan_imp->arastirma_seviyesi.seviye_1.elit_egitim.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_1.elit_egitim;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }
    if(insan_imp->arastirma_seviyesi.seviye_2.elit_egitim.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_2.elit_egitim;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }
    if(insan_imp->arastirma_seviyesi.seviye_3.elit_egitim.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_3.elit_egitim;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }

    if(ork_imp->arastirma_seviyesi.seviye_1.elit_egitim.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_1.elit_egitim;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }
    if(ork_imp->arastirma_seviyesi.seviye_2.elit_egitim.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_2.elit_egitim;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }
    if(ork_imp->arastirma_seviyesi.seviye_3.elit_egitim.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_3.elit_egitim;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }



    if(insan_imp->arastirma_seviyesi.seviye_1.kusatma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_1.kusatma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }
    if(insan_imp->arastirma_seviyesi.seviye_2.kusatma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_2.kusatma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }
    if(insan_imp->arastirma_seviyesi.seviye_3.kusatma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_3.kusatma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }

    if(ork_imp->arastirma_seviyesi.seviye_1.kusatma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_1.kusatma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }
    if(ork_imp->arastirma_seviyesi.seviye_2.kusatma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_2.kusatma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }
    if(ork_imp->arastirma_seviyesi.seviye_3.kusatma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_3.kusatma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }



    if(insan_imp->arastirma_seviyesi.seviye_1.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_1.saldiri_gelistirmesi;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }
    if(insan_imp->arastirma_seviyesi.seviye_2.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_2.saldiri_gelistirmesi;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }
    if(insan_imp->arastirma_seviyesi.seviye_3.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_3.saldiri_gelistirmesi;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }

    if(ork_imp->arastirma_seviyesi.seviye_1.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_1.saldiri_gelistirmesi;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }
    if(ork_imp->arastirma_seviyesi.seviye_2.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_2.saldiri_gelistirmesi;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }
    if(ork_imp->arastirma_seviyesi.seviye_3.saldiri_gelistirmesi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_3.saldiri_gelistirmesi;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }



    if(insan_imp->arastirma_seviyesi.seviye_1.savunma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_1.savunma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }
    if(insan_imp->arastirma_seviyesi.seviye_2.savunma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_2.savunma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }
    if(insan_imp->arastirma_seviyesi.seviye_3.savunma_ustaligi.deger > 0)
    {
        arastirma = &insan_imp->arastirma_seviyesi.seviye_3.savunma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        insan_arastirma_etki_uygula(arastirma, insan_imp, etki);
    }

    if(ork_imp->arastirma_seviyesi.seviye_1.savunma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_1.savunma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }
    if(ork_imp->arastirma_seviyesi.seviye_2.savunma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_2.savunma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }
    if(ork_imp->arastirma_seviyesi.seviye_3.savunma_ustaligi.deger > 0)
    {
        arastirma = &ork_imp->arastirma_seviyesi.seviye_3.savunma_ustaligi;
        float etki = (100 + arastirma->deger)/100;
        ork_arastirma_etki_uygula(arastirma, ork_imp, etki);
    }
}

void canavar_kahraman_bonusu_uygula(char *bonus_turu, float bonus_degeri, float *savunma, float *saldiri, float *kritik_sans)
{
    if (!strcmp(bonus_turu, "savunma"))
        *savunma *= (100 + bonus_degeri) / 100;

    else if (!strcmp(bonus_turu, "saldiri"))
        *saldiri *= (100 + bonus_degeri) / 100;

    else if(!strcmp(bonus_turu, "kritik_sans"))
        *kritik_sans *= (100 + bonus_degeri) / 100;
}

void ork_kahraman_etkisi(struct KahramanOzellikler *kahraman, struct BirimlerOrg *birimlerOrg)
{
    if ((int)kahraman->bonus_degeri != 0)
    {
        float bonus = kahraman->bonus_degeri;

        if (!strcmp(kahraman->etkiledigi_birim, "ork_dovusculeri"))
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->ork_dovusculeri.savunma, &birimlerOrg->ork_dovusculeri.saldiri, &birimlerOrg->ork_dovusculeri.kritik_sans);

        else if (!strcmp(kahraman->etkiledigi_birim, "mizrakcilar"))
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->mizrakcilar.savunma, &birimlerOrg->mizrakcilar.saldiri, &birimlerOrg->mizrakcilar.kritik_sans);

        else if (!strcmp(kahraman->etkiledigi_birim, "varg_binicileri"))
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->varg_binicileri.savunma, &birimlerOrg->varg_binicileri.saldiri, &birimlerOrg->varg_binicileri.kritik_sans);

        else if (!strcmp(kahraman->etkiledigi_birim, "troller"))
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->troller.savunma, &birimlerOrg->troller.saldiri, &birimlerOrg->troller.kritik_sans);

        else if (!strcmp(kahraman->etkiledigi_birim, "tum_birimlere"))
        {
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->ork_dovusculeri.savunma, &birimlerOrg->ork_dovusculeri.saldiri, &birimlerOrg->ork_dovusculeri.kritik_sans);
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->mizrakcilar.savunma, &birimlerOrg->mizrakcilar.saldiri, &birimlerOrg->mizrakcilar.kritik_sans);
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->varg_binicileri.savunma, &birimlerOrg->varg_binicileri.saldiri, &birimlerOrg->varg_binicileri.kritik_sans);
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerOrg->troller.savunma, &birimlerOrg->troller.saldiri, &birimlerOrg->troller.kritik_sans);
        }
    }
}

void ork_canavar_etkisi(struct CanavarOzellikler *canavar, struct BirimlerOrg *birimlerOrg)
{
    if ((int)canavar->etki_degeri != 0)
    {
        float bonus = canavar->etki_degeri;

        if (!strcmp(canavar->etkiledigi_birim, "ork_dovusculeri"))
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->ork_dovusculeri.savunma, &birimlerOrg->ork_dovusculeri.saldiri, &birimlerOrg->ork_dovusculeri.kritik_sans);

        else if (!strcmp(canavar->etkiledigi_birim, "mizrakcilar"))
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->mizrakcilar.savunma, &birimlerOrg->mizrakcilar.saldiri, &birimlerOrg->mizrakcilar.kritik_sans);

        else if (!strcmp(canavar->etkiledigi_birim, "varg_binicileri"))
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->varg_binicileri.savunma, &birimlerOrg->varg_binicileri.saldiri, &birimlerOrg->varg_binicileri.kritik_sans);

        else if (!strcmp(canavar->etkiledigi_birim, "troller"))
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->troller.savunma, &birimlerOrg->troller.saldiri, &birimlerOrg->troller.kritik_sans);

        else if (!strcmp(canavar->etkiledigi_birim, "tum_birimlere"))
        {
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->ork_dovusculeri.savunma, &birimlerOrg->ork_dovusculeri.saldiri, &birimlerOrg->ork_dovusculeri.kritik_sans);
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->mizrakcilar.savunma, &birimlerOrg->mizrakcilar.saldiri, &birimlerOrg->mizrakcilar.kritik_sans);
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->varg_binicileri.savunma, &birimlerOrg->varg_binicileri.saldiri, &birimlerOrg->varg_binicileri.kritik_sans);
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerOrg->troller.savunma, &birimlerOrg->troller.saldiri, &birimlerOrg->troller.kritik_sans);
        }
    }
}

void ork_kahraman_canavar_etkisi(struct KahramanlarOrg *kahraman, struct CanavarlarOrg *canavar, struct BirimlerOrg *birimler)
{
    ork_kahraman_etkisi(&kahraman->Goruk_Vahsi, birimler);
    ork_kahraman_etkisi(&kahraman->Thruk_Kemikkiran, birimler);
    ork_kahraman_etkisi(&kahraman->Ugar_Zalim, birimler);
    ork_kahraman_etkisi(&kahraman->Vrog_Kafakiran, birimler);
    ork_canavar_etkisi(&canavar->Ates_Iblisi, birimler);
    ork_canavar_etkisi(&canavar->Buz_Devleri, birimler);
    ork_canavar_etkisi(&canavar->Camur_Devleri, birimler);
    ork_canavar_etkisi(&canavar->Golge_Kurtlari, birimler);
    ork_canavar_etkisi(&canavar->Kara_Troll, birimler);
    ork_canavar_etkisi(&canavar->Makrog_Savas_Beyi, birimler);
}

void insan_kahraman_etkisi(struct KahramanOzellikler *kahraman, struct BirimlerInsan *birimlerInsan)
{
    if ((int)kahraman->bonus_degeri != 0)
    {
        float bonus = kahraman->bonus_degeri;

        if (!strcmp(kahraman->etkiledigi_birim, "piyade"))
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->piyadeler.savunma, &birimlerInsan->piyadeler.saldiri, &birimlerInsan->piyadeler.kritik_sans);

        else if (!strcmp(kahraman->etkiledigi_birim, "okcu"))
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->okcular.savunma, &birimlerInsan->okcular.saldiri, &birimlerInsan->okcular.kritik_sans);

        else if (!strcmp(kahraman->etkiledigi_birim, "kusatma_makinesi"))
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->kusatma_makineleri.savunma, &birimlerInsan->kusatma_makineleri.saldiri, &birimlerInsan->kusatma_makineleri.kritik_sans);

        else if (!strcmp(kahraman->etkiledigi_birim, "suvari"))
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->suvariler.savunma, &birimlerInsan->suvariler.saldiri, &birimlerInsan->suvariler.kritik_sans);

        else if (!strcmp(kahraman->etkiledigi_birim, "tum_birimlere"))
        {
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->piyadeler.savunma, &birimlerInsan->piyadeler.saldiri, &birimlerInsan->piyadeler.kritik_sans);
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->okcular.savunma, &birimlerInsan->okcular.saldiri, &birimlerInsan->okcular.kritik_sans);
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->kusatma_makineleri.savunma, &birimlerInsan->kusatma_makineleri.saldiri, &birimlerInsan->kusatma_makineleri.kritik_sans);
            canavar_kahraman_bonusu_uygula(kahraman->bonus_turu, bonus, &birimlerInsan->suvariler.savunma, &birimlerInsan->suvariler.saldiri, &birimlerInsan->suvariler.kritik_sans);
        }
    }
}

void insan_canavar_etkisi(struct CanavarOzellikler *canavar, struct BirimlerInsan *birimlerInsan)
{
    if ((int)canavar->etki_degeri != 0)
    {
        float bonus = canavar->etki_degeri;

        if (!strcmp(canavar->etkiledigi_birim, "piyade"))
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->piyadeler.savunma, &birimlerInsan->piyadeler.saldiri, &birimlerInsan->piyadeler.kritik_sans);

        else if (!strcmp(canavar->etkiledigi_birim, "okcu"))
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->okcular.savunma, &birimlerInsan->okcular.saldiri, &birimlerInsan->okcular.kritik_sans);

        else if (!strcmp(canavar->etkiledigi_birim, "kusatma_makinesi"))
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->kusatma_makineleri.savunma, &birimlerInsan->kusatma_makineleri.saldiri, &birimlerInsan->kusatma_makineleri.kritik_sans);

        else if (!strcmp(canavar->etkiledigi_birim, "suvari"))
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->suvariler.savunma, &birimlerInsan->suvariler.saldiri, &birimlerInsan->suvariler.kritik_sans);

        else if (!strcmp(canavar->etkiledigi_birim, "tum_birimlere"))
        {
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->piyadeler.savunma, &birimlerInsan->piyadeler.saldiri, &birimlerInsan->piyadeler.kritik_sans);
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->okcular.savunma, &birimlerInsan->okcular.saldiri, &birimlerInsan->okcular.kritik_sans);
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->kusatma_makineleri.savunma, &birimlerInsan->kusatma_makineleri.saldiri, &birimlerInsan->kusatma_makineleri.kritik_sans);
            canavar_kahraman_bonusu_uygula(canavar->etki_turu, bonus, &birimlerInsan->suvariler.savunma, &birimlerInsan->suvariler.saldiri, &birimlerInsan->suvariler.kritik_sans);
        }
    }
}

void insan_kahraman_canavar_etkisi(struct KahramanlarInsan *kahraman, struct CanavarlarInsan *canavar, struct BirimlerInsan *birimler)
{
    insan_kahraman_etkisi(&kahraman->Alparslan, birimler);
    insan_kahraman_etkisi(&kahraman->Mete_Han, birimler);
    insan_kahraman_etkisi(&kahraman->Fatih_Sultan_Mehmet, birimler);
    insan_kahraman_etkisi(&kahraman->Tugrul_Bey, birimler);
    insan_kahraman_etkisi(&kahraman->Yavuz_Sultan_Selim, birimler);
    insan_canavar_etkisi(&canavar->Agri_Dagi_Devleri, birimler);
    insan_canavar_etkisi(&canavar->Ejderha, birimler);
    insan_canavar_etkisi(&canavar->Karakurt, birimler);
    insan_canavar_etkisi(&canavar->Samur, birimler);
    insan_canavar_etkisi(&canavar->Tepegoz, birimler);
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

void birim_kayip_ork(struct BirimlerOrg *birim)
{
    if (birim->mizrakcilar.birimSayi > 0)
        birim->mizrakcilar.birimSayi -= birim->mizrakcilar.hasar_dagilimi / birim->mizrakcilar.saglik;

    if (birim->varg_binicileri.birimSayi > 0)
        birim->varg_binicileri.birimSayi -= birim->varg_binicileri.hasar_dagilimi / birim->varg_binicileri.saglik;

    if (birim->ork_dovusculeri.birimSayi > 0)
        birim->ork_dovusculeri.birimSayi -= birim->ork_dovusculeri.hasar_dagilimi / birim->ork_dovusculeri.saglik;

    if (birim->troller.birimSayi > 0)
        birim->troller.birimSayi -= birim->troller.hasar_dagilimi / birim->troller.saglik;



    printf("\nmizrakci sayisi: %d",birim->mizrakcilar.birimSayi);
    printf("\nmizrakci hasar dagilimi : %f",birim->mizrakcilar.hasar_dagilimi);
    if (birim->mizrakcilar.birimSayi <= 0)
    {
        printf("\nmizrakci oldu\n");
        birim->mizrakcilar.birimSayi = 0;
        olen_hasardagit_ork(birim, birim->mizrakcilar.hasar_dagilimi, orkSavunmaHesapla(birim));
        printf("ork yeni hasar dagilimi : %f", birim->ork_dovusculeri.hasar_dagilimi);
        printf("trol yeni hasar dagilimi : %f", birim->troller.hasar_dagilimi);
        printf("varg_binicileri yeni hasar dagilimi : %f", birim->varg_binicileri.hasar_dagilimi);
    }




    printf("\nork_dovusculeri sayisi: %d",birim->ork_dovusculeri.birimSayi);
    printf("\nork_dovusculeri hasar_dagilimi: %f",birim->ork_dovusculeri.hasar_dagilimi);
    if (birim->ork_dovusculeri.birimSayi <= 0)
    {
        printf("\nork_dovusculeri oldu\n");
        birim->ork_dovusculeri.birimSayi = 0;
        olen_hasardagit_ork(birim, birim->ork_dovusculeri.hasar_dagilimi, orkSavunmaHesapla(birim));
        printf("mizrakci yeni hasar dagilimi : %f", birim->mizrakcilar.hasar_dagilimi);
        printf("trol yeni hasar dagilimi : %f", birim->troller.hasar_dagilimi);
        printf("varg_binicileri yeni hasar dagilimi : %f", birim->varg_binicileri.hasar_dagilimi);
    }


    printf("\ntroller sayisi : %d",birim->troller.birimSayi);
    printf("\ntroller hasar_dagilimi : %f",birim->troller.hasar_dagilimi);
    if (birim->troller.birimSayi <= 0)
    {
        printf("\ntroller oldu\n");
        birim->troller.birimSayi = 0;
        olen_hasardagit_ork(birim, birim->troller.hasar_dagilimi, orkSavunmaHesapla(birim));
        printf("mizrakci yeni hasar dagilimi : %f", birim->mizrakcilar.hasar_dagilimi);
        printf("ork yeni hasar dagilimi : %f", birim->ork_dovusculeri.hasar_dagilimi);
        printf("varg_binicileri yeni hasar dagilimi : %f", birim->varg_binicileri.hasar_dagilimi);
    }


    printf("\nvarg_binicileri sayisi : %d",birim->varg_binicileri.birimSayi);
    printf("\nvarg_binicileri hasar_dagilimi : %f",birim->varg_binicileri.hasar_dagilimi);
    if (birim->varg_binicileri.birimSayi <= 0)
    {
        printf("\nvarg_binicileri oldu\n");
        birim->varg_binicileri.birimSayi = 0;
        olen_hasardagit_ork(birim, birim->varg_binicileri.hasar_dagilimi, orkSavunmaHesapla(birim));
        printf("mizrakci yeni hasar dagilimi : %f", birim->mizrakcilar.hasar_dagilimi);
        printf("ork yeni hasar dagilimi : %f", birim->ork_dovusculeri.hasar_dagilimi);
        printf("trol yeni hasar dagilimi : %f", birim->troller.hasar_dagilimi);
    }
}

void saglik_durumu_ork(struct BirimlerOrg *birim)
{
    printf("\n--onceki saglik--\n");
    printf("\nmizrak saglik : %f\n",birim->mizrakcilar.saglik);
    printf("\nork_dovusculeri saglik : %f\n",birim->ork_dovusculeri.saglik);
    printf("\ntroller saglik : %f\n",birim->troller.saglik);
    printf("\nvarg_binicileri saglik : %f\n",birim->varg_binicileri.saglik);

    if (birim->mizrakcilar.birimSayi > 0)
        birim->mizrakcilar.saglik -= birim->mizrakcilar.hasar_dagilimi / (float)birim->mizrakcilar.birimSayi;

    if (birim->varg_binicileri.birimSayi > 0)
        birim->varg_binicileri.saglik -= birim->varg_binicileri.hasar_dagilimi / (float)birim->varg_binicileri.birimSayi;

    if (birim->ork_dovusculeri.birimSayi > 0)
        birim->ork_dovusculeri.saglik -= birim->ork_dovusculeri.hasar_dagilimi / (float)birim->ork_dovusculeri.birimSayi;

    if (birim->troller.birimSayi > 0)
        birim->troller.saglik -= birim->troller.hasar_dagilimi / (float)birim->troller.birimSayi;


    if(birim->mizrakcilar.saglik <= 0)
        birim->mizrakcilar.birimSayi = 0;

    if(birim->varg_binicileri.saglik <= 0)
        birim->varg_binicileri.birimSayi = 0;

    if(birim->ork_dovusculeri.saglik <= 0)
        birim->ork_dovusculeri.birimSayi = 0;

    if(birim->troller.saglik <= 0)
        birim->troller.birimSayi = 0;


    printf("\n--sonraki saglik--\n");
    printf("\nmizrak saglik : %f\n",birim->mizrakcilar.saglik);
    printf("\nork_dovusculeri saglik : %f\n",birim->ork_dovusculeri.saglik);
    printf("\ntroller saglik : %f\n",birim->troller.saglik);
    printf("\nvarg_binicileri saglik : %f\n",birim->varg_binicileri.saglik);

}

void birim_guncelle_ork(struct BirimlerOrg *birim, float netHasar, float toplamSavunmaGucu)
{
    hasar_dagilimi_ork(birim, netHasar, toplamSavunmaGucu);
    birim_kayip_ork(birim);
    saglik_durumu_ork(birim);
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

void birim_kayip_insan(struct BirimlerInsan *birim)
{
    if (birim->kusatma_makineleri.birimSayi > 0)
        birim->kusatma_makineleri.birimSayi -= birim->kusatma_makineleri.hasar_dagilimi / birim->kusatma_makineleri.saglik;

    if (birim->okcular.birimSayi > 0)
        birim->okcular.birimSayi -= birim->okcular.hasar_dagilimi / birim->okcular.saglik;

    if (birim->piyadeler.birimSayi > 0)
        birim->piyadeler.birimSayi -= birim->piyadeler.hasar_dagilimi / birim->piyadeler.saglik;

    if (birim->suvariler.birimSayi > 0)
        birim->suvariler.birimSayi -= birim->suvariler.hasar_dagilimi / birim->suvariler.saglik;

    printf("\nkusatma_makineleri sayisi: %d",birim->kusatma_makineleri.birimSayi);
    printf("\nkusatma_makineleri hasar dagilimi : %f",birim->kusatma_makineleri.hasar_dagilimi);
    if (birim->kusatma_makineleri.birimSayi <= 0)
    {
        printf("\nkusatma_makineleri oldu\n");
        birim->kusatma_makineleri.birimSayi = 0;
        olen_hasardagit_insan(birim, birim->kusatma_makineleri.hasar_dagilimi, insanSavunmaHesapla(birim));
        printf("okcu yeni hasar dagilimi : %f", birim->okcular.hasar_dagilimi);
        printf("piyade yeni hasar dagilimi : %f", birim->piyadeler.hasar_dagilimi);
        printf("suvari yeni hasar dagilimi : %f", birim->suvariler.hasar_dagilimi);
    }

    printf("\nokcular sayisi: %d",birim->okcular.birimSayi);
    printf("\nokcular hasar_dagilimi: %f",birim->okcular.hasar_dagilimi);
    if (birim->okcular.birimSayi <= 0)
    {
        printf("\nokcular oldu\n");
        birim->okcular.birimSayi = 0;
        olen_hasardagit_insan(birim, birim->okcular.hasar_dagilimi, insanSavunmaHesapla(birim));
        printf("piyade yeni hasar dagilimi : %f", birim->piyadeler.hasar_dagilimi);
        printf("suvari yeni hasar dagilimi : %f", birim->suvariler.hasar_dagilimi);
        printf("kusatma mak. yeni hasar dagilimi : %f", birim->kusatma_makineleri.hasar_dagilimi);
    }

    printf("\npiyadeler sayisi : %d",birim->piyadeler.birimSayi);
    printf("\npiyadeler hasar_dagilimi : %f",birim->piyadeler.hasar_dagilimi);
    if (birim->piyadeler.birimSayi <= 0)
    {
        printf("\npiyadeler oldu\n");
        birim->piyadeler.birimSayi = 0;
        olen_hasardagit_insan(birim, birim->piyadeler.hasar_dagilimi, insanSavunmaHesapla(birim));
        printf("suvari yeni hasar dagilimi : %f", birim->suvariler.hasar_dagilimi);
        printf("kusatma mak. yeni hasar dagilimi : %f", birim->kusatma_makineleri.hasar_dagilimi);
        printf("okcu yeni hasar dagilimi : %f", birim->okcular.hasar_dagilimi);
    }

    printf("\nsuvariler sayisi : %d",birim->suvariler.birimSayi);
    printf("\nsuvariler hasar_dagilimi : %f",birim->suvariler.hasar_dagilimi);
    if (birim->suvariler.birimSayi <= 0)
    {
        printf("\nsuvariler oldu\n");
        birim->suvariler.birimSayi = 0;
        olen_hasardagit_insan(birim, birim->suvariler.hasar_dagilimi, insanSavunmaHesapla(birim));
        printf("kusatma mak. yeni hasar dagilimi : %f", birim->kusatma_makineleri.hasar_dagilimi);
        printf("okcu yeni hasar dagilimi : %f", birim->okcular.hasar_dagilimi);
        printf("piyade yeni hasar dagilimi : %f", birim->piyadeler.hasar_dagilimi);
    }
}

void saglik_durumu_insan(struct BirimlerInsan *birim)
{
    printf("\n--onceki saglik--\n");
    printf("\nkusatma_makineleri saglik : %f\n",birim->kusatma_makineleri.saglik);
    printf("\nokcular saglik : %f\n",birim->okcular.saglik);
    printf("\npiyadeler saglik : %f\n",birim->piyadeler.saglik);
    printf("\nsuvariler saglik : %f\n",birim->suvariler.saglik);

    if (birim->kusatma_makineleri.birimSayi > 0)
        birim->kusatma_makineleri.saglik -= birim->kusatma_makineleri.hasar_dagilimi / (float)birim->kusatma_makineleri.birimSayi;

    if (birim->okcular.birimSayi > 0)
        birim->okcular.saglik -= birim->okcular.hasar_dagilimi / (float)birim->okcular.birimSayi;

    if (birim->piyadeler.birimSayi > 0)
        birim->piyadeler.saglik -= birim->piyadeler.hasar_dagilimi / (float)birim->piyadeler.birimSayi;

    if (birim->suvariler.birimSayi > 0)
        birim->suvariler.saglik -= birim->suvariler.hasar_dagilimi / (float)birim->suvariler.birimSayi;


    if(birim->kusatma_makineleri.saglik <= 0)
        birim->kusatma_makineleri.birimSayi = 0;

    if(birim->okcular.saglik <= 0)
        birim->okcular.birimSayi = 0;

    if(birim->piyadeler.saglik <= 0)
        birim->piyadeler.birimSayi = 0;

    if(birim->suvariler.saglik <= 0)
        birim->suvariler.birimSayi = 0;

    printf("\n--sonraki saglik--\n");
    printf("\nkusatma_makineleri saglik : %f\n",birim->kusatma_makineleri.saglik);
    printf("\nokcular saglik : %f\n",birim->okcular.saglik);
    printf("\npiyadeler saglik : %f\n",birim->piyadeler.saglik);
    printf("\nsuvariler saglik : %f\n",birim->suvariler.saglik);
}

void birim_guncelle_insan(struct BirimlerInsan *birim, float netHasar, float toplamSavunmaGucu)
{
    hasar_dagilimi_insan(birim, netHasar, toplamSavunmaGucu);
    birim_kayip_insan(birim);
    saglik_durumu_insan(birim);
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

float kritik_hasar_hesapla(float normal_saldiri, float kritik_sans, int saldiri_sayisi)
{
    if (kritik_sans != 0 && saldiri_sayisi % (int)(100 / kritik_sans) == 0)
        return normal_saldiri * 1.5;

    return normal_saldiri;
}

float insanSaldiriHesapla(struct BirimlerInsan *birim, int saldiri_sayisi)
{
    float kusatma_makineleri_saldiri = kritik_hasar_hesapla(birim->kusatma_makineleri.saldiri, birim->kusatma_makineleri.kritik_sans, saldiri_sayisi);
    float okcular_saldiri = kritik_hasar_hesapla(birim->okcular.saldiri, birim->okcular.kritik_sans, saldiri_sayisi);
    float piyadeler_saldiri = kritik_hasar_hesapla(birim->piyadeler.saldiri, birim->piyadeler.kritik_sans, saldiri_sayisi);
    float suvariler_saldiri = kritik_hasar_hesapla(birim->suvariler.saldiri, birim->suvariler.kritik_sans, saldiri_sayisi);

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

float orkSaldiriHesapla(struct BirimlerOrg *birim, int saldiri_sayisi)
{

    float mizrakcilar_saldiri = kritik_hasar_hesapla(birim->mizrakcilar.saldiri, birim->mizrakcilar.kritik_sans, saldiri_sayisi);
    float ork_dovusculeri_saldiri = kritik_hasar_hesapla(birim->ork_dovusculeri.saldiri, birim->ork_dovusculeri.kritik_sans, saldiri_sayisi);
    float troller_saldiri = kritik_hasar_hesapla(birim->troller.saldiri, birim->troller.kritik_sans, saldiri_sayisi);
    float varg_binicileri_saldiri = kritik_hasar_hesapla(birim->varg_binicileri.saldiri, birim->varg_binicileri.kritik_sans, saldiri_sayisi);

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

        if (sonuc == CURLE_OK)
            printf("Dosya basariyla indirildi.\n");
        else
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
        if (access(dosyaYolu, F_OK) == 0)
            printf("mevcut %s\n", dosyaYolu);
        else
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
        if (access(dosyaYolu, F_OK) == 0)
            printf("mevcut %s\n", dosyaYolu);
        else
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
        if (access(dosyaYolu, F_OK) == 0)
            printf("mevcut %s\n", dosyaYolu);
        else
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
        strcpy(arastir->etki_turu, "kritik");
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
    if (access(dosyaYolu, F_OK) == 0)
        printf("mevcut %s\n", dosyaYolu);
    else
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
    if (access(dosyaYolu, F_OK) == 0)
        printf("mevcut %s\n", dosyaYolu);
    else
    {
        printf("Dosya '%s' bulunamadi. Lutfen dosyanin dogru isimde ve dogru dizinde oldugundan emin olun.", dosyaYolu);
        return;
    }


    int kontrol = 0;
    int deger;
    int birimSay;
    char okunan_satir[200];
    char okunan_satir2[200];
    char *ayirici = "[,\"] ";
    char *token;
    char *kahraman;
    char *canavar;

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
            while (fgets(okunan_satir, 200, dosya) && !strchr(okunan_satir, '}'))
            {
                sscanf(okunan_satir, "%*s %d", &birimSay);
                if (strstr(okunan_satir, "piyadeler"))
                {
                    s->insanImparatorlugu.birimler.piyadeler.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "piyadeler");
                }
                else if (strstr(okunan_satir, "okcular"))
                {
                    s->insanImparatorlugu.birimler.okcular.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "okcular");
                }
                else if (strstr(okunan_satir, "suvariler"))
                {
                    s->insanImparatorlugu.birimler.suvariler.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "suvariler");
                }
                else if (strstr(okunan_satir, "kusatma_makineleri"))
                {
                    s->insanImparatorlugu.birimler.kusatma_makineleri.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "kusatma_makineleri");
                }
                else if (strstr(okunan_satir, "ork_dovusculeri"))
                {
                    s->orkLegi.birimler.ork_dovusculeri.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "ork_dovusculeri");
                }
                else if (strstr(okunan_satir, "mizrakcilar"))
                {
                    s->orkLegi.birimler.mizrakcilar.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "mizrakcilar");
                }
                else if (strstr(okunan_satir, "varg_binicileri"))
                {
                    s->orkLegi.birimler.varg_binicileri.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "varg_binicileri");
                }
                else if (strstr(okunan_satir, "troller"))
                {
                    s->orkLegi.birimler.troller.birimSayi = birimSay;
                    dosyaOku(s, "birimler", "troller");
                }
            }
        }
        else if(strstr(okunan_satir2, "kahramanlar"))
        {
            token = strtok(okunan_satir2, "[");
            token = strtok(NULL, "]");

            kahraman = strtok(token, ayirici);
            while (kahraman)
            {
                if(!strcmp(kahraman,"Alparslan"))
                {
                    strcpy(s->insanImparatorlugu.kahramanlar.Alparslan.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Alparslan");
                }
                else if(!strcmp(kahraman,"Fatih_Sultan_Mehmet"))
                {
                    strcpy(s->insanImparatorlugu.kahramanlar.Fatih_Sultan_Mehmet.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Fatih_Sultan_Mehmet");
                }
                else if(!strcmp(kahraman,"Mete_Han"))
                {
                    strcpy(s->insanImparatorlugu.kahramanlar.Mete_Han.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Mete_Han");
                }
                else if(!strcmp(kahraman,"Yavuz_Sultan_Selim"))
                {
                    strcpy(s->insanImparatorlugu.kahramanlar.Yavuz_Sultan_Selim.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Yavuz_Sultan_Selim");
                }
                else if(!strcmp(kahraman,"Tugrul_Bey"))
                {
                    strcpy(s->insanImparatorlugu.kahramanlar.Tugrul_Bey.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Tugrul_Bey");
                }
                else if(!strcmp(kahraman,"Goruk_Vahsi"))
                {
                    strcpy(s->orkLegi.kahramanlar.Goruk_Vahsi.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Goruk_Vahsi");
                }
                else if(!strcmp(kahraman,"Thruk_Kemikkiran"))
                {
                    strcpy(s->orkLegi.kahramanlar.Thruk_Kemikkiran.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Thruk_Kemikkiran");
                }
                else if(!strcmp(kahraman,"Vrog_Kafakiran"))
                {
                    strcpy(s->orkLegi.kahramanlar.Vrog_Kafakiran.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Vrog_Kafakiran");
                }
                else if(!strcmp(kahraman,"Ugar_Zalim"))
                {
                    strcpy(s->orkLegi.kahramanlar.Ugar_Zalim.senaryoKahrAd,kahraman);
                    dosyaOku(s, "kahramanlar", "Ugar_Zalim");
                }
                kahraman = strtok(NULL, ayirici);
            }
        }
        else if(strstr(okunan_satir2, "canavarlar"))
        {
            token = strtok(okunan_satir2, "[");
            token = strtok(NULL, "]");

            canavar = strtok(token, ayirici);
            while (canavar)
            {
                if(!strcmp(canavar,"Ejderha"))
                {
                    strcpy(s->insanImparatorlugu.canavarlar.Ejderha.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Ejderha");
                }
                else if(!strcmp(canavar,"Agri_Dagi_Devleri"))
                {
                    strcpy(s->insanImparatorlugu.canavarlar.Agri_Dagi_Devleri.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Agri_Dagi_Devleri");
                }
                else if(!strcmp(canavar,"Tepegoz"))
                {
                    strcpy(s->insanImparatorlugu.canavarlar.Tepegoz.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Tepegoz");
                }
                else if(!strcmp(canavar,"Karakurt"))
                {
                    strcpy(s->insanImparatorlugu.canavarlar.Karakurt.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Karakurt");
                }
                else if(!strcmp(canavar,"Samur"))
                {
                    strcpy(s->insanImparatorlugu.canavarlar.Samur.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Samur");
                }
                else if(!strcmp(canavar,"Kara_Troll"))
                {
                    strcpy(s->orkLegi.canavarlar.Kara_Troll.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Kara_Troll");
                }
                else if(!strcmp(canavar,"Golge_Kurtlari"))
                {
                    strcpy(s->orkLegi.canavarlar.Golge_Kurtlari.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Golge_Kurtlari");
                }
                else if(!strcmp(canavar,"Camur_Devleri"))
                {
                    strcpy(s->orkLegi.canavarlar.Camur_Devleri.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Camur_Devleri");
                }
                else if(!strcmp(canavar,"Ates_Iblisi"))
                {
                    strcpy(s->orkLegi.canavarlar.Ates_Iblisi.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Ates_Iblisi");
                }
                else if(!strcmp(canavar,"Makrog_Savas_Beyi"))
                {
                    strcpy(s->orkLegi.canavarlar.Makrog_Savas_Beyi.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Makrog_Savas_Beyi");
                }
                else if(!strcmp(canavar,"Buz_Devleri"))
                {
                    strcpy(s->orkLegi.canavarlar.Buz_Devleri.senaryoCanvAd,canavar);
                    dosyaOku(s, "canavarlar", "Buz_Devleri");
                }
                canavar = strtok(NULL, ayirici);
            }
        }

    }

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
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "insan_imparatorlugu", deger);
                            }
                        }
                        else if(deger == 2)
                        {
                            if (strstr(okunan_satir, "savunma_ustaligi"))
                            {
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "insan_imparatorlugu", deger);
                            }
                        }
                        else if(deger == 3)
                        {
                            if (strstr(okunan_satir, "savunma_ustaligi"))
                            {
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "insan_imparatorlugu", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
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
                                s->orkLegi.arastirma_seviyesi.seviye_1.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_1.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_1.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_1.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "ork_legi", deger);
                            }
                        }
                        else if(deger == 2)
                        {
                            if (strstr(okunan_satir, "savunma_ustaligi"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_2.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_2.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_2.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_2.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "ork_legi", deger);
                            }
                        }
                        else if(deger == 3)
                        {
                            if (strstr(okunan_satir, "savunma_ustaligi"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_3.savunma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "savunma_ustaligi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "saldiri_gelistirmesi"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_3.saldiri_gelistirmesi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "saldiri_gelistirmesi", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "elit_egitim"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_3.elit_egitim.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "elit_egitim", "ork_legi", deger);
                            }
                            else if (strstr(okunan_satir, "kusatma_ustaligi"))
                            {
                                s->orkLegi.arastirma_seviyesi.seviye_3.kusatma_ustaligi.senaryoSeviyesi = deger;
                                arastirmaAyristir(s, "kusatma_ustaligi", "ork_legi", deger);
                            }
                        }

                    }
                }
            }
        }
    }

    fclose(dosya);
}

