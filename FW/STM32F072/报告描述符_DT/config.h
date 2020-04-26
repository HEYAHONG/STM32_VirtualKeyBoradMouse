// D:\project_in_git\VirtualKeyBoradMouse\FW\STM32F072\±¨¸æÃèÊö·û_DT\config.h


char ReportDescriptor[44] = {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x00,                    // COLLECTION (Physical)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x40,                    //   REPORT_COUNT (64)
    0x81, 0x80,                    //   INPUT (Data,Ary,Abs,Vol)
    0x09, 0x02,                    //   USAGE (Vendor Usage 2)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x40,                    //   REPORT_COUNT (64)
    0x91, 0x80,                    //   OUTPUT (Data,Ary,Abs,Vol)
    0xc0                           // END_COLLECTION
};

