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
    namespace access {
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
        struct reg_mixed : read_write {};
        /** @brief Tag to control read and write access. Used to indicate a bit can be cleared with a write. */
        struct write_clear : read_write {};
    }

}// rp2040::reg_access

#endif //RP2040_REG_ACCESS_DEF_H
