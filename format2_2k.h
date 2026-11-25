//Создадим имена параметров и присвоим полубайтам соответствующие номера кодов в массиве
//============================================================================
//Режим работы №2 - ГК + НДМ
//============================================================================
// Паузы
#define PAUZA1_R2		1
#define PAUZA2_R2		17
#define PAUZA3_R2		33
#define PAUZA4_R2		48
#define PAUZA5_R2		63
#define PAUZA6_R2		78
#define PAUZA7_R2		93
// ГКзтс
#define GK1_H_R2        4//тут же OBOR(7-6) в ст.битах
#define GK1_L_R2        5
#define GK2_H_R2        20//тут же KOD_REJ(3-2) в ст.битах
#define GK2_L_R2        21
#define GK3_H_R2        36//тут же HY(3-2) в ст.битах
#define GK3_L_R2        37
#define GK4_H_R2        51//тут же OBORndm(8-7) в ст.битах
#define GK4_L_R2        52
#define GK5_H_R2        66//тут же NAGR(8-7) в ст.битах
#define GK5_L_R2        67
#define GK6_H_R2        81//тут же MOMENT(2-1) в ст.битах
#define GK6_L_R2        82
#define GK7_H_R2        96//тут же dU(3-2) в ст.битах
#define GK7_L_R2        97
// ГКндм
#define GK1up_H_R2      6//тут же OBOR(5-4) в ст.битах
#define GK1up_L_R2      7
#define GK1dn_H_R2      10//тут же OBOR(1-0) в ст.битах
#define GK1dn_L_R2      11
#define GK2up_H_R2      22//тут же KOD_REJ(1-0) в ст.битах
#define GK2up_L_R2      23
#define GK2dn_H_R2      26//тут же VIBR(3-2) в ст.битах
#define GK2dn_L_R2      27
#define GK3up_H_R2      38//тут же HY(1-0) в ст.битах
#define GK3up_L_R2      39
#define GK3dn_H_R2      42//тут же HZ(9-8) в ст.битах
#define GK3dn_L_R2      43
#define GK4up_H_R2      53//тут же OBORndm(6-5) в ст.битах
#define GK4up_L_R2      54
#define GK4dn_H_R2      57//тут же OBORndm(4-3) в ст.битах
#define GK4dn_L_R2      58
#define GK5up_H_R2      68//тут же NAGR(6-5) в ст.битах
#define GK5up_L_R2      69
#define GK5dn_H_R2      72//тут же NAGR(2-1) в ст.битах
#define GK5dn_L_R2      73
#define GK6up_H_R2      83//тут же MOMENT(0)+DAVL(8) в ст.битах
#define GK6up_L_R2      84
#define GK6dn_H_R2      87//тут же DAVL(5-4) в ст.битах
#define GK6dn_L_R2      88
#define GK7up_H_R2      98//тут же dU(1-0) в ст.битах
#define GK7up_L_R2      99
#define GK7dn_H_R2      102//пусто(2 бита) в ст.битах
#define GK7dn_L_R2      103
// Отклонители ЗТС
#define OTK1_H_R2       2//тут же OBOR(9-8) в ст.битах
#define OTK1_L_R2       3
#define AZ_OTK1_H_R2    8//тут же OBOR(3-2) в ст.битах
#define AZ_OTK1_L_R2    9
#define OTK2_H_R2       18//тут же KOD_REJ(5-4) в ст.битах
#define OTK2_L_R2       19
#define AZ_OTK2_H_R2    24//тут же VIBR(5-4) в ст.битах
#define AZ_OTK2_L_R2    25
#define OTK3_H_R2       34//тут же VIBR(1-0) в ст.битах
#define OTK3_L_R2       35
#define AZ_OTK3_H_R2    40//тут же HZ(11-10) в ст.битах
#define AZ_OTK3_L_R2    41
#define OTK4_H_R2       49//тут же ZUndm(1-0) в ст.битах
#define OTK4_L_R2       50
#define AZ_OTK4_H_R2    55//тут же Image.ROLE(1бит) и KOD_REJ(0) в ст.битах
#define AZ_OTK4_L_R2    56
#define OTK5_H_R2       64//тут же TOK(1-0) в ст.битах
#define OTK5_L_R2       65
#define AZ_OTK5_H_R2    70//тут же NAGR(4-3) в ст.битах
#define AZ_OTK5_L_R2    71
#define OTK6_H_R2       79//тут же MOMENT(4-3) в ст.битах
#define OTK6_L_R2       80
#define AZ_OTK6_H_R2    85//тут же DAVL(7-6) в ст.битах
#define AZ_OTK6_L_R2    86
#define OTK7_H_R2       94//тут же dU(5-4) в ст.битах
#define OTK7_L_R2       95
#define AZ_OTK7_H_R2    100//пусто(2 бита) в ст.битах 
#define AZ_OTK7_L_R2    101
// УГЛЫ --------------------------------------------------------------------------
#define FIRST_KOD_R2    0//тут же в старшем бите указатель статики
#define ZU_H_R2         12
#define ZU_L_R2         13
#define GMOD_H_R2		14
#define GMOD_L_R2		28
#define OTKu_H_R2       15
#define OTKu_L_R2       16
#define HX_H_R2         29
#define HX_M_R2         30
#define HX_L_R2         31
#define HY_H_R2         32
#define HZ_H_R2         44
#define HZ_L_R2         45//тут же КачествоЗТС(1бит) + пусто(1бит) в мл. битах
#define ZUndm_H_R2		46//тут же Признаки деления H(x|y|z) в ст. битах
#define ZUndm_L_R2		47
#define OBORndm_L_R2	59
#define INDndm_H_R2		60
#define TOK_H_R2		61
#define TOK_L_R2		62
#define MOMENT_H_R2		74
#define PEAK_H_R2		75
#define PEAK_M_R2		76
#define PEAK_L_R2		77
#define DAVL_H_R2		89
#define TEMPndm_H_R2	90
#define TEMPndm_L_R2	91
#define dU_H_R2			92
//============================================================================
#define DLINA_CODA_R2 	104
#define NUM_PAUSE_R2	7//кол-во пауз

