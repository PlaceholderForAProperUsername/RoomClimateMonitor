/**
 * @file gpio.h
 * @brief The GPIO module to use the gpios of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 26.07.2026
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

#ifndef RP2040_SYSTEM_GPIO_H
#define RP2040_SYSTEM_GPIO_H

#include <bitset>
#include <cstdint>
#include <expected>

#include "gpio_def.h"
#include "status.h"


namespace rp2040::system::gpio {
    /**
     * @addtogroup rp2040_gpio
     * @{
     */

    /**
     * @brief The GPIO type to configure and use the GPIOs.
     *
     * @tparam ioBankBase The address of the User Bank IO registers.
     * @tparam padsBankBase The address of the User Bank Pad Control registers.
     * @tparam sioBase The address of the Software IO registers.
     */
    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    class GpioType {
        static_assert((ioBankBase == IO_BANK0_BASE) && (padsBankBase == PADS_BANK0_BASE) && (sioBase == SIO_BASE), "Invalid gpio base addresses.");

    public:
        /**
         * @brief Gets the Singleton instance.
         *
         * @return GpioType& The Singleton instance.
         */
        static GpioType<ioBankBase, padsBankBase, sioBase>& getInstance();

        /**
         * @brief Initializes the GPIO module by taking it out of reset.
         *
         * @pre None
         *
         * @post The GPIO is taken out of reset and the GPIO is configurable/usable.
         */
        void init();

        /**
         * @brief Deinitializes the GPIO module by putting it in reset.
         *
         * @pre None
         *
         * @post The GPIO is deinitialized.
         */
        void deinit();

        /**
         * @brief Sets the function for the specified GPIO pin.
         *
         * @tparam pin The GPIO pin for which the function is to be set.
         * @tparam func The function to which the GPIO is to be set.
         * @return utils::status::Status The status code.
         * @retval OK The function has been successfully set.
         * @retval ERROR_INIT The GPIO module was not initialized.
         *
         * @pre The GPIO module must be initialized. @see init
         *
         * @post The GPIO pin has been configured for the function.
         */
        template <Pins pin, FuncSelect func>
        status::Status set_function();


        /**
         * @brief Enables/disables the output for a pin.
         *
         * @tparam pin The GPIO pin for which the output is to be enabled/disabled.
         * @tparam enable Boolean to enable (true) or disable (false) the output.
         * @return utils::status::Status The status code.
         * @retval OK The output has been successfully enabled/disabled.
         * @retval ERROR_INIT The GPIO module was not initialized.
         *
         * @pre The GPIO module must be initialized. @see init
         *
         * @post The output for the GPIO pin is enabled/disabled.
         */
        template <Pins pin, bool enable>
        status::Status enable_output();

        /**
         * @brief Sets the output level of a GPIO pin.
         *
         * @tparam pin The GPIO pin for which the output is to be set.
         * @tparam high Boolean to set the output high (true) or low (false).
         * @return utils::status::Status The status code.
         * @retval OK The output has been successfully set.
         * @retval ERROR_INIT The GPIO module was not initialized.
         * @retval ERROR_GENERIC The GPIO pin is not configured to be an output.
         *
         * @pre The GPIO module must be initialized and be set as an output pin. @see init @see enable_output
         *
         * @post The output level for the GPIO pin has been set.
         */
        template <Pins pin, bool high>
        status::Status set_output();

        /**
         * @brief Toggles the output level of a GPIO pin.
         *
         * @tparam pin The GPIO pin for which the output is to be toggled.
         * @return utils::status::Status The status code.
         * @retval OK The output has been successfully toggled.
         * @retval ERROR_INIT The GPIO module was not initialized.
         * @retval ERROR_GENERIC The GPIO pin is not configured to be an output.
         *
         * @pre The GPIO module must be initialized and be set as an output pin. @see init @see enable_output
         *
         * @post The output level for the GPIO pin has been toggled.
         */
        template <Pins pin>
        status::Status toggle_output();

        /**
         * @brief Enables/disables the input for a pin.
         *
         * @tparam pin The GPIO pin for which the input is to be enabled/disabled.
         * @tparam enable Boolean to enable (true) or disable (false) the input.
         * @return utils::status::Status The status code.
         * @retval OK The input has been successfully enabled/disabled.
         * @retval ERROR_INIT The GPIO module was not initialized.
         *
         * @pre The GPIO module must be initialized. @see init
         *
         * @post The input for the GPIO pin is enabled/disabled.
         */
        template <Pins pin, bool enable>
        status::Status enable_input();

        /**
         * @brief Gets the input value of a GPIO pin.
         *
         * @tparam pin The GPIO pin for which the input is read.
         * @return std::expected<bool, utils::status::Status> Either a boolean indicating the level or a status code in case of failure.
         * @retval Bool True if GPIO level is high or false if low.
         * @retval ERROR_INIT The GPIO module was not initialized.
         * @retval ERROR_GENERIC The GPIO pin is not configured to be an input pin.
         *
         * @pre The GPIO module must be initialized and the GPIO pin configured to be an input. @see init @see enable_input
         *
         * @post None
         */
        template <Pins pin>
        std::expected<bool, status::Status> get_input();

        /**
         * @brief Enables/disables the internal pull-up resistor for a pin.
         *
         * @tparam pin The GPIO pin for which the pull-up resistor is to be enabled/disabled.
         * @tparam enable Boolean to enable (true) or disable (false) the pull-up resistor.
         * @return utils::status::Status The status code.
         * @retval OK The pull-up resistor has been successfully enabled/disabled.
         * @retval ERROR_INIT The GPIO module was not initialized.
         *
         * @pre The GPIO module must be initialized. @see init
         *
         * @post The pull-up resistor for the GPIO pin is enabled/disabled.
         */
        template <Pins pin, bool enable>
        status::Status enable_pull_up();

        /**
         * @brief Enables/disables the internal pull-down resistor for a pin.
         *
         * @tparam pin The GPIO pin for which the pull-down resistor is to be enabled/disabled.
         * @tparam enable Boolean to enable (true) or disable (false) the pull-down resistor.
         * @return utils::status::Status The status code.
         * @retval OK The pull-down resistor has been successfully enabled/disabled.
         * @retval ERROR_INIT The GPIO module was not initialized.
         *
         * @pre The GPIO module must be initialized. @see init
         *
         * @post The pull-down resistor for the GPIO pin is enabled/disabled.
         */
        template <Pins pin, bool enable>
        status::Status enable_pull_down();

        /**
         * @brief Enables/disables the schmitt trigger for a pin.
         *
         * @tparam pin The GPIO pin for which the schmitt trigger is to be enabled/disabled.
         * @tparam enable Boolean to enable (true) or disable (false) the schmitt trigger.
         * @return utils::status::Status The status code.
         * @retval OK The schmitt trigger has been successfully enabled/disabled.
         * @retval ERROR_INIT The GPIO module was not initialized.
         *
         * @pre The GPIO module must be initialized. @see init
         *
         * @post The schmitt trigger for the GPIO pin is enabled/disabled.
         */
        template <Pins pin, bool enable>
        status::Status enable_schmitt_trigger();

        /**
         * @brief Sets the mode of slew rate of a pin.
         *
         * @tparam pin The GPIO pin for which the slew rate mode is to be set.
         * @tparam enable_fast_mode Boolean to enable fast mode (true) or slow mode (false).
         * @return utils::status::Status The status code.
         * @retval OK The slew rate has been successfully set.
         * @retval ERROR_INIT The GPIO module was not initialized.
         *
         * @pre The GPIO module must be initialized. @see init
         *
         * @post The slew rate mode for the GPIO pin has been set.
         */
        template <Pins pin, bool enable_fast_mode>
        status::Status set_slew_rate();

    private:
        GpioType() : m_flags(0), m_output_enabled(0) {
            m_input_enabled.set(); // The input is enabled for all GPIO at reset.
        };

        enum Flags : std::uint32_t {
            Initialized = 0U,
            NumberOfFlags,
        };

        std::bitset<static_cast<std::uint32_t>(Flags::NumberOfFlags)> m_flags;
        std::bitset<static_cast<std::uint32_t>(Pins::NumberOfGPIOs)> m_input_enabled;
        std::bitset<static_cast<std::uint32_t>(Pins::NumberOfGPIOs)> m_output_enabled;

        using io_bank = IO_BankRegMap<ioBankBase>; /**< @brief The user IO bank to read the status and configure the pin. */
        using pad_bank = PadsBankRegMap<padsBankBase>; /**< @brief The user pads bank to configure electrical parameters of the pad. */
        using sio_bank = SIORegMap<sioBase>; /**< @brief Software input/output control. */
    };


    using GPIO = GpioType<IO_BANK0_BASE, PADS_BANK0_BASE, SIO_BASE>; /**< @brief The GPIO. */

    /** @} */ // rp2040_gpio
} // rp2040::system::gpio



#include "../../../gpio.tpp"

#endif //RP2040_SYSTEM_GPIO_H
