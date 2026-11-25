typedef long long	    uint64_t;
typedef unsigned int    uint32_t;
typedef int    			int32_t;
typedef unsigned short  uint16_t;
typedef short  			int16_t;
typedef unsigned char  	uint8_t;
typedef char  			int8_t;

typedef union sun64{uint64_t word; int8_t b[8];} sun64;
typedef union sun32{int word; int8_t b[4];} sun32;
typedef union un32{uint32_t word; uint8_t b[4];} un32;
typedef union sun16{int16_t word; struct {uint8_t L, H;};} sun16;//little endian
typedef union un16{uint16_t word; struct {uint8_t L, H;};} un16;
typedef union fl32{ float f; uint16_t w[2]; uint8_t b[4];} fl32;

#include "LOG_structure.h"

typedef union
{
    PARAM_LOG_Type	param;
    char 			mb[512];
} BUF_LOG_Type;
#define DLINA_BUF_LOG		(sizeof(BUF_LOG_Type))

typedef union
{
    uint32_t	word;
    struct          {uint16_t Lo, Hi;};
} UN32to16_Type;

typedef struct //структура для записи в черный ящик (тип)
{
 char D[10];//дата, тект
 char T[10];//время, тект
 float R[6];//углы
 int O[5];//ОТК
 char DOP[12];
 float F;//уровень сигнала, мВ
 char TAR[30];//имя файла тарировки
 char RRR[12];//кадр, углы или отк
 char R_MO[12];//тип прибора и зарезка, тект
 int O2[4];//АЗ-ОТК
 float SS[8];//настройки: угол МПиТ, АЗориент, ЗУориент, порог СП, УМН, Hop, порог напр.,
 int HH[3];//Магнитное поле
} SDAT;

typedef struct //структура для записи в очередь
{
 unsigned char mas[4];
} qStruc;

#pragma pack(push, 1)
typedef struct //структура пакета Геомаш 34б
{
	uint8_t Header;
    uint8_t Version;
    uint32_t Number;
    uint64_t Moment;
    uint8_t PayLoad[16];
    uint32_t CRC32;
} GEOMASH_type;
#pragma pack(pop)
 