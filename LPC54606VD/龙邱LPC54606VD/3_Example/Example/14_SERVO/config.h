/*!
  * @file     config.h
  *
  * @brief    ����һ�������ļ� ���һЩȫ�ֺ궨��
  *
  * @company  �����������ܿƼ�
  *
  * @author   LQ-005
  *
  * @note     
  *
  * @version  V1.0
  *
  * @Software IAR8.22 or MDK5.25
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2019/10/28 ����һ
  */ 

#ifndef __CONFIG_H
#define __CONFIG_H

/* �жϿ��غ궨�� */
#define EnableInterrupts  __asm(" CPSIE i");//�����ж�
#define DisableInterrupts __asm(" CPSID i");//�����ж�


/* ������Ҫ���һЩȫ�ֺ궨�� */

/* ��ʾ��Ļ */
//#define OLED
#define TFT1_8


#endif
