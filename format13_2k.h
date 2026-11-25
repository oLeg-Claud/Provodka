//Создадим имена параметров и присвоим 5кам соответствующие номера кодов в массиве
//============================================================================
//Режим работы №13 - укороченный без ГК, но с РЕТРом
//============================================================================
#define FIRST_KOD_R13	0//тут же в старшем бите указатель статики

#define PAUZA1_R13		1
#define OTK1_H_R13		2//тут же OBOR(9-8) в ст.битах
#define OTK1_L_R13		3
#define ZU_H_R13		4
#define ZU_L_R13		5
#define GMOD_H_R13		6
#define AZ_OTK1_H_R13	7//тут же OBOR(7-6) в ст.битах
#define AZ_OTK1_L_R13	8
#define OTKu_H_R13		9
#define OTKu_L_R13		10

#define PAUZA2_R13		11
#define OTK2_H_R13		12//тут же OBOR(5-4) в ст.битах
#define OTK2_L_R13		13
#define GMOD_L_R13		14
#define HX_H_R13		15//тут же признаки деления (x,y,z) и QUAL_STAT в ст.битах
#define HX_M_R13		16
#define AZ_OTK2_H_R13	17//тут же OBOR(3-2) в ст.битах
#define AZ_OTK2_L_R13	18
#define HX_L_R13		19
#define HY_H_R13		20

#define PAUZA3_R13		21
#define OTK3_H_R13		22//тут же OBOR(1-0) в ст.битах
#define OTK3_L_R13		23
#define HY_M_R13		24
#define HY_L_R13		25
#define AZ_OTK3_H_R13	26//тут же KOD_REJ(4-3) в ст.битах 
#define AZ_OTK3_L_R13	27
#define HZ_H_R13		28
#define HZ_L_R13		29

#define PAUZA4_R13		30
#define OTK4_H_R13		31//тут же KOD_REJ(2-1) в ст.битах
#define OTK4_L_R13		32
#define VIBR_H_R13		33
#define VIBR_L_R13		34
#define AZ_OTK4_H_R13	35//тут же Image.ROLE(1бит) и KOD_REJ(0) в ст.битах
#define AZ_OTK4_L_R13	36
//============================================================================
#define DLINA_CODA_R13	37	//(32*29=928бита, Tкадра=(928+128)*PERIOD=116.39сек на 5Гц)
#define NUM_PAUSE_R13	4//кол-во пауз

