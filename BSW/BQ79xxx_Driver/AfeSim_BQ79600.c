


#include "AfeSim_BQ79600.h"

static AfeSim_BQ79600_ManagerType BQ79600_Manager;
AfeSim_BQ79600_ManagerType* const BQ79600Man = &BQ79600_Manager;


void AfeSim_BQ79600_DeviceManagerInit(void)
{
	uint8 devId = 0;
	BQ79600Man->stat.RecvTimes = 0;
	BQ79600Man->stat.SpiErrorTimes = 0;
	BQ79600Man->stat.TransTimes = 0;

	BQ79600Man->stack.AutoAddrState = 0;
	BQ79600Man->stack.commDir = 0;
	BQ79600Man->stack.StackNum[0] = 0;
	BQ79600Man->stack.StackNum[1] = 0;
}


void AfeSim_BQ79600_Init(void)
{
	xil_printf(		"\r\n Texas Instruments Analog Front End Integrated Circuit Simulator"
	    		   	"\r\n BQ79600 - BQ79616 - BQ79718\r\n");

	AfeSim_BQ79600_DeviceManagerInit();

	AfeSim_HAL_BQ79600_Init();
}

void AfeSim_BQ79600_PrintMosi(void)
{
	uint8 loop;
	printf("MOSI: ");
	for(loop = 0u; loop < BQ79600Man->spi.StringLength; loop++)
	{
		printf("%02X ", BQ79600Man->spi.MOSI_RxData[loop]);
	}
	printf("\r\n");
}

void AfeSim_BQ79600_PrintMiso(uint8 txLength)
{
	uint8 loop;
	printf("  MISO: ");
	for(loop = 0u; loop < txLength; loop++)
	{
		printf("%02X ", BQ79600Man->spi.MISO_TxData[loop]);
	}
	printf("\r\n\r\n");
}

/* 写指令响应
 * 已进行AFE仿真驱动升级
 * Action: 需要测试升级后响应时间 Pass */
void AfeSimX1_BQ79600_WriteCommandProcess(void)
{
	uint8 retVal;
	uint8 devId;
	uint8 offset;
	uint8 loop;
	if(CRC16_Check_Modbus(BQ79600Man->spi.MOSI_RxData, BQ79600Man->spi.StringLength) != 0u)
	{
		printf("\r\nERROR CRC16 INVALID\r\n");
		return;
	}
	switch(BQ79600Man->cmd.CmdType)
	{
	case BQ79600_SINGLE_WRITE:
		devId = BQ79600Man->spi.MOSI_RxData[1u];
		BQ79600Man->cmd.deviceId = BQ79600Man->spi.MOSI_RxData[1u];
		BQ79600Man->cmd.rwRegisterAddr = (BQ79600Man->spi.MOSI_RxData[2u] << 8u) + BQ79600Man->spi.MOSI_RxData[3u];
#if AFESIM_SIMULEVEL > 0
		offset = 4u;
		retVal = AfeSimX1_DeviceSingleWrite(devId,
					BQ79600Man->cmd.rwRegisterAddr,
					&BQ79600Man->spi.MOSI_RxData[offset],
					BQ79600Man->cmd.rwBytes,
					BQ79600Man->stack.commDir);
		if(retVal != TRUE)
		{
			BQ79600Man->stat.SpiErrorTimes++;
		}
#else
		for(offset = 4u, loop = 0u; loop < BQ79600Man->cmd.rwBytes; loop++)
		{
			AfeSim_Reg[devId][BQ79600Man->cmd.rwRegisterAddr + loop] = BQ79600Man->spi.MOSI_RxData[offset + loop];
		}
#endif
		break;
	case BQ79600_STACK_WRITE:
		BQ79600Man->cmd.deviceId = AFESIM_DEVID_STACK;
		BQ79600Man->cmd.rwRegisterAddr = (BQ79600Man->spi.MOSI_RxData[1u] << 8u) + BQ79600Man->spi.MOSI_RxData[2u];
#if AFESIM_SIMULEVEL > 0
		offset = 3u;
		retVal = AfeSimX1_DeviceStackWrite(
					BQ79600Man->cmd.rwRegisterAddr,
					&BQ79600Man->spi.MOSI_RxData[offset],
					BQ79600Man->cmd.rwBytes,
					BQ79600Man->stack.commDir,
					0/*is not broadcast*/);
		if(retVal != TRUE)
		{
			BQ79600Man->stat.SpiErrorTimes++;
		}
#else
		for(devId = 1u; devId <= BQ79600Man->stack.HStackNum; devId++)
		{
			for(offset = 3u, loop = 0u; loop < BQ79600Man->cmd.rwBytes; loop++)
			{
				AfeSim_Reg[devId][BQ79600Man->cmd.rwRegisterAddr + loop] = BQ79600Man->spi.MOSI_RxData[offset + loop];
			}
		}
#endif
		break;
	case BQ79600_BROADCAST_WRITE:
		BQ79600Man->cmd.deviceId = AFESIM_DEVID_BROADCAST;
		BQ79600Man->cmd.rwRegisterAddr = (BQ79600Man->spi.MOSI_RxData[1u] << 8u) + BQ79600Man->spi.MOSI_RxData[2u];
#if AFESIM_SIMULEVEL > 0
		offset = 3u;
		retVal = AfeSimX1_DeviceStackWrite(
					BQ79600Man->cmd.rwRegisterAddr,
					&BQ79600Man->spi.MOSI_RxData[offset],
					BQ79600Man->cmd.rwBytes,
					BQ79600Man->stack.commDir,
					1/*is broadcast*/);
		if(retVal != TRUE)
		{
			BQ79600Man->stat.SpiErrorTimes++;
		}
#else
		for(devId = 0u; devId <= BQ79600Man->stack.HStackNum; devId++)
		{
			for(offset = 3u, loop = 0u; loop < BQ79600Man->cmd.rwBytes; loop++)
			{
				AfeSim_Reg[devId][BQ79600Man->cmd.rwRegisterAddr + loop] = BQ79600Man->spi.MOSI_RxData[offset + loop];
			}
		}
#endif
		break;
		/* 反向广播写仅适用于配置通信方向！*/
	case BQ79600_BROADCAST_REVERSE_WRITE:
		BQ79600Man->cmd.deviceId = AFESIM_DEVID_BROADCAST;
		BQ79600Man->cmd.rwRegisterAddr = (BQ79600Man->spi.MOSI_RxData[1u] << 8u) + BQ79600Man->spi.MOSI_RxData[2u];
#if AFESIM_SIMULEVEL > 0
		offset = 3u;
		retVal = AfeSimX1_DeviceStackWrite(
					BQ79600Man->cmd.rwRegisterAddr,
					&BQ79600Man->spi.MOSI_RxData[offset],
					BQ79600Man->cmd.rwBytes,
					BQ79600Man->stack.commDir,
					2/*is broadcast reverse*/);
		if(retVal != TRUE)
		{
			BQ79600Man->stat.SpiErrorTimes++;
		}
#else
		for(devId = 0u; devId <= BQ79600Man->stack.HStackNum; devId++)
		{
			for(offset = 3u, loop = 0u; loop < BQ79600Man->cmd.rwBytes; loop++)
			{
				AfeSim_Reg[devId][BQ79600Man->cmd.rwRegisterAddr + loop] = BQ79600Man->spi.MOSI_RxData[offset + loop];
			}
		}
#endif
		break;
	}
}

AfeSim_BQ79600_CommandClass AfeSim_BQ79600_CommandPreProcess(uint8 data)
{
	uint8 retFun;
	uint8 cmd = data & 0xF0;
	uint8 len = data & 0x0F;
	AfeSim_BQ79600_CommandClass retVal = BQ79600_INVALID_CMD;
	BQ79600Man->cmd.CmdType = cmd;
	/* Write Command Recognize */
	if(cmd == BQ79600_SINGLE_WRITE || cmd == BQ79600_STACK_WRITE || cmd == BQ79600_BROADCAST_WRITE || cmd == BQ79600_BROADCAST_REVERSE_WRITE)
	{
		BQ79600Man->cmd.rwBytes = len + 1;
		if(cmd != BQ79600_SINGLE_WRITE)
		{
			BQ79600Man->cmd.IsMultipleOp = 1;
		}
		else
		{
			BQ79600Man->cmd.IsMultipleOp = 0;
		}
		retVal = BQ79600_WRITE;
	}
	/* Read Command Recognize */
	else if(cmd == BQ79600_SINGLE_READ || cmd == BQ79600_STACK_READ || cmd == BQ79600_BROADCAST_READ)
	{
		retVal = BQ79600_READ;
		if(cmd != BQ79600_SINGLE_READ)
		{
			BQ79600Man->cmd.IsMultipleOp = 1;
		}
		else
		{
			BQ79600Man->cmd.IsMultipleOp = 0;
		}
	}
	/* Read Back Command Recognize */
	else if(cmd == BQ79600_READ_BACK_FF)
	{
		retVal = BQ79600_READ_BACK;
	}
	/* Ping Recognize */
	else if(data == BQ79600_CLEAR)
	{
		if(BQ79600Man->spi.StringLength == 1)	/* is comm clear */
		{
			printf("Comm Clear\r\n");
			retFun = AfeSim_AssertDeviceIsAwake(BQ79600_DEVID);
			BQ79600Man->stat.SpiErrorTimes++;
			if(TRUE == retFun)
			{
				AfeSim_HAL_BQ79600_MisoReplyFF();
				AfeSim_HAL_BQ79600_SetRdyPinHigh();
			}
		}
		else if(BQ79600Man->spi.StringLength > 1)	/* is wake ping */
		{
			printf("Wake Ping\r\n");
			retFun = AfeSim_OneDeviceWakeup(BQ79600_DEVID);
			if(TRUE == retFun)
			{
				AfeSim_HAL_BQ79600_MisoReplyFF();
				AfeSim_HAL_BQ79600_SetRdyPinHigh();
			}
		}
	}
	else
	{
		retVal = BQ79600_INVALID_CMD;
	}
	return retVal;
}

void AfeSim_BQ79600_ReadCmdSingleProcess(uint8 devId, uint16 *offset)
{
	uint8 loop = 0u;
	uint16 crc16 = 0u;

	uint16 dataCnt = 0u;
	BQ79600Man->spi.MISO_TxFullData[*offset + dataCnt++] = BQ79600Man->cmd.rwBytes - 1;
	BQ79600Man->spi.MISO_TxFullData[*offset + dataCnt++] = devId;
	BQ79600Man->spi.MISO_TxFullData[*offset + dataCnt++] = (BQ79600Man->cmd.rwRegisterAddr >> 8u) & 0xFF;
	BQ79600Man->spi.MISO_TxFullData[*offset + dataCnt++] = BQ79600Man->cmd.rwRegisterAddr & 0xFF;
	for(loop = 0u; loop < BQ79600Man->cmd.rwBytes; loop++)
	{
		BQ79600Man->spi.MISO_TxFullData[*offset + dataCnt++] = AfeSim_Reg[devId][BQ79600Man->cmd.rwRegisterAddr + loop];
	}
	crc16 = CRC16_Check_Modbus(BQ79600Man->spi.MISO_TxFullData+*offset, dataCnt);
	BQ79600Man->spi.MISO_TxFullData[*offset + dataCnt++] = (crc16 >> 8u) & 0xFF;
	BQ79600Man->spi.MISO_TxFullData[*offset + dataCnt++] = crc16 & 0xFF;
	*offset += dataCnt;
}

void AfeSimX1_BQ79600_ReadCommandProcess(void)
{
	int i;
	uint8 readEnable = 0;
	uint8 retFun;
	int devId = 0u;
	uint16 offset = 0u;
	uint8 txLength = 0u;
	uint8 devIdStart = 0u;
	uint8 devIdEnd = 0u;
	if(CRC16_Check_Modbus(BQ79600Man->spi.MOSI_RxData, BQ79600Man->spi.StringLength) != 0u)
	{
		printf("\r\nERROR CRC16 INVALID\r\n");
		return;
	}
	BQ79600Man->spi.MISO_TxData = BQ79600Man->spi.MISO_TxFullData;
	switch(BQ79600Man->cmd.CmdType)
	{
	case BQ79600_SINGLE_READ:
		devId = BQ79600Man->spi.MOSI_RxData[1u];
		BQ79600Man->cmd.deviceId = devId;
		BQ79600Man->cmd.rwRegisterAddr = (BQ79600Man->spi.MOSI_RxData[2u] << 8u) + BQ79600Man->spi.MOSI_RxData[3u];
		BQ79600Man->cmd.rwBytes = BQ79600Man->spi.MOSI_RxData[4u] + 1;
		BQ79600Man->spi.replyBufferLength = 6 + BQ79600Man->cmd.rwBytes;
		BQ79600Man->spi.ReadbackLastBytes = BQ79600Man->spi.replyBufferLength;
		devIdStart = devId;
		devIdEnd = devId;
#if AFESIM_SIMULEVEL > 0
		retFun = AfeSimX1_DeviceSingleRead(devId,
					BQ79600Man->cmd.rwRegisterAddr,
					BQ79600Man->spi.MISO_TxFullData,
					BQ79600Man->cmd.rwBytes,
					BQ79600Man->stack.commDir);
		if(retFun == TRUE)
		{
			readEnable = 1;
		}
#else
		readEnable = 1;
#endif
		break;
	case BQ79600_STACK_READ:
		BQ79600Man->cmd.deviceId = AFESIM_DEVID_STACK;
		BQ79600Man->cmd.rwRegisterAddr = (BQ79600Man->spi.MOSI_RxData[1u] << 8u) + BQ79600Man->spi.MOSI_RxData[2u];
		BQ79600Man->cmd.rwBytes = BQ79600Man->spi.MOSI_RxData[3u] + 1;
		BQ79600Man->spi.replyBufferLength = 6 + BQ79600Man->cmd.rwBytes;
		BQ79600Man->spi.replyBufferLength = BQ79600Man->spi.replyBufferLength * BQ79600Man->stack.StackNum[BQ79600Man->stack.commDir];
		BQ79600Man->spi.ReadbackLastBytes = BQ79600Man->spi.replyBufferLength;
		devIdStart = BQ79600Man->stack.StackNum[BQ79600Man->stack.commDir];
		devIdEnd = 1;
#if AFESIM_SIMULEVEL > 0
		retFun = AfeSimX1_DeviceStackRead(
							BQ79600Man->cmd.rwRegisterAddr,
							BQ79600Man->spi.MISO_TxFullData,
							BQ79600Man->cmd.rwBytes,
							BQ79600Man->stack.commDir,
							0/*is not broadcast*/);
		if(retFun == TRUE)
		{
			readEnable = 1;
		}
#else
		readEnable = 1;
#endif
		break;
	case BQ79600_BROADCAST_READ:
		BQ79600Man->cmd.deviceId = AFESIM_DEVID_BROADCAST;
		BQ79600Man->cmd.rwRegisterAddr = (BQ79600Man->spi.MOSI_RxData[1u] << 8u) + BQ79600Man->spi.MOSI_RxData[2u];
		BQ79600Man->cmd.rwBytes = BQ79600Man->spi.MOSI_RxData[3u] + 1;
		BQ79600Man->spi.replyBufferLength = 6 + BQ79600Man->cmd.rwBytes;
		BQ79600Man->spi.replyBufferLength = BQ79600Man->spi.replyBufferLength * (BQ79600Man->stack.StackNum[BQ79600Man->stack.commDir] + 1);
		BQ79600Man->spi.ReadbackLastBytes = BQ79600Man->spi.replyBufferLength;
		devIdStart = BQ79600Man->stack.StackNum[BQ79600Man->stack.commDir];
		devIdEnd = 0;
#if AFESIM_SIMULEVEL > 0
		retFun = AfeSimX1_DeviceStackRead(
							BQ79600Man->cmd.rwRegisterAddr,
							BQ79600Man->spi.MISO_TxFullData,
							BQ79600Man->cmd.rwBytes,
							BQ79600Man->stack.commDir,
							1/*is broadcast*/);
		if(retFun == TRUE)
		{
			readEnable = 1;
		}
#else
		readEnable = 1;
#endif
		break;
	}

#if AFESIM_SIMULEVEL > 0
	if(readEnable)
	{
		if(BQ79600Man->spi.replyBufferLength > 128)
		{
			txLength = 128;
			BQ79600Man->spi.ReadbackLastBytes = BQ79600Man->spi.replyBufferLength - 128;
		}
		else
		{
			txLength = BQ79600Man->spi.replyBufferLength;
			BQ79600Man->spi.ReadbackLastBytes = 0;
		}
		AfeSim_HAL_BQ79600_MisoDataWrite(BQ79600Man->spi.MISO_TxData, txLength);
		AfeSim_HAL_BQ79600_MisoReplyData();
		AfeSim_HAL_BQ79600_MisoReplyData();
		AfeSim_HAL_BQ79600_MisoReplyData();
		AfeSim_HAL_BQ79600_SetRdyPinHigh();
		AfeSim_HAL_BQ79600_SetRdyPinHigh();
		AfeSim_HAL_BQ79600_SetRdyPinHigh();
		//AfeSim_BQ79600_PrintMiso(txLength);
	}
	else
	{
		BQ79600Man->spi.replyBufferLength = 0;
		BQ79600Man->spi.ReadbackLastBytes = 0;
		AfeSim_HAL_BQ79600_MisoReplyFF();
		AfeSim_HAL_BQ79600_MisoReplyFF();
		AfeSim_HAL_BQ79600_MisoReplyFF();
		AfeSim_HAL_BQ79600_SetRdyPinLow();
		AfeSim_HAL_BQ79600_SetRdyPinLow();
		AfeSim_HAL_BQ79600_SetRdyPinLow();
	}
#else
	AfeSim_BQ79600_ReadCommandResponse();

	for(devId = devIdStart; devId >= devIdEnd ; devId--)
	{
		AfeSim_BQ79600_ReadCmdSingleProcess(devId, &offset);
	}

	if(BQ79600Man->spi.replyBufferLength > 128)
	{
		txLength = 128;
		BQ79600Man->spi.ReadbackLastBytes = BQ79600Man->spi.replyBufferLength - 128;
	}
	else
	{
		txLength = BQ79600Man->spi.replyBufferLength;
		BQ79600Man->spi.ReadbackLastBytes = 0;
	}
	AfeSim_HAL_BQ79600_MisoDataWrite(BQ79600Man->spi.MISO_TxData, txLength);
	AfeSim_HAL_BQ79600_MisoReplyData();
	AfeSim_HAL_BQ79600_SetRdyPinHigh();
	//AfeSim_BQ79600_PrintMiso(txLength);
#endif
}

uint32 AfeSim_BQ79600_ReadBackProcess(void)
{
	uint8 txLength;
	uint32 retVal=0;
	if(BQ79600Man->spi.ReadbackLastBytes > 0)
	{
		BQ79600Man->spi.MISO_TxData += 128;
		if(BQ79600Man->spi.ReadbackLastBytes > 128)
		{
			txLength = 128;
			BQ79600Man->spi.ReadbackLastBytes -= 128;
		}
		else
		{
			txLength = BQ79600Man->spi.ReadbackLastBytes;
			BQ79600Man->spi.ReadbackLastBytes = 0;
		}
		AfeSim_HAL_BQ79600_MisoDataWrite(BQ79600Man->spi.MISO_TxData, txLength);
		AfeSim_HAL_BQ79600_MisoReplyData();
		AfeSim_HAL_BQ79600_MisoReplyData();
		AfeSim_HAL_BQ79600_MisoReplyData();
		AfeSim_HAL_BQ79600_SetRdyPinHigh();
		AfeSim_HAL_BQ79600_SetRdyPinHigh();
		AfeSim_HAL_BQ79600_SetRdyPinHigh();
		//AfeSim_BQ79600_PrintMiso(txLength);
		retVal = 1;
	}
	else
	{
		AfeSim_HAL_BQ79600_MisoReplyFF();
		AfeSim_HAL_BQ79600_MisoReplyFF();
		AfeSim_HAL_BQ79600_MisoReplyFF();
		AfeSim_HAL_BQ79600_SetRdyPinHigh();
		AfeSim_HAL_BQ79600_SetRdyPinHigh();
		AfeSim_HAL_BQ79600_SetRdyPinHigh();
		retVal = 0;
	}
	return retVal;
}



/* AfeSimX1_BQ79600_CommandResp_DIR0_ADDR
 * 自动编址过程会广播写入设备ID
 * */
void AfeSimX1_BQ79600_CommandResp_DIR0_ADDR(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir)
{
	uint8 i;
	uint8* regVal = &AfeSim_Reg[phyDevId][BQ79600_REG_DIR0_ADDR];
	uint8* prevDevRegVal;

	if(phyDevId == BQ79600_DEVID)
	{
		*regVal = BQ79600_DEVID;
		BQ79600Man->stack.StackNum[AFESIM_DEVICE_USE_COMH] = *regVal;
	}
	else
	{
		if(commDir == AFESIM_DEVICE_USE_COMH) /*正链*/
		{
			prevDevRegVal = &AfeSim_Reg[phyDevId - 1][BQ79600_REG_DIR0_ADDR];
			*regVal = *prevDevRegVal + 1;
			printf("Dev %d in Stack Id = %d \r\n", phyDevId, *regVal);
		}
	}
}


/* AfeSim_BQ79600_CommandResp_DIR0_ADDR
 * when AfeDrv runs Auto-Addressing, host send broadcast write command
 * BroadcastWrite DIR0_ADDR = 0,
 * BroadcastWrite DIR0_ADDR = 1,
 * BroadcastWrite DIR0_ADDR = 2, ...
 * BroadcastWrite DIR0_ADDR = 15,
 * */
void AfeSim_BQ79600_CommandResp_DIR1_ADDR(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir)
{
	uint8 i;
	uint8* regVal = &AfeSim_Reg[phyDevId][BQ79600_REG_DIR1_ADDR];
	uint8* prevDevRegVal;

	if(phyDevId == BQ79600_DEVID)
	{
		*regVal = BQ79600_DEVID;
	}
	else
	{
		if(commDir == AFESIM_DEVICE_USE_COML) /*反链*/
		{
			if(phyDevId == AfeMan->afeChipRealNum) /*反链第一个设备*/
			{
				prevDevRegVal = &AfeSim_Reg[BQ79600_DEVID][BQ79600_REG_DIR1_ADDR];
			}
			else
			{
				prevDevRegVal = &AfeSim_Reg[phyDevId + 1][BQ79600_REG_DIR1_ADDR];
			}
			*regVal = *prevDevRegVal + 1;
			printf("Dev %d in Stack Id = %d \r\n", phyDevId, *regVal);
		}
	}
}


/* AfeSim_BQ79600_CommandResp_COMM_CTRL 0x0308
 * when AfeDrv runs Auto-Addressing, host send write command
 * BroadcastWrite          COMM_CTRL[STACK_DEV] = 1, COMM_CTRL[TOP_STACK] = 0
 * SingleWrite BaseDevice  COMM_CTRL[STACK_DEV] = 0, COMM_CTRL[TOP_STACK] = 0
 * SingleWrite TopDevice   COMM_CTRL[STACK_DEV] = 0, COMM_CTRL[TOP_STACK] = 1
 * */
void AfeSim_BQ79600_CommandResp_COMM_CTRL(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir)
{
	if(BQ79600Man->stack.AutoAddrState == AFESIM_AUTOADDR_STATE_ACTIVE)
	{

	}
}


/* BQ79600 Reg CONTROL1 0x0309 */
void AfeSimX1_BQ79600_CommandResp_CONTROL1(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir)
{
	AFESIM_BQ79600_REG_CONTROL1_RegType* CONTROL1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79600_REG_CONTROL1];
	uint8 wakeDevNum = 0;
	uint8 shutdownDevNum = 0;

	if(CONTROL1->reg.field.DIR_SEL == AFESIM_DEVICE_USE_COML)
	{
		printf("Dev %d Reverse Comm\r\n", phyDevId);
		if(phyDevId == BQ79600_DEVID)
			BQ79600Man->stack.commDir = AFESIM_DEVICE_USE_COML;
	}
	else
	{
		printf("Dev %d Forward Comm\r\n", phyDevId);
		if(phyDevId == BQ79600_DEVID)
			BQ79600Man->stack.commDir = AFESIM_DEVICE_USE_COMH;
	}

	if(CONTROL1->reg.field.ADDR_WR == TRUE)	/* AutoAddress Step1: Write CONTROL1[ADDR_WR] = 1 */
	{
		AfeSim_Reg[phyDevId][BQ79xxx_REG_COMM_CTRL] = 0;
		if(BQ79600Man->stack.AutoAddrState == AFESIM_AUTOADDR_INTERNAL_DLL8)
		{
			printf("Auto Addressing Enable\r\n");
			BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_STATE_ACTIVE;
		}
		else
		{
			printf("Auto Addressing Fail, Not Fully Synchronized Internal DLL\r\n");
		}
	}

	if(CONTROL1->reg.field.SOFT_RESET == TRUE)
	{
		printf("Soft Reset\r\n");
#if AFESIM_SIMULEVEL > 0
#else
#endif
	}

	if(CONTROL1->reg.field.GOTO_SLEEP == TRUE)
	{
#if AFESIM_SIMULEVEL > 0
		AfeSim_OneDeviceSleep(phyDevId);
#else
		printf("Goto Sleep Command, ");
		if(BQ79600Man->stack.Device[phyDevId].PowerStatus == AFESIM_DEVICE_POWER_WAKE)
		{
			printf("Wake Dev %d go to sleep success\r\n", phyDevId);
			BQ79600Man->stack.Device[phyDevId].PowerStatus = AFESIM_DEVICE_POWER_SLEEP;
		}
		else
		{
			printf("Dev %d is not wake and cannot go to sleep\r\n", phyDevId);
		}
#endif
	}

	if(CONTROL1->reg.field.GOTO_SHUTDOWN == TRUE)
	{
		printf("Goto Shutdown Command, ");
#if AFESIM_SIMULEVEL > 0
		AfeSim_OneDeviceShutdown(phyDevId);
#else
		if(BQ79600Man->stack.Device[phyDevId].PowerStatus == AFESIM_DEVICE_POWER_WAKE)
		{
			printf("Wake Dev %d go to shutdown success\r\n", phyDevId);
			BQ79600Man->stack.Device[phyDevId].PowerStatus = AFESIM_DEVICE_POWER_SHUTDOWN;
		}
		else
		{
			printf("Dev %d is not wake and cannot go to shutdown\r\n", phyDevId);
		}
#endif
	}

	if(CONTROL1->reg.field.SEND_SLPTOACT == TRUE)
	{
		printf("SleepToActive Command\r\n");
#if AFESIM_SIMULEVEL > 0
		wakeDevNum = AfeSim_DeviceWakeUp(BQ79600Man->stack.commDir, AFESIM_SLEEP_TO_ACTIVE);
#else
		for(phyDevId += 1; phyDevId < AFESIM_MAX_DEVICE_NUM; phyDevId++)
		{
			if(BQ79600Man->stack.Device[phyDevId].PowerStatus == AFESIM_DEVICE_POWER_SLEEP)
			{
				printf("Dev %d SleepToActive\r\n", phyDevId);
				BQ79600Man->stack.Device[phyDevId].PowerStatus = AFESIM_DEVICE_POWER_WAKE;
			}
			else if(BQ79600Man->stack.Device[phyDevId].PowerStatus == AFESIM_DEVICE_POWER_SHUTDOWN)
			{
				printf("Dev %d is shutdown and cannot SleepToActive\r\n");
				break;
			}
		}
#endif
	}

	if(CONTROL1->reg.field.SEND_WAKE == TRUE)
	{
		printf("Send Wake Command\r\n");
#if AFESIM_SIMULEVEL > 0
		wakeDevNum = AfeSim_DeviceWakeUp(BQ79600Man->stack.commDir, AFESIM_SHUTDOWN_TO_ACTIVE);
#else
		for(phyDevId += 1; phyDevId < AFESIM_MAX_DEVICE_NUM; phyDevId++)
		{
			switch(BQ79600Man->stack.Device[phyDevId].PowerStatus)
			{
			case AFESIM_DEVICE_POWER_WAKE:		printf("Dev %d is Wake\r\n", phyDevId); 				break;
			case AFESIM_DEVICE_POWER_SLEEP:		printf("Dev %d is Sleep, Wakeup\r\n", phyDevId); 		break;
			case AFESIM_DEVICE_POWER_SHUTDOWN: 	printf("Dev %d is Shutdown, Wakeup\r\n", phyDevId); 	break;
			default: break;
			}
			BQ79600Man->stack.Device[phyDevId].PowerStatus = AFESIM_DEVICE_POWER_WAKE;
		}
#endif
	}

	if(CONTROL1->reg.field.SEND_SHUTDOWN == TRUE)
	{
		printf("Send Shutdown Command\r\n", phyDevId);
#if AFESIM_SIMULEVEL > 0
		shutdownDevNum = AfeSim_DeviceSleepShutdown(BQ79600Man->stack.commDir, AFESIM_ACTIVE_TO_SHUTDOWN);
#else
		for(phyDevId += 1; phyDevId < AFESIM_MAX_DEVICE_NUM; phyDevId++)
		{
			if(BQ79600Man->stack.Device[phyDevId].PowerStatus == AFESIM_DEVICE_POWER_WAKE)
			{
				printf("Dev %d Shutdown\r\n");
				BQ79600Man->stack.Device[phyDevId].PowerStatus = AFESIM_DEVICE_POWER_SHUTDOWN;
			}
			else
			{
				printf("Dev %d is not wake and cannot Shutdown\r\n");
				break;
			}
		}
#endif
	}
	CONTROL1->reg.clear.clear = 0;
}


void AfeSim_BQ79600_CommandResp_SyncInternalDLL(AfeSim_AutoAddrStateType internalDll)
{
	printf("Command Sync up internal DLL %04X, %d\r\n", internalDll, BQ79600Man->stack.AutoAddrState);
	switch(BQ79600Man->stack.AutoAddrState)
	{
	case AFESIM_AUTOADDR_STATE_IDLE: 	if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_343) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL1; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL1: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_344) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL2; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL2: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_345) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL3; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL3: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_346) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL4; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL4: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_347) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL5; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL5: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_348) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL6; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL6: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_349) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL7; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL7: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_34A) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL8; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL8: break;
	case AFESIM_AUTOADDR_STATE_ACTIVE: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_343) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL9; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL9: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_344) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL10; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL10: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_345) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL11; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL11: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_346) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL12; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL12: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_347) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL13; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL13: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_348) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL14; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL14: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_349) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_INTERNAL_DLL15; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL15: if(internalDll == BQ79600_SYNCUP_INTERNAL_DLL_34A) BQ79600Man->stack.AutoAddrState = AFESIM_AUTOADDR_STATE_IDLE; break;
	case AFESIM_AUTOADDR_INTERNAL_DLL_INVALID: break;
	default: break;
		printf("Auto Addressing Success\r\n"); break;
	}
}


void AfeSim_BQ79600_CommErrorHandler(uint8 commDir, uint8 faultType)
{
	AFESIM_BQ79600_REG_FAULT_COMM2_RegType* FAULT_COMM2 = AfeSim_Reg[BQ79600_DEVID][AFESIM_BQ79600_REG_FAULT_COMM2];
	if(faultType < 2)
	{
		switch(faultType + commDir*2)
		{
			case 0: FAULT_COMM2->reg.field.COMH_PHY   = 1; break;
			case 1: FAULT_COMM2->reg.field.COMH_FRAME = 1; break;
			case 2: FAULT_COMM2->reg.field.COML_PHY   = 1; break;
			case 3: FAULT_COMM2->reg.field.COML_FRAME = 1; break;
		}
	}
}

/*Function State Machine*/
void AfeSim_BQ79600_IrqCallBack(void)
{
	uint32 retFun;

	AfeSim_BQ79600_CommandClass commandClass;

	BQ79600Man->spi.StringLength = AfeSim_HAL_BQ79600_MosiDataRead(BQ79600Man->spi.MOSI_RxData);
	commandClass = AfeSim_BQ79600_CommandPreProcess(BQ79600Man->spi.MOSI_RxData[0]);

	if( FALSE == AfeSim_AssertBaseDeviceCanRW() )
	{
		printf("Can not respond, BQ79600 is disabled\r\n");
		return;
	}

	switch(commandClass)
	{
	case BQ79600_WRITE:
		{
			BQ79600Man->stat.RecvTimes++;
			AfeSimX1_BQ79600_WriteCommandProcess();
		}
		break;
	case BQ79600_READ:
		{
			BQ79600Man->stat.RecvTimes++;
			AfeSimX1_BQ79600_ReadCommandProcess();
		}
		break;
	case BQ79600_READ_BACK:
		{
			BQ79600Man->stat.TransTimes++;
			AfeSim_BQ79600_ReadBackProcess();
		}
		break;
	case BQ79600_INVALID_CMD:
		break;
	default:
		break;
	}
}


void AfeSim_BQ79600_Fsm(void)
{

}




