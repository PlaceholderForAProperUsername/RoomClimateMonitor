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


#ifndef UTILS_REG_ACCESS_H
#define UTILS_REG_ACCESS_H

#include <cstdint>
#include <type_traits>

/**
 * @defgroup reg_access Reg_Access: Provides utilities to use registers.
 * @ingroup utils
 */

namespace utils::reg_access {
    /**
     * @defgroup access_tags Tags to control access to register utilities.
     * @ingroup reg_access
     * @{
     */

    /** @brief Tag to control read access. */
    struct read_access {};
    /** @brief Tag to control write access. */
    struct write_access {};
    /** @brief The register supports atomic operations. */
    struct atomic {};
    /** @brief Tag to enable atomic set operation. */
    struct atomic_set : write_access, atomic {};
    /** @brief Tag to enable atomic clear operation. */
    struct atomic_clear : write_access, atomic {};
    /** @brief Tag to enable atomic xor operation. */
    struct atomic_xor : write_access, atomic {};
    /** @brief Tag to control read and write access. */
    struct read_write_access : read_access, write_access {};
    /** @brief Tag to control read and write access. Used for registers where access varies between bits. */
    struct reg_mixed_access : read_write_access {};
    /** @brief Tag to control read and write access. Used to indicate a bit can be cleared with a write. */
    struct bit_write_clear : read_write_access {};
    /** @}*/ // end group access_tags


    /**
     * @brief Checks the validity of a BitField.
     *
     * A BitField represents specific bits of a register. As minimum requirements, the BitField needs the following type
     * aliases:
     *  - reg: type alias to the registers
     *  - T: base data type of the register
     *
     * Furthermore, the following variables must be defined:
     *  - position: position of the first bit in the register
     *  - mask: Bitmask of the bits contained in the BitField
     *  - value: An enum with the allowed values for the BitField.
     *
     * For generic examples, @see BitFieldEnableDisable and @see BitFieldValues
     *
     * @tparam BitField The BitField instance to check.
     * @tparam Reg The register to which the BitField belongs.
     * @tparam T The base data type of the register.
     *
     * @ingroup reg_access
     */
    template <typename BitField, typename Reg, typename T>
    concept BitFieldConcept =
        std::is_same_v<Reg, typename BitField::reg> &&
        std::is_enum_v<typename BitField::value> &&
        std::is_same_v<std::underlying_type_t<typename BitField::value>, T>;

    /**
     * @brief Represents a register and provides functions to configure the register.
     *
     * @tparam Address The address of the register.
     * @tparam RegAccess The access specifier for the whole register. @see access_tags
     * @tparam T The data type of the register.
     *
     * @ingroup reg_access
     */
    template <std::uintptr_t Address, typename RegAccess = read_write_access, typename T = std::uint32_t>
    struct Reg {
        using ThisReg = Reg<Address, RegAccess, T>; /**< @brief Alias for the register. */
        using RegType = T; /**< @brief Alias for the underlying type of the register. */

        /**
         * @brief Represents specific bits of the enclosing register and provides functions to configure the bits of the register.
         *
         * @tparam BitField The BitField specifying the bits.
         * @tparam BitsAccess The access specifier for the group of bits.
         */
        template <typename BitField, typename BitsAccess = RegAccess>
        requires BitFieldConcept<BitField, ThisReg, T>
        struct Bits {
            /**
             * @brief Sets the bits to the value.
             *
             * @tparam BitsAccess_ Access specifier for the bits.
             * @param[in] bits_value The value to be set.
             * @return None
             */
            template <typename BitsAccess_ = BitsAccess>
            static std::enable_if_t<std::is_base_of_v<read_write_access, BitsAccess_>, void>
            set(BitField::value bits_value)
            {
                auto reg_value = read();
                reg_value &= ~BitField::mask;
                reg_value |= ((static_cast<T>(bits_value) << BitField::position) & BitField::mask);
                write(reg_value);
            }

            /**
             * @brief Sets the bits to the value.
             *
             * Requires atomic set register.
             *
             * @tparam BitsAccess_ Access specifier for the bits.
             * @param[in] bits_value The value to be set.
             * @return
             */
            template <typename BitsAccess_ = BitsAccess>
            static std::enable_if_t<std::is_base_of_v<atomic_set, BitsAccess_>, void>
            set(BitField::value bits_value)
            {
                T reg_value = ((static_cast<T>(bits_value) << BitField::position) & BitField::mask);
                write(reg_value);
            }

            /**
             * @brief Returns the bits.
             *
             * @tparam BitsAccess_ Access specifier for the bits.
             * @return T The unshifted bits.
             */
            template <typename BitsAccess_ = BitsAccess>
            static std::enable_if_t<std::is_base_of_v<read_access, BitsAccess_>, T>
            getBits()
            {
                auto reg_value = read();
                reg_value &= BitField::mask;
                return reg_value;
            }

            /**
             * @brief Returns the value of the bits.
             *
             * @tparam BitsAccess_ Access specifier for the bits.
             * @return T The value of the bits (Bits are shifted to position 0).
             */
            template <typename BitsAccess_ = BitsAccess>
            static std::enable_if_t<std::is_base_of_v<read_access, BitsAccess_>, T>
            getValue()
            {
                auto reg_value = read();
                reg_value &= BitField::mask;
                reg_value >>= BitField::position;
                return reg_value;
            }

            /**
             * @brief Toggles the bits.
             *
             * @tparam BitsAccess_ Access specifier for the bits.
             * @return None
             */
            template <typename BitsAccess_ = BitsAccess>
            static std::enable_if_t<std::is_base_of_v<read_write_access, BitsAccess_>, void>
            toggle()
            {
                auto reg_value = read();
                reg_value ^= BitField::mask;
                write(reg_value);
            }

            /**
             * @brief Toggles the bits.
             *
             * Requires atomic xor register.
             *
             * @tparam BitsAccess_ Access specifier for the bits.
             * @return None
             */
            template <typename BitsAccess_ = BitsAccess>
            static std::enable_if_t<std::is_base_of_v<atomic_xor, BitsAccess_>, void>
            toggle()
            {

                T reg_value = BitField::mask;
                write(reg_value);
            }

            /**
             * @brief Clears the bits.
             *
             * @tparam BitsAccess_ Access specifier for the bits.
             * @return None
             */
            template <typename BitsAccess_ = BitsAccess>
            static std::enable_if_t<std::is_base_of_v<read_write_access, BitsAccess_>, void>
            clear()
            {
                auto reg_value = read();
                reg_value &= ~BitField::mask;
                write(reg_value);
            }


            /**
             * @brief Clears the bits.
             *
             * Requires atomic clear register.
             *
             * @tparam BitsAccess_ Access specifier for the bits.
             * @return None
             */
            template <typename BitsAccess_ = BitsAccess>
            static std::enable_if_t<std::is_base_of_v<atomic_clear, BitsAccess_>, void>
            clear()
            {
                auto reg_value = read();
                reg_value &= ~BitField::mask;
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
        static std::enable_if_t<std::is_base_of_v<read_access, RegAccess_>, T>
        read ()
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
        static std::enable_if_t<std::is_base_of_v<write_access, RegAccess_>, void>
        write(T value)
        {
            *reinterpret_cast<volatile T*>(Address) = value;
        }
    };

    /**
     * @brief General BitField for a single enable/disable bit.
     *
     * @tparam Reg The register to which the BitField belongs. @see Reg
     * @tparam Pos The position of the bit within the register.
     *
     * @ingroup reg_access
     */
    template <typename Reg, std::uint32_t Pos>
    struct BitFieldEnableDisable {
        using reg = Reg; /**< @brief Alias for the register the bitfield belongs to. */
        using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */

        static constexpr T position = Pos; /**< @brief The position of the bitfield in the register. */
        static constexpr T mask = (0x1UL << position); /**< @brief The mask of the bitfield used in bit operations. */

        /**
         * @brief The possible values for this bitfield.
         */
        enum class value : T {
            disable = 0,
            enable = 1
        };
    };

    /**
     * @brief General BitField for several bits with continuous values.
     *
     * @tparam Value The value for the BitField.
     * @tparam Reg The register to which the BitField belongs. @see Reg
     * @tparam Mask The unshifted mask of the bits. As an example, for 3 bits the Mask will be 0x07 regardless of position.
     * @tparam Pos The position of the bit within the register.
     *
     * @ingroup reg_access
     */
    template <auto Value, typename Reg, std::uint32_t Mask, std::uint32_t Pos>
    struct BitFieldValues {
        using reg = Reg; /**< @brief Alias for the register the bitfield belongs to. */
        using T = reg::RegType; /**< @brief Alias for the underlying data type of the register. */

        static_assert(std::is_same_v<T, decltype(Value)>);

        static constexpr T position = Pos; /**< @brief The position of the bitfield in the register. */
        static constexpr T mask = (Mask << position); /**< @brief The mask of the bitfield used in bit operations. */

        static_assert(Value <= Mask);

        /**
         * @brief The possible value for this bitfield.
         *
         * Implementation note: Even though this bitfield doesn't necessarily need an enum, it is used to have a unified
         * interface for the bitfield type.
         */
        enum class value : T {
            val = Value
        };
    };

} // utils::reg_access

#endif //UTILS_REG_ACCESS_H
