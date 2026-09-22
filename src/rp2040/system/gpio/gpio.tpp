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

        resets::Resets::get_instance().reset<resets::Subsystems::io_bank0>();
        resets::Resets::get_instance().reset<resets::Subsystems::pads_bank0>();

        m_flags.set(static_cast<std::uint32_t>(Flags::Initialized));
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    void GpioType<ioBankBase, padsBankBase, sioBase>::deinit() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return;
        }

        resets::Resets::get_instance().disable<resets::Subsystems::io_bank0>();
        resets::Resets::get_instance().disable<resets::Subsystems::pads_bank0>();

        m_flags.reset(static_cast<std::uint32_t>(Flags::Initialized));
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, FuncSelect func>
    status::Status GpioType<ioBankBase, padsBankBase, sioBase>::set_function() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return status::Status::ERROR_INIT;
        }

        static_assert(pin < Pins::number_of_gpio, "Invalid GPIO pin!");
        static_assert(func < FuncSelect::number_of_functions &&
            !(((pin < Pins::gpio20) || (pin > Pins::gpio25)) && (func == FuncSelect::clock)),
            "Invalid function or invalid function assignment (Clock functionality only available for GPIO20-GPIO25)!");

        io_bank::template Gpio<pin>::Ctrl::FuncSel::Bits::set(func);

        return status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool enable>
    status::Status GpioType<ioBankBase, padsBankBase, sioBase>::enable_output() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return status::Status::ERROR_INIT;
        }
        static_assert(pin < Pins::number_of_gpio, "Invalid GPIO pin!");

        using pad_t = pad_bank::template Pad<static_cast<Pads>(std::to_underlying(pin))>;

        if constexpr (enable) {
            if (!m_output_enabled.test(static_cast<std::uint32_t>(pin))) {
                pad_t::OutputDisable::Bits::clear();
                sio_bank::OutputEnable::Set::template Gpio<pin>::set();
                m_output_enabled.set(static_cast<std::uint32_t>(pin));
            }
        } else {
            if (m_output_enabled.test(static_cast<std::uint32_t>(pin))) {
                sio_bank::OutputEnable::Clear::template Gpio<pin>::clear();
                m_output_enabled.reset(static_cast<std::uint32_t>(pin));
            }
        }

        return status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool high>
    status::Status GpioType<ioBankBase, padsBankBase, sioBase>::set_output() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return status::Status::ERROR_INIT;
        }
        if (!m_output_enabled.test(static_cast<std::uint32_t>(pin))) {
            return status::Status::ERROR_GENERIC;
        }
        static_assert(pin < Pins::number_of_gpio, "Invalid GPIO pin!");

        if constexpr (high) {
            sio_bank::Output::Set::template Gpio<pin>::set();
        } else {
            sio_bank::Output::Clear::template Gpio<pin>::clear();
        }
        return status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin>
    status::Status GpioType<ioBankBase, padsBankBase, sioBase>::toggle_output() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return status::Status::ERROR_INIT;
        }
        if (!m_output_enabled.test(static_cast<std::uint32_t>(pin))) {
            return status::Status::ERROR_GENERIC;
        }
        static_assert(pin < Pins::number_of_gpio, "Invalid GPIO pin!");

        sio_bank::Output::Toggle::template Gpio<pin>::toggle();

        return status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool enable>
    status::Status GpioType<ioBankBase, padsBankBase, sioBase>::enable_input() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return status::Status::ERROR_INIT;
        }

        static_assert(pin < Pins::number_of_gpio, "Invalid GPIO pin!");

        using pad_t = pad_bank::template Pad<static_cast<Pads>(std::to_underlying(pin))>;

        if constexpr (enable) {
            if (!m_input_enabled.test(static_cast<std::uint32_t>(pin))) {
                pad_t::InputEnable::Bits::set(pad_t::InputEnable::Value::enable);
                m_input_enabled.set(static_cast<std::uint32_t>(pin));
            }
        } else {
            if (m_input_enabled.test(static_cast<std::uint32_t>(pin))) {
                pad_t::InputEnable::Bits::clear();
                m_input_enabled.reset(static_cast<std::uint32_t>(pin));
            }
        }

        return status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin>
    std::expected<bool, status::Status> GpioType<ioBankBase, padsBankBase, sioBase>::get_input() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return std::unexpected(status::Status::ERROR_INIT);
        }
        if (!m_input_enabled.test(static_cast<std::uint32_t>(pin))) {
            return std::unexpected(status::Status::ERROR_GENERIC);
        }
        static_assert(pin < Pins::number_of_gpio, "Invalid GPIO pin!");

        bool output_level = sio_bank::output::value::template value_bits<pin>::getValue();

        return output_level;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool enable>
    status::Status GpioType<ioBankBase, padsBankBase, sioBase>::enable_pull_up() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return status::Status::ERROR_INIT;
        }

        static_assert(pin < Pins::number_of_gpio, "Invalid GPIO pin!");

        /** short hand for the pad register of the GPIO pin */
        using pad_t = pad_bank::template Pad<static_cast<Pads>(std::to_underlying(pin))>;

        if constexpr (enable) {
            pad_t::PullUpEnable::Bits::set(pad_t::PullUpEnable::Value::enable);
        } else {
            pad_t::PullUpEnable::Bits::set(pad_t::PullUpEnable::Value::disable);
        }

        return status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool enable>
    status::Status GpioType<ioBankBase, padsBankBase, sioBase>::enable_pull_down() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return status::Status::ERROR_INIT;
        }

        static_assert(pin < Pins::number_of_gpio, "Invalid GPIO pin!");

        /** short hand for the pad register of the GPIO pin */
        using pad_t = pad_bank::template Pad<static_cast<Pads>(std::to_underlying(pin))>;

        if constexpr (enable) {
            pad_t::PullDownEnable::Bits::set(pad_t::PullDownEnable::Value::enable);
        } else {
            pad_t::PullDownEnable::Bits::set(pad_t::PullDownEnable::Value::disable);
        }

        return status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool enable>
    status::Status GpioType<ioBankBase, padsBankBase, sioBase>::enable_schmitt_trigger() {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return status::Status::ERROR_INIT;
        }

        static_assert(pin < Pins::number_of_gpio, "Invalid GPIO pin!");

        /** short hand for the pad register of the GPIO pin */
        using pad_t = pad_bank::template Pad<static_cast<Pads>(std::to_underlying(pin))>;

        if constexpr (enable) {
            pad_t::Schmitt::Bits::set(pad_t::Schmitt::Value::enable);
        } else {
            pad_t::Schmitt::Bits::set(pad_t::Schmitt::Value::disable);
        }

        return status::Status::OK;
    }

    template <std::uintptr_t ioBankBase, std::uintptr_t padsBankBase, std::uintptr_t sioBase>
    template <Pins pin, bool enable_fast_mode>
    status::Status GpioType<ioBankBase, padsBankBase, sioBase>::set_slew_rate()
    {
        if (!m_flags.test(static_cast<std::uint32_t>(Flags::Initialized))) {
            return status::Status::ERROR_INIT;
        }

        static_assert(pin < Pins::number_of_gpio, "Invalid GPIO pin!");

        using pad_t = pad_bank::template padX<static_cast<Pads>(std::to_underlying(pin))>;

        if constexpr (enable_fast_mode) {
            pad_t::SlewFast::Bits::set(pad_t::SlewFast::Value::Fast);
        } else {
            pad_t::SlewFast::Bits::set(pad_t::SlewFast::Value::Slow);
        }

        return status::Status::OK;
    }

    /** @} */ // rp2040_gpio
} // rp2040::system::gpio

#endif //RP2040_SYSTEM_GPIO_TPP
