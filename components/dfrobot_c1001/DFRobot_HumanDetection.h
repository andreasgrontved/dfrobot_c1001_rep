/*!
 * @file DFRobot_HumanDetection.h
 * @brief This is the declaration part of the human millimeter-wave driver library
 *        adapted for ESP-IDF.
 * @copyright   
 * @License     The MIT License (MIT)
 * @author      
 * @version     V1.0
 * @date        2024-06-03
 * @url         https://github.com/DFRobot/DFRobot_HumanDetection
 */

#ifndef _DFROBOT_HUMAN_DETECTION_
#define _DFROBOT_HUMAN_DETECTION_

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* 
 * Minimal Stream interface for ESP-IDF.
 * You must provide an implementation (for example, a wrapper for the UART driver)
 * that implements the following methods:
 *   - int available();
 *   - int read();
 *   - size_t write(const uint8_t *buffer, size_t size);
 */
class Stream {
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
};

#ifdef ENABLE_DBG
#include "esp_log.h"
#define DBG(...) ESP_LOGI("HumanDetection", __VA_ARGS__)
#else
#define DBG(...)
#endif

#define TIME_OUT (5 * 1000)

#define CMD_HEAD    0
#define CMD_CONFIG  1
#define CMD_CMD     2
#define CMD_LEN_H   3
#define CMD_LEN_L   4
#define CMD_END_H   5
#define CMD_END_L   6
#define CMD_WHITE   7
#define CMD_DATA    8

/**
 * @brief Sleep composite state data
 */
typedef struct
{
    uint8_t presence;           ///< Presence state
    uint8_t sleepState;         ///< Sleep state
    uint8_t averageRespiration; ///< Average respiration
    uint8_t averageHeartbeat;   ///< Average heartbeat
    uint8_t turnoverNumber;     ///< Turnover number
    uint8_t largeBodyMove;      ///< Large body movement percentage
    uint8_t minorBodyMove;      ///< Minor body movement percentage
    uint8_t apneaEvents;        ///< Apnea events
} sSleepComposite;

/**
 * @brief Sleep statistics query
 */
typedef struct
{
    uint8_t  sleepQualityScore;      ///< Sleep quality score
    uint16_t sleepTime;              ///< Sleep duration in minutes
    uint8_t  wakeDuration;           ///< Wake duration
    uint8_t  shallowSleepPercentage; ///< Shallow sleep duration percentage
    uint8_t  deepSleepPercentage;    ///< Deep sleep duration percentage
    uint8_t  timeOutOfBed;           ///< Time out of bed
    uint8_t  exitCount;              ///< Exit count
    uint8_t  turnOverCount;          ///< Turnover count
    uint8_t  averageRespiration;     ///< Average respiration
    uint8_t  averageHeartbeat;       ///< Average heartbeat
    uint8_t  apneaEvents;            ///< Apnea events
} sSleepStatistics;

class DFRobot_HumanDetection
{
public:
    /**
     * @brief Working mode configuration structure
     */
    typedef enum
    {
        eSleepMode   = 0x02,
        eFallingMode = 0x01,
    } eWorkMode;

    /**
     * @brief LED light selection
     */
    typedef enum
    {
        eFALLLed = 0x03,
        eHPLed   = 0x04,
    } eLed;

    /**
     * @brief Human-related data in sleep mode
     */
    typedef enum
    {
        eHumanPresence,    ///< Human presence query
        eHumanMovement,    ///< Movement information query
        eHumanMovingRange, ///< Movement distance, range 0~100
        eHumanDistance,
    } esmHuman;

    /**
     * @brief Sleep data in sleep mode
     */
    typedef enum
    {
        eInOrNotInBed,           ///< Get in or out of bed status
        eSleepState,             ///< Get sleep state
        eWakeDuration,           ///< Get wake duration
        eLightsleep,             ///< Shallow sleep
        eDeepSleepDuration,      ///< Get deep sleep duration
        eSleepQuality,           ///< Get sleep quality
        eSleepDisturbances,      ///< Sleep abnormality query
        eSleepQualityRating,     ///< Sleep quality rating
        eAbnormalStruggle,       ///< Abnormal struggle
        eUnattendedState,        ///< No one timing query
        eAbnormalStruggleSwitch, ///< Abnormal struggle switch query
        eUnattendedSwitch,       ///< No timing switch query
        eUnattendedTime,         ///< No timing time query
        esleepDeadline,          ///< Sleep deadline
        eReportingmode,          ///< Reporting mode
    } eSmSleep;

    /**
     * @brief Sleep function configuration in sleep mode
     */
    typedef enum
    {
        eReportingmodeC,    ///< Reporting mode
        eAbnormalStruggleC, ///< Abnormal struggle
        eUnattendedStateC,  ///< No timing
        eUnattendedTimeC,   ///< No timing time
        esleepDeadlineC,    ///< Sleep deadline
    } eSmSleepConfig;

    /**
     * @brief Human data in falling mode
     */
    typedef enum
    {
        eExistence,                ///< Human presence query
        eMotion,                   ///< Motion query
        eBodyMove,                 ///< Body movement query
        eTrajectorySwitch,         ///< Trajectory point switch query
        eSeatedHorizontalDistance, ///< Seated horizontal distance
        eMotionHorizontalDistance, ///< Motion horizontal distance
    } eDmHuman;

    /**
     * @brief Get fall data
     */
    typedef enum
    {
        eFallState,             ///< Fall state
        eFallBreakHeight,       ///< Fall break height
        eHeightRatioSwitch,     ///< Height ratio switch query
        estaticResidencyState,  ///< Static residency state
        estaticResidencySwitch, ///< Static residency switch
        eFallSensitivity,       ///< Fall sensitivity
    } eDmFall;

    /**
     * @brief Human configuration in falling mode
     */
    typedef enum
    {
        eSeatedHorizontalDistanceC, ///< Seated horizontal distance
        eMotionHorizontalDistanceC, ///< Motion horizontal distance
    } eDmHumanConfig;

    /**
     * @brief Get fall data configuration options
     */
    typedef enum
    {
        eFallBreakHeightC,   ///< Fall break height
        eHeightRatioSwitchC, ///< Height ratio switch
        eReportFreqC,        ///< Trajectory point information reporting frequency
        eReportSwitchC,      ///< Trajectory point reporting switch
        eAltTimeC,           ///< Height cumulative time
        eFallSensitivityC,   ///< Fall sensitivity setting
        eResidenceSwitchC,   ///< Residency switch
        eResidenceTime,      ///< Residency time
    } eDmFallConfig;

    /**
     * @brief Constructor
     * @param s Pointer to a Stream-like object for serial communication.
     */
    DFRobot_HumanDetection(Stream *s);
    ~DFRobot_HumanDetection() {};

    uint8_t begin(void);
    uint8_t configWorkMode(eWorkMode mode);
    uint8_t getWorkMode(void);
    uint8_t configLEDLight(eLed led, uint8_t sta);
    uint8_t getLEDLightState(eLed led);
    uint8_t sensorRet(void);
    uint16_t smHumanData(esmHuman hm);
    uint8_t getHeartRate(void);
    uint8_t getBreatheState(void);
    uint8_t getBreatheValue(void);
    uint16_t smSleepData(eSmSleep sl);
    sSleepComposite getSleepComposite(void);
    sSleepStatistics getSleepStatistics(void);
    uint8_t configSleep(eSmSleepConfig sl, uint8_t data);
    void dmInstallAngle(int16_t x, int16_t y, int16_t z);
    void dmGetInstallAngle(int16_t *x, int16_t *y, int16_t *z);
    void dmInstallHeight(uint16_t he);
    uint16_t dmGetInstallHeight(void);
    uint16_t dmAutoMeasureHeight(void);
    uint16_t dmHumanData(eDmHuman dh);
    void track(uint16_t *x, uint16_t *y);
    uint32_t trackFrequency(void);
    uint32_t getUnmannedTime(void);
    void dmUnmannedTime(uint32_t Time);
    uint16_t getFallData(eDmFall dm);
    uint32_t getFallTime(void);
    void dmFallTime(uint32_t Time);
    uint32_t getStaticResidencyTime(void);
    uint32_t accumulatedHeightDuration(void);
    uint8_t dmHumanConfig(eDmHumanConfig con, uint16_t data);
    uint8_t unattendedTimeConfig(uint32_t time);
    uint8_t dmFallConfig(eDmFallConfig con, uint32_t data);

private:
    uint8_t getData(uint8_t con, uint8_t cmd, uint16_t len, uint8_t *senData, uint8_t *retData);
    uint8_t sumData(uint8_t len, uint8_t *buf);
    Stream *_s = NULL;
};

#endif
