//Структура пакета LOG в именах параметров и размерах
typedef struct
{
    //В начале идут 18 uint16_t
    uint16_t 	BlockSize;//Длина блока - 2
    uint16_t 	res1;//Резерв - 4
    uint16_t 	res2;//Резерв - 6
    uint16_t 	res3;//Резерв - 8
    uint16_t 	AZzts;//АЗ*100 - 10
    uint16_t 	ZUzts;//ЗУ*100 - 12
    uint16_t 	Rnagr;//Rнагрузки - 14
    uint16_t 	res6;//Резерв - 16
    uint16_t 	ZUndm;//ЗУндм*100 - 18
    uint16_t 	res7;//Резерв - 20
    uint16_t 	res8;//Резерв - 22
    uint16_t 	res9;//Резерв - 24
    uint16_t 	res10;//Резерв - 26
    uint16_t 	res11;//Резерв - 28
    uint16_t 	res12;//Резерв - 30
    uint16_t 	res13;//Резерв - 32 (если не 1, то неправильный зенитный угол)
    uint16_t 	res14;//Резерв - 34 (если зачистить, то не принимает ГК2, только ГК1)
    uint16_t 	res15;//Резерв - 36
    //сборная по 32 бита
    uint32_t	GlubDol;//Глубина долота - 40
    uint32_t 	GlubZU;//Глубина ЗУзтс - 44
    uint32_t	GlubRnagr;//Глубина Rнагрузки - 48
    uint32_t	GlubGK1;//Глубина ГК1 - 52
    uint32_t	GlubNDM;//Глубина НДМ - 56
    uint32_t	GlubGK2;//Глубина ГК2 - 60
    uint32_t	GlubIKP;//Глубина ИКС и ИКП - 64
    uint32_t 	res17;//Резерв - 68
    uint32_t	GlubNNK;//Глубина ННК - 72
    uint32_t 	res18;//Резерв - 76
    uint32_t 	res19;//Резерв - 80
    uint32_t 	res20;//Резерв - 84
	float		GK1;//ГК1 - 88
    float		GKndm;//ГКндм интегральный - 92
    float		DAVLin;//Давление внутреннее - 96
    float		DAVLout;//Давление внешнее - 100
    float		GKndm_up;//ГКндм верх - 104
    float		GKndm_dn;//ГКндм низ - 108
    float		KCndm;//КСндм - 112
    float		NAGRndm;//Осевая нагрузка НДМ - 116
    float		GK2;//ГК2 - 120
    float		IKP1;//ИКП1 - 124
    float		IKP2;//ИКП2 - 128
    float		IKP3;//ИКП3 - 132
    float		IKP4;//ИКП4 - 136
    float		res21;//Резерв - 140
    float		res22;//Резерв - 144
    float		NNK_LZ;//ННК малый зонд - 148
    float		NNK_BZ;//ННК большой зонд - 152
    float		NGK;//НГК - 156
    float		KP;//КП - 160
    float		res23;//Резерв - 164
    float		res24;//Резерв - 168
    float		res25;//Резерв - 172
    float		res26;//Резерв - 176
    double		Datetime;// 	- 184
    float		res27;//Резерв - 188
    uint16_t 	res27ii;
    uint16_t	OBORndm;//Обороты долота - 192
    uint32_t 	res27i;//Резерв - 196
    uint32_t 	res28;//Резерв - 200
    uint32_t 	res29;//Резерв - 204
    uint32_t 	res30;//Резерв - 208
    uint32_t 	res31;//Резерв - 212
    uint32_t 	res32;//Резерв - 216
    uint32_t 	res33;//Резерв - 220
    uint32_t 	IshIKP1;//Исх ИКП1 - 224
    uint32_t 	IshIKP2;//Исх ИКП2 - 228
    uint32_t 	IshIKP3;//Исх ИКП3 - 232
    uint32_t 	IshIKP4;//Исх ИКП4 - 236
    uint8_t     Year;//Год, после 2000 - 237
    uint8_t     Day;//День - 238
    uint8_t     Month;//Месяц - 239
    uint8_t     Hour;//Часы - 240
    uint8_t     Minut;//Минуты - 241
    uint8_t     Sec;//Секунды - 242
    uint16_t 	res34;//Резерв - 244
    uint32_t 	res36;//Резерв - 248
    uint32_t 	res37;//Резерв - 252
    uint32_t 	res38;//Резерв - 256 (по адресу 253 должно быть записано 0,1 или 2, ругается на время)
    uint32_t 	res39;//Резерв - 260
    uint32_t 	res40;//Резерв - 264
    uint32_t 	res41;//Резерв - 268
    uint32_t 	res42;//Резерв - 272
    uint32_t 	res43;//Резерв - 276
    uint32_t 	res44;//Резерв - 280
    uint32_t 	res45;//Резерв - 284
    uint32_t 	res46;//Резерв - 288
    uint32_t 	res47;//Резерв - 292
    uint32_t 	res48;//Резерв - 296
    uint32_t 	res49;//Резерв - 300
    uint32_t 	res50;//Резерв - 304
    uint32_t 	res51;//Резерв - 308
    uint32_t 	res52;//Резерв - 312
    uint32_t 	res53;//Резерв - 316
    uint32_t 	res54;//Резерв - 320
    uint32_t 	res55;//Резерв - 324
    float	 	IKS1;//ИКС1 - 328
    float	 	IKS2;//ИКС2 - 332
    float	 	IKS3;//ИКС3 - 336
    float	 	IKS4;//ИКС4 - 340
    uint32_t 	res56;//Резерв - 344
    uint32_t 	res57;//Резерв - 348
    uint32_t 	res58;//Резерв - 352
    uint32_t 	res59;//Резерв - 356
    uint32_t 	res60;//Резерв - 360
    uint32_t 	res61;//Резерв - 364
    uint32_t 	res62;//Резерв - 368
    uint32_t 	res63;//Резерв - 372
    uint32_t 	res64;//Резерв - 376
    uint32_t 	res65;//Резерв - 380
    uint32_t 	res66;//Резерв - 384
    uint32_t 	res67;//Резерв - 388
    uint32_t 	res68;//Резерв - 392
    uint32_t 	res69;//Резерв - 396
    uint32_t 	res70;//Резерв - 400
    uint32_t 	res71;//Резерв - 404
    uint32_t 	res72;//Резерв - 408
    uint32_t 	res73;//Резерв - 412
    uint32_t 	res74;//Резерв - 416
    uint32_t 	res75;//Резерв - 420
    uint32_t 	res76;//Резерв - 424
    uint32_t 	res77;//Резерв - 428
    uint32_t 	res78;//Резерв - 432
    uint32_t 	res79;//Резерв - 436
    uint32_t 	res80;//Резерв - 440
    uint32_t 	res81;//Резерв - 444
    uint32_t 	res82;//Резерв - 448
    uint32_t 	res83;//Резерв - 452
    uint32_t 	res84;//Резерв - 456
    uint32_t 	res85;//Резерв - 460
    uint32_t 	res86;//Резерв - 464
    uint32_t 	res87;//Резерв - 468
    uint32_t 	res88;//Резерв - 472
    uint32_t 	res89;//Резерв - 476
    uint32_t 	res90;//Резерв - 480
    uint32_t 	res91;//Резерв - 484
    uint32_t 	res92;//Резерв - 488
    uint32_t 	res93;//Резерв - 492
    uint32_t 	res94;//Резерв - 496
    uint32_t 	res95;//Резерв - 500
    uint32_t 	res96;//Резерв - 504
    uint32_t 	res97;//Резерв - 508
    uint32_t 	res98;//Резерв - 512

} PARAM_LOG_Type;

#define DLINA_Param_LOG		(sizeof(PARAM_LOG_Type))
 