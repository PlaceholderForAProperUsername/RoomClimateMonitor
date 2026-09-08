//
//  on 01.09.26.
//

#ifndef RP2040_REG_ACCESS_DEF_H
#define RP2040_REG_ACCESS_DEF_H

namespace rp2040::reg_access {

    /**
     * @defgroup access_tags Tags to control access to register utilities.
     * @ingroup reg_access
     */
    namespace access_tag {
        /** @brief Tag to control read access. */
        struct read {};
        /** @brief Tag to control write access. */
        struct write {};
        /** @brief The register supports atomic operations. */
        struct atomic {};
        /** @brief Tag to enable atomic set operation. */
        struct atomic_set : write, atomic {};
        /** @brief Specialized Tag to enable atomic set operation where the value is the mask. */
        struct atomic_set_mask : write, atomic {};
        /** @brief Tag to enable atomic clear operation. */
        struct atomic_clear : write, atomic {};
        /** @brief Tag to enable atomic xor operation. */
        struct atomic_xor : write, atomic {};
        /** @brief Tag to control read and write access. */
        struct read_write : read, write {};
        /** @brief Tag to control read and write access. Used for registers where access varies between bits. */
        struct mixed_access : read_write {};
        /** @brief Tag to control read and write access. Used to indicate a bit can be cleared with a write. */
        struct write_clear : read_write {};
    }

    /**
     * @brief Checks the validity of a BitType.
     *
     * A BitType represents specific bits of a register. As minimum requirements, the BitType needs the following type
     * aliases:
     *  - reg: type alias to the registers
     *  - T: base data type of the register
     *
     * Furthermore, the following variables must be defined:
     *  - position: position of the first bit in the register
     *  - mask: Bitmask of the bits contained in the BitType
     *
     *  For write access, an enum must be provided @see HasEnumConcept, except for atomic set operation, where the
     *  bit mask is the value.
     *
     * For generic examples, @see BitTypeEnableDisable and @see BitTypeValues
     *
     * @tparam BitType The BitType instance to check.
     * @tparam Reg The register to which the BitType belongs.
     * @tparam T The base data type of the register.
     */
    template <typename BitType, typename Reg, typename T>
    concept BitTypeConcept =
            std::is_same_v<Reg, typename BitType::reg> &&
            std::is_same_v<decltype(BitType::position), const T> &&
            std::is_same_v<decltype(BitType::mask), const T> &&
            std::is_enum_v<typename BitType::value> &&
            std::is_same_v<std::underlying_type_t<typename BitType::value>, T>;

}// rp2040::reg_access

#endif //RP2040_REG_ACCESS_DEF_H
