//
//  on 06.06.26.
//

#ifndef UTILS_REG_ACCESS_H
#define UTILS_REG_ACCESS_H

#include <cstdint>
#include <type_traits>

namespace utils::reg_access {
    struct read_access {};
    struct write_access {};
    struct read_write_access : read_access, write_access {};
    struct mixed_access : read_access, write_access {};

    template <typename BitField, typename Reg, typename T>
    concept BitFieldConcept =
        std::is_same_v<Reg, typename BitField::reg> &&
        std::is_enum_v<typename BitField::value> &&
        std::is_same_v<std::underlying_type_t<typename BitField::value>, T>;

    template <std::uintptr_t Address, typename RegAccess = read_write_access, typename T = std::uint32_t>
    struct Reg {
        using RegType = T;
        using ThisReg = Reg<Address, RegAccess, T>;

        template <typename BitField, typename BitsAccess = RegAccess>
        requires BitFieldConcept<BitField, ThisReg, T>
        struct Bits {
            template <typename BitsAccess_ = BitsAccess>
            static std::enable_if_t<std::is_base_of_v<write_access, BitsAccess_>, void>
            set(BitField::value bits_value)
            {
                auto reg_value = read();
                reg_value &= ~BitField::mask;
                reg_value |= ((static_cast<T>(bits_value) << BitField::position) & BitField::mask);
                write(reg_value);
            }
        };

        template <typename RegAccess_ = RegAccess>
        static std::enable_if_t<std::is_base_of_v<read_access, RegAccess_>, T>
        read ()
        {
            return *reinterpret_cast<volatile T*>(Address);
        }

    private:
        template <typename RegAccess_ = RegAccess>
        static std::enable_if_t<std::is_base_of_v<write_access, RegAccess_>, void>
        write(T value)
        {
            *reinterpret_cast<volatile T*>(Address) = value;
        }
    };


    template <typename Reg, std::uint32_t Pos>
    struct RegBitsEnableDisable {
        using reg = Reg;
        using T = reg::RegType;

        static constexpr T position = Pos;
        static constexpr T mask = (0x1UL << position);

        enum class value : T {
            disable = 0,
            enable = 1
        };
    };


    template <auto Bits, typename Reg, std::uint32_t Mask, std::uint32_t Pos>
    struct RegBits {
        using reg = Reg;
        using T = reg::RegType;

        static_assert(std::is_same_v<T, decltype(Bits)>);

        static constexpr T position = Pos;
        static constexpr T mask = (Mask << position);

        static_assert(Bits <= Mask);

        enum class value : T {
            val = Bits
        };
    };


    template <typename T>
    consteval T getMask(T bits)
    {
        T result = 0;
        for (T i {0}; i < bits; ++i) {
            result += (1U << i);
        }
        return result;
    }

} // utils::reg_access

#endif //UTILS_REG_ACCESS_H
