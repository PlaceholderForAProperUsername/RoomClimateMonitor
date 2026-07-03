
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
    PLL_Type<pll_addr>& PLL_Type<pll_addr>::getInstance() {
        static PLL_Type<pll_addr> instance;
        return instance;
    }

    template <std::uintptr_t pll_addr>
    void PLL_Type<pll_addr>::init() {
        if (this->m_flags.test(static_cast<std::uint32_t>(Flags::isInitialized))) {
            return;
        }
        if constexpr (pll_addr == pll_sys_base) {
            Resets::getInstance().enable<SubsystemBits::PLL_SYS>();
        } else if constexpr (pll_addr == pll_usb_base) {
            Resets::getInstance().enable<SubsystemBits::PLL_USB>();
        }
        this->m_flags.set(static_cast<std::uint32_t>(Flags::isInitialized));
        this->m_frequency = 0;
    }

    template <std::uintptr_t pll_addr>
    void PLL_Type<pll_addr>::deinit() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::isInitialized))) {
            return;
        }
        if constexpr (pll_addr == pll_sys_base) {
            Resets::getInstance().disable<SubsystemBits::PLL_SYS>();
        } else if constexpr (pll_addr == pll_usb_base) {
            Resets::getInstance().disable<SubsystemBits::PLL_USB>();
        }
        this->m_flags.reset(static_cast<std::uint32_t>(Flags::isInitialized));
        this->m_frequency = 0;
    }

    template <std::uintptr_t pll_addr>
    template <PLL_ConfigType config, std::uint32_t refFreq_Hz>
    std::expected<std::uint32_t, utils::status::Status> PLL_Type<pll_addr>::setFrequency() {
        if (!this->m_flags.test(static_cast<std::uint32_t>(Flags::isInitialized))) {
            return std::unexpected(utils::status::Status::ERROR_INIT);
        }

        static_assert((limits::refClockMinFreq_Hz <= refFreq_Hz) && (refFreq_Hz <= limits::refClockMaxFreq_Hz) , "Invalid reference clock frequency");
        static_assert((limits::fbdivMinVal <= config.fbdiv) && (config.fbdiv <= limits::fbdivMaxVal), "Invalid fbd divider");
        static_assert((limits::postdivMinVal <= config.postdiv1) && (config.postdiv1 <= limits::postdivMaxVal), "Invalid postdiv1 divider");
        static_assert((limits::postdivMinVal <= config.postdiv2) && (config.postdiv2 <= limits::postdivMaxVal), "Invalid postdiv2 divider");

        constexpr std::uint32_t vco = config.fbdiv * refFreq_Hz;

        static_assert((limits::VCO_minFreq_Hz <= vco) && (vco <= limits::VCO_maxFreq_Hz), "Invalid VCO frequency");

        constexpr std::uint32_t achievedFreq = vco / (config.postdiv1 * config.postdiv2);

        if constexpr (pll_addr == pll_sys_base) {
            static_assert(achievedFreq <= limits::PLL_SysMaxFreq_Hz, "System pll frequency must be lower than or equal to 133 MHz");
        } else if constexpr (pll_addr == pll_usb_base) {
            static_assert(achievedFreq <= limits::PLL_USB_MaxFreq_Hz, "USB pll frequency must be lower than or equal to 48 MHz");
        }

        // If the PLL is already running with the correct frequency, there is no need to configure it again.
        if (achievedFreq == this->m_frequency) {
            return this->m_frequency;
        }

        // If the PLL is already running with a different frequency, reset the PLL
        if (cs_r::lock_bits::getValue() == static_cast<cs_r::cs_reg::RegType>(cs_r::LockBitField::value::locked)) {
            if constexpr (pll_addr == pll_sys_base) {
                Resets::getInstance().reset<SubsystemBits::PLL_SYS>();
                while (!Resets::getInstance().isEnabled<SubsystemBits::PLL_SYS>()) {}
            } else if constexpr (pll_addr == pll_usb_base) {
                Resets::getInstance().reset<SubsystemBits::PLL_USB>();
                while (!Resets::getInstance().isEnabled<SubsystemBits::PLL_USB>()) {}
            }
        }

        cs_r::template refdiv_bits<config.refdiv>::set(cs_r::template RefDivBitField<config.refdiv>::value::val);
        fbdiv_int_r::template fbdiv_int_bits<config.fbdiv>::set(fbdiv_int_r::template FBDivBitField<config.fbdiv>::value::val);
        pwr_r::pd_bits::clear();
        pwr_r::vcopd_bits::clear();
        while (cs_r::lock_bits::getValue() == static_cast<cs_r::cs_reg::RegType>(cs_r::LockBitField::value::unstable)) {}
        prim_r::template postdiv1_bits<config.postdiv1>::set(prim_r::template PostDiv1BitField<config.postdiv1>::value::val);
        prim_r::template postdiv2_bits<config.postdiv2>::set(prim_r::template PostDiv2BitField<config.postdiv2>::value::val);
        pwr_r::postdivpd_bits::clear();

        this->m_frequency = achievedFreq;

        return achievedFreq;
    }

    /** @} */ // rp2040_pll
} // rp2040::system


#endif // RP2040_SYSTEM_PLL_T_H