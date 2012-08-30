/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2012 LeafLabs, LLC.
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file libmaple/include/libmaple/adc.h
 * @author Marti Bolivar <mbolivar@leaflabs.com>,
 *         Perry Hung <perry@leaflabs.com>
 * @brief Analog-to-Digital Conversion (ADC) header.
 */

#ifndef _LIBMAPLE_ADC_H_
#define _LIBMAPLE_ADC_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <libmaple/libmaple.h>
#include <libmaple/bitband.h>
#include <libmaple/rcc.h>
/* We include the series header below, after defining the register map
 * and device structs. */

/*
 * Register map
 */

/** ADC register map type. */
typedef struct adc_reg_map {
    __io uint32 SR;             ///< Status register
    __io uint32 CR1;            ///< Control register 1
    __io uint32 CR2;            ///< Control register 2
    __io uint32 SMPR1;          ///< Sample time register 1
    __io uint32 SMPR2;          ///< Sample time register 2
    __io uint32 JOFR1;          ///< Injected channel data offset register 1
    __io uint32 JOFR2;          ///< Injected channel data offset register 2
    __io uint32 JOFR3;          ///< Injected channel data offset register 3
    __io uint32 JOFR4;          ///< Injected channel data offset register 4
    __io uint32 HTR;            ///< Watchdog high threshold register
    __io uint32 LTR;            ///< Watchdog low threshold register
    __io uint32 SQR1;           ///< Regular sequence register 1
    __io uint32 SQR2;           ///< Regular sequence register 2
    __io uint32 SQR3;           ///< Regular sequence register 3
    __io uint32 JSQR;           ///< Injected sequence register
    __io uint32 JDR1;           ///< Injected data register 1
    __io uint32 JDR2;           ///< Injected data register 2
    __io uint32 JDR3;           ///< Injected data register 3
    __io uint32 JDR4;           ///< Injected data register 4
    __io uint32 DR;             ///< Regular data register
} adc_reg_map;

/** ADC device type. */
typedef struct adc_dev {
    adc_reg_map *regs; /**< Register map */
    void *const priv;  /**< Private; don't touch */
    rcc_clk_id clk_id; /**< RCC clock information */
} adc_dev;

/* Pull in the series header (which may need the above struct
 * definitions).
 *
 * IMPORTANT NOTES FOR IMPLEMENTORS:
 *
 * The series header must define the following:
 *
 * - enum adc_extsel_event (and typedef to adc_extsel_event): One per
 *   external event used to trigger start of conversion of a regular
 *   group. If two different series support the same event as a
 *   trigger, they must use the same token for the enumerator for that
 *   event. (The value of the enumerator is of course allowed to be
 *   different).
 *
 * - enum adc_smp_rate (and typedef to adc_smp_rate): One per
 *   available sampling time.  These must be in the form ADC_SMPR_X_Y
 *   for X.Y cycles (e.g. ADC_SMPR_1_5 means 1.5 cycles), or
 *   ADC_SMPR_X for X cycles (e.g. ADC_SMPR_3 means 3 cycles).
 *
 * - enum adc_prescaler (and typedef): One per available prescaler,
 *   suitable for adc_set_prescaler. Series which have the same
 *   prescaler dividers (e.g. STM32F1 and STM32F2 both divide PCLK2 by
 *   2, 4, 6, or 8) must provide the same tokens as enumerators, for
 *   portability.
 *
 * Additionally, when adding a new series, you must keep the
 * definition and documentation for adc_interrupt_id up to date. If it
 * needs changes, then the implementations of adc_enable_interrupts()
 * and adc_disable_interrupts() will probably need to be updated as
 * well.
 */
#include <series/adc.h>

/*
 * Register bit definitions
 */

/* Status register */

#define ADC_SR_AWD_BIT                  0
#define ADC_SR_EOC_BIT                  1
#define ADC_SR_JEOC_BIT                 2
#define ADC_SR_JSTRT_BIT                3
#define ADC_SR_STRT_BIT                 4

#define ADC_SR_AWD                      BIT(ADC_SR_AWD_BIT)
#define ADC_SR_EOC                      BIT(ADC_SR_EOC_BIT)
#define ADC_SR_JEOC                     BIT(ADC_SR_JEOC_BIT)
#define ADC_SR_JSTRT                    BIT(ADC_SR_JSTRT_BIT)
#define ADC_SR_STRT                     BIT(ADC_SR_STRT_BIT)

/* Control register 1 */

#define ADC_CR1_EOCIE_BIT               5
#define ADC_CR1_AWDIE_BIT               6
#define ADC_CR1_JEOCIE_BIT              7
#define ADC_CR1_SCAN_BIT                8
#define ADC_CR1_AWDSGL_BIT              9
#define ADC_CR1_JAUTO_BIT               10
#define ADC_CR1_DISCEN_BIT              11
#define ADC_CR1_JDISCEN_BIT             12
#define ADC_CR1_JAWDEN_BIT              22
#define ADC_CR1_AWDEN_BIT               23

#define ADC_CR1_AWDCH                   (0x1F)
#define ADC_CR1_EOCIE                   BIT(ADC_CR1_EOCIE_BIT)
#define ADC_CR1_AWDIE                   BIT(ADC_CR1_AWDIE_BIT)
#define ADC_CR1_JEOCIE                  BIT(ADC_CR1_JEOCIE_BIT)
#define ADC_CR1_SCAN                    BIT(ADC_CR1_SCAN_BIT)
#define ADC_CR1_AWDSGL                  BIT(ADC_CR1_AWDSGL_BIT)
#define ADC_CR1_JAUTO                   BIT(ADC_CR1_JAUTO_BIT)
#define ADC_CR1_DISCEN                  BIT(ADC_CR1_DISCEN_BIT)
#define ADC_CR1_JDISCEN                 BIT(ADC_CR1_JDISCEN_BIT)
#define ADC_CR1_DISCNUM                 (0xE000)
#define ADC_CR1_JAWDEN                  BIT(ADC_CR1_JAWDEN_BIT)
#define ADC_CR1_AWDEN                   BIT(ADC_CR1_AWDEN_BIT)

/* Control register 2 */

/* Because this register varies significantly by series (e.g. some
 * bits moved and others disappeared in the F1->F2 transition), its
 * definitions are in the series headers. */

/* Sample time register 1 */

#define ADC_SMPR1_SMP17                 (0x7 << 21)
#define ADC_SMPR1_SMP16                 (0x7 << 18)
#define ADC_SMPR1_SMP15                 (0x7 << 15)
#define ADC_SMPR1_SMP14                 (0x7 << 12)
#define ADC_SMPR1_SMP13                 (0x7 << 9)
#define ADC_SMPR1_SMP12                 (0x7 << 6)
#define ADC_SMPR1_SMP11                 (0x7 << 3)
#define ADC_SMPR1_SMP10                 0x7

/* Sample time register 2 */

#define ADC_SMPR2_SMP9                  (0x7 << 27)
#define ADC_SMPR2_SMP8                  (0x7 << 24)
#define ADC_SMPR2_SMP7                  (0x7 << 21)
#define ADC_SMPR2_SMP6                  (0x7 << 18)
#define ADC_SMPR2_SMP5                  (0x7 << 15)
#define ADC_SMPR2_SMP4                  (0x7 << 12)
#define ADC_SMPR2_SMP3                  (0x7 << 9)
#define ADC_SMPR2_SMP2                  (0x7 << 6)
#define ADC_SMPR2_SMP1                  (0x7 << 3)
#define ADC_SMPR2_SMP0                  0x7

/* Injected channel data offset register */

#define ADC_JOFR_JOFFSET                0x3FF

/* Watchdog high threshold register */

#define ADC_HTR_HT                      0x3FF

/* Watchdog low threshold register */

#define ADC_LTR_LT                      0x3FF

/* Regular sequence register 1 */

#define ADC_SQR1_L                      (0x1F << 20)
#define ADC_SQR1_SQ16                   (0x1F << 15)
#define ADC_SQR1_SQ15                   (0x1F << 10)
#define ADC_SQR1_SQ14                   (0x1F << 5)
#define ADC_SQR1_SQ13                   0x1F

/* Regular sequence register 2 */

#define ADC_SQR2_SQ12                   (0x1F << 25)
#define ADC_SQR2_SQ11                   (0x1F << 20)
#define ADC_SQR2_SQ10                   (0x1F << 16)
#define ADC_SQR2_SQ9                    (0x1F << 10)
#define ADC_SQR2_SQ8                    (0x1F << 5)
#define ADC_SQR2_SQ7                    0x1F

/* Regular sequence register 3 */

#define ADC_SQR3_SQ6                    (0x1F << 25)
#define ADC_SQR3_SQ5                    (0x1F << 20)
#define ADC_SQR3_SQ4                    (0x1F << 16)
#define ADC_SQR3_SQ3                    (0x1F << 10)
#define ADC_SQR3_SQ2                    (0x1F << 5)
#define ADC_SQR3_SQ1                    0x1F

/* Injected sequence register */

#define ADC_JSQR_JL                     (0x3 << 20)
#define ADC_JSQR_JL_1CONV               (0x0 << 20)
#define ADC_JSQR_JL_2CONV               (0x1 << 20)
#define ADC_JSQR_JL_3CONV               (0x2 << 20)
#define ADC_JSQR_JL_4CONV               (0x3 << 20)
#define ADC_JSQR_JSQ4                   (0x1F << 15)
#define ADC_JSQR_JSQ3                   (0x1F << 10)
#define ADC_JSQR_JSQ2                   (0x1F << 5)
#define ADC_JSQR_JSQ1                   0x1F

/* Injected data registers */

#define ADC_JDR_JDATA                   0xFFFF

/* Regular data register */

#define ADC_DR_ADC2DATA                 (0xFFFF << 16)
#define ADC_DR_DATA                     0xFFFF

/*
 * Routines
 */

/* Misc. */

/**
 * @brief Call a function on all ADC devices.
 * @param fn Function to call on each ADC device.
 */
extern void adc_foreach(void (*fn)(const adc_dev*));

/* Initialize, enable/disable */

/**
 * @brief Enable an ADC and configure it for regular channel conversion.
 *
 * This function performs any initialization necessary to allow the
 * ADC device to do software-triggered conversion of regular (i.e.,
 * not injected) ADC channels. For example, the device will be ready
 * for use with adc_read() after you call this function on it.
 *
 * @param dev Device to enable.
 * @see adc_read()
 * @see adc_config_gpio()
 */
extern void adc_enable_reg_swstart(const adc_dev* dev);

void adc_init(const adc_dev *dev);

/**
 * @brief Enable an ADC peripheral
 * @param dev ADC device to enable
 */
static inline void adc_enable(const adc_dev *dev) {
    *bb_perip(&dev->regs->CR2, ADC_CR2_ADON_BIT) = 1;
}

/**
 * @brief Disable an ADC peripheral
 * @param dev ADC device to disable
 */
static inline void adc_disable(const adc_dev *dev) {
    *bb_perip(&dev->regs->CR2, ADC_CR2_ADON_BIT) = 0;
}

/**
 * @brief Disable all ADC peripherals.
 */
static inline void adc_disable_all(void) {
    adc_foreach(adc_disable);
}

/* Configuration */

void adc_set_extsel(const adc_dev *dev, adc_extsel_event event);
void adc_set_sample_rate(const adc_dev *dev, adc_smp_rate smp_rate);

/**
 * @brief Set the ADC prescaler.
 *
 * This determines the ADC clock for all devices.
 */
extern void adc_set_prescaler(adc_prescaler pre);

struct gpio_dev;
/**
 * @brief Configure a GPIO pin for ADC conversion.
 * @param dev ADC device to use for conversion (currently ignored on
 *            all targets).
 * @param gdev GPIO device to configure.
 * @param bit Bit on gdev to configure for ADC conversion.
 */
extern void adc_config_gpio(const struct adc_dev *dev,
                            struct gpio_dev *gdev,
                            uint8 bit);

/**
 * @brief Set the regular channel sequence length.
 *
 * Defines the total number of conversions in the regular channel
 * conversion sequence.
 *
 * @param dev ADC device.
 * @param length Regular channel sequence length, from 1 to 16.
 */
static inline void adc_set_reg_seqlen(const adc_dev *dev, uint8 length) {
    uint32 tmp = dev->regs->SQR1;
    tmp &= ~ADC_SQR1_L;
    tmp |= (length - 1) << 20;
    dev->regs->SQR1 = tmp;
}

void adc_enable_scan(const adc_dev *dev);
void adc_disable_scan(const adc_dev *dev);

void adc_enable_continuous(const adc_dev *dev);
void adc_disable_continuous(const adc_dev *dev);

void adc_set_reg_seq(const adc_dev *dev, const uint8 *channels, uint8 len);

/* Conversion */

uint16 adc_read(const adc_dev *dev, uint8 channel);

/**
 * @brief Start the next conversion in the regular sequence.
 *
 * This function is nonblocking. It starts converting the regular
 * sequence of ADC channels. After calling it, you can either poll the
 * device (using adc_is_conv_complete()) or use interrupts (using
 * adc_attach_interrupt()) to determine when conversion is finished.
 *
 * You must set the channel(s) to convert before calling this
 * function. Do that with adc_set_reg_seq().
 *
 * Implementation note: this function uses the SWSTART bit to start
 * the conversion. Some targets require extra configuration for
 * SWSTART to have an effect. You can portably perform this
 * configuration with adc_enable_reg_swstart().
 *
 * @param dev ADC device whose regular conversion sequence to start.
 * @see adc_enable_reg_swstart()
 * @see adc_set_reg_seq()
 * @see adc_is_conv_complete()
 * @see adc_attach_interrupt()
 */
static __always_inline void adc_start_conv(const adc_dev *dev) {
    dev->regs->CR2 |= ADC_CR2_SWSTART;
}

/**
 * @brief Check if a regular conversion has completed.
 *
 * This function is useful e.g. when watching ADC conversions started
 * with adc_start_conv(). When a channel's conversion finishes, this
 * function will return true.
 *
 * @param dev Device to check.
 * @return True if conversion has completed, false otherwise.
 * @see adc_start_conv()
 */
static __always_inline int adc_is_conv_complete(const adc_dev *dev) {
    return dev->regs->SR & ADC_SR_EOC;
}

/**
 * @brief Get converted data from an ADC's regular sequence.
 *
 * Call this function to get the results of the most recent
 * conversion.
 *
 * You must ensure that the data are available before calling this
 * function. Do that either by waiting until adc_is_conv_complete()
 * returns true, or (from an interrupt handler) checking that your
 * handler was called due to ADC_CONV_INTERRUPT via its
 * adc_callback_data argument.
 *
 * @see adc_is_conv_complete()
 * @see enum adc_interrupt_id
 * @see adc_attach_interrupt()
 * @see struct adc_callback_data
 */
static __always_inline uint32 adc_get_data(const adc_dev *dev) {
    return dev->regs->DR;
}

/* Interrupts */

/**
 * @brief ADC interrupts
 *
 * These are useful for enabling/disabling interrupts, as well as
 * decoding which interrupts were responsible for an ADC IRQ
 * firing. They are bit flags and thus can safely be ORed together.
 *
 * Not all targets support all interrupts given here. In cases where
 * availability is limited, the flag's documentation states which
 * targets support the flag. Functions taking adc_interrupt_id
 * arguments ignore any values that are unavailable on the target.
 *
 * @see ADC_ALL_INTERRUPTS
 * @see adc_attach_interrupt()
 * @see adc_enable_interrupts()
 * @see adc_disable_interrupts()
 */
typedef enum adc_interrupt_id {
    /** Regular channel end of conversion interrupt */
    ADC_CONV_INTERRUPT = ADC_SR_EOC,

    /** Injected channel end of conversion interrupt */
    ADC_INJ_CONV_INTERRUPT = ADC_SR_JEOC,

    /** Analog watchdog interrupt */
    ADC_WATCHDOG_INTERRUPT = ADC_SR_AWD,

#ifdef ADC_SR_OVR
    ADC_OVERRUN_INTERRUPT = ADC_SR_OVR,
#else
    /**
     * @brief Overrun interrupt.
     *
     * Availability: STM32F2, STM32F4. */
    ADC_OVERRUN_INTERRUPT = 0,
#endif
} adc_interrupt_id;

/**
 * @brief Logical or of all adc_interrupt_id values
 * @see enum adc_interrupt_id */
#define ADC_ALL_INTERRUPTS (ADC_CONV_INTERRUPT | ADC_INJ_CONV_INTERRUPT | \
                            ADC_WATCHDOG_INTERRUPT | ADC_OVERRUN_INTERRUPT)

/**
 * @brief Used as argument to attached ADC interrupt handlers
 * @see adc_attach_interrupt()
 */
typedef struct adc_callback_data {
    uint32 irq_flags;
    void *arg;
} adc_callback_data;

void adc_attach_interrupt(const adc_dev *dev, uint32 interrupt_flags,
                          void (*handler)(adc_callback_data*), void* arg);
void adc_detach_interrupt(const adc_dev *dev);

void adc_enable_interrupts(const adc_dev *dev, uint32 interrupt_flags);
void adc_disable_interrupts(const adc_dev *dev, uint32 interrupt_flags);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
