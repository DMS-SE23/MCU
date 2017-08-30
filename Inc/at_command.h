//#ifndef __AT_COMMAND_H
//#define __AT_COMMAND_H
//
//void ATCMD_ATF2(void);
//void ATCMD_ATFD(void);
//void ATCMD_ATWS(void);
//void ATCMD_ATSN(void);
//void ATCMD_ATME(void);

//// DIO Related Commands
//void ATCMD_DIG(void);
//// EEPROM Related Commands
//void ATCMD_EET(void);
//void ATCMD_EEG(unsigned char add1,
//               unsigned char add2,
//               unsigned char add3,
//               unsigned char add4);
//void ATCMD_EES(unsigned char add1,
//               unsigned char add2,
//               unsigned char add3,
//               unsigned char add4,
//               unsigned char dat1,
//               unsigned char dat2);
//
//void ATCMD_MCBG(void);
//void ATCMD_MCVG(void);
//
//void ATCMD_ATPC(void);
//
//// Debug Command
//void ATCMD_DBG01(void);
//void ATCMD_DBG02(void);
//void ATCMD_DBG03(void);
//
//// DIAG Command
//void ATCMD_DIAG01(void);
//void ATCMD_DIAG02(void);
//void ATCMD_DIAG03(void);
//void ATCMD_DIAG04(void);
//void ATCMD_DIAG05(void);
//void ATCMD_DIAG06(void);
//void ATCMD_DIAG07(void);
//void ATCMD_DIAG08(void);
//void ATCMD_DIAG09(void);
//
//// RTC
//void ATCMD_RTCG(void);
//void ATCMD_RTCS(void);
//
//// Watchdog
//void ATCMD_WDTG();
//void ATCMD_WDTS(unsigned char count_hh, unsigned char count_hl,
//                unsigned char count_lh, unsigned char count_ll);
//void ATCMD_WDCG();
//void ATCMD_WDTR();
//void ATCMD_WDTE();
//void ATCMD_WDTD();
//
//// CAR POWER LOW PROTECTION
//void ATCMD_MCPR(unsigned char power_system);
//void ATCMD_MCPG(unsigned char voltage_system, unsigned char voltage_check_time);
//void ATCMD_MCPD(unsigned char voltage_system);                 
//void ATCMD_MCPS(unsigned char voltage_system, 
//                unsigned char voltage_check_time,
//                unsigned char value_h_h,
//                unsigned char value_h_l,
//                unsigned char value_l_h,
//                unsigned char value_l_l);
//void ATCMD_MCAG();
//void ATCMD_MCAS(unsigned char Keep_alive_mode_enable,
//                unsigned char AT_mode_enable);
//void ATCMD_MCCG();
//void ATCMD_MCCS(unsigned char Pre_boot_enable,
//                unsigned char Post_boot_enable);
//void ATCMD_WKSG();
//
//// Wakeup Mask Get
//void ATCMD_WKMG();
//// Wakeup Mask Set
//void ATCMD_WKMS(unsigned char module_type, unsigned char on_off);
//
//// Module Control
//void ATCMD_MCG(void);
//void ATCMD_MCS(unsigned char module_type, unsigned char on_off);
//
//// Get Alarm Time Setting
//void ATCMD_RTAG(void);
//// Set Alarm Setting
//void ATCMD_RTAS(unsigned char date_h, unsigned char date_l,
//                unsigned char hour_h, unsigned char hour_l,
//                unsigned char min_h, unsigned char min_l);
//// Clear Alarm INT
//void ATCMD_RTAC(void);
//// Reset RTC
//void ATCMD_RTRST(void);
//void ATCMD_RTAE(void);
//
//// Load EEPROM to Factory Default Bypass SN
//void ATCMD_EEDF(void);
//// Save EEPROM to User Default
//void ATCMD_EEUS(void);
//// Load EEPROM to User Default
//void ATCMD_EEUL(void);
//
//// Reset EEPROM to Factory Default
//void ATCMD_EEFA(void);
//
//// Get Ignition Status
//void ATCMD_IGNG(void);
//// Get Working Mode for Ignition Off
//void ATCMD_IGMG(void);
//// Set Working Mode for Ignition Off
//void ATCMD_IGMS(unsigned char mode_set);
//// 獲得ALARM INT Pin的目前電壓準位
//void ATCMD_RTAI(void);
//// Erase EEPROM Contents
//void ATCMD_EERS(void);
//
//void ATCMD_EVQA (unsigned evt_cls_h,
//                 unsigned evt_cls_l,
//                 unsigned evt_idx_h,
//                 unsigned evt_idx_l);
//void ATCMD_EVQG();
//// Event Queue Flush
//void ATCMD_EVQF(void);
//// Dump Parameters Restore From EEPROM
//void ATCMD_EEPP(void);
//// Dump VPM Related Counter, 計數器資料
//void ATCMD_EEPC(void);
//
//// 獲得與設定當CAR Power Low採取之流程
//void ATCMD_MCLG(void);
//void ATCMD_MCLS(unsigned char method);
//
//// 獲得與設定VPM Event Timeout Delay
//void ATCMD_MCUG(unsigned char time_type);
//void ATCMD_MCUS(unsigned char time_type,
//                unsigned char data_hh,
//                unsigned char data_hl,
//                unsigned char data_lh,
//                unsigned char data_ll);
//#endif