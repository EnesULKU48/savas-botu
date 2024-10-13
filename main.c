#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //acces , getcw i�in

struct SavasanOzellikler
{
    int birimSayi;
    int saldiri;
    int savunma;
    int saglik;
    int kritik_sans;
};

struct KahramanOzellikler
{
    char senaryoKahrAd[4][25];
    char bonus_turu[15];
    char etkiledigi_birim[20];
    int bonus_degeri;
};

struct CanavarOzellikler
{
    char senaryoCanvAd[4][25];
    int etki_degeri;
    char etki_turu[15];
    char etkiledigi_birim[20];
};

struct ArastirmaOzellikler
{
    int senaryoSeviyesi;
    int deger;
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

void yaratikEtkiledigiBirimAta(struct CanavarOzellikler *canavar, char *okunan_satir);
void yaratikAtama(struct Savasanlar *s, char *okunan_satir, char *birimAd, int i);
void KahramanEtkiledigiBirimAta(struct KahramanOzellikler *kahraman, char *okunan_satir);
void kahramanAtama(struct Savasanlar *s, char *okunan_satir, char *birimAdi, int i);
void birimAtama(struct Savasanlar *s, char *okunan_satir, char *birimAdi, int i);
void dosyaOku(struct Savasanlar *s, char *ordu, char *birimAdi);
void birimAyristir(struct Savasanlar *s, char *dosyaAdi, char *birimAd);
void kahramanAyristir(struct Savasanlar *s, char *dosyaAdi, char *birimAd);
void yaratikAyristir(struct Savasanlar *s, char *dosyaAdi, char *birimAd);
void arastirmaAtama(struct Savasanlar *s, char *okunan_satir, char *birimAd, char *grup, int seviye);
void arastirmaAyristir(struct Savasanlar *s, char *birimAd, char *grup, int seviye);
void senaryoAyristir(struct Savasanlar *s);

int main()
{
    struct Savasanlar *s = (struct Savasanlar *)malloc(sizeof(struct Savasanlar));
    memset(s, 0, sizeof(struct Savasanlar));

    senaryoAyristir(s);

    free(s);
    return 0;
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
            printf("Dosya dogru bi konumda isimde degil");

        dosyaAdi = "Files\\unit_types.json"; // tam dosya yolunu yollaman gerekiyor
        birimAyristir(s, dosyaAdi, birimAdi);
    }


    else if(!strcmp(ordu,"kahramanlar"))
    {
        getcwd(calismaDizini, sizeof(calismaDizini));
        dosyaYolu = strcat(calismaDizini, "\\Files\\heroes.json");
        if (access(dosyaYolu, F_OK) == 0)
            printf("mevcut %s\n", dosyaYolu);
        else
            printf("Dosya dogru bi konumda isimde degil");

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
            printf("Dosya dogru bi konumda isimde degil");
        dosyaAdi = "Files\\creatures.json";
        yaratikAyristir(s, dosyaAdi, birimAdi);
    }

}

void birimAtama(struct Savasanlar *s, char *okunan_satir, char *birimAdi, int i)
{
    int deger;

    sscanf(okunan_satir, "%*s %d", &deger);

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
    int deger;

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
        sscanf(okunan_satir, " \"%*[^\"]\": \"%d\"", &deger);
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

void yaratikEtkiledigiBirimAta(struct CanavarOzellikler *canavar, char *okunan_satir)
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

void yaratikAtama(struct Savasanlar *s, char *okunan_satir, char *birimAd, int i)
{
    char bos[200];
    char etki_tur[100];
    int deger;

    struct Savasanlar *canavar;

    if (i == 1)
    {
        sscanf(okunan_satir, "%*[^0-9]%d", &deger);
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
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Agri_Dagi_Devleri"))
        {
            canavar = &s->insanImparatorlugu.canavarlar.Agri_Dagi_Devleri;
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Tepegoz"))
        {
            canavar = &s->insanImparatorlugu.canavarlar.Tepegoz;
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Karakurt"))
        {
            canavar = &s->insanImparatorlugu.canavarlar.Karakurt;
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Samur"))
        {
            canavar = &s->insanImparatorlugu.canavarlar.Samur;
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Kara_Troll"))
        {
            canavar = &s->orkLegi.canavarlar.Kara_Troll;
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Golge_Kurtlari"))
        {
            canavar = &s->orkLegi.canavarlar.Golge_Kurtlari;
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Camur_Devleri"))
        {
            canavar = &s->orkLegi.canavarlar.Camur_Devleri;
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Ates_Iblisi"))
        {
            canavar = &s->orkLegi.canavarlar.Ates_Iblisi;
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Makrog_Savas_Beyi"))
        {
            canavar = &s->orkLegi.canavarlar.Makrog_Savas_Beyi;
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
        }
        else if (!strcmp(birimAd, "Buz_Devleri"))
        {
            canavar = &s->orkLegi.canavarlar.Buz_Devleri;
            yaratikEtkiledigiBirimAta(canavar, okunan_satir);
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

void yaratikAyristir(struct Savasanlar *s, char *dosyaAdi, char *birimAd)
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
                    yaratikAtama(s, okunan_satir, birimAd, 1);
                else if (strstr(okunan_satir, "etki_turu"))
                    yaratikAtama(s, okunan_satir, birimAd, 2);
                else if (strstr(okunan_satir, "aciklama"))
                    yaratikAtama(s, okunan_satir, birimAd, 3);
            }
        }
    }

    fclose(dosya);
}

void arastirmaAtama(struct Savasanlar *s, char *okunan_satir, char *birimAd, char *grup, int seviye)
{
    int deger;

    sscanf(okunan_satir, "%*[^0-9]%d", &deger);
    if (!strcmp(birimAd, "savunma_ustaligi"))
    {
        if (seviye == 1)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.savunma_ustaligi.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_1.savunma_ustaligi.deger = deger;
        }
        else if (seviye == 2)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.savunma_ustaligi.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_2.savunma_ustaligi.deger = deger;
        }
        else if (seviye == 3)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.savunma_ustaligi.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_3.savunma_ustaligi.deger = deger;
        }
    }
    else if (!strcmp(birimAd, "saldiri_gelistirmesi"))
    {
        if (seviye == 1)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.saldiri_gelistirmesi.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_1.saldiri_gelistirmesi.deger = deger;
        }
        else if (seviye == 2)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.saldiri_gelistirmesi.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_2.saldiri_gelistirmesi.deger = deger;
        }
        else if (seviye == 3)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.saldiri_gelistirmesi.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_3.saldiri_gelistirmesi.deger = deger;
        }
    }
    else if (!strcmp(birimAd, "elit_egitim"))
    {
        if (seviye == 1)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.elit_egitim.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_1.elit_egitim.deger = deger;
        }
        else if (seviye == 2)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.elit_egitim.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_2.elit_egitim.deger = deger;
        }
        else if (seviye == 3)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.elit_egitim.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_3.elit_egitim.deger = deger;
        }
    }
    else if (!strcmp(birimAd, "kusatma_ustaligi"))
    {
        if (seviye == 1)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_1.kusatma_ustaligi.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_1.kusatma_ustaligi.deger = deger;
        }
        else if (seviye == 2)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_2.kusatma_ustaligi.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_2.kusatma_ustaligi.deger = deger;
        }
        else if (seviye == 3)
        {
            if(!strcmp(grup, "insan_imparatorlugu"))
                s->insanImparatorlugu.arastirma_seviyesi.seviye_3.kusatma_ustaligi.deger = deger;
            else if(!strcmp(grup, "ork_legi"))
                s->orkLegi.arastirma_seviyesi.seviye_3.kusatma_ustaligi.deger = deger;
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
        printf("Dosya dogru bi konumda isimde degil");
    dosyaAdi = "Files\\research.json";

    char ayrisan[200];
    char okunan_satir[100];
    char okunan_satir2[100];

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
                    if (strstr(okunan_satir, "seviye_1"))
                    {
                        while (fgets(okunan_satir2, 100, dosya) && !strchr(okunan_satir2, '}'))
                        {
                            if (strstr(okunan_satir2, "deger"))
                                arastirmaAtama(s, okunan_satir2, birimAd, grup, 1);
                        }
                    }
                }
                else if(seviye == 2)
                {
                    if (strstr(okunan_satir, "seviye_2"))
                    {
                        while (fgets(okunan_satir2, 100, dosya) && !strchr(okunan_satir2, '}'))
                        {
                            if (strstr(okunan_satir2, "deger"))
                                arastirmaAtama(s, okunan_satir2, birimAd, grup, 2);
                        }
                    }
                }
                else if(seviye == 3)
                {
                    if (strstr(okunan_satir, "seviye_3"))
                    {
                        while (fgets(okunan_satir2, 100, dosya) && !strchr(okunan_satir2, '}'))
                        {
                            if (strstr(okunan_satir2, "deger"))
                                arastirmaAtama(s, okunan_satir2, birimAd, grup, 3);
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
        printf("Dosya dogru bi konumda isimde degil");
    dosyaAdi = "Files\\senaryo.json";

    int kontrol = 0;
    int deger;
    char okunan_satir[200];
    char okunan_satir2[200];
    char *ayirici = "[,\"] ";
    char *token;
    char *kahraman;
    char *canavar;

    FILE *dosya;
    dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL)
    {
        printf("\ndosya acilamadi");
        return;
    }


    while (fgets(okunan_satir2, 200, dosya))
    {
        if (strstr(okunan_satir2, "birimler"))
        {
            while (fgets(okunan_satir, 200, dosya) && !strchr(okunan_satir, '}'))
            {
                sscanf(okunan_satir, "%*s %d", &deger);
                if (strstr(okunan_satir, "piyadeler"))
                {
                    s->insanImparatorlugu.birimler.piyadeler.birimSayi = deger;
                    dosyaOku(s, "birimler", "piyadeler");
                }
                else if (strstr(okunan_satir, "okcular"))
                {
                    s->insanImparatorlugu.birimler.okcular.birimSayi = deger;
                    dosyaOku(s, "birimler", "okcular");
                }
                else if (strstr(okunan_satir, "suvariler"))
                {
                    s->insanImparatorlugu.birimler.suvariler.birimSayi = deger;
                    dosyaOku(s, "birimler", "suvariler");
                }
                else if (strstr(okunan_satir, "kusatma_makineleri"))
                {
                    s->insanImparatorlugu.birimler.kusatma_makineleri.birimSayi = deger;
                    dosyaOku(s, "birimler", "kusatma_makineleri");
                }
                else if (strstr(okunan_satir, "ork_dovusculeri"))
                {
                    s->orkLegi.birimler.ork_dovusculeri.birimSayi = deger;
                    dosyaOku(s, "birimler", "ork_dovusculeri");
                }
                else if (strstr(okunan_satir, "mizrakcilar"))
                {
                    s->orkLegi.birimler.mizrakcilar.birimSayi = deger;
                    dosyaOku(s, "birimler", "mizrakcilar");
                }
                else if (strstr(okunan_satir, "varg_binicileri"))
                {
                    s->orkLegi.birimler.varg_binicileri.birimSayi = deger;
                    dosyaOku(s, "birimler", "varg_binicileri");
                }
                else if (strstr(okunan_satir, "troller"))
                {
                    s->orkLegi.birimler.troller.birimSayi = deger;
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


