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
#ifdef BUILD_UBOOT
#include <asm/arch/mt6575_gpio.h>
#else
#include <mach/mt6575_gpio.h>
#endif
#include "lcm_drv.h"


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (1024)
#define FRAME_HEIGHT (600)


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
    unsigned char temp1 = (unsigned char)((cmd >> 8) & 0xFF);
    unsigned char temp2 = (unsigned char)(cmd & 0xFF);

    lcm_util.send_data(0x2000 | temp1);
    lcm_util.send_data(0x0000 | temp2);
}

static __inline void send_data_cmd(unsigned int data)
{
    lcm_util.send_data(0x0004 | data);
}

static __inline void set_lcm_register(unsigned int regIndex,
                                      unsigned int regData)
{
    send_ctrl_cmd(regIndex);
    send_data_cmd(regData);
}

static void init_lcm_registers(void)
{
 
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

    params->type   = LCM_TYPE_DPI;
    params->ctrl   = LCM_CTRL_SERIAL_DBI;
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;
    params->io_select_mode = 0;	

    /* RGB interface configurations */
    
    params->dpi.mipi_pll_clk_ref  = 0;      //the most important parameters: set pll clk to 66Mhz and dpi clk to 33Mhz
    params->dpi.mipi_pll_clk_div1 = 31;
    params->dpi.mipi_pll_clk_div2 = 4;
    params->dpi.dpi_clk_div       = 2;
    params->dpi.dpi_clk_duty      = 1;

    params->dpi.clk_pol           = LCM_POLARITY_FALLING;
    params->dpi.de_pol            = LCM_POLARITY_RISING;
    params->dpi.vsync_pol         = LCM_POLARITY_FALLING;
    params->dpi.hsync_pol         = LCM_POLARITY_FALLING;

    params->dpi.hsync_pulse_width = 128;
    params->dpi.hsync_back_porch  = 152;
    params->dpi.hsync_front_porch = 40;
    params->dpi.vsync_pulse_width = 3;
    params->dpi.vsync_back_porch  = 12;
    params->dpi.vsync_front_porch = 10;
    
    params->dpi.format            = LCM_DPI_FORMAT_RGB888;   // format is 24 bit
    params->dpi.rgb_order         = LCM_COLOR_ORDER_RGB;
    params->dpi.is_serial_output  = 0;

    params->dpi.intermediat_buffer_num = 2;

    params->dpi.io_driving_current = LCM_DRIVING_CURRENT_2MA;
}


static void lcm_init(void)
{
#ifdef BUILD_UBOOT
    // do nothing in uboot
#else
    lcm_util.set_gpio_mode(GPIO12, GPIO_MODE_00);    
    lcm_util.set_gpio_dir(GPIO12, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO12, GPIO_OUT_ONE); // LCM_STBY
    MDELAY(50);
    lcm_util.set_gpio_mode(GPIO14, GPIO_MODE_00);    
    lcm_util.set_gpio_dir(GPIO14, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO14, GPIO_OUT_ONE); // LCM_RST 
    //SET_RESET_PIN(1); // LCM_RST
    MDELAY(50);
    lcm_util.set_gpio_mode(GPIO52, GPIO_MODE_00);    
    lcm_util.set_gpio_dir(GPIO52, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO52, GPIO_OUT_ONE); // LVDS_SHUTDOWN_N   
    MDELAY(120); 
#endif    
}


static void lcm_suspend(void)
{
    lcm_util.set_gpio_mode(GPIO52, GPIO_MODE_00);    
    lcm_util.set_gpio_dir(GPIO52, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO52, GPIO_OUT_ZERO); // LVDS_SHUTDOWN_N      
    MDELAY(10);
    lcm_util.set_gpio_mode(GPIO12, GPIO_MODE_00);    
    lcm_util.set_gpio_dir(GPIO12, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO12, GPIO_OUT_ZERO); // LCM_STBY     
    MDELAY(50);
    lcm_util.set_gpio_mode(GPIO14, GPIO_MODE_00);    
    lcm_util.set_gpio_dir(GPIO14, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO14, GPIO_OUT_ZERO); // LCM_RST
    //SET_RESET_PIN(0); // LCM_RST 
    MDELAY(60); // avoid LCD resume transint
}


static void lcm_resume(void)
{
    lcm_util.set_gpio_mode(GPIO12, GPIO_MODE_00);    
    lcm_util.set_gpio_dir(GPIO12, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO12, GPIO_OUT_ONE); // LCM_STBY
    MDELAY(50);
    lcm_util.set_gpio_mode(GPIO14, GPIO_MODE_00);    
    lcm_util.set_gpio_dir(GPIO14, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO14, GPIO_OUT_ONE); // LCM_RST 
    //SET_RESET_PIN(1); // LCM_RST
    MDELAY(50);
    lcm_util.set_gpio_mode(GPIO52, GPIO_MODE_00);    
    lcm_util.set_gpio_dir(GPIO52, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO52, GPIO_OUT_ONE); // LVDS_SHUTDOWN_N   
    MDELAY(120); 
}

LCM_DRIVER tm070ddh03_lcm_drv = 
{
        .name		= "tm070ddh03",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
};

