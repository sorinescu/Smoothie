/*  
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>. 
*/

// #include "libs/Kernel.h"
//#include "modules/tools/laser/Laser.h"
// #include "modules/tools/extruder/Extruder.h"
// #include "modules/tools/temperaturecontrol/TemperatureControlPool.h"
// #include "modules/robot/Player.h"
// #include "modules/utils/simpleshell/SimpleShell.h"
// #include "modules/utils/pausebutton/PauseButton.h"
//#include "libs/ChaNFSSD/SDFileSystem.h"
// #include "libs/Config.h"
// #include "libs/nuts_bolts.h"
// #include "libs/utils.h"

#include "main.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

#include "stm32f4xx_conf.h"      
//#include "libs/USBCDCMSC/USBCDCMSC.h"
//SDFileSystem sd(p5, p6, p7, p8, "sd");  // LPC17xx specific : comment if you are not using a SD card ( for example with a mBed ).
//LocalFileSystem local("local");       // LPC17xx specific : comment if you are not running a mBed
//USBCDCMSC cdcmsc(&sd);                  // LPC17xx specific : Composite serial + msc USB device

 __ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

__IO uint32_t TimingDelay;

int main() {

	USBD_Init(&USB_OTG_dev,
    USB_OTG_FS_CORE_ID, 
    &USR_desc, 
    &USBD_CDC_cb, 
    &USR_cb);

    // Kernel* kernel = new Kernel();

    // kernel->serial->printf("Smoothie ( grbl port ) version 0.6 \r\nstart\r\n");

//    kernel->add_module( new Laser(p21) );
//    kernel->add_module( new Extruder(p26,p27) );
//    kernel->add_module( new SimpleShell() );
//    kernel->add_module( new TemperatureControlPool() );
//    kernel->add_module( new PauseButton() );
//
//    kernel->add_module( &cdcmsc );

    while(1){
        // kernel->call_event(ON_MAIN_LOOP);
    }
}

