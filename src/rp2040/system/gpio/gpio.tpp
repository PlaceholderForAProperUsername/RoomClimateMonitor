/**
 * @file gpio.tpp
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


#ifndef RP2040_SYSTEM_GPIO_TPP
#define RP2040_SYSTEM_GPIO_TPP

#include <utility>

#include "rp2040/system/gpio.h"
#include "rp2040/system/resets.h"

namespace rp2040::system::gpio {
    /**
     * @addtogroup rp2040_gpio
     * @{
     */

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    GpioType<ioBankBase, padsBankBase, sioBase>& GpioType<ioBankBase, padsBankBase, sioBase>::getInstance() {
        static GpioType<ioBankBase, padsBankBase, sioBase> instance;
        return instance;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    void GpioType<ioBankBase, padsBankBase, sioBase>::init() {
        if (m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return;
        }

        Resets::getInstance().reset<SubsystemBits::IO_BANK0>();
        Resets::getInstance().reset<SubsystemBits::PADS_BANK0>();

        m_flags.set(static_cast<std::uint32_t>(Flags::Initialized));
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    void GpioType<ioBankBase, padsBankBase, sioBase>::deinit() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return;
        }

        Resets::getInstance().disable<SubsystemBits::IO_BANK0>();
        Resets::getInstance().disable<SubsystemBits::PADS_BANK0>();

        m_flags.reset(static_cast<std::uint32_t>(Flags::Initialized));
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, FuncSelect func>
    utils::status::Status GpioType<ioBankBase, padsBankBase, sioBase>::set_function() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return utils::status::Status::ERROR_INIT;
        }

        static_assert(pin < Pins::NumberOfGPIOs, "Invalid GPIO pin!");
        static_assert(func < FuncSelect::NumberOfFunctions &&
            !(((pin < Pins::GPIO20) || (pin > Pins::GPIO25)) && (func == FuncSelect::CLOCK)),
            "Invalid function or invalid function assignment (Clock functionality only available for GPIO20-GPIO25)!");

        io_bank::template GPIOX_RegMap<pin>::ctrl::funcSelect_bits::set(func);

        return utils::status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool enable>
    utils::status::Status GpioType<ioBankBase, padsBankBase, sioBase>::enable_output() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return utils::status::Status::ERROR_INIT;
        }
        static_assert(pin < Pins::NumberOfGPIOs, "Invalid GPIO pin!");

        if constexpr (enable) {
            if (!m_output_enabled.test(static_cast<std::uint32_t>(pin))) {
                pad_bank::template padX<static_cast<Pads>(std::to_underlying(pin))>::outputDisable_bits::clear();
                sio_bank::outputEnable::set::template set_bits<pin>::set();
                m_output_enabled.set(static_cast<std::uint32_t>(pin));
            }
        } else {
            if (m_output_enabled.test(static_cast<std::uint32_t>(pin))) {
                sio_bank::outputEnable::clear::template clear_bits<pin>::clear();
                m_output_enabled.reset(static_cast<std::uint32_t>(pin));
            }
        }

        return utils::status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool high>
    utils::status::Status GpioType<ioBankBase, padsBankBase, sioBase>::set_output() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return utils::status::Status::ERROR_INIT;
        }
        if (!m_output_enabled.test(static_cast<std::uint32_t>(pin))) {
            return utils::status::Status::ERROR_GENERIC;
        }
        static_assert(pin < Pins::NumberOfGPIOs, "Invalid GPIO pin!");

        if constexpr (high) {
            sio_bank::output::set::template set_bits<pin>::set();
        } else {
            sio_bank::output::clear::template clear_bits<pin>::clear();
        }
        return utils::status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin>
    utils::status::Status GpioType<ioBankBase, padsBankBase, sioBase>::toggle_output() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return utils::status::Status::ERROR_INIT;
        }
        if (!m_output_enabled.test(static_cast<std::uint32_t>(pin))) {
            return utils::status::Status::ERROR_GENERIC;
        }
        static_assert(pin < Pins::NumberOfGPIOs, "Invalid GPIO pin!");

        sio_bank::output::toggle::template toggle_bits<pin>::toggle();

        return utils::status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool enable>
    utils::status::Status GpioType<ioBankBase, padsBankBase, sioBase>::enable_input() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return utils::status::Status::ERROR_INIT;
        }

        static_assert(pin < Pins::NumberOfGPIOs, "Invalid GPIO pin!");

        if constexpr (enable) {
            if (!m_input_enabled.test(static_cast<std::uint32_t>(pin))) {
                pad_bank::template padX<static_cast<Pads>(std::to_underlying(pin))>::inputEnable_bits::set(pad_bank::template padX<static_cast<Pads>(std::to_underlying(pin))>::InputEnableBitField::value::enable);
                m_input_enabled.set(static_cast<std::uint32_t>(pin));
            }
        } else {
            if (m_input_enabled.test(static_cast<std::uint32_t>(pin))) {
                pad_bank::template padX<static_cast<Pads>(std::to_underlying(pin))>::inputEnable_bits::clear();
                m_input_enabled.reset(static_cast<std::uint32_t>(pin));
            }
        }

        return utils::status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin>
    std::expected<bool, utils::status::Status> GpioType<ioBankBase, padsBankBase, sioBase>::get_input() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return std::unexpected(utils::status::Status::ERROR_INIT);
        }
        if (!m_input_enabled.test(static_cast<std::uint32_t>(pin))) {
            return std::unexpected(utils::status::Status::ERROR_GENERIC);
        }
        static_assert(pin < Pins::NumberOfGPIOs, "Invalid GPIO pin!");

        bool output_level = sio_bank::output::value::template value_bits<pin>::getValue();

        return output_level;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool enable>
    utils::status::Status GpioType<ioBankBase, padsBankBase, sioBase>::enable_pull_up() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return utils::status::Status::ERROR_INIT;
        }

        static_assert(pin < Pins::NumberOfGPIOs, "Invalid GPIO pin!");

        /** short hand for the pad register of the GPIO pin */
        using pad_t = pad_bank::template padX<static_cast<Pads>(std::to_underlying(pin))>;

        if constexpr (enable) {
            pad_t::pullUpEnable_bits::set(pad_t::PullUpEnableBitField::value::enable);
        } else {
            pad_t::pullUpEnable_bits::set(pad_t::PullUpEnableBitField::value::disable);
        }

        return utils::status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool enable>
    utils::status::Status GpioType<ioBankBase, padsBankBase, sioBase>::enable_pull_down() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return utils::status::Status::ERROR_INIT;
        }

        static_assert(pin < Pins::NumberOfGPIOs, "Invalid GPIO pin!");

        /** short hand for the pad register of the GPIO pin */
        using pad_t = pad_bank::template padX<static_cast<Pads>(std::to_underlying(pin))>;

        if constexpr (enable) {
            pad_t::pullDownEnable_bits::set(pad_t::PullDownEnableBitField::value::enable);
        } else {
            pad_t::pullDownEnable_bits::set(pad_t::PullDownEnableBitField::value::disable);
        }

        return utils::status::Status::OK;
    }

    /** @} */ // rp2040_gpio
} // rp2040::system::gpio

#endif //RP2040_SYSTEM_GPIO_TPP
