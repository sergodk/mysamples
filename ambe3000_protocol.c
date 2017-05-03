//---------------------------------------------------------------------------
#include "ambe3000_protocol.h"
//---------------------------------------------------------------------------

//-----------------------Инициализация структуры кадра Ambe3000--------------------------------
void InitAmbe3000Frame(
Ambe3000Frame (*frm), //Указатель на структуру кадра
bool crc_need,		  //Кадр с контрольной суммой или без 
int frametype) 		  //Тип кадра
{
(*frm).size=0;
(*frm).crc_on=crc_need;
(*frm).data[0]=0x61;
(*frm).data[1]=0x00; //frame length MSB
(*frm).data[2]=0x00; //frame length LSB
(*frm).data[3]=frametype;
(*frm).size=4;
(*frm).fieldslength=0;
}
//-------------------------Добавление поля в кадр--------------------------------------------------
void AddFieldAmbe3000Frame(
Ambe3000Frame (*frm), //Указатель на структуру кадра
int fieldtype,		  //Тип поля
int fielddatasize,	  //Размер данных поля
BYTE *fielddata) 	  //Указатель на массив данных поля
{
if (fieldtype!=fieldCodes[PKT_UNKNOWN]) {				// для добавления данных в пакет с использованием идентификатора поля
	(*frm).data[(*frm).size++]=fieldtype;
}
if (fielddatasize>0) {
        memcpy(&((*frm).data[(*frm).size]),fielddata,fielddatasize);
        (*frm).size+=fielddatasize;
}

}
//----------------------------------Вычисляет контрольную сумму-----------------------------------------
BYTE CRC_XOR(
BYTE *buf, //Указатель на буффер с данными для рассчета КС
int n)     //Число байт для которых будет рассчитана КС
{
BYTE crc=0;
int i;
for (i=0;i<n;i++) {
        crc^=buf[i];
}
return crc;        
}
//--------------------------Устанавливает бит в байте в 0 или 1-------------------------------------------------
void SetBitInByte(
BYTE *b,    //Указатель на байт данных
int nbit,	//Номер бита
bool v) 	//Значение бита
{
*b=v?*b|(1<<nbit):*b&~(1<<nbit);
}
//-------------------------Подготавливает кадр к передаче--------------------------------------------------
bool FinalizeAmbe3000Frame(
Ambe3000Frame (*frm)  //Указатель на структуру кадра
) 
{
if((*frm).size>MAXFRAMESIZE) {
        return false;
}
if ((*frm).crc_on) {
        (*frm).data[(*frm).size++]=0x2f; // CRC field id
        (*frm).data[(*frm).size]=0x00; // CRC value
        (*frm).fieldslength=(*frm).size+1-4;
        (*frm).data[1]=0xFF&(((*frm).fieldslength)>>8); //frame length MSB
        (*frm).data[2]=0xFF&((*frm).fieldslength);       //frame length LSB

}
else {
        (*frm).fieldslength=(*frm).size-4;
        (*frm).data[1]=0xFF&(((*frm).fieldslength)>>8); //frame length MSB
        (*frm).data[2]=0xFF&((*frm).fieldslength);       //frame length LSB
}

if ((*frm).crc_on) {
        (*frm).data[(*frm).size]=CRC_XOR((*frm).data+1,(*frm).size-1);
        (*frm).size++;
}
return true;
}
//-------------------------Получает из буффера структуру с кадром если это возможно-------------------------------------------
unsigned char GetAmbe3000Frame(unsigned char *buffer, //Указатель на буффер с данными
int bufsize, 	//Размер буффера
Ambe3000Frame (*gfr)) 	//Указатель на структуру кадра
{

unsigned char *buf=buffer;
int frmlength=0;
int begin_offset=0;

for (;buf[0]!=0x61&&begin_offset<bufsize;buf++,begin_offset++);  // find start of frame

if(buf[0]!=0x61) {                                //start of frame not found
        return 0;
}

//if (buf[3]==TYPE_FRAME_CONTROL) {               // only control frames we need
if (buf[3]==TYPE_FRAME_CONTROL||buf[3]==TYPE_FRAME_CHANDATA) { 
        frmlength=((int)buf[1]<<8)|buf[2];
        (*gfr).fieldslength=frmlength;
        (*gfr).size=frmlength+4;
        if((*gfr).size>(bufsize-begin_offset)) {    //buffer size is less than needed for full frame
                return 0;
        }
        else {
                memcpy((*gfr).data,buf,frmlength+4);
                return 1;
        }
}
return 0;
}
//-----------------------------------------Проверяет является ли байт идентификатором поля----------------------------------
int IsFieldID(BYTE b) {
int i;
for (i=0;i<FIELD_TYPES_COUNT;i++) {
    if (b==fieldCodes[i])
        return i;
}
return -1;
}
//---------------------------------------------------------------------------
unsigned char GetAmbe3000Field(unsigned char *buffer,int bufsize, Ambe3000Field (*gfield),unsigned char is_in_frame) {
int i=0;
i=IsFieldID(buffer[0]);
if (i>0 && i!=PKT_UNKNOWN) {
        (*gfield).type=(fieldType)i;
        if (is_in_frame) {                                              // if frame is input (from AMBE3000)
                (*gfield).datasize=fieldResponseLength[(*gfield).type];     //we parse response
        }else  {                                                        // if frame is output (to AMBE3000)
                (*gfield).datasize=fieldDataLength[(*gfield).type];           //we parse request
        }
        if ((*gfield).datasize<bufsize)  {
                memcpy((*gfield).data,buffer+1,(*gfield).datasize);
                return 1;
        }
        else {
                return 0;
        }
}
else {
        return 0;
}
}

//---------------------------------------------------------------------------
/*
void ParseAmbe3000Frame(Ambe3000Frame frm, Ambe3000Field fields[], int *nfields,bool is_in_frame) {
BYTE *buffer;
int bufsize;
int i=0;
bufsize=frm.size-4;
buffer=frm.data+4;

while (GetAmbe3000Field(buffer,bufsize,&fields[i],is_in_frame)&&i<MAXFIELDSINFRAME) {
       bufsize-=(fields[i].datasize+1);
       buffer+=(fields[i].datasize+1);
       i++;
}
*nfields=i;
}
*/
//---------------------------------------------------------------------------
bool ParseAmbe3000Frame(Ambe3000Frame *frm, Ambe3000Field fields[], int *nfields,unsigned char is_in_frame, unsigned char *channel_data) {
unsigned char *buffer;
int bufsize;
int i=0;
bufsize=(*frm).size-4;
buffer=(*frm).data+4;
if ((*frm).data[3]==TYPE_FRAME_CHANDATA) {    //process channel data - vocodered speech from AMBE3000
        *channel_data=true;
		
        if ((*frm).data[4]==0x01) {
                fields[0].type=PKT_UNKNOWN; //doesn't matter, we set unknown field type, but assumes that the field conatains bits number in channel data (vocoded speech)
                fields[0].datasize=1;
                fields[0].data[0]=(*frm).data[5];

                fields[1].type=PKT_UNKNOWN; //doesn't matter, we set unknown field type, but assumes that the field conatains channel data (vocoded speech)
                fields[1].datasize=fields[0].data[0]/8;
                memcpy(fields[1].data,(*frm).data+6,fields[1].datasize);
                i=2;
        }

}
else if ((*frm).data[3]==TYPE_FRAME_CONTROL) { //process control data from AMBE3000
        *channel_data=false;
		
        while (GetAmbe3000Field(buffer,bufsize,&fields[i],is_in_frame)&&i<MAXFIELDSINFRAME) {
               bufsize-=(fields[i].datasize+1);
               buffer+=(fields[i].datasize+1);
               i++;
        }
		
}
else {
	return false;
}
*nfields=i;
return true;
}
//---------------------------------------------------------------------------
void CreateAmbe3000SpeechFrame (Ambe3000Frame *afr, BYTE sizebits,BYTE *speechdatabuf) {
	BYTE v;
	fieldType current_field;

	InitAmbe3000Frame(afr,1,TYPE_FRAME_CHANDATA);
	current_field=PKT_UNKNOWN;
	v=1;
	AddFieldAmbe3000Frame(afr,fieldCodes[current_field],1,&v);
	AddFieldAmbe3000Frame(afr,fieldCodes[current_field],1,&sizebits);
	AddFieldAmbe3000Frame(afr,fieldCodes[current_field],sizebits/8,speechdatabuf);
	FinalizeAmbe3000Frame(afr);
}
//---------------------------------------------------------------------------
