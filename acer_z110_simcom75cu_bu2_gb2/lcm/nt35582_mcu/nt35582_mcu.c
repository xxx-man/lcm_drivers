/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#include <linux/string.h>

#include "lcm_drv.h"


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (800)
#define LCM_ID       (0x5582)

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

static __inline void send_ctrl_cmd(unsigned int cmd)
{
    lcm_util.send_cmd(cmd);
}

static __inline void send_data_cmd(unsigned int data)
{
    lcm_util.send_data(data&0xff);
}

static __inline unsigned short read_data_cmd(void)
{
    return (unsigned short)(lcm_util.read_data());
}

static __inline void set_lcm_register(unsigned int regIndex,
                                      unsigned int regData)
{
    send_ctrl_cmd(regIndex);
    send_data_cmd(regData);
}

static void sw_clear_panel(unsigned int color)
{
    short  x0, y0, x1, y1, x, y;
	short   h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;

		
    x0 = (short)0;
    y0 = (short)0;
    x1 = (short)FRAME_WIDTH-1;
    y1 = (short)FRAME_HEIGHT-1;

	h_X_start=((x0&0x0300)>>8);
	l_X_start=(x0&0x00FF);
	h_X_end=((x1&0x0300)>>8);
	l_X_end=(x1&0x00FF);

	h_Y_start=((y0&0x0300)>>8);
	l_Y_start=(y0&0x00FF);
	h_Y_end=((y1&0x0300)>>8);
	l_Y_end=(y1&0x00FF);

    send_ctrl_cmd( 0x2A00 );
    send_data_cmd( h_X_start);
    send_ctrl_cmd( 0x2A01 );
    send_data_cmd( l_X_start);
    send_ctrl_cmd( 0x2A02);
    send_data_cmd( h_X_end );
    send_ctrl_cmd( 0x2A03);
    send_data_cmd( l_X_end );
	send_ctrl_cmd( 0x2B00 );
    send_data_cmd( h_Y_start);
    send_ctrl_cmd( 0x2B01 );
    send_data_cmd( l_Y_start);
    send_ctrl_cmd( 0x2B02);
    send_data_cmd( h_Y_end );
    send_ctrl_cmd( 0x2B03);
    send_data_cmd( l_Y_end );
	send_ctrl_cmd(0x3601);  //enable HSM mode
	send_data_cmd(0x01);
    send_ctrl_cmd( 0x2C00 );

    // 18-bit mode (256K color) coding
    for (y = y0; y <= y1; ++ y) {
        for (x = x0; x <= x1; ++ x) {
            lcm_util.send_data(color);
        }
    }
}

static void init_lcm_registers(void)
{
    send_ctrl_cmd(0x1100);//sleep out
    MDELAY(200);

    set_lcm_register(0xC000,0x86);//power control PWCTR1
    set_lcm_register(0xC001,0x00);
    set_lcm_register(0xC002,0x86);
    set_lcm_register(0xC003,0x00);
    set_lcm_register(0xC100,0x45);
    set_lcm_register(0xC200,0x21);
    set_lcm_register(0xC202,0x02);
    set_lcm_register(0xB600,0x30);//SD_OP_SET
    set_lcm_register(0xB602,0x30);
    set_lcm_register(0xB100,0xA7);//wei zeng added
    set_lcm_register(0xC700,0x8F);//vcom
    set_lcm_register(0x3A00,0x06);//SET PIXEL_FORMAT,pre:0x55


    set_lcm_register(0xE000,0x0E);//gamma  GMACTRL1
    set_lcm_register(0xE001,0x14);
    set_lcm_register(0xE002,0x29);
    set_lcm_register(0xE003,0x3A);
    set_lcm_register(0xE004,0x1D);
    set_lcm_register(0xE005,0x30);
    set_lcm_register(0xE006,0x61);
    set_lcm_register(0xE007,0x3D);
    set_lcm_register(0xE008,0x22);
    set_lcm_register(0xE009,0x2A);	
    set_lcm_register(0xE00A,0x87);
    set_lcm_register(0xE00B,0x16);
    set_lcm_register(0xE00C,0x3B);
    set_lcm_register(0xE00D,0x4C);
    set_lcm_register(0xE00E,0x78);
    set_lcm_register(0xE00F,0x96);
    set_lcm_register(0xE010,0x4A);
    set_lcm_register(0xE011,0x4D);

    set_lcm_register(0xE100,0x0E);//GMACTRL2	
    set_lcm_register(0xE101,0x14);
    set_lcm_register(0xE102,0x29);
    set_lcm_register(0xE103,0x3A);
    set_lcm_register(0xE104,0x1D);
    set_lcm_register(0xE105,0x30);
    set_lcm_register(0xE106,0x61);
    set_lcm_register(0xE107,0x3F);
    set_lcm_register(0xE108,0x20);	
    set_lcm_register(0xE109,0x26);
    set_lcm_register(0xE10A,0x83);
    set_lcm_register(0xE10B,0x16);
    set_lcm_register(0xE10C,0x3B);
    set_lcm_register(0xE10D,0x4C);
    set_lcm_register(0xE10E,0x78);
    set_lcm_register(0xE10F,0x96);
    set_lcm_register(0xE110,0x4A);	
    set_lcm_register(0xE111,0x4D);

    set_lcm_register(0xE200,0x0E);//GMACTRL3
    set_lcm_register(0xE201,0x14);
    set_lcm_register(0xE202,0x29);
    set_lcm_register(0xE203,0x3A);
    set_lcm_register(0xE204,0x1D);
    set_lcm_register(0xE205,0x30);
    set_lcm_register(0xE206,0x61);	
    set_lcm_register(0xE207,0x3D);
    set_lcm_register(0xE208,0x22);
    set_lcm_register(0xE209,0x2A);
    set_lcm_register(0xE20A,0x87);
    set_lcm_register(0xE20B,0x16);
    set_lcm_register(0xE20C,0x3B);
    set_lcm_register(0xE20D,0x4C);
    set_lcm_register(0xE20E,0x78);	
    set_lcm_register(0xE20F,0x96);
    set_lcm_register(0xE210,0x4A);
    set_lcm_register(0xE211,0x4D);


    set_lcm_register(0xE300,0x0E);//GMACTRL4
    set_lcm_register(0xE301,0x14);
    set_lcm_register(0xE302,0x29);
    set_lcm_register(0xE303,0x3A);
    set_lcm_register(0xE304,0x1D);	
    set_lcm_register(0xE305,0x30);
    set_lcm_register(0xE306,0x61);
    set_lcm_register(0xE307,0x3F);
    set_lcm_register(0xE308,0x20);
    set_lcm_register(0xE309,0x26);
    set_lcm_register(0xE30A,0x83);
    set_lcm_register(0xE30B,0x16);
    set_lcm_register(0xE30C,0x3B);	
    set_lcm_register(0xE30D,0x4C);
    set_lcm_register(0xE30E,0x78);
    set_lcm_register(0xE30F,0x96);
    set_lcm_register(0xE310,0x4A);
    set_lcm_register(0xE311,0x4D);

    set_lcm_register(0xE400,0x0E);//GMACTRL5
    set_lcm_register(0xE401,0x14);
    set_lcm_register(0xE402,0x29);	
    set_lcm_register(0xE403,0x3A);
    set_lcm_register(0xE404,0x1D);
    set_lcm_register(0xE405,0x30);
    set_lcm_register(0xE406,0x61);
    set_lcm_register(0xE407,0x3D);
    set_lcm_register(0xE408,0x22);
    set_lcm_register(0xE409,0x2A);
    set_lcm_register(0xE40A,0x87);	
    set_lcm_register(0xE40B,0x16);
    set_lcm_register(0xE40C,0x3B);
    set_lcm_register(0xE40D,0x4C);
    set_lcm_register(0xE40E,0x78);
    set_lcm_register(0xE40F,0x96);
    set_lcm_register(0xE410,0x4A);
    set_lcm_register(0xE411,0x4D);


    set_lcm_register(0xE500,0x0E);
    set_lcm_register(0xE501,0x14);
    set_lcm_register(0xE502,0x29);	
    set_lcm_register(0xE503,0x3A);
    set_lcm_register(0xE504,0x1D);
    set_lcm_register(0xE505,0x30);
    set_lcm_register(0xE506,0x61);
    set_lcm_register(0xE507,0x3F);
    set_lcm_register(0xE508,0x20);
    set_lcm_register(0xE509,0x26);
    set_lcm_register(0xE50A,0x83);	
    set_lcm_register(0xE50B,0x16);
    set_lcm_register(0xE50C,0x3B);
    set_lcm_register(0xE50D,0x4C);
    set_lcm_register(0xE50E,0x78);
    set_lcm_register(0xE50F,0x96);
    set_lcm_register(0xE510,0x4A);
    set_lcm_register(0xE511,0x4D);

	// enable tering free
	set_lcm_register(0x3500,0x02);

    set_lcm_register(0x3600,0x00);
    send_ctrl_cmd(0x2900);
    MDELAY(200);
    send_ctrl_cmd(0x2C00);//wei zeng

}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));

    params->type   = LCM_TYPE_DBI;
    params->ctrl   = LCM_CTRL_PARALLEL_DBI;
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;
	params->io_select_mode = 3; //note:this para is different between 6573 and 6575
    
	params->dbi.port                    = 0;
    params->dbi.clock_freq              = LCM_DBI_CLOCK_FREQ_104M;
    params->dbi.data_width              = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_MSB_FIRST;
    params->dbi.data_format.padding     = LCM_DBI_PADDING_ON_LSB;
    params->dbi.data_format.format      = LCM_DBI_FORMAT_RGB666;
    params->dbi.data_format.width       = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.cpu_write_bits          = LCM_DBI_CPU_WRITE_16_BITS;
    params->dbi.io_driving_current      = LCM_DRIVING_CURRENT_8MA;

    params->dbi.parallel.write_setup    = 1;
    params->dbi.parallel.write_hold     = 1;
    params->dbi.parallel.write_wait     = 3;
    params->dbi.parallel.read_setup     = 3;
	params->dbi.parallel.read_hold      = 0;
    params->dbi.parallel.read_latency   = 15;
    params->dbi.parallel.wait_period    = 1;
	params->dbi.parallel.cs_high_width  = 0; //cycles of cs high level between each transfer
	// enable tearing-free
//    params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_ONLY;
//    params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;
}


static void lcm_init(void)
{
    SET_RESET_PIN(0);
    MDELAY(25);
    SET_RESET_PIN(1);
    MDELAY(50);

    init_lcm_registers();
    sw_clear_panel(0x0);    // Clean panel as black
}


static void lcm_suspend(void)
{
    send_ctrl_cmd(0x1000);
    MDELAY(20);
}


static void lcm_resume(void)
{
    send_ctrl_cmd(0x1100);
    MDELAY(200);
    
    // xuecheng, do we need to write 0x2900??
    send_ctrl_cmd(0x2900);
}

static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    short  x0, y0, x1, y1;
	short   h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;

		
    x0 = (short)x;
    y0 = (short)y;
    x1 = (short)x+width-1;
    y1 = (short)y+height-1;

	h_X_start=((x0&0x0300)>>8);
	l_X_start=(x0&0x00FF);
	h_X_end=((x1&0x0300)>>8);
	l_X_end=(x1&0x00FF);

	h_Y_start=((y0&0x0300)>>8);
	l_Y_start=(y0&0x00FF);
	h_Y_end=((y1&0x0300)>>8);
	l_Y_end=(y1&0x00FF);

    send_ctrl_cmd( 0x2A00 );
    send_data_cmd( h_X_start);
    send_ctrl_cmd( 0x2A01 );
    send_data_cmd( l_X_start);
    send_ctrl_cmd( 0x2A02);
    send_data_cmd( h_X_end );
    send_ctrl_cmd( 0x2A03);
    send_data_cmd( l_X_end );
	send_ctrl_cmd( 0x2B00 );
    send_data_cmd( h_Y_start);
    send_ctrl_cmd( 0x2B01 );
    send_data_cmd( l_Y_start);
    send_ctrl_cmd( 0x2B02);
    send_data_cmd( h_Y_end );
    send_ctrl_cmd( 0x2B03);
    send_data_cmd( l_Y_end );
	send_ctrl_cmd(0x3601);  //enable HSM mode
	send_data_cmd(0x01);
    send_ctrl_cmd( 0x2C00 );
}

static unsigned int lcm_compare_id(void)
{
    unsigned int id = 0;

	send_ctrl_cmd(0x1180);
    id = read_data_cmd();

#if defined(BUILD_UBOOT)
	printf("%s, id1 = 0x%08x\n", __func__, id);
#endif

	send_ctrl_cmd(0x1080);
	id |= (read_data_cmd() << 8);

#if defined(BUILD_UBOOT)
	printf("%s, id2 = 0x%08x\n", __func__, id);
#endif

	return (LCM_ID == id)?1:0;
}

LCM_DRIVER nt35582_mcu_lcm_drv = 
{
	.name			= "nt35582_mcu",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.update         = lcm_update,
	.compare_id     = lcm_compare_id
};
