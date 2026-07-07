//
//  on 03.07.26.
//

#ifndef RP2040_SYSTEM_CLOCKS_H
#define RP2040_SYSTEM_CLOCKS_H

#include <cstdint>

#include "clocks_def.h"

namespace rp2040::system::clocks {
    /**
     * @addtogroup rp2040_clocks
     * @{
     */

    /**
     * @brief Controls the reference clock.
     *
     * @tparam ref_addr The base address of the reference clock control registers.
     */
    template <std::uintptr_t ref_addr>
    class Ref_Type {
        static_assert(ref_addr == ref_base, "Invalid reference clock address.");

        using RegMap = Ref_RegMapType<ref_addr>; /**< @brief The register map of the reference clock registers. */
        using ctrl_r = RegMap::ctrl; /**< @brief The control register. */
        using div_r = RegMap::div; /**< @brief The div register. */
        using selected_r = RegMap::selected; /**< @brief The selected clock register. */

    public:
        /**
         * @brief Gets the instance of the reference clock object.
         *
         * @return The singleton instance of the ref clock object.
         */
        Ref_Type& getInstance();

    private:
        Ref_Type() = default;
    };


    using RefClock = Ref_Type<ref_base>;

    /** @}*/ // rp2040_clocks
} // rp2040::system::clocks


#include "../../../clocks.tpp"

#endif // RP2040_SYSTEM_CLOCKS_H
