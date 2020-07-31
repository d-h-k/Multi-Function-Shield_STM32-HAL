/*--------------------------------------------------------------
 * Filename : Multi_Function_Shield.c
 * Discription : Multi-function Shield Board Control
 * Inccludes :
 * Original source from :
 * Modify by :
 *--------------------------------------------------------------
 */
/*==============================================================================================*/
#include "Multi_Function_Shield.h"
#include "stm32f1xx_hal_tim.h"
/*==============================================================================================*/
extern uint16_t MsageTime;
volatile volatile uint8_t Scan_Bit=0;                   // FND Scan 출력비트 데이터
volatile uint8_t Scan_Data=0;                           // FND Scan 세그멘트 데이터 글자가 세그멘트로 바꾸어진 데이터
volatile uint8_t FND_Buffer[4]={0x40,0x40,0x40,0x40};   // 자리수 1000,100,10,1 초기 "0.0.0.0." 모두 표시(테스트를 위하여)
volatile uint8_t Scan_Ctr=0;                            // 4자리이므로 0,1,2,3 을 반복한다.
/*==============================================================================================*/
volatile uint32_t sw_new_data=0;
volatile uint32_t sw_old_data=0xC3;                     // SW 모두 Off 상태로
volatile uint32_t sw_data=0;
volatile uint32_t sw_no=0;
volatile uint16_t chatering_time=0;
/*==============================================================================================*/
volatile uint16_t LED1_Time=0;
volatile uint16_t LED2_Time=0;
volatile uint16_t LED3_Time=0;
volatile uint16_t LED4_Time=0;
volatile uint16_t Buz_Time=0;
/*==============================================================================================*/
/*                         0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F */
const uint8_t Segment[16]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
const uint8_t Scan_Col[4]={0x01,0x02,0x04,0x08};



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*htim)
{    // 타이머 인터럽트 입니다.
    if(htim->Instance==TIM2)
    {
        //----------------------------------
        FND_Disp();
        //----------------------------------
        // 채터링 없애는 기능으로 TMR2 를 1msec 로 설정 하였기때문에 1msec 마다 옵니다.(10msec 로 할경우도 있습니다.)
        sw_new_data=(GPIOB->IDR)&0x01;        // PB0,SW3 데이터를 읽습니다.(포트를 통체로 읽어들입니다.)
        sw_new_data=sw_new_data+((GPIOA->IDR)&0x12);    // PA1,4 SW1,2 데이터 와+합처줍니다.
        //---------------------------------
        if(chatering_time==0)
        {
            chatering_time=20;    // 20msec
            //if(sw_data==0)
            //{    // 0 이아닐때는 메인에서 처리하지 않은것임
                sw_data=sw_new_data^sw_old_data;    // 스위치에 변화가 있는지 검사를 하기위한 1단계 작업이다.
                // 아래 두방법은 아주 재미있는 스위치 구별 법이니 각 각 한번씩 실행하여 보시기 바랍니다.
                sw_data=sw_data&sw_old_data;        // 1단계 데이터를 과거 데이터와&를 취하면 버튼을 누르는순간의 데이터를 분류한다.
                //sw_data=sw_data&sw_new_data;        // 만약 1단계 데이터를 방금 들어온 데이터에&를 취하면 버튼을 떼는 순간의 데이터를 분류할수 있다.
            //}
            sw_old_data=sw_new_data;
            if(sw_data)sw_no=sw_data;
        }
        else chatering_time--;
        //----------------------------------
        if(MsageTime)MsageTime--;
        //----------------------------------
        if(LED1_Time)
        {
            LED1_Time--;
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
            if(LED1_Time==0)HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
        }
        //----------------------------------
        if(LED2_Time)
        {
            LED2_Time--;
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
            if(LED2_Time==0)HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
        }
        //----------------------------------
        if(LED3_Time)
        {
            LED3_Time--;
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
            if(LED3_Time==0)HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
        }
        //----------------------------------
        if(LED4_Time)
        {
            LED4_Time--;
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
            if(LED4_Time==0)HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);
        }
        //----------------------------------
        if(Buz_Time)
        {
            Buz_Time--;
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
            if(Buz_Time==0)HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
        }
    }
}



//======================================================================================
// 인터럽트에서 호출되면 버퍼에 있는 세그멘트 데이터를 출력하여 준다.
// 현재 4디지트 이므로 4바이트의 세그멘트 데이터를 출력 하게 된다.
// 메인 함수에서는 버퍼에 세그멘트데이터를 넣어주기만 하면 자동 표시된다.
void FND_Disp(void)
{
    uint8_t i;
    Scan_Bit=Scan_Col[Scan_Ctr];        // 스켄용 비트데이터 로드(FND Comon 데이터)
    Scan_Data=FND_Buffer[Scan_Ctr];    // FND 세그멘트 데이터 로드
    // 멀티펑션 보드는 74HC595 에 데이터를 먼저 보내야한다.(회로도 참조)
    for(i=0;i<8;i++)
    {
        if(Scan_Data&0x80)HAL_GPIO_WritePin(SDI_GPIO_Port,SDI_Pin,GPIO_PIN_SET);
        else HAL_GPIO_WritePin(SDI_GPIO_Port,SDI_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(SFT_GPIO_Port,SFT_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(SFT_GPIO_Port,SFT_Pin,GPIO_PIN_RESET);
        Scan_Data=Scan_Data<<1;
    }
    // 멀티펑션 보드는 74HC595 에 FND Com 데이터를 나중 보내야 한다.(회로도 참조)
    for(i=0;i<8;i++)
    {
        if(Scan_Bit&0x80)HAL_GPIO_WritePin(SDI_GPIO_Port,SDI_Pin,GPIO_PIN_SET);
        else HAL_GPIO_WritePin(SDI_GPIO_Port,SDI_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(SFT_GPIO_Port,SFT_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(SFT_GPIO_Port,SFT_Pin,GPIO_PIN_RESET);
        Scan_Bit=Scan_Bit<<1;
    }
    // 위에서 두 데이터를 모두 보냈으므로 래치 펄스를 출력 한다.
    // 74HC595 는 이때 비로소 출력이 나가므로 세그멘트도 이때 바뀐다
    HAL_GPIO_WritePin(LCH_GPIO_Port,LCH_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCH_GPIO_Port,LCH_Pin,GPIO_PIN_RESET);
    Scan_Ctr++;        // 다음 스켄 위치로 변경
    if(Scan_Ctr>3)Scan_Ctr=0;    // 4자리이므로 0,1,2,3 을 반복한다.하여 4이면 0 으로 한다.
}
//===========================================
// 메인 함수에서 사용하며 수자데이터 와 표시할시작위치와 표시할 길이를 보내오면 내용을 해당 위치부터 세그멘트데이터로 바꾸어 길이 만큼 저장한다.
// 4디지트중 최상위가 0 끝이 3 이다.FND_bcd(12,0,2)라고 보내왔다면 "12xx" 이 표시된다.xx 는 앞서 있던 데이터 그대로이다."5678" 이 있었다면 "1278" 이된다.
// 만약 FND_bcd(127,0,2)라고 보내왔다면 길이가 2 이므로 마지막 2디지트 27 만 표시된다.하여 "2778" 이 된다.길이가 3 이라면 "1278" 이 된다.
void FND_bcd(long da,char start,char len)
{
    unsigned char i=0,temp[16];
    unsigned char j,f;
    unsigned char k=0;
    unsigned char dglen;
    long ddd=da;
    // 십진 디지트로 변환은 32비트가 가능한 10자리를 변환 한다
    for(i=0;i<10;i++)
    {
        temp[i]=0;
        temp[i]=(unsigned char)(ddd%10&0x0F);
        ddd=ddd/10;
    }
    // FND_bcd(12,0,2); 라고 했다면 12xx 로 표시 xx 는 앞서 있던 데이터임
    // FND_bcd(34,2,2); 라고 했다면 xx34 로 표시
    // 위 내용을 둘다 실행 했다면 1234 가 표시 됨.변환한 값이 12345 이지만 길이가 2라면 뒤 두자 45 만 표시한다.
    dglen=len+start;
    for(i=start;i<dglen;i++)
    {
        j=(unsigned char)(temp[(dglen-1)-i]);
        if(j==0)
        {
            if(k==1)
            {
                f=Segment[j];                // FND Data 변환
                if(i<4)FND_Buffer[i]=f;        // 변환된것 표시  i=0~3
            }
            else
            {
                if(i==(dglen-1))
                {
                    f=Segment[j];
                    if(i<4)FND_Buffer[i]=f;
                }
                else
                {
                    //if(i<4)FND_Buffer[i]=0xC0;    // 그냥 0 을 표시 할때
                    if(i<4)FND_Buffer[i]=0xFF;        // 앞자리가 0이면 FND Off 할때
                }
            }
        }
        else
        {
            f=Segment[j];
            if(i<4)FND_Buffer[i]=f;
            //put2_char(j+'0');    // 모니터
            k=1;
        }
    }
}
//===========================================
// 메인 함수에서 사용하며 수자데이터 와 표시할 시작위치와 표시할 길이를 보내오면 내용을 해당 위치부터 세그멘트데이터로 바꾸어 길이 만큼 저장한다.
// 4디지트중 최상위가 0 끝이 3 이다.FND_hex(12,0,2)라고 보내왔다면 "0Cxx" 이 표시된다.xx 는 앞서 있던 데이터 그대로이다."5678" 이 있었다면 "0C78" 이된다.
// 만약 FND_hex(127,0,2)라고 보내왔다면 길이가 2 이므로 마지막 2디지트 7F 만 표시된다.하여 "7F78" 이 된다.길이가 3 이라면 "07F8" 이 된다.
// hex 표시이므로 위 십진과 내용만 다른것이며 사용법은 같으다.만약 32비트 값인 FND_hex(0x56789ABC,2,2)로 왔다면 마지막 BC 만 표시된다.
void FND_hex(long da,char start,char len)
{
    unsigned char i=0,temp[16];
    unsigned char j,f;
    unsigned char k=0;
    long ddd=da;
    unsigned char dglen;
    // 개별 디지트로 변환
    for(i=0;i<8;i++)// 32비트
    {
        temp[i]=0;
        temp[i]=(unsigned char)(ddd%16);
        ddd=ddd/16;
    }
    // FND 표시
    dglen=len+start;
    for(i=start;i<dglen;i++)
    {
        j=(unsigned char)(temp[(dglen-1)-i]);
        if(j==0)
        {
            if(k==1)
            {
                f=Segment[j];                            // FND 세그멘트 Data 변환
                if(i<4)FND_Buffer[i]=f;        // 변환된것 버퍼에 저장(이것이 인터럽트에서 표시 된다)i=0~3
            }
            else
            {
                if(i==(dglen-1))
                {
                    f=Segment[j];
                    if(i<4)FND_Buffer[i]=f;
                }
                else
                {
                    //if(i<4)FND_Buffer[i]=0x7F;    // FND Off
                    if(i<4)FND_Buffer[i]=0xC0;        // FND '0'
                }
            }
        }
        else
        {
            f=Segment[j];
            if(i<4)FND_Buffer[i]=f;
            k=1;
        }
    }
}
//===========================================
// FND 테스트를 위하여 전체를 같은 데이터로 표시한다.
// 특수 문자 세그멘트 데이터를 모를때 해봐도 된다.
void fndbuf_set(uint8_t data)
{
    uint8_t i;
    for(i=0;i<4;i++)
    {
        FND_Buffer[i]=data;    // FND 모두 data 로  표시
    }
}


void mtfs_ledblink(uint8_t LED) {//1,2,3,4
  if(LED==1) {
    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
  }
  if(LED==2) {
    HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
  }
  if(LED==3) {
    HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
  }
  if(LED==4) {
    HAL_GPIO_TogglePin(LED4_GPIO_Port,LED4_Pin);
  }
}


uint32_t mtfs_switchRead(uint32_t SW) {
  if(SW==1) {
    if(0==HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin)){
      return 1;//pressed
    }
    else 0;
  }
  if(SW==2) {
    if(1 == HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)){
      return 1;
    }
    else 0;
  }

  if(SW==3) {
    if(0==HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin)){
      return 1;
    }
    else 0;
  }
  if(SW==4) {

  }


}
