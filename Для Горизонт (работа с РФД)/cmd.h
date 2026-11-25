#ifndef _CMD_H_
#define _CMD_H_

#define FULL_DATA_AMK             0X0064 // 100) полный пакет данных от АМК -> ответ 0x1064
#define SET_ZENITH                0X0065 // 101) установка зенитного угла -> ответ 0x1065
#define SET_AZIMUTH               0X0066 // 102) установка азимута -> ответ 0x1066
#define SET_TOOL_FACE             0X0067 // 103) установка отклонителя -> ответ 0x1067
#define SET_TOOL_FACE_MODE        0X0068 // 104) установка режима отклонителя, магнитный или азимутальный -> ответ 0x1068

typedef struct tag_Full_Data_Package_AMK{
	float zen;     // зенит 
	float azm;     // азимут
	float tf;      // отклонитель
	uint8_t mode;  //режим работы отклонителья, магнитный - 0, азимутальный - 1	
}__attribute__((packed, aligned(1))) Full_Data_Package_AMK;

typedef struct tag_set_zenith_AMK{
	float zen;     // зенит 
}__attribute__((packed, aligned(1))) set_zenith_AMK;

typedef struct tag_set_azimuth_AMK{
	float azm;     // азимут 
}__attribute__((packed, aligned(1))) set_azimuth_AMK;

typedef struct tag_set_toolFace_AMK{
	float tf;      // отклонитель 
}__attribute__((packed, aligned(1))) set_toolFace_AMK;

typedef struct tag_set_TFmode_AMK{
	uint8_t mode;  // режим работы отклонителя, магнитный - 0, азимутальный - 1
}__attribute__((packed, aligned(1))) set_TFmode_AMK;


#endif // _CMD_H_
//------ EOF ------
