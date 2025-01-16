#include "data_packet.h"
#include "string.h" 
#include "transmit_task.h"


	uint16_t data_address;
//调用打包
static uint8_t protocol_packet_pack(uint16_t cmd_id, uint8_t *p_data, uint16_t data_length, uint8_t sof, uint8_t *tx_buf)//tx_buf在被调用的函数中定义
{
//  static uint8_t seq;
  
	//加上头尾
  uint16_t frame_length = HEADER_LEN + CMD_LEN + data_length + CRC_LEN;
	//在函数里创建了个指针！？
  frame_header_t *p_header = (frame_header_t*)tx_buf;
  
  p_header->sof          = sof;
  p_header->data_length  = data_length;
  
  
//  if (sof == UP_REG_ID)
//  {
//    if (seq++ >= 255)
//      seq = 0;
//    
//    p_header->seq = seq;
//  }
//  else
//  {
    p_header->seq = 0;
//  }
  
  
  memcpy(&tx_buf[HEADER_LEN], (uint8_t*)&cmd_id, CMD_LEN);//在frame_header后+cmd
  append_crc8_check_sum(tx_buf, HEADER_LEN);//将CRC8附加到数据的末尾
  memcpy(&tx_buf[HEADER_LEN + CMD_LEN], p_data, data_length);//将所输入p_data放入cmd后
  append_crc16_check_sum(tx_buf, frame_length);//将CRC16附加到数据的末尾
  
  return *tx_buf;
}
//最终,返回一个输出数据的地址
void data_packet_pack(uint16_t cmd_id, uint8_t *p_data, uint8_t *data_output, uint16_t data_length, uint8_t sof)
{
	uint8_t tx_buf[PROTOCAL_FRAME_MAX_SIZE];
	uint8_t i;
//  uint16_t frame_length = HEADER_LEN + CMD_LEN + len + CRC_LEN;
  data_address=protocol_packet_pack(cmd_id, p_data, data_length, sof, tx_buf);
	for(i=0;i<data_length+JUDGE_FORMAT_LENGTH;i++)
	{
		data_output[i]=tx_buf[i];
	}
  return;
}

