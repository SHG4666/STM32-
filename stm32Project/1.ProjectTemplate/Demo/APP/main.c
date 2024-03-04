#include "public.h"

void delay(unsigned int milliseconds) {
    // ����ÿ��ѭ��������ʱԼ1����
    unsigned int i,j;
    for (i = 0; i < milliseconds; ++i) {
        // ������������һЩ��ʱ�޹صĴ�����룬�Է�ֹ�������Ż���ѭ��
        // ���磬�������һЩ���������������һ����CPUʱ��
        volatile int dummy = 0;
        for (j = 0; j < 1000; ++j) {
            dummy++;
        }
    }
}

/**
��ʼ��С�Ƶ�����
**/
void ControlLightSwitch(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitTypeDefStruct;
    GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitTypeDefStruct.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitTypeDefStruct);
    
    GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitTypeDefStruct.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitTypeDefStruct);
    
}

void USARTInit(){
    
}
int main(){
    ControlLightSwitch();
   while(1){
       GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);
       GPIO_WriteBit(GPIOE,GPIO_Pin_5,Bit_RESET);
       delay(10000);
       GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET);
       GPIO_WriteBit(GPIOE,GPIO_Pin_5,Bit_SET);
       delay(10000);
   } 
   return 0; 
}

 