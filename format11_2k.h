//Создадим имена параметров и присвоим 5кам соответствующие номера кодов в массиве
//============================================================================
//Режим работы №11 - с ГК, с РЕТРом
//============================================================================
#define FIRST_KOD_R11	0//тут же в старшем бите указатель статики
#define PAUZA1_R11		1
#define OTK1_H_R11		2//тут же OBOR(9-8) в ст.битах
#define OTK1_L_R11		3
#define GK1_H_R11		4//тут же OBOR(7-6) в ст.битах
#define GK1_L_R11		5
#define ZU_H_R11		6
#define ZU_L_R11		7
#define GMOD_H_R11		8
#define AZ_OTK1_H_R11	9//тут же OBOR(5-4) в ст.битах
#define AZ_OTK1_L_R11	10
#define OTKu_H_R11		11
#define OTKu_L_R11		12

#define PAUZA2_R11		13
#define OTK2_H_R11		14//тут же OBOR(3-2) в ст.битах
#define OTK2_L_R11		15
#define GK2_H_R11		16//тут же OBOR(1-0) в ст.битах
#define GK2_L_R11		17
#define GMOD_L_R11		18
#define HX_H_R11		19//тут же признаки деления (x,y,z) и QUAL_STAT в ст.битах
#define HX_M_R11		20
#define AZ_OTK2_H_R11	21//тут же KOD_REJ(5-4) в ст.битах
#define AZ_OTK2_L_R11	22
#define HX_L_R11		23
#define HY_H_R11		24

#define PAUZA3_R11		25
#define OTK3_H_R11		26//тут же KOD_REJ(3-2) в ст.битах
#define OTK3_L_R11		27
#define GK3_H_R11		28//тут же KOD_REJ(1-0) в ст.битах
#define GK3_L_R11		29
#define HY_M_R11		30
#define HY_L_R11		31
#define AZ_OTK3_H_R11	32//тут же VIBR(5-4) в ст.битах 
#define AZ_OTK3_L_R11	33
#define HZ_H_R11		34
#define HZ_L_R11		35

#define PAUZA4_R11		36
#define OTK4_H_R11		37//тут же VIBR(3-2) в ст.битах
#define OTK4_L_R11		38
#define GK4_H_R11		39//тут же VIBR(1-0) в ст.битах
#define GK4_L_R11		40
#define AZ_OTK4_H_R11	41//тут же 2 пустой в ст.битах
#define AZ_OTK4_L_R11	42
//============================================================================
#define DLINA_CODA_R11	43	//(32*41=1312бита, Tкадра=(1312+128)*PERIOD=141.55сек на 5Гц)
#define NUM_PAUSE_R11	4//кол-во пауз

