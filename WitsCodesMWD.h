//Wits-коды параметров
//шапка
AnsiString skv_id=      "01";//Идентификатор скважины
AnsiString stvol_num=   "02";//Номер ствола, 0=один ствол
AnsiString kadr_id=     "03";//150+идентификатор записи
AnsiString kadr_num=    "04";//Счетчик записей, порядковый номер по выходу
AnsiString kadr_data=   "05";//Дата в обратном порядке - год, месяц, число, по 2 цифры вместе
AnsiString kadr_time=   "06";//Время - часы, минуты, секунды, по 2 цифры вместе
AnsiString work_type=   "07";//Код проводимой работы, 2 - бурение
AnsiString hole_depth=  "08";//Глубина по стволу, с точкой (можно писать -999.25 в параметрах)
AnsiString hole_vert=   "09";//Глубина по вертикали, с точкой (можно писать -999.25 в параметрах)
AnsiString bit_depth=   "10";//Глубина долота по стволу, с точкой
AnsiString bit_vert=    "11";//Глубина долота по вертикали, с точкой
//07-инклинометрия
AnsiString zts_zu=      "0713";//Зенитный угол ЗТС
AnsiString zts_az=      "0714";//Азимут сырой ЗТС
AnsiString zts_azk=     "0715";//Азимут корр ЗТС
AnsiString zts_az_otk=  "0716";//OTK магн ЗТС
AnsiString zts_otk=     "0717";//OTK грав ЗТС
AnsiString zts_Ht=      "0723";//Htotal, Гаусс
AnsiString zts_Hx=      "0724";//Hx, Гаусс
AnsiString zts_Hy=      "0725";//Hy, Гаусс
AnsiString zts_Hz=      "0726";//Hz, Гаусс
AnsiString zts_Gt=      "0727";//Gtotal
AnsiString zts_Gx=      "0728";//Gx
AnsiString zts_Gy=      "0729";//Gy
AnsiString zts_Gz=      "0730";//Gz
AnsiString zts_temp=    "0731";//Температура ЗТС
AnsiString ndm_depth=   "0732";//Глубина датчика ЗУ_НДМ, м
AnsiString ndm_zu=      "0733";//Зенитный угол НДМ
//08-
AnsiString ks_depth=    "0814";//Глубина по стволу датчика КС, м
AnsiString ks_raw=      "0815";//Показания датчика КС, ом*м
AnsiString gk1_depth=   "0821";//Глубина по стволу датчика ГК1, м
AnsiString gk1_raw=     "0823";//Показания датчика ГК1, мкР/ч
AnsiString gk1_korr=    "0824";//Показания датчика ГК1 корректированные, мкР/ч
AnsiString gk2_depth=   "0825";//Глубина по стволу датчика ГК2, м
AnsiString gk2_raw=     "0827";//Показания датчика ГК2, мкР/ч
AnsiString gk2_korr=    "0828";//Показания датчика ГК2 корректированные, мкР/ч
//AnsiString zts_temp=    "0836";//Температура ЗТС
//AnsiString ndm_zu=      "0847";//Зенитный угол НДМ
//AnsiString ndm_gk=      "0828";//ГК интегр НДМ, мкР/ч 
//09-
AnsiString ndm_davl=    "0914";//Давление внутр НДМ, МПа
AnsiString ndm_nagr=    "0916";//Нагрузка на долото НДМ, кН
AnsiString ndm_mom=     "0918";//Крутящий момент НДМ, кН
AnsiString ndm_obor=    "0919";//Обороты долота НДМ, об/мин
AnsiString zts_obor=    "0920";//Обороты генратора ЗТС, об/мин
AnsiString zts_vibr=    "0921";//Вибрации ЗТС, g
AnsiString ndm_temp=    "0923";//Температура НДМ
