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
     * @brief The register map for the I2C.
     *
     * @tparam base The base address of the registers belonging to a specific I2C.
     */
    template <std::uintptr_t base>
    struct I2CRegMap {
        static_assert((base == address::i2c0_base) || (base == address::i2c1_base), "Wrong i2c base address!");

        static constexpr std::uint32_t ctrl_offset = 0x00U; /**< @brief Control register offset. */
        static constexpr std::uint32_t tar_offset = 0x04U; /**< @brief Target address register offset. */
        static constexpr std::uint32_t data_offset = 0x10U; /**< @brief Rx/Tx data buffer and command register offset. */
        static constexpr std::uint32_t ss_scl_hcnt_offset = 0x14U; /**< @brief Standard speed I2C clock SCL high count register offset. */
        static constexpr std::uint32_t ss_scl_lcnt_offset = 0x18U; /**< @brief Standard speed I2C clock SCL low count register offset. */
        static constexpr std::uint32_t fs_scl_hcnt_offset = 0x1CU; /**< @brief Fast speed I2C clock SCL high count register offset. */
        static constexpr std::uint32_t fs_scl_lcnt_offset = 0x20U; /**< @brief Fast speed I2C clock SCL low count register offset. */
        static constexpr std::uint32_t enable_offset = 0x6CU; /**< @brief I2C enable register offset. */
        static constexpr std::uint32_t status_offset = 0x70U; /**< @brief I2C status register offset. */
        static constexpr std::uint32_t fs_spklen_offset = 0xA0U; /**< @brief I2C SS, FS or FM+ spike suppression limit register offset. */

        /**
         * @brief The I2C control register. (Note: This register is called 'con' in the datasheet.)
         *
         * The I2C must be disabled in order to write to this register. Writes while the I2C is enabled have no effect.
         */
        struct Ctrl {
            static constexpr std::uintptr_t addr = base + ctrl_offset; /**< @brief The address of the control register. */
            using ctrl_reg = reg_access::Reg<addr, reg_access::access_tag::mixed_access, std::uint32_t>; /**< @brief The ctrl register. */

            /**
             * @brief The slave disable bit.
             */
            struct IcSlaveDisable {
                /**
                 * @brief Description of the slave_disable bit.
                 */
                struct BitType {
                    using reg = ctrl_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 6U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x01U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief The slave can either be enabled or disabled.
                     */
                    enum class value : T {
                        slave_enabled = 0x0U,
                        slave_disabled = 0x1U,
                    };
                };
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to control the slave disable bit. */
                using Value = BitType::value; /**< @brief The values for the slave disable bit. */
            };

            /**
             * @brief Enables sending restart condition.
             */
            struct IcRestartEn {
                static constexpr ctrl_reg::RegType pos = 5U; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<ctrl_reg, pos>; /**< @brief Description of the restart enable bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to control the restart enable bit. */
                using Value = BitType::value; /**< @brief Values of the restart enable bit. */
            };

            /**
             * @brief Sets the addressing mode when acting as a master.
             */
            struct Ic10BitAddrMaster {
                /**
                 * @brief Description of the addressing mode bit.
                 */
                struct BitType {
                    using reg = ctrl_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 4U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x01U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief The addressing modes when acting as a master.
                     */
                    enum class value : T {
                        addr_7bits = 0x00U,
                        addr_10bits = 0x01U,
                    };
                };
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to control the 10_bit_addr_master bit. */
                using Value = BitType::value; /**< @brief Values of the 10_bit_addr_master bit. */
            };

            /**
             * @brief Controls the speed at which the i2c controller operates.
             */
            struct Speed {
                /**
                 * @brief Description of the speed bits.
                 */
                struct BitType {
                    using reg = ctrl_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 1U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x03U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief Available speed modes.
                     */
                    enum class value : T {
                        standard_mode = 0x00U,
                        fast_mode = 0x01U,
                        high_speed_mode = 0x02U,
                    };
                };
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to control the speed bits. */
                using Value = BitType::value; /**< @brief Values of the speed bits. */
            };

            /**
             * @brief Enables/disables the master mode.
             *
             * Note: If master mode is enabled, the slave mode should be disabled. @see IcSlaveDisable
             */
            struct MasterMode {
                static constexpr ctrl_reg::RegType pos = 0U; /**< @brief Position of the bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<ctrl_reg, pos>; /**< @brief Description of the master mode bit. */
                using Bits = ctrl_reg::template Bits<BitType>; /**< @brief Object to control the master mode bit. */
                using Value = BitType::value; /**< @brief Values of the master mode bit. */
            };
        };

        /**
         * @brief Target address and special command register.
         *
         * This register can only be written to when the I2C is disabled.
         */
        struct Tar {
            static constexpr std::uintptr_t addr = base + tar_offset;
            using tar_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>;

            /**
             * @brief Enables special commands (General call, start byte)
             */
            struct Special {
                static constexpr tar_reg::RegType pos = 11U; /**< @brief Position of the bit in the target register. */
                using BitType = reg_access::BitTypeEnableDisable<tar_reg, pos>; /**< @brief Description of the special bit. */
                using Bits = tar_reg::template Bits<BitType>; /**< @brief Object to handle the special bit. */
                using Value = BitType::value; /**< @brief Values of the special bit. */
            };

            /**
             * @brief General call or start byte command bit.
             *
             * The @see Special bit must be enabled to send either a general call or a start byte.
             */
            struct GcOrStart {
                /**
                 * @brief Description of the GcOrStart bit.
                 */
                struct BitType {
                    using reg = tar_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 10U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x01U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief Values to select general call or start byte command.
                     */
                    enum class value : T {
                        general_call = 0x00U,
                        start_byte = 0x01U,
                    };
                };
                using Bits = tar_reg::template Bits<BitType>; /**< @brief Object to handle the GcOrStart bit. */
                using Value = BitType::value; /**< @brief Values of the GcOrStart bit. */
            };

            /**
             * @brief Target address for any master transaction.
             *
             * @tparam target_address The address of the target.
             */
            template <typename tar_reg::RegType target_address>
            struct IcTar {
                /**
                 * @brief Description of the target address bits.
                 */
                struct BitType {
                    using reg = tar_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x3FFU << position); /**< @brief The mask of the bit used in bit operations. */

                    static_assert(target_address <= mask, "Invalid value for target address!");

                    /**
                     * @brief Target address value.
                     */
                    enum class value : T {
                        val = target_address,
                    };
                };
                using Bits = tar_reg::template Bits<BitType>; /**< @brief Object to handle the target address bits. */
                using Value = BitType::value; /**< @brief Value of the target address bits. */
            };
        };
    };

    /** @} */ // rp2040_i2c
} // rp2040::hardware::i2c

#endif //RP2040_HARDWARE_I2C_DEF_H
