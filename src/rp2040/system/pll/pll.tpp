
#ifndef RP2040_SYSTEM_PLL_T_H
#define RP2040_SYSTEM_PLL_T_H

#include "include/rp2040/system/pll.h"
#include "rp2040/system/resets.h"

namespace rp2040::system::pll {
    /**
     * @addtogroup rp2040_pll
     * @{
     */

    template <std::uintptr_t pll_addr>
    PllType<pll_addr>& PllType<pll_addr>::get_instance() {
        static PllType<pll_addr> instance;
        return instance;
    }

    template <std::uintptr_t pll_addr>
    void PllType<pll_addr>::init() {
        if (this->m_flags.test(static_cast<std::uint32_t>(Flags::is_initialized))) {
            return;
        }
        if constexpr (pll_addr == address::pll_sys) {
            resets::Resets::get_instance().enable<resets::Subsystems::pll_sys>();
        } else if constexpr (pll_addr == address::pll_usb) {
            resets::Resets::get_instance().enable<resets::Subsystems::pll_usb>();
        }
        this->m_flags.set(static_cast<std::uint32_t>(Flags::is_initialized));
        this->m_frequency_hz = 0;
    }

    template <std::uintptr_t pll_addr>
    void PllType<pll_addr>::deinit() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::is_initialized))) {
            return;
        }
        if constexpr (pll_addr == address::pll_sys) {
            resets::Resets::get_instance().disable<resets::Subsystems::pll_sys>();
        } else if constexpr (pll_addr == address::pll_usb) {
            resets::Resets::get_instance().disable<resets::Subsystems::pll_usb>();
        }
        this->m_flags.reset(static_cast<std::uint32_t>(Flags::is_initialized));
        this->m_frequency_hz = 0;
    }

    template <std::uintptr_t pll_addr>
    template <PllConfigType config, std::uint32_t refFreq_Hz>
    std::expected<unsigned int, status::Status> PllType<pll_addr>::set_frequency() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::is_initialized))) {
            return std::unexpected(status::Status::ERROR_INIT);
        }

        static_assert((limits::ref_clock_min_freq_hz <= refFreq_Hz) && (refFreq_Hz <= limits::ref_clock_max_freq_hz) , "Invalid reference clock frequency");
        static_assert((limits::fbdiv_min_val <= config.fbdiv) && (config.fbdiv <= limits::fbdiv_max_val), "Invalid fbd divider");
        static_assert((limits::postdiv_min_val <= config.postdiv1) && (config.postdiv1 <= limits::postdiv_max_val), "Invalid postdiv1 divider");
        static_assert((limits::postdiv_min_val <= config.postdiv2) && (config.postdiv2 <= limits::postdiv_max_val), "Invalid postdiv2 divider");

        constexpr std::uint32_t vco = config.fbdiv * refFreq_Hz;

        static_assert((limits::vco_min_freq_hz <= vco) && (vco <= limits::vco_max_freq_hz), "Invalid VCO frequency");

        constexpr unsigned int achievedFreq = vco / (config.postdiv1 * config.postdiv2);

        if constexpr (pll_addr == address::pll_sys) {
            static_assert(achievedFreq <= limits::pll_sys_max_freq_hz, "System pll frequency must be lower than or equal to 133 MHz");
        } else if constexpr (pll_addr == address::pll_usb) {
            static_assert(achievedFreq == limits::pll_usb_freq_hz, "USB pll frequency must be 48 MHz");
        }

        // If the PLL is already running with the correct frequency, there is no need to configure it again.
        if (achievedFreq == this->m_frequency_hz) {
            return this->m_frequency_hz;
        }

        using lock_bits_t = cs_r::Lock::Bits;
        using lock_value_t = cs_r::Lock::Value;
        // If the PLL is already running with a different frequency, reset the PLL
        if (lock_bits_t::getValue() == static_cast<cs_r::cs_reg::RegType>(lock_value_t::locked)) {
            if constexpr (pll_addr == address::pll_sys) {
                resets::Resets::get_instance().reset<resets::Subsystems::pll_sys>();
                while (!resets::Resets::get_instance().is_enabled<resets::Subsystems::pll_sys>()) {}
            } else if constexpr (pll_addr == address::pll_usb) {
                resets::Resets::get_instance().reset<resets::Subsystems::pll_usb>();
                while (!resets::Resets::get_instance().is_enabled<resets::Subsystems::pll_usb>()) {}
            }
        }

        cs_r::template RefDiv<config.refdiv>::Bits::set(cs_r::template RefDiv<config.refdiv>::Value::val);

        fbdiv_int_r::template FbDiv<config.fbdiv>::Bits::set(fbdiv_int_r::template FbDiv<config.fbdiv>::Value::val);

        pwr_r::Pd::Bits::clear();
        pwr_r::VcoPd::Bits::clear();

        while (lock_bits_t::getValue() == static_cast<cs_r::cs_reg::RegType>(lock_value_t::unstable)) {}

        prim_r::template PostDiv1<config.postdiv1>::Bits::set(prim_r::template PostDiv1<config.postdiv1>::Value::val);
        prim_r::template PostDiv2<config.postdiv2>::Bits::set(prim_r::template PostDiv2<config.postdiv2>::Value::val);

        pwr_r::PostDivPd::Bits::clear();

        this->m_frequency_hz = achievedFreq;

        return achievedFreq;
    }

    template <std::uintptr_t pll_addr>
    std::expected<unsigned int, status::Status> PllType<pll_addr>::get_frequency() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::is_initialized))) {
            return std::unexpected(status::Status::ERROR_INIT);
        }

        if (this->m_frequency_hz == 0) {
            return std::unexpected(status::Status::ERROR_RESOURCE);
        }

        return this->m_frequency_hz;
    }

    /** @} */ // rp2040_pll
} // rp2040::system


#endif // RP2040_SYSTEM_PLL_T_H