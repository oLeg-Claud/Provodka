//Создадим имена параметров и присвоим 5кам соответствующие номера кодов в массиве
//============================================================================
//Режим работы №12 - укороченный стандарт, без ГК, без РЕТР
//============================================================================
//В обоих кадрах - УГЛАХ и ОТК
#define FIRST_KOD_R12	0//тут же в старшем бите указатель статики
#define OTK1_H_R12		1//тут же OBOR(9-8) в ст.битах
#define OTK1_L_R12		2
#define ZU_H_R12		3
#define ZU_L_R12		4
#define GMOD_H_R12		5
#define AZ_OTK1_H_R12	6//тут же OBOR(7-6) в ст.битах
#define AZ_OTK1_L_R12	7
#define OTKu_H_R12		8
#define OTKu_L_R12		9

#define OTK2_H_R12		10//тут же OBOR(5-4) в ст.битах
#define OTK2_L_R12		11
#define GMOD_L_R12		12
#define HX_H_R12		13//тут же признаки деления (x,y,z) и QUAL_STAT в ст.битах
#define HX_M_R12		14
#define AZ_OTK2_H_R12	15//тут же OBOR(3-2) в ст.битах
#define AZ_OTK2_L_R12	16
#define HX_L_R12		17
#define HY_H_R12		18

#define OTK3_H_R12		19//тут же OBOR(1-0) в ст.битах
#define OTK3_L_R12		20
#define HY_M_R12		21
#define HY_L_R12		22
#define AZ_OTK3_H_R12	23//тут же KOD_REJ(4-3) в ст.битах 
#define AZ_OTK3_L_R12	24
#define HZ_H_R12		25
#define HZ_L_R12		26

#define OTK4_H_R12		27//тут же KOD_REJ(2-1) в ст.битах
#define OTK4_L_R12		28
#define VIBR_H_R12		29
#define VIBR_L_R12		30
#define AZ_OTK4_H_R12	31//тут же Image.ROLE(1бит) и KOD_REJ(0) в ст.битах
#define AZ_OTK4_L_R12	32
//============================================================================
#define DLINA_CODA_R12	33	//(32*29=928бита, Tкадра=(928+128)*PERIOD=103.8сек на 5Гц)

