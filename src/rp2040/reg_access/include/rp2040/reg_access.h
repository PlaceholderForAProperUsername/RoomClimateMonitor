/**
 * @file reg_access.h
 * @brief Provides utilities to use registers.
 * @author Thorsten Hoffmeister
 * @version 1.0
 * @date 06.06.2026
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


#ifndef RP2040_REG_ACCESS_H
#define RP2040_REG_ACCESS_H

#include <cstdint>
#include <type_traits>

#include "reg_access_def.h"

/**
 * @defgroup reg_access Reg_Access: Provides utilities to use registers.
 * @ingroup rp2040
 */

namespace rp2040::reg_access {
    /**
     * @addtogroup reg_access
     * @{
     */

    /**
     * @brief Represents a register and provides functions to configure the register.
     *
     * @tparam Address The address of the register.
     * @tparam RegAccess The access specifier for the whole register. @see access_tags
     * @tparam T The data type of the register.
     */
    template <std::uintptr_t Address, typename RegAccess = access_tag::read_write, typename T = std::uint32_t>
    struct Reg {
        using ThisReg = Reg<Address, RegAccess, T>; /**< @brief Alias for the register. */
        using RegType = T; /**< @brief Alias for the underlying type of the register. */
        using reg_access = RegAccess; /**< @brief Access tag to control register operations. */

        /**
         * @brief Represents specific bits of the enclosing register and provides functions to configure the bits of the register.
         *
         * @tparam BitType The BitType specifying the bits.
         */
        template <typename BitType, typename BitsAccess_ = reg_access>
        requires BitTypeConcept<BitType, ThisReg, RegType>
        struct Bits {
            /**
             * @brief Sets the bits to the value.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @param[in] bits_value The value to be set.
             * @return None
             */
            template <typename BitsAccess = BitsAccess_, typename BitType_ = BitType>
            requires HasValueConcept<BitType_, RegType>
            static std::enable_if_t<std::is_base_of_v<access_tag::read_write, BitsAccess>, void>
            set(BitType_::value bits_value)
            {
                auto reg_value = read();
                reg_value &= ~BitType::mask;
                reg_value |= ((static_cast<T>(bits_value) << BitType::position) & BitType::mask);
                write(reg_value);
            }

            /**
             * @brief Sets the bits to the value.
             *
             * This overload is for bits, where the value can't be known at compile time.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @param[in] bits_value The value to be set.
             * @return None
             */
            template <typename BitsAccess = BitsAccess_, typename BitType_ = BitType>
            requires (!HasValueConcept<BitType_, RegType>)
            static std::enable_if_t<std::is_base_of_v<access_tag::read_write, BitsAccess>, void>
            set(std::uint32_t bits_value)
            {
                auto reg_value = read();
                reg_value &= ~BitType::mask;
                reg_value |= ((static_cast<T>(bits_value) << BitType::position) & BitType::mask);
                write(reg_value);
            }

            /**
             * @brief Sets the bits to the value.
             *
             * Requires atomic set register.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @param[in] bits_value The value to be set.
             * @return None
             */
            template <typename BitsAccess = BitsAccess_, typename BitType_ = BitType>
            requires HasValueConcept<BitType_, RegType>
            static std::enable_if_t<std::is_base_of_v<access_tag::atomic_set, BitsAccess>, void>
            set(BitType_::value bits_value)
            {
                T reg_value = ((static_cast<T>(bits_value) << BitType::position) & BitType::mask);
                write(reg_value);
            }

            /**
             * @brief Sets all the masked bits to one.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @return None
             */
            template <typename BitsAccess = BitsAccess_>
            static std::enable_if_t<std::is_base_of_v<access_tag::read_write, BitsAccess>, void>
            set_mask()
            {
                auto reg_value = read();
                reg_value |= BitType::mask;
                write(reg_value);
            }

            /**
             * @brief Sets all the masked bits to one.
             *
             * Requires atomic set register.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @return None
             */
            template <typename BitsAccess = BitsAccess_>
            static std::enable_if_t<std::is_base_of_v<access_tag::atomic_set, BitsAccess>, void>
            set_mask()
            {
                T reg_value = BitType::mask;
                write(reg_value);
            }

            /**
             * @brief Returns the bits.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @return T The unshifted bits.
             */
            template <typename BitsAccess = BitsAccess_>
            static std::enable_if_t<std::is_base_of_v<access_tag::read, BitsAccess>, T>
            get_bits()
            {
                auto reg_value = read();
                reg_value &= BitType::mask;
                return reg_value;
            }

            /**
             * @brief Returns the value of the bits.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @return T The value of the bits (Bits are shifted to position 0).
             */
            template <typename BitsAccess = BitsAccess_>
            static std::enable_if_t<std::is_base_of_v<access_tag::read, BitsAccess>, T>
            get_value()
            {
                auto reg_value = read();
                reg_value &= BitType::mask;
                reg_value >>= BitType::position;
                return reg_value;
            }

            /**
             * @brief Toggles the bits.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @return None
             */
            template <typename BitsAccess = BitsAccess_>
            static std::enable_if_t<std::is_base_of_v<access_tag::read_write, BitsAccess>, void>
            toggle()
            {
                auto reg_value = read();
                reg_value ^= BitType::mask;
                write(reg_value);
            }

            /**
             * @brief Toggles the bits.
             *
             * Requires atomic xor register.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @return None
             */
            template <typename BitsAccess = BitsAccess_>
            static std::enable_if_t<std::is_base_of_v<access_tag::atomic_xor, BitsAccess>, void>
            toggle()
            {

                T reg_value = BitType::mask;
                write(reg_value);
            }

            /**
             * @brief Clears the bits.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @return None
             */
            template <typename BitsAccess = BitsAccess_>
            static std::enable_if_t<std::is_base_of_v<access_tag::read_write, BitsAccess>, void>
            clear()
            {
                auto reg_value = read();
                reg_value &= ~BitType::mask;
                write(reg_value);
            }


            /**
             * @brief Clears the bits.
             *
             * Requires atomic clear register.
             *
             * @tparam BitsAccess Access specifier for the bits.
             * @return None
             */
            template <typename BitsAccess = BitsAccess_>
            static std::enable_if_t<std::is_base_of_v<access_tag::atomic_clear, BitsAccess>, void>
            clear()
            {
                T reg_value = BitType::mask;
                write(reg_value);
            }
        };

        /**
         * @brief Reads the whole register.
         *
         * @tparam RegAccess_ Access specifier for the whole register.
         * @return T The value of the register.
         */
        template <typename RegAccess_ = RegAccess>
        static std::enable_if_t<std::is_base_of_v<access_tag::read, RegAccess_>, T>
        read()
        {
            return *reinterpret_cast<volatile T*>(Address);
        }

    private:
        /**
         * @brief Writes the value to the register.
         *
         * @tparam RegAccess_ Access specifier for the whole register.
         * @param[in] value The value to be written to the register
         * @return None
         */
        template <typename RegAccess_ = RegAccess>
        static std::enable_if_t<std::is_base_of_v<access_tag::write, RegAccess_>, void>
        write(T value)
        {
            *reinterpret_cast<volatile T*>(Address) = value;
        }
    };


    constexpr unsigned int bits_per_byte = 8U; /**< @brief Constant used in general BitType error checking. */

    /**
     * @brief General BitType for a single enable/disable bit.
     *
     * @tparam Reg The register to which the BitType belongs. @see Reg
     * @tparam Pos The position of the bit within the register.
     * @tparam BitAccess The access tag to control the bit operation allowed for the bit.
     */
    template <typename Reg, std::uint32_t Pos, typename BitAccess = Reg::reg_access>
    struct BitTypeEnableDisable {
        using reg = Reg; /**< @brief Alias for the register the BitType belongs to. */
        using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
        using bit_access = BitAccess; /**< @brief Access tag to control access to bit operations. */

        static_assert(Pos < (sizeof(T) * bits_per_byte), "Pos too large for register data type.");

        static constexpr T position = Pos; /**< @brief The position of the BitType in the register. */
        static constexpr T mask = (0x1UL << position); /**< @brief The mask of the BitType used in bit operations. */

        /**
         * @brief The possible values for this BitType.
         */
        enum class value : T {
            disable = 0,
            enable = 1
        };
    };

    /**
     * @brief General BitType for a single high/low bit.
     *
     * @tparam Reg The register to which the BitType belongs. @see Reg
     * @tparam Pos The position of the bit within the register.
     * @tparam BitAccess The access tag to control the bit operation allowed for the bit.
     */
    template <typename Reg, std::uint32_t Pos, typename BitAccess = Reg::reg_access>
    struct BitTypeHighLow {
        using reg = Reg; /**< @brief Alias for the register the BitType belongs to. */
        using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
        using bit_access = BitAccess; /**< @brief Access tag to control access to bit operations. */

        static_assert(Pos < (sizeof(T) * bits_per_byte), "Pos too large for register data type.");

        static constexpr T position = Pos; /**< @brief The position of the BitType in the register. */
        static constexpr T mask = (0x1UL << position); /**< @brief The mask of the BitType used in bit operations. */

        /**
         * @brief The possible values for this BitType.
         */
        enum class value : T {
            low = 0,
            high = 1
        };
    };

    /**
     * @brief General BitType for several bits with continuous values.
     *
     * @tparam Value The value for the BitType.
     * @tparam Reg The register to which the BitType belongs. @see Reg
     * @tparam Mask The unshifted mask of the bits. As an example, for 3 bits the Mask will be 0x07 regardless of position.
     * @tparam Pos The position of the bit within the register.
     * @tparam BitAccess The access tag to control the bit operation allowed for the bit.
     */
    template <auto Value, typename Reg, std::uint32_t Mask, std::uint32_t Pos, typename BitAccess = Reg::reg_access>
    struct BitTypeValues {
        using reg = Reg; /**< @brief Alias for the register the BitType belongs to. */
        using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
        using bit_access = BitAccess; /**< @brief Access tag to control access to bit operations. */

        static_assert(std::is_same_v<T, decltype(Value)>);
        static_assert(Pos < (sizeof(T) * bits_per_byte), "Pos too large for register data type.");
        static_assert(((Mask << Pos) >> Pos) == Mask, "Mask overflows after shifting by Pos.");

        static constexpr T position = Pos; /**< @brief The position of the BitType in the register. */
        static constexpr T mask = (Mask << position); /**< @brief The mask of the BitType used in bit operations. */

        static_assert(Value <= Mask);

        /**
         * @brief The possible value for this BitType.
         *
         * Implementation note: Even though this BitType doesn't necessarily need an enum, it is used to have a unified
         * interface for the BitType type.
         */
        enum class value : T {
            val = Value
        };
    };

    /**
     * @brief Specialized BitType for atomic operations where only a single bit needs to be set.
     *
     * @tparam Reg The register to which the BitType belongs. @see Reg
     * @tparam Pos The position of the bit within the register.
     * @tparam BitAccess The access tag to control the bit operation allowed for the bit.
     */
    template <typename Reg, std::uint32_t Pos, typename BitAccess = Reg::reg_access>
    struct BitTypeAtomicBitOp {
        using reg = Reg; /**< @brief Alias for the register the BitType belongs to. */
        using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */
        using bit_access = BitAccess; /**< @brief Access tag to control access to bit operations. */

        static_assert(Pos < (sizeof(T) * bits_per_byte), "Pos too large for register data type.");

        static constexpr T position = Pos; /**< @brief The position of the BitType in the register. */
        static constexpr T mask = (0x1UL << position); /**< @brief The mask of the BitType used in bit operations. */
    };


    /** @}*/ // reg_access
} // rp2040::reg_access

#endif //RP2040_REG_ACCESS_H
