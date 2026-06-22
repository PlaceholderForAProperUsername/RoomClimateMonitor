/**
 * @file status.h
 * @brief Provides status codes.
 * @author Thorsten Hoffmeister
 * @version 1.0
 * @date 22.06.2026
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

#ifndef UTILS_STATUS_H
#define UTILS_STATUS_H

/**
 * @defgroup status Status codes to verify the status of the program.
 * @ingroup utils
 */

namespace utils::status {
    /**
     * @brief The status codes.
     *
     * @ingroup status
     */
    enum class Status : unsigned {
        OK                      = 0U,   /**< @brief All systems nominal. */
        ERROR_GENERIC           = 1U,   /**< @brief Generic error code. */
        ERROR_NULL              = 2U,   /**< @brief Null pointer error. */
        ERROR_PARAM             = 3U,   /**< @brief Invalid parameter provided error. */
        ERROR_RANGE             = 4U,   /**< @brief Out of bounds error. */
        BUSY                    = 5U,   /**< @brief Resource is busy. */
        ERROR_TIMEOUT           = 6U,   /**< @brief A timeout occurred. */
        ERROR_RESOURCE          = 7U,   /**< @brief Resource is unavailable. */
        ERROR_INIT              = 8U,   /**< @brief Not initialized error. */
        ERROR_STATE             = 9U,   /**< @brief Erroneous state occurred. */
        ERROR_NOT_IMPLEMENTED   = 10U,  /**< @brief Resource not implemented error. */
        PENDING                 = 11U   /**< @brief Resource is pending. */
    };

} // utils::status

#endif // UTILS_STATUS_H
