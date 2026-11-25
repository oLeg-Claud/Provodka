//Создадим имена параметров и присвоим 5кам соответствующие номера кодов в массиве
//============================================================================
//Режим работы №1 - стандарт, с ГК, без РЕТР
//============================================================================
#define FIRST_KOD_R1	0//тут же в старшем бите указатель статики
#define OTK1_H_R1		1//тут же OBOR(9-8) в ст.битах
#define OTK1_L_R1		2
#define GK1_H_R1		3//тут же OBOR(7-6) в ст.битах
#define GK1_L_R1		4
#define ZU_H_R1			5
#define ZU_L_R1			6
#define GMOD_H_R1		7
#define AZ_OTK1_H_R1	8//тут же OBOR(5-4) в ст.битах
#define AZ_OTK1_L_R1	9
#define OTKu_H_R1		10
#define OTKu_L_R1		11

#define OTK2_H_R1		12//тут же OBOR(3-2) в ст.битах
#define OTK2_L_R1		13
#define GK2_H_R1		14//тут же OBOR(1-0) в ст.битах
#define GK2_L_R1		15
#define GMOD_L_R1		16
#define HX_H_R1			17//тут же признаки деления (x,y,z) и QUAL_STAT в ст.битах
#define HX_M_R1			18
#define AZ_OTK2_H_R1	19//тут же KOD_REJ(5-4) в ст.битах
#define AZ_OTK2_L_R1	20
#define HX_L_R1			21
#define HY_H_R1			22

#define OTK3_H_R1		23//тут же KOD_REJ(3-2) в ст.битах
#define OTK3_L_R1		24
#define GK3_H_R1		25//тут же KOD_REJ(1-0) в ст.битах
#define GK3_L_R1		26
#define HY_M_R1			27
#define HY_L_R1			28
#define AZ_OTK3_H_R1	29//тут же VIBR(5-4) в ст.битах 
#define AZ_OTK3_L_R1	30
#define HZ_H_R1			31
#define HZ_L_R1			32

#define OTK4_H_R1		33//тут же VIBR(3-2) в ст.битах 
#define OTK4_L_R1		34
#define GK4_H_R1		35//тут же VIBR(1-0) в ст.битах 
#define GK4_L_R1		36
#define AZ_OTK4_H_R1	37//тут же 2 пустой в ст.битах
#define AZ_OTK4_L_R1	38
//============================================================================
#define DLINA_CODA_R1	39

