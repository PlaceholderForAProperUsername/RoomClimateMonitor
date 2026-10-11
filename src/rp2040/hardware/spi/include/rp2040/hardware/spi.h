/**
 * @file spi.h
 * @brief The SPI module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 06.10.2026
 *
 *
 * @copyright (C) 2026  Thorsten Hoffmeister
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef RP2040_HARDWARE_SPI_H
#define RP2040_HARDWARE_SPI_H

#include <bitset>

#include "status.h"

#include "spi_def.h"

namespace rp2040::hardware::spi {
    /**
     * @addtogroup rp2040_spi
     * @{
     */

    /**
     * @brief Configuration to configure the SPI controller as a master.
     */
    struct MasterConfig {
        DataSize size;
        std::uint8_t clock_rate;
        std::uint8_t prescale_divisor;
        FrameFormat format;
        bool clock_polarity;
        bool clock_phase;

        constexpr MasterConfig(const DataSize size, const std::uint8_t clock_rate, const std::uint8_t prescale_divisor,
            const FrameFormat format, const bool clock_polarity = false, const bool clock_phase = false) :
        size(size), clock_rate(clock_rate), prescale_divisor(prescale_divisor), format(format),
        clock_polarity(clock_polarity), clock_phase(clock_phase) {}
    };

    /**
     * @brief The SPI controller
     *
     * @tparam base Base address of the SPI controller.
     */
    template <std::uintptr_t base>
    class SpiType {
        static_assert((base == address::spi0) || (base == address::spi1), "Invalid base address for SPI register map!");

    public:
        /**
         * @brief Gets the singleton instance.
         *
         * @return SpiType The singleton instance.
         */
        static SpiType& get_instance();

        /**
         * @brief Initializes the SPI controller by taking it out of reset.
         *
         * @pre None
         *
         * @post The SPI controller is ready to be configured.
         */
        void init();

        /**
         * @brief Deinitializes the SPI controller.
         *
         * @pre None
         *
         * @post The SPI controller is put in reset and its registers are no longer accessible.
         */
        void deinit();

        /**
         * @brief Configures the SPI controller as a master.
         *
         * @tparam config The configuration.
         * @return status::Status The status code.
         * @retval OK The SPI controller was successfully configured as a master.
         * @retval ERROR_INIT The SPI controller was not initialized. @see init
         *
         * @pre The SPI controller must be initialized. @see init
         *
         * @post The SPI controller is configured as a master and ready to be used.
         */
        template <MasterConfig config>
        status::Status configure();

        /**
         * @brief Sends data over SPI.
         *
         * After putting the data in the transmit FIFO, the function waits until the whole transmission is complete before
         * returning.
         *
         * @param data Data to be sent.
         * @return status::Status The status code.
         * @retval OK The data has been successfully sent.
         * @retval ERROR_INIT The SPI has not been configured. @see configure
         * @retval ERROR_PARAM The SPI has been configured to use data sizes larger than 8 bit, but the data uses only 8 bit.
         *
         * @pre The SPI must be initialized and configured. @see init @see configure
         *
         * @post None
         */
        status::Status write_blocking(std::span<std::uint8_t> data);

        /**
         * @brief Sends data over SPI.
         *
         * After putting the data in the transmit FIFO, the function waits until the whole transmission is complete before
         * returning.
         *
         * @param data Data to be sent.
         * @return status::Status The status code.
         * @retval OK The data has been successfully sent.
         * @retval ERROR_INIT The SPI has not been configured. @see configure
         * @retval ERROR_PARAM The SPI has been configured for data sizes less than or equal to 8 bit, but the data size
         * is 16 bit.
         *
         * @pre The SPI must be initialized and configured. @see init @see configure
         *
         * @post None
         */
        status::Status write_blocking(std::span<std::uint16_t> data);

    private:
        using Registers = SpiRegMap<base>;

        SpiType() : m_flags(0) {};

        enum class Flags : std::uint32_t {
            initialized = 0U,
            configured = 1U,
            data_size_greater_than_8bits = 2U,
            number_of_flags
        };

        std::bitset<std::to_underlying(Flags::number_of_flags)> m_flags;
    };

    using Spi0 = SpiType<address::spi0>; /**< @brief SPI0 controller. */
    using Spi1 = SpiType<address::spi1>; /**< @brief SPI1 controller. */

    /** @}*/ // rp2040_spi
}

#include "../../../spi.tpp"

#endif //RP2040_HARDWARE_SPI_H
