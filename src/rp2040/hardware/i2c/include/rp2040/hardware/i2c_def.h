//
//  on 25.08.26.
//

#ifndef RP2040_HARDWARE_I2C_DEF_H
#define RP2040_HARDWARE_I2C_DEF_H

#include <cstdint>

#include "rp2040/reg_access.h"

namespace rp2040::hardware::i2c {
    /**
     * @addtogroup rp2040_i2c
     * @{
     */

    /**
     * @brief Base addresses of the I2C.
     */
    namespace address {
        constexpr std::uintptr_t i2c0_base = 0x40044000U; /**< @brief Base address of I2C0. */
        constexpr std::uintptr_t i2c1_base = 0x40048000U; /**< @brief Base address of I2C1. */


    }

    /**
     * @brief Offset of the specific registers from the base address.
     */
    namespace offset {
        constexpr std::uint32_t ctrl = 0x00U; /**< @brief Control register offset. */
        constexpr std::uint32_t tar = 0x04U; /**< @brief Target address register offset. */
        constexpr std::uint32_t data = 0x10U; /**< @brief Rx/Tx data buffer and command register offset. */
        constexpr std::uint32_t ss_scl_hcnt = 0x14U; /**< @brief Standard speed I2C clock SCL high count register offset. */
        constexpr std::uint32_t ss_scl_lcnt = 0x18U; /**< @brief Standard speed I2C clock SCL low count register offset. */
        constexpr std::uint32_t fs_scl_hcnt = 0x1CU; /**< @brief Fast speed I2C clock SCL high count register offset. */
        constexpr std::uint32_t fs_scl_lcnt = 0x20U; /**< @brief Fast speed I2C clock SCL low count register offset. */
        constexpr std::uint32_t enable = 0x6CU; /**< @brief I2C enable register offset. */
        constexpr std::uint32_t status = 0x70U; /**< @brief I2C status register offset. */
        constexpr std::uint32_t fs_spklen = 0xA0U; /**< @brief I2C SS, FS or FM+ spike suppression limit register offset. */
    }


    /**
     * @brief The register map for the I2C.
     *
     * @tparam base The base address of the registers belonging to a specific I2C.
     */
    template <std::uintptr_t base>
    struct I2CRegMap {
        static_assert((base == address::i2c0_base) || (base == address::i2c1_base), "Wrong i2c base address!");

        /**
         * @brief The I2C control register. (Note: This register is called 'con' in the datasheet.)
         *
         * The I2C must be disabled in order to write to this register. Writes while the I2C is enabled have no effect.
         */
        struct ctrl {
            static constexpr std::uintptr_t addr = base + offset::ctrl; /**< @brief The address of the control register. */
            using ctrl_reg = reg_access::Reg<addr, reg_access::access_tag::mixed_access, std::uint32_t>; /**< @brief The ctrl register. */

            /**
             * @brief The slave disable bit.
             */
            struct ic_slave_disable {
                /**
                 * @brief A Bitfield to describe the slave disable bit.
                 */
                struct bitfield {
                    using reg = ctrl_reg; /**< @brief Alias for the register the bitfield belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */

                    static constexpr T position = 0x06U; /**< @brief The position of the bitfield in the register. */
                    static constexpr T mask = (0x01U << position); /**< @brief The mask of the bitfield used in bit operations. */

                    /**
                     * @brief The slave can either be enabled or disabled.
                     */
                    enum class value : T {
                        slave_enabled = 0x0U,
                        slave_disabled = 0x1U,
                    };
                };

                using value = bitfield::value; /**< @brief The values for the slave disable bit. */

                using bits = ctrl_reg::template Bits<bitfield>; /**< @brief The bits object to control the slave disable bit. */
            };

            /**
             * @brief 
             */
            struct ic_restart_en {
                static constexpr ctrl_reg::RegType Pos = 0x05U;

                using bits = ctrl_reg::template Bits<reg_access::BitFieldEnableDisable<ctrl_reg, Pos>>;

            };
        };
    };

    /** @} */ // rp2040_i2c
} // rp2040::hardware::i2c

#endif //RP2040_HARDWARE_I2C_DEF_H
