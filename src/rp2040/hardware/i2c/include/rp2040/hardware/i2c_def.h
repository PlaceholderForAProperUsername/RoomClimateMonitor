/**
 * @file i2c_def.h
 * @brief The definitions for the I2C module of the RP2040.
 * @author Thorsten Hoffmeister
 * @version 0.1
 * @date 02.10.2026
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
     * @brief The speed modes available to the RP2040's I2C controllers.
     */
    enum class SpeedMode : std::uint32_t {
        standard = 0x01U, /**< @brief Data rates of <= 100kbps. */
        fast = 0x02U, /**< @brief Data rates of <= 400kbps. */
        fast_plus = 0x03U /**< @brief Data rates of <= 1000kbps. */
    };

    /**
     * @brief The address modes.
     */
    enum class AddressMode : std::uint32_t {
        addr_7bits = 0x00U,
        addr_10bits = 0x01U,
    };

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
        static constexpr std::uint32_t sda_hold_offset = 0x7CU; /**< @brief I2C SDA hold register offset. */
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
                    using value = AddressMode;
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
                    using value = SpeedMode;
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
            static constexpr std::uintptr_t addr = base + tar_offset; /**< @brief The address of the target address register. */
            using tar_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The target address register. */

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

        struct DataCmd {
            static constexpr std::uintptr_t addr = base + data_offset; /**< @brief The address of the data cmd register. */
            using data_reg = reg_access::Reg<addr, reg_access::access_tag::mixed_access, std::uint32_t>; /**< @brief The data cmd register. */

            /**
             * @brief Indicates if the byte received is the first data byte after address phase or an sequential byte.
             */
            struct FirstDataByte {
                /**
                 * @brief Description of the FirstDataByte bit.
                 */
                struct BitType {
                    using reg = data_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = reg_access::access_tag::read; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 11U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x01U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief Values to select general call or start byte command.
                     */
                    enum class value : T {
                        inactive = 0x00U, /**< @brief Byte received is not the first data byte. */
                        active = 0x01U, /**< @brief Byte received is the first data byte. */
                    };
                };
                using Bits = data_reg::template Bits<BitType>; /**< @brief Object to handle the FirstDataByte bit. */
                using Value = BitType::value; /**< @brief Values of the FirstDataByte bit. */
            };

            /**
             * @brief The data, cmd, stop and restart bits.
             *
             * These bits must be written together, as a write to the DataCmd register start a transaction (both
             * receiving and transmission).
             */
            struct DataCommand {
                /**
                 * @brief Description of the data bits.
                 */
                struct BitType {
                    using reg = data_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = reg_access::access_tag::read_write; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x7FFU << position); /**< @brief The mask of the bit used in bit operations. */
                };
                struct Flags {
                    static constexpr std::uint8_t write_cmd = 0U;
                    static constexpr std::uint8_t read_cmd = 1U;
                    static constexpr std::uint8_t send_stop = 2U;
                    static constexpr std::uint8_t send_restart = 4U;
                };
                using Bits = data_reg::template Bits<BitType>; /**< @brief Object to handle the data bit. */
                data_reg::RegType Value = [] (const std::uint8_t data, const std::uint8_t flags = 0) {
                    static constexpr std::uint32_t flag_pos = 8U;
                    static constexpr std::uint8_t flag_mask = 0x7U;
                    return (data & 0xFFU) | ((flags & flag_mask) << flag_pos);
                };
            };
        };

        /**
         * @brief The scl count register base.
         *
         * There are four count registers with the same layout: ss_scl_hcnt, ss_scl_lcnt, fs_scl_hcnt and fs_scl_lcnt.
         *
         * @tparam offset The register offset from the base address.
         */
        template <std::uintptr_t offset>
        struct CountBase {
            static constexpr std::uintptr_t addr = base + offset; /**< @brief The address of the count register. */
            using cnt_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The count register. */

            /**
             * @brief The count bits.
             *
             * @tparam count The count value.
             */
            template <typename cnt_reg::RegType count>
            struct Cnt {
                /** @brief Description of the count bits. */
                struct BitType {
                    using reg = cnt_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = cnt_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0xFFFFU << position); /**< @brief The mask of the bit used in bit operations. */

                    static_assert(count <= mask, "Invalid value for cnt bits!");

                    /**
                     * @brief The count value.
                     */
                    enum class value : T {
                        val = count,
                    };
                };
                using Bits = cnt_reg::template Bits<BitType>; /**< @brief Object to handle the count bits. */
                using Value = BitType::value; /**< @brief Values of the count bits. */
            };
        };

        using SsSclHcnt = CountBase<ss_scl_hcnt_offset>; /**< @brief The standard speed scl high count register. */
        using SsSclLcnt = CountBase<ss_scl_lcnt_offset>; /**< @brief The standard speed scl low count register. */
        using FsSclHcnt = CountBase<fs_scl_hcnt_offset>; /**< @brief The fast speed scl high count register. */
        using FsSclLcnt = CountBase<fs_scl_lcnt_offset>; /**< @brief The fast speed scl low count register. */

        /**
         * @brief The enable register.
         */
        struct IcEnable {
            static constexpr std::uintptr_t addr = base + enable_offset;
            using enable_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>;

            /**
             * @brief Blocks the transmission in master mode
             */
            struct TxCmdBlock {
                /**
                 * @brief Description of the TxCmdBlock bit.
                 */
                struct BitType {
                    using reg = enable_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = enable_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 2U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x1U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief The count value.
                     */
                    enum class value : T {
                        not_blocked = 0x00U, /**< @brief Transmission command execution not blocked. */
                        blocked = 0x01U, /**< @brief Transmission command execution blocked. */
                    };
                };
                using Bits = enable_reg::template Bits<BitType>; /**< @brief Object to handle the TxCmdBlock bit. */
                using Value = BitType::value; /**< @brief Values of the TxCmdBlock bit. */
            };

            /**
             * @brief Enables the transfer abort.
             */
            struct Abort {
                static constexpr enable_reg::RegType pos = 1U; /**< @brief Position of the abort bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<enable_reg, pos>; /**< @brief Description of the abort bit. */
                using Bits = enable_reg::template Bits<BitType>; /**< @brief Object to handle the abort bit. */
                using Value = BitType::value; /**< @brief Values of the abort bit. */
            };

            /**
             * @brief Enables the i2c controller.
             */
            struct Enable {
                static constexpr enable_reg::RegType pos = 0U; /**< @brief Position of the enable bit in the register. */
                using BitType = reg_access::BitTypeEnableDisable<enable_reg, pos>; /**< @brief Description of the enable bit. */
                using Bits = enable_reg::template Bits<BitType>; /**< @brief Object to handle the enable bit. */
                using Value = BitType::value; /**< @brief Values of the enable bit. */
            };
        };

        /**
         * @brief The status register.
         */
        struct Status {
            static constexpr std::uintptr_t addr = base + status_offset; /**< @brief Address of the status register. */
            using status_reg = reg_access::Reg<addr, reg_access::access_tag::read, std::uint32_t>; /**< @brief The status register. */


            /**
             * @brief Slave FSM activity status.
             */
            struct SlvActivity {
                /**
                 * @brief Description of the slave activity bit.
                 */
                struct BitType {
                    using reg = status_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = status_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 6U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x1U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief The activity value.
                     */
                    enum class value : T {
                        idle = 0x00U, /**< @brief Slave is idle. */
                        active = 0x01U, /**< @brief Slave is active. */
                    };
                };
                using Bits = status_reg::template Bits<BitType>; /**< @brief Object to handle the slave activity bit. */
                using Value = BitType::value; /**< @brief Values of the slave activity bit. */
            };

            /**
            * @brief Master FSM activity status.
            */
            struct MstActivity {
                /**
                 * @brief Description of the master activity bit.
                 */
                struct BitType {
                    using reg = status_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = status_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 5U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x1U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief The activity value.
                     */
                    enum class value : T {
                        idle = 0x00U, /**< @brief Master is idle. */
                        active = 0x01U, /**< @brief Master is active. */
                    };
                };
                using Bits = status_reg::template Bits<BitType>; /**< @brief Object to handle the Master activity bit. */
                using Value = BitType::value; /**< @brief Values of the Master activity bit. */
            };

            /**
            * @brief Receive FIFO full bit.
            */
            struct Rff {
                /**
                 * @brief Description of the rff bit.
                 */
                struct BitType {
                    using reg = status_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = status_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 4U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x1U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief The fullness value.
                     */
                    enum class value : T {
                        not_full = 0x00U, /**< @brief Receive FIFO not full. */
                        full = 0x01U, /**< @brief Receive FIFO full. */
                    };
                };
                using Bits = status_reg::template Bits<BitType>; /**< @brief Object to handle the rff bit. */
                using Value = BitType::value; /**< @brief Values of the rff bit. */
            };

            /**
            * @brief Receive FIFO not empty bit.
            */
            struct Rfne {
                /**
                 * @brief Description of the rfne bit.
                 */
                struct BitType {
                    using reg = status_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = status_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 3U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x1U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief The fullness value.
                     */
                    enum class value : T {
                        empty = 0x00U, /**< @brief Receive FIFO is empty. */
                        not_empty = 0x01U, /**< @brief Receive FIFO not empty. */
                    };
                };
                using Bits = status_reg::template Bits<BitType>; /**< @brief Object to handle the rfne bit. */
                using Value = BitType::value; /**< @brief Values of the rfne bit. */
            };

            /**
            * @brief Transmit FIFO empty bit.
            */
            struct Tfe {
                /**
                 * @brief Description of the tfe bit.
                 */
                struct BitType {
                    using reg = status_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = status_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 2U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x1U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief The fullness value.
                     */
                    enum class value : T {
                        non_empty = 0x00U, /**< @brief Transmit FIFO not empty. */
                        empty = 0x01U, /**< @brief Transmit FIFO is empty. */
                    };
                };
                using Bits = status_reg::template Bits<BitType>; /**< @brief Object to handle the tfe bit. */
                using Value = BitType::value; /**< @brief Values of the tfe bit. */
            };

            /**
            * @brief Transmit FIFO not full bit.
            */
            struct Tfnf {
                /**
                 * @brief Description of the tfnf bit.
                 */
                struct BitType {
                    using reg = status_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = status_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 1U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x1U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief The fullness value.
                     */
                    enum class value : T {
                        full = 0x00U, /**< @brief Receive FIFO is full. */
                        not_full = 0x01U, /**< @brief Receive FIFO not full. */
                    };
                };
                using Bits = status_reg::template Bits<BitType>; /**< @brief Object to handle the tfnf bit. */
                using Value = BitType::value; /**< @brief Values of the tfnf bit. */
            };

            /**
            * @brief I2C activity status.
            */
            struct Activity {
                /**
                 * @brief Description of the I2C activity bit.
                 */
                struct BitType {
                    using reg = status_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = status_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0x1U << position); /**< @brief The mask of the bit used in bit operations. */

                    /**
                     * @brief The activity value.
                     */
                    enum class value : T {
                        inactive = 0x00U, /**< @brief I2C is idle. */
                        active = 0x01U, /**< @brief I2C is active. */
                    };
                };
                using Bits = status_reg::template Bits<BitType>; /**< @brief Object to handle the I2C activity bit. */
                using Value = BitType::value; /**< @brief Values of the I2C activity bit. */
            };
        };

        /**
         * @brief The SDA hold register.
         *
         * Sets the hold time when acting as a receiver (SdaRxHold) or transmitter (SdaTxHold). The bits can only be set
         * when the I2C controller is disabled. @See IcEnable
         */
        struct SdaHold {
            static constexpr std::uintptr_t addr = base + sda_hold_offset; /**< @brief Address of the SDA hold register. */
            using sda_hold_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The SDA hold register. */

            /**
             * @brief The SDA receive hold register.
             *
             * @tparam hold_count The hold time in clock cycles.
             */
            template <typename sda_hold_reg::RegType hold_count>
            struct SdaRxHold {
                /** @brief Description of the SDA receive hold bits. */
                struct BitType {
                    using reg = sda_hold_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = sda_hold_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 16U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0xFFU << position); /**< @brief The mask of the bit used in bit operations. */

                    static_assert(hold_count <= mask, "Invalid hold count value");

                    /**
                     * @brief The SDA hold time in clock cycles.
                     */
                    enum class value : T {
                        val = hold_count,
                    };
                };
                using Bits = sda_hold_reg::template Bits<BitType>; /**< @brief Object to handle the SDA receive hold bit. */
                using Value = BitType::value; /**< @brief Values of the SDA receive hold bit. */
            };

            /**
             * @brief The SDA transmission hold register.
             *
             * @tparam hold_count The hold time in clock cycles.
             */
            template <typename sda_hold_reg::RegType hold_count>
            struct SdaTxHold {
                /** @brief Description of the SDA transmission hold bits. */
                struct BitType {
                    using reg = sda_hold_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = sda_hold_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0xFFFFU << position); /**< @brief The mask of the bit used in bit operations. */

                    static_assert(hold_count <= mask, "Invalid hold count value");

                    /**
                     * @brief The SDA transmission hold time in clock cycles.
                     */
                    enum class value : T {
                        val = hold_count,
                    };
                };
                using Bits = sda_hold_reg::template Bits<BitType>; /**< @brief Object to handle the SDA transmission hold bit. */
                using Value = BitType::value; /**< @brief Values of the SDA transmission hold bit. */
            };
        };

        /**
         * @brief The spike length suppression register.
         */
        struct IcFsSpkLen {
            static constexpr std::uintptr_t addr = base + fs_spklen_offset; /**< @brief Address of the spike length register. */
            using spk_len_reg = reg_access::Reg<addr, reg_access::access_tag::read_write, std::uint32_t>; /**< @brief The spike length register. */

            /**
             * @brief The spike length suppression bits.
             *
             * @tparam spk_len The length of the suppression in ic_clk cycles.
             */
            template <typename spk_len_reg::RegType spk_len>
            struct SpkLen {
                /** @brief Description of the spike length suppression bits. */
                struct BitType {
                    using reg = spk_len_reg; /**< @brief Alias for the register the bits belongs to. */
                    using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
                    using bit_access = spk_len_reg::reg_access; /**< @brief The access tag defining bit operations. */

                    static constexpr T position = 0U; /**< @brief The position of the bit in the register. */
                    static constexpr T mask = (0xFFU << position); /**< @brief The mask of the bit used in bit operations. */

                    static_assert((1 <= spk_len) && (spk_len <= mask), "Invalid spk_len value");

                    /**
                     * @brief The length of the longest spike filtered out.
                     */
                    enum class value : T {
                        val = spk_len,
                    };
                };
                using Bits = spk_len_reg::template Bits<BitType>; /**< @brief Object to handle the spike length suppression bit. */
                using Value = BitType::value; /**< @brief Values of the spike length suppression bit. */
            };

        };
    };

    /** @} */ // rp2040_i2c
} // rp2040::hardware::i2c

#endif //RP2040_HARDWARE_I2C_DEF_H
